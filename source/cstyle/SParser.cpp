#include "../include/cstyle/SParser.h"
#include "../include/cstyle/SEnums.h"
#include "../include/cstyle/SToken.h"
#include "../include/cstyle/SProc.h"

#include <vector>
#include <string>
#include <memory>
#include <cctype>
#include <typeinfo>

SParser::SParser()
{

}

void SParser::initialize(std::shared_ptr<ChromaStyle> console)
{
	this->console = console;
}

// 
std::vector<std::shared_ptr<SStmt>> SParser::parseTokens(std::vector<std::shared_ptr<SToken>> tokens)
{
	current = 0;
	this->tokens = tokens;
	std::vector<std::shared_ptr<SStmt>> statements;
	while (!isAtEnd())
	{
		std::shared_ptr<SStmt> statement = declaration();
		if (statement != nullptr)
		{
			statements.push_back(statement);
		}
	}
	return statements;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// styleParser Statement Eval Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

//
std::shared_ptr<SStmt> SParser::declaration()
{
	// scriptStmt
	if (match({ STokenType::SCRIPT_TAG }))
	{
		return scriptStmt();
	}
	// styleStmt
	else if (check({ STokenType::POUND, STokenType::DOT, STokenType::WILDCARD, STokenType::IDENTIFIER }))
	{
		return styleStmt();
	}
	//
	error(peek(), "[parser:0101] Invalid statement.");
	synchronize();
	return nullptr;
}

//
std::shared_ptr<SStmt> SParser::styleStmt()
{
	std::vector<std::vector<std::shared_ptr<SToken>>> scopeStacks;
	std::vector<std::shared_ptr<SToken>> tempStack;
	if (peek().get()->type == STokenType::IDENTIFIER && scry(1).get()->type == STokenType::SCOPE)
	{
		while (peek().get()->type == STokenType::IDENTIFIER && scry(1).get()->type == STokenType::SCOPE)
		{
			advance();
			tempStack.push_back(previous());
			advance();
		}
	}
	scopeStacks.push_back(tempStack);
	std::vector<std::shared_ptr<SToken>> selectors;
	if (match({ STokenType::POUND, STokenType::DOT, STokenType::WILDCARD }))
	{
		selectors.push_back(previous());
	}
	else { selectors.push_back(nullptr); }

	std::vector<std::shared_ptr<SToken>> identifiers;

	identifiers.push_back(
		consume(STokenType::IDENTIFIER, "[parser:0201] Expected identifier before properties block."));

	size_t count = 0;
	while (match({ STokenType::AND }))
	{
		count++;
		tempStack.clear();
		if (peek().get()->type == STokenType::IDENTIFIER && scry(1).get()->type == STokenType::SCOPE)
		{
			while (peek().get()->type == STokenType::IDENTIFIER && scry(1).get()->type == STokenType::SCOPE)
			{
				advance();
				tempStack.push_back(previous());
				advance();
			}
		}
		scopeStacks.push_back(tempStack);
		if (match({ STokenType::POUND, STokenType::DOT, STokenType::WILDCARD }))
		{
			selectors.push_back(previous());
		}
		else { selectors.push_back(nullptr); }

		if (check({ STokenType::IDENTIFIER }))
		{
			advance();
			identifiers.push_back(previous());
		}
		else
		{
			identifiers.push_back(nullptr);
			error(advance(), "[parser:0202] Expected identifier after '&' in style inheritance list.");
		}
	}

	std::vector<std::shared_ptr<SExpr>> properties;

	std::shared_ptr<SToken> openBrace = 
		consume(STokenType::CURLY_LEFT, "[parser:0203] Expected opening brace '{' after style declaration.");

	while (!check({ STokenType::CURLY_RIGHT }))
	{
		std::shared_ptr<SExpr> expr = styleProperty();
		if (expr != nullptr)
		{
			properties.push_back(expr);
		}
	}
	consume(STokenType::CURLY_RIGHT, "[parser:0204] Expected closing brace '}' after style declaration.");

	return std::make_shared<SStmt_Style>(
		openBrace.get()->line, 
		selectors,
		identifiers,
		properties,
		scopeStacks);
}

//
std::shared_ptr<SStmt> SParser::scriptStmt()
{
	int line = previous().get()->line;
	std::shared_ptr<SToken> script = consume(STokenType::SCRIPT,
		"[parser:0301] Expected $script$ following 'script' expression.");
	return std::make_shared<SStmt_Script>(line, script);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// styleParser Expression Eval Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

//
std::shared_ptr<SExpr> SParser::styleProperty()
{
	if (isValidPropertyType(peek()))
	{
		std::shared_ptr<SToken> propertyName = advance();

		consume(STokenType::COLON, "[parser:0401] Expected ':' after property declaration.");

		std::vector<std::shared_ptr<SToken>> values;
		
		while (!check({ STokenType::SEMICOLON }))
		{
			if (isValidValueType(peek()))
			{
				values.push_back(advance());
			}
			else 
			{ 
				error(advance(), "[parser:402] Invalid value type."); 
			}
			if (check({ STokenType::COMMA })) { advance(); }
		}

		consume(STokenType::SEMICOLON, "[parser:0403] Expected ';' after property values.");

		return std::make_shared<SExpr_Property>(propertyName.get()->line, propertyName, values);
	}
	else
	{
		advance();
		error(previous(), "[parser:0404] Expected valid property name.");
		synchronize();
		return nullptr;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// styleParser Utility Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

//
std::shared_ptr<SToken> SParser::advance()
{
	if (!isAtEnd())
	{
		current++;
	}
	return previous();
}

//
std::shared_ptr<SToken> SParser::peek()
{
	return tokens[current];
}

//
std::shared_ptr<SToken> SParser::scry(size_t depth)
{
	if (tokens.size() > current + depth) { return tokens[current + depth]; }
	return nullptr;
}

//
std::shared_ptr<SToken> SParser::consume(STokenType type, std::string message)
{
	if (check({ type }))
	{
		return advance();
	}

	error(peek(), message);
	synchronize();
	return tokens[current];
}

//
std::shared_ptr<SToken> SParser::previous()
{
	return tokens[current - 1];
}

//
bool SParser::match(std::vector<STokenType> types)
{
	for (STokenType type : types)
	{
		if (check({ type }))
		{
			advance();
			return true;
		}
	}
	return false;
}

//
bool SParser::check(std::vector<STokenType> types)
{
	if (isAtEnd())
	{
		return false;
	}
	for (STokenType type : types)
	{
		if (peek().get()->type == type)
		{
			return true;
		}
	}
	return false;
}

//
bool SParser::isValidPropertyType(std::shared_ptr<SToken> token)
{
	switch (token.get()->type)
	{
	case STokenType::BOUNDS:
	case STokenType::WIDTH:
	case STokenType::HEIGHT:
	case STokenType::MIN_WIDTH:
	case STokenType::MIN_HEIGHT:
	case STokenType::MAX_WIDTH:
	case STokenType::MAX_HEIGHT:
	case STokenType::POSITION:
	case STokenType::LEFT: 
	case STokenType::RIGHT: 
	case STokenType::TOP: 
	case STokenType::BOTTOM:
	case STokenType::ANCHOR: 
	case STokenType::FLOAT: 
	case STokenType::VISIBILITY:
	case STokenType::$OVERFLOW: 
	case STokenType::OVERFLOW_X: 
	case STokenType::OVERFLOW_Y:
	case STokenType::CLIPPING: 
	case STokenType::DIMENSIONS:
	case STokenType::INNER_RADIUS:
	case STokenType::OUTER_RADIUS:
	case STokenType::VERTEX_COLOR:
	case STokenType::VERTEX_COLOR_RED:
	case STokenType::VERTEX_COLOR_GREEN:
	case STokenType::VERTEX_COLOR_BLUE:
	case STokenType::VERTEX_COLOR_ALPHA:
	case STokenType::FILL_COLOR:
	case STokenType::FILL_COLOR_RED:
	case STokenType::FILL_COLOR_GREEN:
	case STokenType::FILL_COLOR_BLUE:
	case STokenType::FILL_COLOR_ALPHA:
	case STokenType::BACKGROUND_COLOR:
	case STokenType::BACKGROUND_COLOR_RED:
	case STokenType::BACKGROUND_COLOR_GREEN:
	case STokenType::BACKGROUND_COLOR_BLUE:
	case STokenType::BACKGROUND_COLOR_ALPHA:
	case STokenType::BACKGROUND_TEXTURE:
	case STokenType::BOX_SHADOW_COLOR:
	case STokenType::BOX_SHADOW_COLOR_RED:
	case STokenType::BOX_SHADOW_COLOR_GREEN:
	case STokenType::BOX_SHADOW_COLOR_BLUE:
	case STokenType::BOX_SHADOW_COLOR_ALPHA:
	case STokenType::BOX_SHADOW_SIZE:
	case STokenType::BOX_SHADOW_SIZE_X:
	case STokenType::BOX_SHADOW_SIZE_Y:
	case STokenType::BORDER: 
	case STokenType::BORDER_TOP: 
	case STokenType::BORDER_BOTTOM: 
	case STokenType::BORDER_LEFT: 
	case STokenType::BORDER_RIGHT:
	case STokenType::MARGIN: 
	case STokenType::MARGIN_TOP: 
	case STokenType::MARGIN_BOTTOM: 
	case STokenType::MARGIN_LEFT: 
	case STokenType::MARGIN_RIGHT:
	case STokenType::FONT_PATH: 
	case STokenType::FONT_SIZE:
		return true;
		break;
	default:
		return false;
		break;
	}
}
bool SParser::isValidValueType(std::shared_ptr<SToken> token)
{
	switch (token.get()->type)
	{
	case STokenType::NIL:
	case STokenType::SCRIPT:
	case STokenType::IDENTIFIER:
	case STokenType::STRING:
	case STokenType::BOOL:
	case STokenType::NUM:
	case STokenType::INHERIT:
	case STokenType::FILL:
	case STokenType::MIN:
	case STokenType::MAX:
	case STokenType::NOREPORT:
	case STokenType::_ABSOLUTE:
	case STokenType::_RELATIVE:
	case STokenType::FIXED:
	case STokenType::UPPER_LEFT:
	case STokenType::UPPER_CENTER:
	case STokenType::UPPER_RIGHT:
	case STokenType::CENTER_LEFT:
	case STokenType::CENTER:
	case STokenType::CENTER_RIGHT:
	case STokenType::LOWER_LEFT:
	case STokenType::LOWER_CENTER:
	case STokenType::LOWER_RIGHT:
	case STokenType::VISIBILE:
	case STokenType::SHOW:
	case STokenType::HIDDEN:
	case STokenType::HIDE:
	case STokenType::SCROLL:
	case STokenType::AUTO:
	case STokenType::LEFT:
	case STokenType::RIGHT:
		return true;
		break;
	default:
		return false;
		break;
	}
}

//
bool SParser::isAtEnd()
{
	return peek().get()->type == STokenType::END;
}

//
void SParser::synchronize()
{
	advance();
	if (previous().get()->type == STokenType::SEMICOLON) return;

	while (!isAtEnd())
	{
		if (isValidPropertyType(peek())) return;

		switch (peek().get()->type)
		{
		case STokenType::CURLY_RIGHT:
			return;
		case STokenType::SEMICOLON:
			advance();
			return;
		}

		advance();
	}
	return;
}

//
void SParser::error(std::shared_ptr<SToken> token, std::string message)
{
	console.get()->error(token, message);
}