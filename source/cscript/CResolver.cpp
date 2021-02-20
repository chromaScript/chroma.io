#include "../include/cscript/CResolver.h"
#include "../include/cscript/CInterpreter.h"
#include "../include/cscript/CEnvironment.h"
#include "../include/cscript/ChromaScript.h"
#include "../include/cscript/CToken.h"
#include "../include/cscript/CExpr.h"
#include "../include/cscript/CStmt.h"

#include <string>
#include <vector>
#include <exception>
#include <map>
#include <stack>

//
CResolver::CResolver()
{

}

void CResolver::initialize(std::shared_ptr<ChromaScript> console)
{
	this->console = console;
}

//
void CResolver::resolveStatements(std::vector<std::shared_ptr<CStmt>> statements, 
	std::shared_ptr<CEnvironment> rootEnvironment)
{
	setEnvironment(rootEnvironment);
	for (std::shared_ptr<CStmt> statement : statements)
	{
		resolve(statement);
	}
	setEnvironment(console.get()->global);
}
//
void CResolver::clearUnresolvedSymbols()
{
	unresolvedSymbols.clear();
}
//
void CResolver::checkUnresolvedSymbols()
{
	for (auto const& item : unresolvedSymbols)
	{
		if (item.second != nullptr)
		{
			console.get()->error("[resolver:0101] Unresolved symbol declaration at scope address: '"
				+ item.first + "'. Check spelling errors and make sure symbol is defined.");
		}
	}
}
//
std::string CResolver::scopeToString(std::vector<std::shared_ptr<CToken>> scopeStack)
{
	std::string out;
	for (std::shared_ptr<CToken> token : scopeStack)
	{
		out += token.get()->lexeme;
		out += "::";
	}
	return out;
}
//
std::string CResolver::paramsToString(
	std::vector<std::string> names, std::vector<std::shared_ptr<CToken>> types)
{
	std::string out;
	if (names.size() != types.size())
	{
		console.get()->error("[resolver:0201] error: Encountered function declaration with missing parameter type specifiers.");
		return "";
	}
	else
	{
		for (size_t i = 0; i < types.size(); i++)
		{
			out += types[i].get()->lexeme + " " + names[i];
			if (i != types.size() - 1)
			{
				out += ",";
			}
		}
	}
	return out;
}
//
std::string CResolver::getCurrentAddress(std::shared_ptr<CEnvironment> environment)
{
	std::string out;
	std::shared_ptr<CEnvironment> address = environment;
	while (address.get()->enclosing != nullptr)
	{
		out.insert(0, address.get()->_namespace + "::");
		address = address.get()->enclosing;

	}
	out.insert(0, address.get()->_namespace + "::");
	return out;
}
//
void CResolver::resolve(std::shared_ptr<CStmt> stmt)
{
	stmt->accept(*this);
}
void CResolver::resolve(std::shared_ptr<CExpr> expr)
{
	expr->accept(*this);
}
void CResolver::visit(std::shared_ptr<CStmt_Block> stmt)
{
	// "Catch" the namespace variable
	if (currentNamespace != "")
	{
		// Clear the variable
		currentNamespace = "";
		// Resolve the statements
		for (std::shared_ptr<CStmt> statement : stmt.get()->statements)
		{
			resolve(statement);
		}
	}
	// "Throw" an anonymous environment in the event an anonymous block statement is visited
	else
	{
		// Capture and find/create a new environment
		std::shared_ptr<CEnvironment> previous = currentEnvironment;
		stmt.get()->_namespace = randomNamespace(anonLength);
		currentEnvironment = currentEnvironment.get()->lookupEnvironment(stmt.get()->_namespace, true);
		// Resolve the statements
		for (std::shared_ptr<CStmt> statement : stmt.get()->statements)
		{
			resolve(statement);
		}
		// Reset the environment
		currentEnvironment = previous;
	}
}
void CResolver::visit(std::shared_ptr<CStmt_Break> stmt)
{
	// Do Nothing
}
void CResolver::visit(std::shared_ptr<CStmt_Class> stmt)
{
	// If the statement is only a declaration, and is not paired with a scope operator, create environment, forward declare object
	if (stmt.get()->isNameDeclarationOnly && stmt.get()->scopeStack.size() == 0)
	{
		// Add the unresolved symbol to the stack
		std::string address;
		address += getCurrentAddress(currentEnvironment);
		address += stmt.get()->name.get()->lexeme;
		unresolvedSymbols.insert(std::pair<std::string, std::shared_ptr<CStmt>>(address, stmt));
		// Create the environment, but do not enter it
		currentEnvironment.get()->lookupEnvironment(stmt.get()->name.get()->lexeme, true);
		// Declare the function as a new object within this environment's values map
		int result = currentEnvironment.get()->declare(stmt.get()->name.get()->lexeme);
		// Discard result for now, multiple forward declarations should not cause errors
	}
	else if (stmt.get()->isNameDeclarationOnly && stmt.get()->scopeStack.size() != 0)
	{
		currentEnvironment.get()->lookupEnvironment(stmt.get()->scopeStack, stmt.get()->name.get()->lexeme);
		int result = currentEnvironment.get()->declare(stmt.get()->name.get()->lexeme);
		if (result == 0)
		{
			console.get()->error(stmt.get()->name,
				"[resolver:0301] Cannot declare new class '" +
				stmt.get()->name.get()->lexeme +
				"' from outside original scope. Forward declare the class from within '" +
				scopeToString(stmt.get()->scopeStack) +
				"' first.");
			return;
		}
	}
	// Define the class
	else
	{
		// Check if the class is already defined, and report error if so
		if (currentEnvironment.get()->get(stmt.get()->scopeStack, stmt.get()->name.get()->lexeme) != nullptr)
		{
			console.get()->error(stmt.get()->name,
				"[resolver:0302] Cannot redefine previously defined function '" +
				stmt.get()->name.get()->lexeme +
				"'.");
			return;
		}

		// Capture current environment and find/create a new environment
		std::shared_ptr<CEnvironment> previous = currentEnvironment;
		if (stmt.get()->scopeStack.size() != 0)
		{
			// Get the environment associated with the class
			currentEnvironment = currentEnvironment.get()->lookupEnvironment(stmt.get()->scopeStack, stmt.get()->name.get()->lexeme);
			if (currentEnvironment == nullptr)
			{
				console.get()->error(stmt.get()->name,
					"[resolver:0303] Cannot declare and define new class '" +
					stmt.get()->name.get()->lexeme +
					"' from outside original scope. Forward declare the class from within '" +
					scopeToString(stmt.get()->scopeStack) +
					"' first.");
				return;
			}
		}
		else
		{
			// Get the environment associated with the class
			currentEnvironment = currentEnvironment.get()->lookupEnvironment(stmt.get()->name.get()->lexeme, true);
		}
		std::string address;
		address += getCurrentAddress(currentEnvironment.get()->enclosing);
		address += stmt.get()->name.get()->lexeme;
		if (unresolvedSymbols.count(address) == 1)
		{
			if (dynamic_cast<CStmt_Class*>(unresolvedSymbols.at(address).get()) != nullptr)
			{
				std::shared_ptr<CStmt_Class> old = std::dynamic_pointer_cast<CStmt_Class>(unresolvedSymbols.at(address));
				*old = *stmt;
				// Also clear the scopeStack because that shouldn't be copied
				old.get()->scopeStack.clear();
				unresolvedSymbols.at(address) = nullptr;
			}
			else
			{
				// This should not ever happen, adding warning message just in case
				console.get()->error(stmt.get()->name, "[resolver:0304] Encountered unexpected symbol redefinition.");
			}
		}
		// Define the class
		std::shared_ptr<CEnvironment> superEnv = nullptr;
		std::shared_ptr<CClass> superClass = nullptr;
		if (stmt.get()->superclass != nullptr)
		{
			superEnv = currentEnvironment.get()->fetchEnvironment(stmt.get()->superclass.get()->lexeme);
			superClass = std::get<std::shared_ptr<CClass>>(superEnv.get()->enclosing.get()->get(stmt.get()->superclass.get()->lexeme).get()->obj);
		}
		int result = currentEnvironment.get()->enclosing.get()->define(
			stmt.get()->name.get()->lexeme,
			std::make_shared<CObject>(stmt, stmt.get()->name.get()->lexeme, currentEnvironment, superClass));
		// Resolve the class constructor, methods and properties
		if (stmt.get()->constructor != nullptr)
		{
			resolve(stmt.get()->constructor);
		}
		for (std::shared_ptr<CStmt> prop : stmt.get()->properties)
		{
			resolve(prop);
		}
		for (std::shared_ptr<CStmt> method : stmt.get()->methods)
		{
			resolve(method);
		}
		// Reset the environment
		currentEnvironment = previous;
	}
}
void CResolver::visit(std::shared_ptr<CStmt_Continue> stmt)
{
	// Do Nothing
}
void CResolver::visit(std::shared_ptr<CStmt_Expression> stmt)
{
	// Do Nothing
}
void CResolver::visit(std::shared_ptr<CStmt_For> stmt)
{
	std::shared_ptr<CEnvironment> previous = currentEnvironment;
	stmt.get()->_namespace = randomNamespace(anonLength);
	currentEnvironment = currentEnvironment.get()->lookupEnvironment(stmt.get()->_namespace, true);
	// Resolve the conditions
	resolve(stmt.get()->initializer);
	resolve(stmt.get()->condition);
	resolve(stmt.get()->increment);
	// Resolve the statements
	resolve(stmt.get()->body);
	// Reset the environment
	currentEnvironment = previous;
}
void CResolver::visit(std::shared_ptr<CStmt_Foreach> stmt)
{
	std::shared_ptr<CEnvironment> previous = currentEnvironment;
	stmt.get()->_namespace = randomNamespace(anonLength);
	currentEnvironment = currentEnvironment.get()->lookupEnvironment(stmt.get()->_namespace, true);
	// Resolve the conditions
	currentEnvironment.get()->declare(stmt.get()->iteratorName.get()->lexeme);
	currentEnvironment.get()->define(stmt.get()->iteratorName.get()->lexeme,
		std::make_shared<CObject>(stmt.get()->typeName, stmt.get()->iteratorName.get()->lexeme));
	// Resolve the statements
	resolve(stmt.get()->body);
	// Reset the environment
	currentEnvironment = previous;
}
void CResolver::visit(std::shared_ptr<CStmt_Function> stmt)
{
	// Cure the function name for constructors to avoid name conflicts
	if (stmt.get()->name.get()->type == CTokenType::CONSTRUCTOR)
	{
		stmt.get()->name.get()->lexeme.insert(0, "constructor:");
	}
	// If the statement is only a declaration, and is not paired with a scope operator, create environment, forward declare object
	if (stmt.get()->isNameDeclarationOnly && stmt.get()->scopeStack.size() == 0)
	{
		// Add the unresolved symbol to the stack
		std::string address;
		address += getCurrentAddress(currentEnvironment);
		address += stmt.get()->name.get()->lexeme;
		address += "(" + paramsToString(stmt.get()->paramsNames, stmt.get()->paramsTypes) + ")";
		unresolvedSymbols.insert(std::pair<std::string, std::shared_ptr<CStmt>>(address, stmt));
		// Create the environment, but do not enter it
		currentEnvironment.get()->lookupEnvironment(stmt.get()->name.get()->lexeme, true);
		// Declare the function as a new object within this environment's values map
		int result = currentEnvironment.get()->declare(stmt.get()->name.get()->lexeme);
		// Discard result for now, multiple forward declarations within original scope should not cause errors
	}
	// If statement is only a declaration, and has scope operator, throw error if not previously forward declared
	else if (stmt.get()->isNameDeclarationOnly && stmt.get()->scopeStack.size() != 0)
	{
		std::shared_ptr<CEnvironment> peek = currentEnvironment.get()->lookupEnvironment(stmt.get()->scopeStack, stmt.get()->name.get()->lexeme);
		if (peek == nullptr)
		{
			std::string scopePrint;
			for (std::shared_ptr<CToken> token : stmt.get()->scopeStack)
			{
				scopePrint += token.get()->lexeme;
				scopePrint += "::";
			}
			console.get()->error(stmt.get()->name,
				"[resolver:0401] Cannot declare new function '" +
				stmt.get()->name.get()->lexeme +
				"' from outside original scope. Forward declare the function from within '" +
				scopePrint +
				"' first.");
			return;
		}
	}
	// Define the function
	else
	{
		if (currentEnvironment.get()->get(stmt.get()->scopeStack, stmt.get()->name.get()->lexeme) != nullptr)
		{
			console.get()->error(stmt.get()->name,
				"[resolver:0402] Cannot redefine previously defined function '" +
				stmt.get()->name.get()->lexeme +
				"'.");
			return;
		}

		
		// Capture current environment and find/create a new environment
		std::shared_ptr<CEnvironment> previous = currentEnvironment;
		if (stmt.get()->scopeStack.size() != 0)
		{
			// Get the environment associated with the function
			currentEnvironment = currentEnvironment.get()->lookupEnvironment(stmt.get()->scopeStack, stmt.get()->name.get()->lexeme);
			if (currentEnvironment == nullptr)
			{
				std::string scopePrint;
				for (std::shared_ptr<CToken> token : stmt.get()->scopeStack)
				{
					scopePrint += token.get()->lexeme;
					scopePrint += "::";
				}
				console.get()->error(stmt.get()->name,
					"[resolver:0403] Cannot declare and define new function '" +
					stmt.get()->name.get()->lexeme +
					"' from outside original scope. Forward declare the function from within '" +
					scopePrint +
					"' first.");
				return;
			}
			else
			{
				// "Try" the namespace because the body should always be a block statement
				currentNamespace = currentEnvironment.get()->_namespace;
			}
		}
		else
		{
			// Get the environment associated with the function
			currentEnvironment = currentEnvironment.get()->lookupEnvironment(stmt.get()->name.get()->lexeme, true);
			// "Try" the namespace because the body should always be a block statement
			currentNamespace = currentEnvironment.get()->_namespace;
		}
		std::string address;
		address += getCurrentAddress(currentEnvironment.get()->enclosing);
		address += stmt.get()->name.get()->lexeme;
		address += "(" + paramsToString(stmt.get()->paramsNames, stmt.get()->paramsTypes) + ")";
		if (unresolvedSymbols.count(address) == 1)
		{
			if (dynamic_cast<CStmt_Function*>(unresolvedSymbols.at(address).get()) != nullptr)
			{
				std::shared_ptr<CStmt_Function> old = std::dynamic_pointer_cast<CStmt_Function>(unresolvedSymbols.at(address));
				*old = *stmt;
				// Also clear the scopeStack because that shouldn't be copied
				old.get()->scopeStack.clear();
				unresolvedSymbols.at(address) = nullptr;
			}
			else
			{
				// This should not ever happen, adding warning message just in case
				console.get()->error(stmt.get()->name, "[resolver:0405] Encountered unexpected symbol redefinition.");
			}
		}
		// Define the function
		int result = currentEnvironment.get()->enclosing.get()->define(
			stmt.get()->name.get()->lexeme, 
			std::make_shared<CObject>(stmt, currentEnvironment));
		// Declare the parameters
		for (size_t i = 0; i < stmt.get()->paramsNames.size(); i++)
		{
			currentEnvironment.get()->declare(stmt.get()->paramsNames[i]);
			currentEnvironment.get()->define(stmt.get()->paramsNames[i],
				std::make_shared<CObject>(stmt.get()->paramsTypes[i], stmt.get()->paramsNames[i]));
		}
		// Resolve the block statement
		resolve(stmt.get()->body);
		// Reset the environment
		currentNamespace = "";
		currentEnvironment = previous;
	}
}
void CResolver::visit(std::shared_ptr<CStmt_Namespace> stmt)
{
	// Capture and find/create a new environment
	std::shared_ptr<CEnvironment> previous = currentEnvironment;
	currentEnvironment = currentEnvironment.get()->lookupEnvironment(stmt.get()->name.get()->lexeme, true);
	// "Try" the namespace because the body should always be a block statement
	currentNamespace = stmt.get()->name.get()->lexeme;
	// Resolve the block statement
	resolve(stmt.get()->body);
	// Reset the environment
	currentNamespace = "";
	currentEnvironment = previous;
}
void CResolver::visit(std::shared_ptr<CStmt_If> stmt)
{
	std::shared_ptr<CEnvironment> previous = currentEnvironment;
	stmt.get()->_namespace = randomNamespace(anonLength);
	currentEnvironment = currentEnvironment.get()->lookupEnvironment(stmt.get()->_namespace, true);
	// Verify that the number of execution branches equals the number of condition statements
	if (stmt.get()->conditionsList.size() != stmt.get()->executionList.size())
	{
		console.get()->error(stmt.get()->line, "[resolver:0501] Could not find matching number of condition expressions for 'if/else' statement.");
	}
	// Resolve the statements
	for (std::shared_ptr<CStmt> statement : stmt.get()->executionList)
	{
		resolve(statement);
	}
	if (stmt.get()->elseBranch != nullptr)
	{
		resolve(stmt.get()->elseBranch);
	}
	// Reset the environment
	currentEnvironment = previous;
}
void CResolver::visit(std::shared_ptr<CStmt_Print> stmt)
{
	// Do Nothing
}
void CResolver::visit(std::shared_ptr<CStmt_Return> stmt)
{
	// Do Nothing
}
void CResolver::visit(std::shared_ptr<CStmt_TryCatch> stmt)
{
	std::shared_ptr<CEnvironment> previous = currentEnvironment;
	stmt.get()->_namespace = randomNamespace(anonLength);
	currentEnvironment = currentEnvironment.get()->lookupEnvironment(stmt.get()->_namespace, true);
	// Resolve the statements
	resolve(stmt.get()->tryBlock);
	resolve(stmt.get()->catchBlock);
	// Reset the environment
	currentEnvironment = previous;
}
void CResolver::visit(std::shared_ptr<CStmt_Variable> stmt)
{
	if (stmt.get()->initializer != nullptr)
	{
		stmt.get()->initializer.get()->accept(*this);
	}
	int declareResult = currentEnvironment.get()->declare(stmt.get()->name.get()->lexeme);
	if (declareResult == 1)
	{
		console.get()->error(stmt.get()->name, 
			"[resolver:0601] Cannot redclare previously declared variable '" + 
			stmt.get()->name.get()->lexeme + 
			"' within same scope.");
		return;
	}
	else
	{
		int defineResult = currentEnvironment.get()->define(
			stmt.get()->name.get()->lexeme,
			std::make_shared<CObject>(stmt.get()->typeName, stmt.get()->name.get()->lexeme));
	}
}
void CResolver::visit(std::shared_ptr<CStmt_While> stmt)
{
	std::shared_ptr<CEnvironment> previous = currentEnvironment;
	stmt.get()->_namespace = randomNamespace(anonLength);
	currentEnvironment = currentEnvironment.get()->lookupEnvironment(stmt.get()->_namespace, true);
	// Resolve the statements
	resolve(stmt.get()->body);
	// Reset the environment
	currentEnvironment = previous;
}
//

