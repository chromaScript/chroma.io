#include "../include/cscript/CParser.h"
#include "../include/cscript/CEnums.h"
#include "../include/cscript/CToken.h"
#include "../include/cscript/CExpr.h"
#include "../include/cscript/CStmt.h"

#include <vector>
#include <string>
#include <memory>
#include <cctype>
#include <typeinfo>

CParser::CParser()
{

}

void CParser::initialize(std::shared_ptr<ChromaScript> console)
{
	this->console = console;
}

// 
std::vector<std::shared_ptr<CStmt>> CParser::parseTokens(std::vector<std::shared_ptr<CToken>> tokens)
{
	current = 0;
	this->tokens = tokens;
	std::vector<std::shared_ptr<CStmt>> statements;
	while (!isAtEnd())
	{
		statements.push_back(declarationStmt());
	}
	return statements;
}
std::vector<std::shared_ptr<CStmt>> CParser::parseTokensExpr(std::vector<std::shared_ptr<CToken>> tokens)
{
	current = 0;
	this->tokens = tokens;
	std::vector<std::shared_ptr<CStmt>> statements;
	while (!isAtEnd())
	{
		statements.push_back(expressionStmt(true));
	}
	return statements;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// chromaParser Statement Eval Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<CStmt> CParser::declarationStmt()
{
	if (match({ CTokenType::CLASS }))
	{
		return classDeclarationStmt();
	}
	if (match({ CTokenType::NAMESPACE }))
	{
		return namespaceDeclarationStmt();
	}
	// Declaration of functions or variables must always have a minimum of two type/identifier tokens
	if (isValidVarType(peek()) && isValidVarType(scry(1)))
	{
		// Build the return type
		std::shared_ptr<CToken> typeSpecifier = createTypeSpecifier(peek());
		// Check/build for scope stack
		std::vector<std::shared_ptr<CToken>> scopeStack;
		if (peek().get()->type == CTokenType::IDENTIFIER && scry(1).get()->type == CTokenType::SCOPE)
		{
			while (peek().get()->type == CTokenType::IDENTIFIER && scry(1).get()->type == CTokenType::SCOPE)
			{
				advance();
				scopeStack.push_back(previous());
				advance();
			}
		}
		if (peek().get()->type == CTokenType::IDENTIFIER && scry(1).get()->type == CTokenType::PAREN_LEFT)
		{
			return funcDeclarationStmt(typeSpecifier, scopeStack, "function");
		}
		else if (peek().get()->type == CTokenType::IDENTIFIER)
		{
			if (isArrayType(typeSpecifier))
			{
				return arrayDeclarationStmt(typeSpecifier);
			}
			else
			{
				return varDeclarationStmt(typeSpecifier);
			}
		}
		else
		{
			error(peek(), "[parser:0102] Expected identifier after type declaration.");
			synchronize();
		}
	}
	if (peek().get()->type == CTokenType::IDENTIFIER && scry(1).get()->type == CTokenType::SCOPE)
	{
		size_t last = current;
		std::vector<std::shared_ptr<CToken>> scopeStack;
		while (peek().get()->type == CTokenType::IDENTIFIER && scry(1).get()->type == CTokenType::SCOPE)
		{
			advance();
			scopeStack.push_back(previous());
			advance();
		}
		if (match({ CTokenType::CAST }))
		{
			consume(CTokenType::LESS, "[parser:0103] Expect opening angle bracket '<' after 'cast' declaration.");
			std::shared_ptr<CToken> typeSpecifier = createTypeSpecifier(peek());
			consume(CTokenType::MORE, "[parser:0104] Expect closing angle bracket '>' after type name in 'cast' declaration.");
			consume(CTokenType::CURLY_LEFT, "[parser:0105] Must define 'cast' function when defined outside class statement.");
			std::shared_ptr<CStmt> body = block();
			return std::make_shared<CStmt_Function>(
				typeSpecifier,
				std::make_shared<CToken>(CTokenType::CAST, typeSpecifier.get()->typeString(), previous().get()->line),
				false, scopeStack, body);
		}
		else
		{
			current = last;
		}
	}
	return statement();
}

std::shared_ptr<CStmt> CParser::classDeclarationStmt()
{
	std::vector<std::shared_ptr<CToken>> scopeStack;
	while (peek().get()->type == CTokenType::IDENTIFIER && scry(1).get()->type == CTokenType::SCOPE)
	{
		advance();
		scopeStack.push_back(previous());
		advance();
	}
	std::shared_ptr<CToken> name = consume(CTokenType::IDENTIFIER, "[parser:0201] Expect class name.");
	std::shared_ptr<CToken> superclass = nullptr;
	if (match({ CTokenType::INHERIT }))
	{
		superclass = consume(CTokenType::IDENTIFIER, "[parser:0202] Expect superclass name.");
	}

	std::shared_ptr<CStmt> constructor = nullptr;
	std::vector<std::shared_ptr<CStmt>> methods;
	std::vector<std::shared_ptr<CStmt>> properties;
	std::vector<std::shared_ptr<CToken>> emptyStack;
	// Check if class declaration is only a forward declaration
	if (match({ CTokenType::SEMICOLON }))
	{
		 return std::make_shared<CStmt_Class>(scopeStack, name, superclass, true, nullptr, methods, properties);
	}

	consume(CTokenType::CURLY_LEFT, "[parser:0203] Expect '{' before class body.");

	bool hasConstructor = false;
	while (!check(CTokenType::CURLY_RIGHT) && !isAtEnd())
	{
		// Build constructor function
		if (hasConstructor == false && 
			peek().get()->type == CTokenType::IDENTIFIER && 
			peek().get()->lexeme == name.get()->lexeme &&
			scry(1).get()->type == CTokenType::PAREN_LEFT)
		{
			consume(CTokenType::IDENTIFIER, "[parser:0204] Expect constructor name to match class name.");
			consume(CTokenType::PAREN_LEFT, "[parser:0205] Expect '(' after constructor name.");
			std::vector<std::shared_ptr<CToken>> paramsTypes;
			std::vector<std::string> paramsNames;
			if (!check(CTokenType::PAREN_RIGHT))
			{
				do
				{
					if (paramsTypes.size() >= 64)
					{
						error(peek(), "[parser:0206] Can't have more than 64 parameters.");
						while (peek().get()->type != CTokenType::PAREN_RIGHT)
						{
							advance();
						}
						break;
					}
					if (isValidVarType(peek()))
					{
						paramsTypes.push_back(createTypeSpecifier(peek()));
						paramsNames.push_back(consume(CTokenType::IDENTIFIER, "[parser:0207] Expect parameter name.").get()->lexeme);
					}
				} while (match({ CTokenType::COMMA }));
			}
			consume(CTokenType::PAREN_RIGHT, "[parser:0208] Expect ')' after parameters.");
			if (match({ CTokenType::SEMICOLON }))
			{
				constructor = std::make_shared<CStmt_Function>(
					name,
					std::make_shared<CToken>(CTokenType::CONSTRUCTOR, name.get()->lexeme, name.get()->line),
					true, emptyStack, paramsTypes, paramsNames, nullptr);
			}
			else
			{
				consume(CTokenType::CURLY_LEFT, "[parser:0209] Expect opening bracket '{' after 'constructor' statement.");
				std::shared_ptr<CStmt> body = block();
				constructor = std::make_shared<CStmt_Function>(
					name,
					std::make_shared<CToken>(CTokenType::CONSTRUCTOR, name.get()->lexeme, name.get()->line),
					false, emptyStack, paramsTypes, paramsNames, body);
			}
			hasConstructor = true;
		}
		// Reject multiple constructors
		else if (peek().get()->type == CTokenType::IDENTIFIER &&
			peek().get()->lexeme == name.get()->lexeme &&
			scry(1).get()->type == CTokenType::PAREN_LEFT)
		{
			consume(CTokenType::IDENTIFIER, "[parser:0210] Expect constructor name to match class name.");
			error(previous(), "[parser:0211] Class definition cannot have more than one constructor.");
			synchronize();
		}
		// Cast function builder
		if (match({ CTokenType::CAST }))
		{
			consume(CTokenType::LESS, "[parser:0212] Expect opening angle bracket '<' after 'cast' declaration.");
			std::shared_ptr<CToken> typeSpecifier = createTypeSpecifier(peek());
			consume(CTokenType::MORE, "[parser:0213] Expect closing angle bracket '>' after type name in 'cast' declaration.");
			if (match({ CTokenType::SEMICOLON }))
			{
				methods.push_back(std::make_shared<CStmt_Function>(
					typeSpecifier,
					std::make_shared<CToken>(CTokenType::CAST, typeSpecifier.get()->typeString(), name.get()->line),
					true, emptyStack, nullptr));
			}
			else
			{
				consume(CTokenType::CURLY_LEFT, "[parser:0214] Expect opening bracket '{' after 'cast' statement.");
				std::shared_ptr<CStmt> body = block();
				methods.push_back(std::make_shared<CStmt_Function>(
					typeSpecifier,
					std::make_shared<CToken>(CTokenType::CAST, typeSpecifier.get()->typeString(), name.get()->line),
					false, emptyStack, body));
			}
		}
		if (isValidVarType(peek()))
		{
			std::shared_ptr<CToken> typeSpecifier = createTypeSpecifier(peek());

			if (peek().get()->type == CTokenType::IDENTIFIER && scry(1).get()->type == CTokenType::PAREN_LEFT)
			{
				methods.push_back(funcDeclarationStmt(typeSpecifier, emptyStack, "method"));
			}
			else if (peek().get()->type == CTokenType::IDENTIFIER)
			{
				if (isArrayType(typeSpecifier))
				{
					properties.push_back(arrayDeclarationStmt(typeSpecifier));
				}
				else
				{
					properties.push_back(varDeclarationStmt(typeSpecifier));
				}
			}
			else
			{
				error(peek(), "[parser:0216] Expected identifier after type declaration.");
				synchronize();
			}
		}
	}
	// Throw warnings for uninitialized variables when the class has no constructor
	if (hasConstructor == false)
	{
		for (std::shared_ptr<CStmt> _property : properties)
		{
			if (dynamic_cast<CStmt_Variable*>(_property.get()) != nullptr)
			{
				if (std::dynamic_pointer_cast<CStmt_Variable>(_property).get()->initializer == nullptr)
				{
					console.get()->warning(std::dynamic_pointer_cast<CStmt_Variable>(_property).get()->name, 
						"[parser:0217] Class '" + name.get()->lexeme + "' has uninitialized member variable '" + 
						std::dynamic_pointer_cast<CStmt_Variable>(_property).get()->name.get()->lexeme +
						"'. Consider giving default initialization to avoid runtime errors.");
				}
			}
		}
	}
	

	consume(CTokenType::CURLY_RIGHT, "[parser:0218] Expect '}' after class body.");

	return std::make_shared<CStmt_Class>(scopeStack, name, superclass, false, constructor, methods, properties);
}
std::shared_ptr<CStmt> CParser::funcDeclarationStmt(
	std::shared_ptr<CToken> typeSpecifier, 
	std::vector<std::shared_ptr<CToken>> scopeStack, 
	std::string kind)
{
	while (peek().get()->type == CTokenType::IDENTIFIER && scry(1).get()->type == CTokenType::SCOPE)
	{
		advance();
		scopeStack.push_back(previous());
		advance();
	}
	std::shared_ptr<CToken> name = nullptr;
	if (kind != "constructor" && kind != "cast")
	{
		name = consume(CTokenType::IDENTIFIER, "Expect " + kind + " name.");
	}
	else if (kind == "constructor")
	{
		name = std::make_shared<CToken>(CTokenType::CONSTRUCTOR, typeSpecifier.get()->line);
	}
	consume(CTokenType::PAREN_LEFT, "[parser:0301] Expect '(' after " + kind + " name.");
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	std::vector<std::string> paramsNames;
	if (!check(CTokenType::PAREN_RIGHT))
	{
		do
		{
			if (paramsTypes.size() >= 64)
			{
				error(peek(), "[parser:0302] Can't have more than 64 parameters.");
				while (peek().get()->type != CTokenType::PAREN_RIGHT)
				{
					advance();
				}
				break;
			}
			if (isValidVarType(peek()))
			{
				paramsTypes.push_back(createTypeSpecifier(peek()));
				paramsNames.push_back(consume(CTokenType::IDENTIFIER, "[parser:0303] Expect parameter name.").get()->lexeme);
			}
		} while (match({ CTokenType::COMMA }));
	}
	consume(CTokenType::PAREN_RIGHT, "[parser:0304] Expect ')' after parameters.");
	if (match({ CTokenType::SEMICOLON }))
	{
		return std::make_shared<CStmt_Function>(typeSpecifier, name, true, scopeStack, paramsTypes, paramsNames, nullptr);
	}
	consume(CTokenType::CURLY_LEFT, "[parser:0305] Expect '{' before " + kind + " body.");
	std::shared_ptr<CStmt> body = block();
	return std::make_shared<CStmt_Function>(typeSpecifier, name, false, scopeStack, paramsTypes, paramsNames, body);
}

std::shared_ptr<CStmt> CParser::arrayDeclarationStmt(std::shared_ptr<CToken> typeSpecifier)
{
	std::shared_ptr<CToken> name = consume(CTokenType::IDENTIFIER, "[parser:0401] Expect variable name.");
	std::shared_ptr<CExpr> initializer;
	if (match({ CTokenType::ASSIGN }))
	{
		initializer = expression();
	}
	consume(CTokenType::SEMICOLON, "[parser:0402] Expect ';' after variable declaration.");
	return std::make_shared<CStmt_Variable>(typeSpecifier, name, initializer);
}

std::shared_ptr<CStmt> CParser::varDeclarationStmt(std::shared_ptr<CToken> typeSpecifier)
{
	std::shared_ptr<CToken> name = consume(CTokenType::IDENTIFIER, "[parser:0501] Expect variable name.");
	std::shared_ptr<CExpr> initializer = nullptr;
	if (match({ CTokenType::ASSIGN }))
	{
		initializer = expression();
	}
	consume(CTokenType::SEMICOLON, "[parser:0502] Expect ';' after variable declaration.");
	return std::make_shared<CStmt_Variable>(typeSpecifier, name, initializer);
}

std::shared_ptr<CStmt> CParser::namespaceDeclarationStmt()
{
	std::shared_ptr<CToken> name = consume(CTokenType::IDENTIFIER, "[parser:0601] Expected a identifier name after namespace declaration.");
	if (match({ CTokenType::CURLY_LEFT }))
	{
		std::shared_ptr<CStmt> body = block();
		return std::make_shared<CStmt_Namespace>(name, body);
	}
	else
	{
		return nullptr;
	}
}

std::shared_ptr<CStmt> CParser::statement()
{
	if (match({ CTokenType::PRINT })) { return printStmt(); }
	if (match({ CTokenType::RETURN })) { return returnStmt(); }
	if (match({ CTokenType::BREAK })) { return breakStmt(); }
	if (match({ CTokenType::CONTINUE })) { return continueStmt(); }
	if (match({ CTokenType::TRY })) { return tryCatchStmt(); }
	if (match({ CTokenType::IF })) { return ifStmt(); }
	if (match({ CTokenType::WHILE })) { return whileStmt(); }
	if (match({ CTokenType::FOR })) { return forStmt(); }
	if (match({ CTokenType::FOREACH })) { return foreachStmt(); }
	if (match({ CTokenType::CURLY_LEFT })) { return block(); }
	return expressionStmt(false);
}

std::shared_ptr<CStmt> CParser::printStmt()
{
	std::shared_ptr<CToken> line = previous();
	std::shared_ptr<CExpr> value = expression();
	consume(CTokenType::SEMICOLON, "[parser:0701] Expect ';' after value.");
	return std::make_shared<CStmt_Print>(value, line);
}

std::shared_ptr<CStmt> CParser::returnStmt()
{
	std::shared_ptr<CToken> keyword = previous();
	std::shared_ptr<CExpr> value = nullptr;
	if (!check(CTokenType::SEMICOLON))
	{
		value = expression();
	}

	consume(CTokenType::SEMICOLON, "[parser:0801] Expect ';' after return statement.");
	return std::make_shared<CStmt_Return>(keyword, value);
}

std::shared_ptr<CStmt> CParser::breakStmt()
{
	std::shared_ptr<CToken> keyword = previous();
	consume(CTokenType::SEMICOLON, "[parser:0901] Expect ';' after break statement.");
	return std::make_shared<CStmt_Break>(keyword);
}

std::shared_ptr<CStmt> CParser::continueStmt()
{
	std::shared_ptr<CToken> keyword = previous();
	consume(CTokenType::SEMICOLON, "[parser:1001] Expect ';' after break statement.");
	return std::make_shared<CStmt_Continue>(keyword);
}

std::shared_ptr<CStmt> CParser::tryCatchStmt()
{
	std::shared_ptr<CToken> line = previous();
	consume(CTokenType::CURLY_LEFT, "[parser:1101] Expected opening bracket '{' after try statement.");
	std::shared_ptr<CStmt> tryBlock = block();
	consume(CTokenType::CATCH, "[parser:1102] Expected catch statement after try block.");
	consume(CTokenType::CURLY_LEFT, "[parser:1103] Expected opening bracket '{' after catch statement.");
	std::shared_ptr<CStmt> catchBlock = block();
	return std::make_shared<CStmt_TryCatch>(tryBlock, catchBlock, line);
}

std::shared_ptr<CStmt> CParser::ifStmt()
{
	std::shared_ptr<CToken> line = previous();
	consume(CTokenType::PAREN_LEFT, "[parser:1201] Expect opening '(' after 'if'.");
	std::vector<std::shared_ptr<CExpr>> conditionsList; 
	conditionsList.emplace_back(expression());
	consume(CTokenType::PAREN_RIGHT, "[parser:1202] Expect closing ')' after 'if' condition");

	std::vector<std::shared_ptr<CStmt>> executionList;
	executionList.emplace_back(statement());
	std::shared_ptr<CStmt> elseBranch = nullptr;
	while (match({ CTokenType::ELSE }))
	{
		if (peek().get()->type == CTokenType::IF)
		{
			advance();
			consume(CTokenType::PAREN_LEFT, "[parser:1203] Expect opening '(' after 'else if'.");
			conditionsList.emplace_back(expression());
			consume(CTokenType::PAREN_RIGHT, "[parser:1204] Expect closing ')' after 'else if' condition");
			executionList.emplace_back(statement());
		}
		else
		{
			elseBranch = statement();
		}
		
	}

	return std::make_shared<CStmt_If>(conditionsList, executionList, elseBranch, line);
}

std::shared_ptr<CStmt> CParser::whileStmt()
{
	std::shared_ptr<CToken> line = previous();
	consume(CTokenType::PAREN_LEFT, "[parser:1301] Expect opening '(' after 'while'.");
	std::shared_ptr<CExpr> condition = expression();
	consume(CTokenType::PAREN_RIGHT, "[parser:1302] Expect closing ')' after 'while' condition");
	std::shared_ptr<CStmt> body = statement();

	return std::make_shared<CStmt_While>(condition, body, line);
}

std::shared_ptr<CStmt> CParser::forStmt()
{
	std::shared_ptr<CToken> line = previous();
	consume(CTokenType::PAREN_LEFT, "[parser:1401] Expect opening '(' after 'for'.");
	// Establish the initializer statement
	std::shared_ptr<CStmt> initializer;
	if (match({ CTokenType::SEMICOLON }))
	{
		initializer = nullptr;
	}
	else if (isValidVarType(peek()))
	{
		std::shared_ptr<CToken> typeSpecifier = createTypeSpecifier(peek());
		if (isArrayType(typeSpecifier))
		{
			initializer = arrayDeclarationStmt(typeSpecifier);
		}
		else
		{
			initializer = varDeclarationStmt(typeSpecifier);
		}
	}
	else
	{
		initializer = expressionStmt(false);
	}

	// Establish the condition statement
	std::shared_ptr<CExpr> condition = nullptr;
	if (!check(CTokenType::SEMICOLON))
	{
		condition = expression();
	}
	consume(CTokenType::SEMICOLON, "[parser:1402] Expect ';' after for condition expression.");

	// Establish the increment statement
	std::shared_ptr<CExpr> increment = nullptr;
	if (!check(CTokenType::PAREN_RIGHT))
	{
		increment = expression();
	}
	consume(CTokenType::PAREN_RIGHT, "[parser:1403] Expect ')' after for clauses.");

	// Establish the body statement
	std::shared_ptr<CStmt> body = statement();

	return std::make_shared<CStmt_For>(initializer, condition, increment, body, line);
}

std::shared_ptr<CStmt> CParser::foreachStmt()
{
	std::shared_ptr<CToken> line = previous();
	std::shared_ptr<CToken> typeSpecifier = nullptr;
	std::shared_ptr<CToken> iteratorName = nullptr;
	std::shared_ptr<CExpr> listExpression = nullptr;
	std::shared_ptr<CStmt> body = nullptr;
	consume(CTokenType::PAREN_LEFT, "[parser:1501] Expect opening '(' after 'foreach'.");
	if (isValidVarType(peek()))
	{
		typeSpecifier = createTypeSpecifier(peek());
		iteratorName = consume(CTokenType::IDENTIFIER, "[parser:1502] Expected identifier name for iterator after variable type.");
		consume(CTokenType::COLON, "[parser:1503] Expected ':' separator between iterator declaration and container expression.");
		listExpression = expression();
		consume(CTokenType::PAREN_RIGHT, "[parser:1504] Expected closing brace ')' after container expression in foreach statement.");
		body = statement();
	}
	else
	{
		error(peek(), "[parser:1504] For Each loop must contain iterator declaration of valid variable type.");
	}
	return std::make_shared<CStmt_Foreach>(typeSpecifier, iteratorName, listExpression, body, line);
}

std::shared_ptr<CStmt> CParser::block()
{
	std::shared_ptr<CToken> line = previous();
	std::vector<std::shared_ptr<CStmt>> statements;
	while (!check(CTokenType::CURLY_RIGHT) && !isAtEnd())
	{
		statements.push_back(declarationStmt());
	}
	consume(CTokenType::CURLY_RIGHT, "[parser:1601] Expect '}' after declaring block.");
	
	return std::make_shared<CStmt_Block>(statements, line);
}


std::shared_ptr<CStmt> CParser::expressionStmt(bool ignoreSemicolon)
{
	std::shared_ptr<CExpr> expr = expression();
	if (!ignoreSemicolon)
	{
		consume(CTokenType::SEMICOLON, "[parser:1701] Expected ';' after expression.");
	}
	std::shared_ptr<CToken> line = previous();
	std::shared_ptr<CStmt_Expression> outbound = std::make_shared<CStmt_Expression>(expr, line);
	return outbound;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// chromaParser Expression Eval Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<CExpr> CParser::expression()
{
	return assignment();
}
std::shared_ptr<CExpr> CParser::assignment()
{
	std::shared_ptr<CExpr> expr = logicalOr();
	if (match({ CTokenType::ASSIGN, 
		CTokenType::ASSIGN_ADD, 
		CTokenType::ASSIGN_DIVIDE, 
		CTokenType::ASSIGN_MULTIPLY, 
		CTokenType::ASSIGN_SUBTRACT }))
	{
		std::shared_ptr<CToken> _operator = previous();
		std::shared_ptr<CExpr> value = assignment();

		return std::make_shared<CExpr_Assign>(expr, _operator, value);
	}

	return expr;
}
std::shared_ptr<CExpr> CParser::logicalOr()
{
	std::shared_ptr<CExpr> expr = logicalAnd();

	while (match({ CTokenType::LOGIC_OR }))
	{
		std::shared_ptr<CToken> _operator = previous();
		std::shared_ptr<CExpr> right = logicalAnd();
		expr = std::make_shared<CExpr_Logical>(expr, _operator, right);
	}

	return expr;
}
std::shared_ptr<CExpr> CParser::logicalAnd()
{
	std::shared_ptr<CExpr> expr = equality();

	while (match({ CTokenType::LOGIC_AND }))
	{
		std::shared_ptr<CToken> _operator = previous();
		std::shared_ptr<CExpr> right = equality();
		expr = std::make_shared<CExpr_Logical>(expr, _operator, right);
	}

	return expr;
}
std::shared_ptr<CExpr> CParser::equality()
{
	std::shared_ptr<CExpr> expr = comparison();

	while (match({ CTokenType::EQUAL, CTokenType::NOT_EQUAL }))
	{
		std::shared_ptr<CToken> _operator = previous();
		std::shared_ptr<CExpr> right = comparison();
		expr = std::make_shared<CExpr_Binary>(expr, _operator, right);
	}

	return expr;
}
std::shared_ptr<CExpr> CParser::comparison()
{
	std::shared_ptr<CExpr> expr = term();

	if (match({ CTokenType::APPROXIMATE }))
	{
		if (peek().get()->type == CTokenType::PAREN_LEFT)
		{
			consume(CTokenType::PAREN_LEFT, "[parser:1801] Expected opening brace '(' after '~=' approximate operator.");
			return approximate(expr);
		}
		else
		{
			console.get()->error(peek(), "[parser:1802] Invalid occurence of '?' validate operator.");
		}
	}

	while (match({ CTokenType::MORE, CTokenType::MORE_EQUAL, CTokenType::LESS, CTokenType::LESS_EQUAL }))
	{
		std::shared_ptr<CToken> _operator = previous();
		std::shared_ptr<CExpr> right = term();
		expr = std::make_shared<CExpr_Binary>(expr, _operator, right);
	}

	return expr;
}
std::shared_ptr<CExpr> CParser::approximate(std::shared_ptr<CExpr> left)
{
	std::shared_ptr<CExpr> value = expression();
	std::shared_ptr<CToken> line = consume(CTokenType::PAREN_RIGHT, "[parser:1901] Expected closing brace ')' after expression in '~=' approximate operator.");
	std::shared_ptr<CExpr> right = term();
	return std::make_shared<CExpr_Approximate>(left, value, right, line);
}
std::shared_ptr<CExpr> CParser::term()
{
	std::shared_ptr<CExpr> expr = factor();

	while (match({ CTokenType::MINUS, CTokenType::PLUS }))
	{
		std::shared_ptr<CToken> _operator = previous();
		std::shared_ptr<CExpr> right = factor();
		expr = std::make_shared<CExpr_Binary>(expr, _operator, right);
	}

	return expr;
}
std::shared_ptr<CExpr> CParser::factor()
{
	std::shared_ptr<CExpr> expr = exponent();

	while (match({ CTokenType::MODULO, CTokenType::DIVIDE, CTokenType::MULTIPLY }))
	{
		std::shared_ptr<CToken> _operator = previous();
		std::shared_ptr<CExpr> right = exponent();
		expr = std::make_shared<CExpr_Binary>(expr, _operator, right);
	}

	return expr;
}
std::shared_ptr<CExpr> CParser::exponent()
{
	std::shared_ptr<CExpr> expr = scope();

	while (match({ CTokenType::POWER }))
	{
		std::shared_ptr<CToken> _operator = previous();
		std::shared_ptr<CExpr> right = scope();
		expr = std::make_shared<CExpr_Binary>(expr, _operator, right);
	}

	return expr;
}
std::shared_ptr<CExpr> CParser::scope()
{
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::shared_ptr<CToken> line = nullptr;
	while (peek().get()->type == CTokenType::IDENTIFIER && scry(1).get()->type == CTokenType::SCOPE)
	{
		advance();
		line = previous();
		scopeStack.push_back(previous());
		advance();
	}
	if (!scopeStack.empty())
	{
		std::shared_ptr<CExpr> right = typeCast();
		return std::make_shared<CExpr_Scope>(std::make_shared<std::vector<std::shared_ptr<CToken>>>(scopeStack), right, line);
	}

	return typeCast();
}
std::shared_ptr<CExpr> CParser::typeCast()
{
	if (match({ CTokenType::LESS }))
	{
		if (isValidVarType(peek()))
		{
			std::shared_ptr<CToken> type = createTypeSpecifier(peek());
			consume(CTokenType::MORE, "[parser:2001] Expected closing '>' after type in typecast.");
			std::shared_ptr<CExpr> right = instanceOf();
			return std::make_shared<CExpr_TypeCast>(type, right);
		}
		else
		{
			console.get()->error(peek(), "[parser:2002] Invalid type declaration in use of '<' typecast operator.");
		}
	}
	return instanceOf();
}
std::shared_ptr<CExpr> CParser::instanceOf()
{
	if (match({ CTokenType::VALIDATE }))
	{
		std::shared_ptr<CToken> line = previous();
		if (peek().get()->type == CTokenType::PAREN_LEFT)
		{
			consume(CTokenType::PAREN_LEFT, "[parser:2101] Expected opening brace '(' after '?' validate operator.");
			std::shared_ptr<CToken> type = nullptr;
			if (isValidVarType(peek()))
			{
				type = createTypeSpecifier(peek());
			}
			else
			{
				type = std::make_shared<CToken>(CTokenType::NIL, peek().get()->line);
				console.get()->error(peek(), "[parser:2102] Invalid type declaration in use of '<' typecast operator.");
			}
			consume(CTokenType::PAREN_RIGHT, "[parser:2103] Expected closing brace ')' after type in '?' validate operator.");
			std::shared_ptr<CExpr> right = unary();
			return std::make_shared<CExpr_InstanceOf>(type, right, line);
		}
		else
		{
			console.get()->error(peek(), "[parser:2104] Invalid occurence of '?' validate operator.");
		}
	}
	return unary();
}
std::shared_ptr<CExpr> CParser::unary()
{
	while (match({ CTokenType::FALSIFY, CTokenType::NEGATE, CTokenType::INCREMENT, CTokenType::DECREMENT }))
	{
		std::shared_ptr<CToken> _operator = previous();
		_operator.get()->lexeme = "prefix";
		std::shared_ptr<CExpr> right = unary();
		std::shared_ptr<CExpr> expr;
		expr = std::make_shared<CExpr_Unary>(_operator, right);
		return expr;
	}
	return call();
}
std::shared_ptr<CExpr> CParser::call()
{
	std::shared_ptr<CExpr> expr = primary();

	while (true)
	{
		if (match({ CTokenType::PAREN_LEFT }))
		{
			expr = finishCall(expr);
		}
		else if (match({ CTokenType::DOT }))
		{
			std::shared_ptr<CToken> line = previous();
			std::shared_ptr<CToken> fieldName = peek();
			std::shared_ptr<CExpr> field = primary();
			expr = std::make_shared<CExpr_Get>(expr, field, line, fieldName);
		}
		else
		{
			break;
		}
	}

	return expr;
}
std::shared_ptr<CExpr> CParser::finishCall(std::shared_ptr<CExpr> callee)
{
	std::vector<std::shared_ptr<CExpr>> arguments;
	if (!check(CTokenType::PAREN_RIGHT))
	{
		do
		{
			if (arguments.size() >= 64)
			{
				console.get()->error(peek(), "[parser:2201] Can't have more than 64 arguments.");
			}
			arguments.push_back(expression());
		} while (match({ CTokenType::COMMA }));
	}
	std::shared_ptr<CToken> paren = consume(CTokenType::PAREN_RIGHT, "[parser:2202] Expect ')' after arguments.");
	std::shared_ptr<CExpr> index = createArraySpecifier(paren);

	return std::make_shared<CExpr_Call>(callee, arguments, index, paren);
}
std::shared_ptr<CExpr> CParser::primary()
{
	if (match({ CTokenType::IDENTIFIER }))
	{
		std::shared_ptr<CToken> name = previous();
		return createArraySpecifier(name);
	}

	if (match({ CTokenType::_THIS, CTokenType::BOOL,
		CTokenType::NIL, CTokenType::NUM, CTokenType::STRING }))
	{ 
		std::shared_ptr<CToken> _literal = previous();
		if (match({ CTokenType::INCREMENT, CTokenType::DECREMENT }))
		{
			std::shared_ptr<CToken> _operator = previous();
			_operator.get()->lexeme = "postfix";
			return std::make_shared<CExpr_Unary>(_operator, std::make_shared<CExpr_Literal>(_literal));
		}
		return std::make_shared<CExpr_Literal>(previous()); 
	}

	if (match({ CTokenType::CURLY_LEFT }))
	{
		std::shared_ptr<CToken> line = previous();
		std::vector<std::shared_ptr<CExpr>> initializer;
		do
		{
			if (initializer.size() >= 255)
			{
				error(peek(), "[parser:2301] Can't have more than 255 values in an array.");
				while (peek().get()->type != CTokenType::CURLY_RIGHT)
				{
					advance();
				}
				break;
			}
			initializer.push_back(expression());

		} while (match({ CTokenType::COMMA }));

		consume(CTokenType::CURLY_RIGHT, "[parser:2302] Expect closing bracket '}' after array declaration.");
		return std::make_shared<CExpr_Array>(initializer, line);
	}

	if (match({ CTokenType::PAREN_LEFT }))
	{
		std::shared_ptr<CToken> line = previous();
		if (match({ CTokenType::CURLY_LEFT }))
		{
			return vector();
		}
		std::shared_ptr<CExpr> expr = expression();
		consume(CTokenType::PAREN_RIGHT, "[parser:2401] Expect ')' after expression.");
		return std::make_shared<CExpr_Grouping>(expr, line);
	}
	error(peek(), "[parser:2402] Expect expression.");
	// Unreachable
	return nullptr;
}
std::shared_ptr<CExpr> CParser::vector()
{
	std::shared_ptr<CToken> token = previous();
	std::shared_ptr<CExpr> x = nullptr;
	std::shared_ptr<CExpr> y = nullptr;
	std::shared_ptr<CExpr> z = nullptr;
	std::shared_ptr<CExpr> w = nullptr;
	x = expression();
	
	if (match({ CTokenType::COMMA }))
	{
		y = expression();
		if (match({ CTokenType::COMMA }))
		{
			z = expression();
			if (match({ CTokenType::COMMA }))
			{
				w = expression();
				closeVector();
				token.get()->lexeme = CTokenTypeNames[(size_t)CTokenType::VECTOR4];
				token.get()->type = CTokenType::VECTOR4;
				return std::make_shared<CExpr_Vector>(token, std::vector({ x, y, z, w }));
			}
			else
			{
				closeVector();
				token.get()->lexeme = CTokenTypeNames[(size_t)CTokenType::VECTOR3];
				token.get()->type = CTokenType::VECTOR3;
				return std::make_shared<CExpr_Vector>(token, std::vector({ x, y, z }));
			}
		}
		else
		{
			closeVector();
			token.get()->lexeme = CTokenTypeNames[(size_t)CTokenType::VECTOR2];
			token.get()->type = CTokenType::VECTOR2;
			return std::make_shared<CExpr_Vector>(token, std::vector({ x, y }));
		}
	}
	else
	{
		error(peek(), "[parser:2501] Expected at least two expressions in vector.");
		closeVector();
		return x;
	}
	return x;
}


////////////////////////////////////////////////////////////////////////////////////////////////
//
// chromaParser Utility Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<CToken> CParser::advance()
{
	if (!isAtEnd())
	{
		current++;
	}
	return previous();
}
std::shared_ptr<CToken> CParser::peek()
{
	return tokens[current];
}
std::shared_ptr<CToken> CParser::scry(size_t depth)
{
	if (tokens.size() > current + depth) { return tokens[current + depth]; }
	return nullptr;
}
std::shared_ptr<CToken> CParser::consume(CTokenType type, std::string message)
{
	if (check(type))
	{
		return advance();
	}

	error(peek(), message);
	synchronize();
	return tokens[current];
}

std::shared_ptr<CToken> CParser::createTypeSpecifier(std::shared_ptr<CToken> start)
{
	std::shared_ptr<CToken> typeSpecifier = start;

	if (typeSpecifier.get()->type == CTokenType::ARRAY)
	{
		if (scry(1) != nullptr && isValidVarType(scry(1)))
		{
			typeSpecifier = std::make_shared<CToken>(
				CTokenType::ARRAY,
				scry(1).get()->type,
				scry(1).get()->lexeme,
				scry(1).get()->line);
			advance();
		}
		else
		{
			error(peek(), "[parser:2601] Array variable must be followed by a valid type specifier.");
			advance();
			synchronize();
		}
	}
	advance();
	return typeSpecifier;
}

std::shared_ptr<CExpr> CParser::createArraySpecifier(std::shared_ptr<CToken> start)
{
	// Check for array specifier, then check for increment or decrement operator
	if (match({ CTokenType::SQUARE_LEFT }))
	{
		if (peek().get()->type == CTokenType::SQUARE_RIGHT)
		{
			advance();
			console.get()->error(start, "[parser:2701] Attempted to nil index array.");
			return std::make_shared<CExpr_Variable>(start);
		}
		std::shared_ptr<CExpr> index = expression();
		consume(CTokenType::SQUARE_RIGHT, "[parser:2702] Expect closing bracket after 'get at index' operator.");
		if (match({ CTokenType::INCREMENT, CTokenType::DECREMENT }))
		{
			std::shared_ptr<CToken> _operator = previous();
			_operator.get()->lexeme = "postfix";
			return std::make_shared<CExpr_Unary>(_operator, std::make_shared<CExpr_Variable>(start, index));
		}
		return std::make_shared<CExpr_Variable>(start, index);
	}
	// Check for increment or decrement operator
	if (match({ CTokenType::INCREMENT, CTokenType::DECREMENT }))
	{
		std::shared_ptr<CToken> _operator = previous();
		_operator.get()->lexeme = "postfix";
		return std::make_shared<CExpr_Unary>(_operator, std::make_shared<CExpr_Variable>(start));
	}
	// Check for call expression
	if (start.get()->type == CTokenType::PAREN_RIGHT)
	{
		// Should return nullptr if the previous code indicated a Call expression
		// and the previous two if statements fell through
		return nullptr;
	}
	return std::make_shared<CExpr_Variable>(start);
}

std::shared_ptr<CToken> CParser::previous()
{
	return tokens[current - 1];
}
bool CParser::match(std::vector<CTokenType> types)
{
	for (CTokenType type : types)
	{
		if (check(type))
		{
			advance();
			return true;
		}
	}

	return false;
}
bool CParser::check(CTokenType type)
{
	if (isAtEnd())
	{
		return false;
	}
	return peek().get()->type == type;
}

bool CParser::isValidVarType(std::shared_ptr<CToken> token)
{
	switch (token.get()->type)
	{
	case CTokenType::ARRAY:
	case CTokenType::NUM:
	case CTokenType::VECTOR2:
	case CTokenType::VECTOR3:
	case CTokenType::VECTOR4:
	case CTokenType::STRING:
	case CTokenType::BOOL:
	case CTokenType::IDENTIFIER:
	case CTokenType::FUNCTION:
	case CTokenType::_VOID:
		return true;
		break;
	default:
		return false;
		break;
	}
}

bool CParser::isArrayType(std::shared_ptr<CToken> token)
{
	switch (token.get()->type)
	{
	case CTokenType::NUM_ARRAY:
	case CTokenType::VECTOR2_ARRAY:
	case CTokenType::VECTOR3_ARRAY:
	case CTokenType::VECTOR4_ARRAY:
	case CTokenType::STRING_ARRAY:
	case CTokenType::BOOL_ARRAY:
	case CTokenType::IDENTIFIER_ARRAY:
	case CTokenType::FUNCTION_ARRAY:
	case CTokenType::VOID_ARRAY:
		return true;
		break;
	default:
		return false;
		break;
	}
}

bool CParser::isAtEnd()
{
	return peek().get()->type == CTokenType::END;
}

//
void CParser::synchronize()
{
	advance();

	while (!isAtEnd())
	{
		if (previous().get()->type == CTokenType::SEMICOLON) return;

		switch (peek().get()->type)
		{
		case CTokenType::CLASS:
		case CTokenType::FUNCTION:
		case CTokenType::ARRAY:
		case CTokenType::NUM:
		case CTokenType::VECTOR2:
		case CTokenType::VECTOR3:
		case CTokenType::VECTOR4:
		case CTokenType::STRING:
		case CTokenType::BOOL:
		case CTokenType::_VOID:
		case CTokenType::FOR:
		case CTokenType::FOREACH:
		case CTokenType::BREAK:
		case CTokenType::CONTINUE:
		case CTokenType::TRY:
		case CTokenType::IF:
		case CTokenType::WHILE:
		case CTokenType::PRINT:
		case CTokenType::RETURN:
			return;
		}

		advance();
	}
}
void CParser::closeVector()
{
	bool foundCurly = false;
	bool foundParen = false;
	advance();

	while (!isAtEnd())
	{
		if (foundCurly == false && previous().get()->type == CTokenType::CURLY_RIGHT)
		{
			foundCurly = true;
		}
		else if (foundParen == false && previous().get()->type == CTokenType::PAREN_RIGHT)
		{
			foundParen = true;
			break;
		}

		advance();
	}

	if (foundCurly == true && foundParen == true) { return; }
	else if (foundParen == true && foundCurly == false)
	{
		console.get()->error(previous().get()->line, "[parser:2502] Unproperly closed vector statement, expected closing brace '}'.");
	}
	else
	{
		console.get()->error(previous().get()->line, "[parser:2503] Unclosed vector statement, expected closing brace '}'.");
		console.get()->error(previous().get()->line, "[parser:2503] Unclosed vector statement, expected closing parenthesis ')'.");
	}
}


//

void CParser::error(std::shared_ptr<CToken> token, std::string message)
{
	console.get()->error(token, message);
}