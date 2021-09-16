#include "../include/ctoolfile/TFParser.h"
#include "../include/ctoolfile/TFEnums.h"
#include "../include/ctoolfile/TFToken.h"
#include "../include/ctoolfile/TFExpr.h"
#include "../include/ctoolfile/TFStmt.h"
#include "../include/cscript/CObject.h"

#include <vector>
#include <string>
#include <memory>
#include <cctype>
#include <typeinfo>

TFParser::TFParser()
{

}

void TFParser::initialize(std::shared_ptr<ChromaToolFile> console)
{
	this->console = console;
}

// 
std::vector<std::shared_ptr<TFStmt>> TFParser::parseTokens(std::vector<std::shared_ptr<TFToken>> tokens)
{
	current = 0;
	this->tokens = tokens;
	std::vector<std::shared_ptr<TFStmt>> statements;
	while (!isAtEnd())
	{
		statements.push_back(toolStmt());
	}
	return statements;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// chromaParser Statement Eval Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<TFStmt> TFParser::toolStmt()
{
	while (peek().get()->type != TFTokenType::HEADER && !isAtEnd())
	{
		// Loop until a header is found
		advance();
	}
	if (match({ TFTokenType::HEADER }))
	{
		std::vector<std::shared_ptr<TFStmt>> interestMaskBlocks;
		std::shared_ptr<TFToken> line = previous();
		// Skip until a valid interest mask is found
		while (!isValidInterestMask(peek().get()->type) && !isAtEnd()) { advance(); }
		while (isValidInterestMask(peek().get()->type))
		{
			interestMaskBlocks.push_back(settingsBlockStmt());
		}
		consume(TFTokenType::ENDFILE, "[parser:0101] Failed to find [end] tag inside tool file.");
		return std::make_shared<TFStmt_Tool>(interestMaskBlocks, line);
	}
	else
	{
		error(peek(), "[parser:0100] Failed to find [header] tag inside tool file.");
	}
	return nullptr;
}

std::shared_ptr<TFStmt> TFParser::settingsBlockStmt()
{
	std::shared_ptr<TFToken> line = advance();
	std::vector<std::shared_ptr<TFStmt>> settings;
	consume(TFTokenType::CURLY_LEFT, "[parser:0201] Expected '{' opening bracket after settings block name.");
	while (peek().get()->type != TFTokenType::CURLY_RIGHT)
	{
		// Skip until an ID tag is found
		while (peek().get()->type != TFTokenType::ID && peek().get()->type != TFTokenType::CURLY_RIGHT && !isAtEnd()) 
		{ 
			advance(); 
		}
		settings.push_back(settingStmt());
	}
	consume(TFTokenType::CURLY_RIGHT, "[parser:0202] Expected ']' closing bracket after settings block values.");
	return std::make_shared<TFStmt_SettingsBlock>(line.get()->type, settings, line);
}


std::shared_ptr<TFStmt> TFParser::settingStmt()
{
	std::shared_ptr<TFExpr> nameExpr = name();
	std::shared_ptr<TFExpr> valueExpr = value();
	std::shared_ptr<TFToken> line = consume(TFTokenType::SEMICOLON, "[parser:0308] Expected ';' closing after settings value.");
	return std::make_shared<TFStmt_Setting>(nameExpr, valueExpr, line);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// chromaParser Expression Eval Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<TFExpr> TFParser::name()
{
	std::shared_ptr<TFToken> line = consume(TFTokenType::ID, "[parser:0403] Expected 'ID=' tag for setting statement.");
	std::shared_ptr<TFToken> idToken = consume(TFTokenType::NUM, "[parser:0404] Expected number after settings 'ID=' tag.");
	consume(TFTokenType::DOUBLE_COLON, "[parser:0404] Expected '::' seperator after settings ID number.");
	consume(TFTokenType::SID, "[parser:0405] Expected 'SID=' tag after settings ID number.");
	std::shared_ptr<TFToken> sidToken = consume(TFTokenType::NUM, "[parser:0406] Expected number after sub settings 'SID=' tag.");
	consume(TFTokenType::DOUBLE_COLON, "[parser:0407] Expected '::' seperator after settings sub SID number.");
	std::shared_ptr<TFToken> nameToken = consume(TFTokenType::STRING, "[parser:0408] Expected settings name after sub settings.");
	return std::make_shared<TFExpr_Name>(std::stoi(idToken.get()->lexeme), std::stoi(sidToken.get()->lexeme), nameToken.get()->lexeme, line);
}
std::shared_ptr<TFExpr> TFParser::value()
{

	std::shared_ptr<TFToken> line = consume(TFTokenType::ASSIGN, "[parser:0501] Expected '=' operator after settings name.");
	std::shared_ptr<TFToken> valueToken = advance();
	
	if (!isValidValueType(valueToken.get()->type))
	{
		error(valueToken, "[parser:0502] Not a valid value type.");
	}
	else
	{
		if (valueToken.get()->type == TFTokenType::STRING)
		{
			std::shared_ptr<CObject> valueOut;
			std::string lex = valueToken.get()->lexeme;
			if (lex == "true")
			{
				valueOut = std::make_shared<CObject>(true);
				//return std::make_shared<TFExpr_Value>(std::make_shared<CObject>(true), line);
			}
			else if (lex == "false")
			{
				valueOut = std::make_shared<CObject>(false);
				//return std::make_shared<TFExpr_Value>(std::make_shared<CObject>(false), line);
			}
			else
			{
				valueOut = std::make_shared<CObject>(lex);
				//return std::make_shared<TFExpr_Value>(std::make_shared<CObject>(lex), line);
			}
			if (valueOut.get()->obj.index() == 0)
 			{
				int i = 0;
			}
			return std::make_shared<TFExpr_Value>(valueOut, line);
		}
		else if (valueToken.get()->type == TFTokenType::NUM)
		{
			return std::make_shared<TFExpr_Value>(std::make_shared<CObject>((double)std::stod(valueToken.get()->lexeme)), line);
		}
	}
	return std::make_shared<TFExpr_Value>(std::make_shared<CObject>(nullptr), line);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// chromaParser Utility Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<TFToken> TFParser::advance()
{
	if (!isAtEnd())
	{
		current++;
	}
	return previous();
}
std::shared_ptr<TFToken> TFParser::peek()
{
	return tokens[current];
}
std::shared_ptr<TFToken> TFParser::scry(size_t depth)
{
	if (tokens.size() > current + depth) { return tokens[current + depth]; }
	return nullptr;
}
std::shared_ptr<TFToken> TFParser::consume(TFTokenType type, std::string message)
{
	if (check(type))
	{
		return advance();
	}

	error(peek(), message);
	synchronize();
	return tokens[current];
}

std::shared_ptr<TFToken> TFParser::previous()
{
	return tokens[current - 1];
}
bool TFParser::match(std::vector<TFTokenType> types)
{
	for (TFTokenType type : types)
	{
		if (check(type))
		{
			advance();
			return true;
		}
	}

	return false;
}
bool TFParser::isValidInterestMask(TFTokenType type)
{
	switch (type)
	{
	case TFTokenType::METADATA:
	case TFTokenType::PAN:
	case TFTokenType::ZOOM:
	case TFTokenType::ROTATE:
	case TFTokenType::SAMPLER:
	case TFTokenType::CONTINUOUS:
	case TFTokenType::DRAG:
	case TFTokenType::ONEPOINT:
	case TFTokenType::TWOPOINT:
	case TFTokenType::THREEPOINT:
	case TFTokenType::BLEND:
	case TFTokenType::BASIC:
	case TFTokenType::IMAGE:
	case TFTokenType::CHARACTER:
	case TFTokenType::SCATTER:
	case TFTokenType::COLOR:
	case TFTokenType::ALPHA:
	case TFTokenType::DISTANCE:
	case TFTokenType::MIXING:
	case TFTokenType::MULTITIP:
	case TFTokenType::TEXTURE:
	case TFTokenType::GRAIN:
	case TFTokenType::REPEAT:
	case TFTokenType::REFERENCE:
	case TFTokenType::SMOOTHING:
	case TFTokenType::ANTIALIASING:
	case TFTokenType::VECTOR:
	case TFTokenType::POLYGON:
	case TFTokenType::POLYLINE:
	case TFTokenType::MESH:
	case TFTokenType::VORTEX:
	case TFTokenType::FAN:
	case TFTokenType::RAKE:
	case TFTokenType::GRADIENT:
	case TFTokenType::FILL:
	case TFTokenType::PATTERN:
	case TFTokenType::FIELD:
	case TFTokenType::SHADER:
	case TFTokenType::FILTER:
	case TFTokenType::FX_FILL:
	case TFTokenType::FX_GRADIENT:
	case TFTokenType::FX_POSTERIZE:
	case TFTokenType::FX_INVERT:
	case TFTokenType::FX_THRESHOLD:
	case TFTokenType::FX_BRIGHTCONTRAST:
	case TFTokenType::FX_HSV:
	case TFTokenType::EFFECTS:
		return true;
	}
	return false;
}
bool TFParser::isValidValueType(TFTokenType type)
{
	switch (type)
	{
	case TFTokenType::STRING:
	case TFTokenType::NUM:
		return true;
	}
	return false;
}
bool TFParser::check(TFTokenType type)
{
	if (isAtEnd())
	{
		return false;
	}
	return peek().get()->type == type;
}

bool TFParser::isAtEnd()
{
	return peek().get()->type == TFTokenType::END;
}

//
void TFParser::synchronize()
{
	advance();

	while (!isAtEnd())
	{
		if (previous().get()->type == TFTokenType::SEMICOLON) return;

		switch (peek().get()->type)
		{
		case TFTokenType::ID:
			return;
		}

		advance();
	}
}

//

void TFParser::error(std::shared_ptr<TFToken> token, std::string message)
{
	console.get()->error(token, message);
}