//
void CResolver::visit(std::shared_ptr<CExpr_Approximate> expr)
{
	// Do Nothing
}
void CResolver::visit(std::shared_ptr<CExpr_Array> stmt)
{
	// Do Nothing
}
void CResolver::visit(std::shared_ptr<CExpr_Assign> expr)
{
	// Do Nothing
}
void CResolver::visit(std::shared_ptr<CExpr_Binary> expr)
{
	// Do Nothing
}
void CResolver::visit(std::shared_ptr<CExpr_Call> expr)
{
	// Do Nothing
}
void CResolver::visit(std::shared_ptr<CExpr_Get> expr)
{
	// Do Nothing
}
void CResolver::visit(std::shared_ptr<CExpr_Grouping> expr)
{
	// Do Nothing
}
void CResolver::visit(std::shared_ptr<CExpr_InstanceOf> expr)
{
	// Do Nothing
}
void CResolver::visit(std::shared_ptr<CExpr_Literal> expr)
{
	// Do Nothing
}
void CResolver::visit(std::shared_ptr<CExpr_Logical> expr)
{
	// Do Nothing
}
void CResolver::visit(std::shared_ptr<CExpr_Scope> expr)
{
	// Do Nothing
}
void CResolver::visit(std::shared_ptr<CExpr_TypeCast> expr)
{
	// Do Nothing
}
void CResolver::visit(std::shared_ptr<CExpr_Unary> expr)
{
	// Do Nothing
}
void CResolver::visit(std::shared_ptr<CExpr_Variable> expr)
{
	// Do Nothing
}
void CResolver::visit(std::shared_ptr<CExpr_Vector> expr)
{
	// Do Nothing
}
