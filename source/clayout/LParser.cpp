#include "../include/clayout/LParser.h"
#include "../include/clayout/LEnums.h"
#include "../include/clayout/LToken.h"
#include "../include/clayout/LProc.h"

#include <vector>
#include <string>
#include <memory>
#include <cctype>
#include <typeinfo>

LParser::LParser()
{

}

void LParser::initialize(std::shared_ptr<ChromaLayout> console)
{
	this->console = console;
}

// 
std::vector<std::shared_ptr<LStmt>> LParser::parseTokens(std::vector<std::shared_ptr<LToken>> tokens)
{
	current = 0;
	this->tokens = tokens;
	std::vector<std::shared_ptr<LStmt>> statements;
	while (!isAtEnd())
	{
		std::shared_ptr<LStmt> statement = declaration();
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
std::shared_ptr<LStmt> LParser::declaration()
{
	// Reject anything not enclosed in <ROOT> or <PROTO>
	while (!isAtEnd() && !check({ LTokenType::ANGLE_LEFT }) && !check(scry(1), { LTokenType::ROOT, LTokenType::PROTO }))
	{
		advance();
	}
	consume(LTokenType::ANGLE_LEFT, "[parser:0101] Expect opening '<' before widget type.");
	if (check({ LTokenType::ROOT }) == true)
	{
		return rootStmt();
	}
	else if (check({ LTokenType::PROTO }))
	{
		return prototypeStmt();
	}
	else
	{
		return nullptr;
	}
}

//
std::shared_ptr<LStmt> LParser::rootStmt()
{
	if (openTags.size() != 0) { openTags.clear(); }
	openTags.push_back(advance());
	int line = openTags.back().get()->line;
	std::shared_ptr<LToken> type = previous();

	std::vector<std::shared_ptr<LExpr>> attributes;
	std::vector<std::shared_ptr<LExpr>> childElements;

	// Add attributes
	while (!isAtEnd() && !check({ LTokenType::ANGLE_RIGHT }))
	{
		std::shared_ptr<LExpr> attrib = rootAttrib();
		if (attrib != nullptr)
		{
			attributes.push_back(attrib);
		}
	}
	consume(LTokenType::ANGLE_RIGHT, "[parser:0201] Expect closing '>' after opening 'ROOT' tag.");

	// Add child widgets
	while (!isAtEnd() &&
			!(check({ LTokenType::ANGLE_LEFT }) &&
			check(scry(1), { LTokenType::CLOSE }) &&
			check(scry(2), { openTags.back().get()->type })))
	{
		if (!isAtEnd() &&
			(check({ LTokenType::ANGLE_LEFT }) &&
				isValidElementType(scry(1))))
		{
			advance();
			std::shared_ptr<LExpr> child = element();
			if (child != nullptr)
			{
				childElements.push_back(child);
			}
		}
		else { break; }
	}
	// Close the tag
	if (!isAtEnd() &&
		(check({ LTokenType::ANGLE_LEFT }) &&
			check(scry(1), { LTokenType::CLOSE }) &&
			check(scry(2), { openTags.back().get()->type })))
	{
		advance(); advance();
		consume(openTags.back().get()->type, "[parser:0202] Expect matching closing tag.");
		consume(LTokenType::ANGLE_RIGHT, "[parser:0203] Expect closing '>' after closing tag name.");
		openTags.pop_back();
	}

	if (openTags.size() != 0)
	{
		console.get()->error(previous(), "[parser:0204] Did not find closing tag for element of name '" + openTags.back().get()->lexeme + "'.");
	}
	openTags.clear();
	return std::make_shared<LStmt_Root>(line, type, childElements, attributes);
}

//
std::shared_ptr<LStmt> LParser::prototypeStmt()
{
	if (openTags.size() != 0) { openTags.clear(); }
	openTags.push_back(advance());
 	int line = openTags.back().get()->line;
	std::shared_ptr<LToken> type = previous();

	std::vector<std::shared_ptr<LExpr>> attributes;
	std::vector<std::shared_ptr<LExpr>> childElements;

	// Add attributes
	while (!isAtEnd() && !check({ LTokenType::ANGLE_RIGHT }))
	{
		std::shared_ptr<LExpr> attrib = rootAttrib();
		if (attrib != nullptr)
		{
			attributes.push_back(attrib);
		}
	}
	consume(LTokenType::ANGLE_RIGHT, "[parser:0301] Expect closing '>' after opening 'PROTOTYPE' tag.");

	// Add child widgets
	while (!isAtEnd() &&
		!(check({ LTokenType::ANGLE_LEFT }) &&
			check(scry(1), { LTokenType::CLOSE }) &&
			check(scry(2), { openTags.back().get()->type })))
	{
		if (!isAtEnd() &&
			(check({ LTokenType::ANGLE_LEFT }) &&
				isValidElementType(scry(1))))
		{
			advance();
			std::shared_ptr<LExpr> child = element();
			if (child != nullptr)
			{
				childElements.push_back(child);
			}
		}
		else { break; }
	}
	// Close the tag
	if (!isAtEnd() &&
		(check({ LTokenType::ANGLE_LEFT }) &&
			check(scry(1), { LTokenType::CLOSE }) &&
			check(scry(2), { openTags.back().get()->type })))
	{
		advance(); advance();
		consume(openTags.back().get()->type, "[parser:0302] Expect matching closing tag.");
		consume(LTokenType::ANGLE_RIGHT, "[parser:0303] Expect closing '>' after closing tag name.");
		openTags.pop_back();
	}

	if (openTags.size() != 0)
	{
		console.get()->error(previous(), "[parser:0303] Did not find closing tag for element of name '" + openTags.back().get()->lexeme + "'.");
	}
	openTags.clear();
	return std::make_shared<LStmt_Proto>(line, type, childElements, attributes);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// styleParser Expression Eval Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

//
std::shared_ptr<LExpr> LParser::element()
{
	size_t tagsSize = openTags.size();
	openTags.push_back(advance());
	int line = openTags.back().get()->line;
	std::shared_ptr<LToken> type = previous();

	std::vector<std::shared_ptr<LExpr>> attributes;
	std::vector<std::shared_ptr<LExpr>> childElements;

	// Add attributes
	while (!isAtEnd() && !check({ LTokenType::ANGLE_RIGHT }))
	{
		std::shared_ptr<LExpr> attrib = elementAttrib();
		if (attrib != nullptr)
		{
			attributes.push_back(attrib);
		}
	}
	consume(LTokenType::ANGLE_RIGHT, "[parser:0401] Expect closing '>' after opening 'ROOT' tag.");

	// Add child widgets
	while (!isAtEnd() &&
		!(check({ LTokenType::ANGLE_LEFT }) &&
			check(scry(1), { LTokenType::CLOSE }) &&
			check(scry(2), { openTags.back().get()->type })))
	{
		if (!isAtEnd() &&
			(check({ LTokenType::ANGLE_LEFT }) &&
				isValidElementType(scry(1))))
		{
			advance();
			std::shared_ptr<LExpr> child = element();
			if (child != nullptr)
			{
				childElements.push_back(child);
			}
		}
		else { break; }
	}

	// Close the tag
	if (!isAtEnd() &&
		(check({ LTokenType::ANGLE_LEFT }) &&
			check(scry(1), { LTokenType::CLOSE }) &&
			check(scry(2), { openTags.back().get()->type })))
	{
		advance(); advance();
		consume(openTags.back().get()->type, "[parser:0402] Expect matching closing tag.");
		consume(LTokenType::ANGLE_RIGHT, "[parser:0403] Expect closing '>' after closing tag name.");
		openTags.pop_back();
	}

	if (openTags.size() != tagsSize)
	{
		console.get()->error(previous(), "[parser:0403] Did not find closing tag for element of name '" + openTags.back().get()->lexeme + "'.");
	}
	return std::make_shared<LExpr_Element>(line, type, childElements, attributes);
}

//
std::shared_ptr<LExpr> LParser::rootAttrib()
{
	if (isValidRootAttribType(peek()))
	{
		std::shared_ptr<LToken> attribName = advance();
		consume(LTokenType::EQUAL, "[parser:0501] Expect '=' after a valid root attribute name.");
		if (isValidValueType(peek()))
		{
			std::shared_ptr<LToken> attribValue = advance();
			return std::make_shared<LExpr_RootAttrib>(attribName.get()->line, attribName, attribValue);
		}
	}
	else
	{
		advance();
		console.get()->error("[parser:0502] Attribute '" + previous().get()->typeString() + "' is not a valid root attribute name.");
		synchronize();
	}
	return nullptr;
}

//
std::shared_ptr<LExpr> LParser::elementAttrib()
{
	if (isValidEleAttribType(peek()))
	{
		std::shared_ptr<LToken> attribName = advance();
		consume(LTokenType::EQUAL, "[parser::0601] Expect '=' after a valid root attribute name.");
		if (isValidValueType(peek()))
		{
			std::shared_ptr<LToken> attribValue = advance();
			return std::make_shared<LExpr_ElementAttrib>(attribName.get()->line, attribName, attribValue);
		}
	}
	else
	{
		advance();
		console.get()->error("[parser:0602] Attribute '" + previous().get()->typeString() + "' is not a valid element attribute name.");
		synchronize();
	}
	return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// styleParser Utility Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

//
std::shared_ptr<LToken> LParser::advance()
{
	if (!isAtEnd())
	{
		current++;
	}
	return previous();
}

//
std::shared_ptr<LToken> LParser::peek()
{
	return tokens[current];
}

//
std::shared_ptr<LToken> LParser::scry(size_t depth)
{
	if (tokens.size() > current + depth) { return tokens[current + depth]; }
	return nullptr;
}

//
std::shared_ptr<LToken> LParser::consume(LTokenType type, std::string message)
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
std::shared_ptr<LToken> LParser::previous()
{
	return tokens[current - 1];
}

//
bool LParser::match(std::vector<LTokenType> types)
{
	for (LTokenType type : types)
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
bool LParser::check(std::vector<LTokenType> types)
{
	if (isAtEnd())
	{
		return false;
	}
	for (LTokenType type : types)
	{
		if (peek().get()->type == type)
		{
			return true;
		}
	}
	return false;
}
//
bool LParser::check(std::shared_ptr<LToken> query, std::vector<LTokenType> types)
{
	if (isAtEnd())
	{
		return false;
	}
	for (LTokenType type : types)
	{
		if (query.get()->type == type)
		{
			return true;
		}
	}
	return false;
}
//
bool LParser::isValidElementType(std::shared_ptr<LToken> token)
{
	switch (token.get()->type)
	{
	case LTokenType::H_BOX:
	case LTokenType::V_BOX:
	case LTokenType::TILE_BOX:
	case LTokenType::COLOR_BOX:
	case LTokenType::HUE_STRIP:
	case LTokenType::HUE_RADIUS:
	case LTokenType::GRADIENT_BOX:
	case LTokenType::LIST:
	case LTokenType::LIST_ITEM:
	case LTokenType::PANEL:
	case LTokenType::IMAGE:
	case LTokenType::TEXT:
	case LTokenType::LINE:
	case LTokenType::BLOCK:
	case LTokenType::PARAGRAPH:
	case LTokenType::TEXT_ENTRY:
		return true;
		break;
	default:
		return false;
		break;
	}
}


//
bool LParser::isValidRootAttribType(std::shared_ptr<LToken> token)
{
	switch (token.get()->type)
	{
	case LTokenType::ID:
	case LTokenType::SCRIPT:
	case LTokenType::DEFAULT_POSITION:
		return true;
		break;
	default:
		return false;
		break;
	}
}

//
bool LParser::isValidEleAttribType(std::shared_ptr<LToken> token)
{
	switch (token.get()->type)
	{
	case LTokenType::ID:
	case LTokenType::CLASS:
	case LTokenType::NAME:
	case LTokenType::STYLE:
	case LTokenType::VALUE:
	case LTokenType::IMG:
	case LTokenType::GROUP:
	case LTokenType::INNER_CONTENT:
	case LTokenType::ON_BLUR:
	case LTokenType::ON_FOCUS:
	case LTokenType::ON_CLICK:
	case LTokenType::ON_RIGGHTCLICK:
	case LTokenType::ON_DBL_CLICK:
	case LTokenType::ON_DBL_RIGHTCLICK:
	case LTokenType::ON_MIDDLECLICK:
	case LTokenType::ON_DBL_MIDDLECLICK:
	case LTokenType::ON_DRAG:
	case LTokenType::ON_DRAG_START:
	case LTokenType::ON_DRAG_END:
	case LTokenType::ON_DRAG_ENTER:
	case LTokenType::ON_DRAG_OVER:
	case LTokenType::ON_DRAG_LEAVE:
	case LTokenType::ON_DROP:
	case LTokenType::ON_KEY_DOWN:
	case LTokenType::ON_KEY_PRESS:
	case LTokenType::ON_KEY_UP:
	case LTokenType::ON_LOAD:
	case LTokenType::ON_COPY:
	case LTokenType::ON_PASTE:
	case LTokenType::ON_CUT:
	case LTokenType::ON_MOUSE_OVER:
	case LTokenType::ON_MOUSE_ENTER:
	case LTokenType::ON_MOUSE_LEAVE:
	case LTokenType::ON_MOUSE_WHEELUP:
	case LTokenType::ON_MOUSE_WHEELDOWN:
	case LTokenType::ON_RELEASE:
	case LTokenType::ON_RESIZE:
	case LTokenType::ON_CANCEL:
	case LTokenType::ON_ENTRY:
	case LTokenType::DRAGGABLE:
	case LTokenType::DROPPABLE:
	case LTokenType::DRAG_TYPE:
	case LTokenType::DROP_TYPE:
		return true;
		break;
	default:
		return false;
		break;
	}
}
bool LParser::isValidValueType(std::shared_ptr<LToken> token)
{
	switch (token.get()->type)
	{
	case LTokenType::STRING:
		return true;
		break;
	default:
		return false;
		break;
	}
}

//
bool LParser::isAtEnd()
{
	return peek().get()->type == LTokenType::END;
}

//
void LParser::synchronize()
{
	if (peek().get()->type == LTokenType::ANGLE_RIGHT) return;

	advance();

	while (!isAtEnd())
	{
		if (isValidEleAttribType(peek())) return;
		if (isValidRootAttribType(peek())) return;

		switch (peek().get()->type)
		{
		case LTokenType::ANGLE_LEFT:
			return;
		case LTokenType::ANGLE_RIGHT:
			return;
		case LTokenType::CLOSE:
			return;
		}

		advance();
	}
	return;
}

//
void LParser::error(std::shared_ptr<LToken> token, std::string message)
{
	console.get()->error(token, message);
}