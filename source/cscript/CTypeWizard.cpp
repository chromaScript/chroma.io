#include "../include/cscript/CTypeWizard.h"
#include "../include/cscript/CEnvironment.h"
#include "../include/cscript/ChromaScript.h"
#include "../include/cscript/CForward.h"
#include "../include/cscript/CExpr.h"
#include "../include/cscript/CStmt.h"
#include "../include/cscript/CEnums.h"
#include "../include/cscript/CToken.h"
#include "../include/cscript/CObject.h"
#include "../include/cscript/CReturn.h"
#include "../include/cscript/CCallable.h"
#include "../include/cscript/CCallable_Lib.h"

#include <string>
#include <vector>
#include <exception>
#include <memory>
#include <variant>

//
CTypeWizard::CTypeWizard()
{

}

void CTypeWizard::initialize(std::shared_ptr<ChromaScript> console)
{
	this->console = console;
}



//
void CTypeWizard::checkTypes(std::vector<std::shared_ptr<CStmt>> statements, std::shared_ptr<CEnvironment> environment)
{
	setEnvironment(environment);
	for (std::shared_ptr<CStmt> stmt : statements)
	{
		stmt.get()->accept(*this);
	}
	setEnvironment(console.get()->global);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Visit Statements
//
////////////////////////////////////////////////////////////////////////////////////////////////

// CStmt_Block
void CTypeWizard::visit(std::shared_ptr<CStmt_Block> stmt)
{
	bool encounteredReturn = false;
	std::shared_ptr<CEnvironment> previous = nullptr;
	// Check for anonymous block statement
	if (currentNamespaces.size() == 0 || (stmt.get()->_namespace != "" && stmt.get()->_namespace != currentNamespaces.back()))
	{
		// Enter anonymous block
		previous = currentEnvironment;
		currentEnvironment = currentEnvironment.get()->lookupEnvironment(stmt.get()->_namespace, false);
		// Check that the environment was actually found. This must return a value or else cascading failures
		// will occur from subsequent environment value lookups inside the function.
		if (currentEnvironment == nullptr)
		{
			console.get()->error(
				"[typeWizard:0101:" + currentEnvironment.get()->_namespace + "] Critical: Anonymous Block Statement: Failed to find the environment for block '" + stmt.get()->_namespace
				+ "'. Report this bug to the program author.");
			return;
		}
		currentNamespaces.push_back(stmt.get()->_namespace);
	}

	for (std::shared_ptr<CStmt> statement : stmt.get()->statements)
	{
		if (currentNamespaces.size() != 0 && 
			currentNamespaces.back() == "namespace" && 
			dynamic_cast<CStmt_Return*>(statement.get()) != nullptr)
		{
			console.get()->error("[typeWizard:0102:" + currentEnvironment.get()->_namespace + "] Cannot place 'return' statement inside of 'namespace' statement.");
		}
		// Note: Move this into the optimizer later
		if (dynamic_cast<CStmt_Return*>(statement.get()) != nullptr)
		{
			statement.get()->accept(*this);
			encounteredReturn = true;
		}
		else
		{
			if (encounteredReturn == true)
			{
				console.get()->warning(
					"[optimizer:0000:" + currentEnvironment.get()->_namespace + "] warning: Encountered statements beyond 'return' statement that will be discarded.");
			}
			statement.get()->accept(*this);
		}
	}
	// If was anonymous block, exit environment
	if (previous != nullptr)
	{
		currentNamespaces.pop_back();
		currentEnvironment = previous;
	}
}
// CStmt_Break
void CTypeWizard::visit(std::shared_ptr<CStmt_Break> stmt)
{
	// Do Nothing
}
// CStmt_Class
void CTypeWizard::visit(std::shared_ptr<CStmt_Class> stmt)
{
	std::shared_ptr<CObject> superClass = nullptr;
	// Verify that no overlapping property names exist
	if (stmt.get()->superclass != nullptr)
	{
		superClass = currentEnvironment.get()->get(stmt.get()->superclass.get()->lexeme);
		if (superClass == nullptr || superClass.get()->objType.type != CLiteralTypes::_CClass)
		{
			console.get()->error(stmt.get()->name, "[typeWizard:0201:" + currentEnvironment.get()->_namespace + "] Class '" + stmt.get()->name.get()->lexeme +
			"' does not have a valid superclass.");
		}
		else
		{
			std::shared_ptr<CEnvironment> thisClassEnv = currentEnvironment.get()->fetchEnvironment(stmt.get()->name.get()->lexeme);
			std::vector<std::string> thisFieldNames = thisClassEnv.get()->fetchValueNames();;
			std::vector<std::string> superFieldNames = getSuperPropertyNames(superClass);
			for (std::string superFieldName : superFieldNames)
			{
				for (std::string thisFieldName : thisFieldNames)
				{
					/*
					if (superFieldName == thisFieldName && superFieldName != "constructor" && superFieldName != "cast")
					{
						console.get()->error(stmt.get()->name,
							"[typeWizard:0202] Cannot declare child class property '" + thisFieldName +
							"' with same name as parent class property.");
					}
					*/
					if (superFieldName == thisFieldName && superFieldName != "cast")
					{
						console.get()->error(stmt.get()->name,
							"[typeWizard:0202:" + currentEnvironment.get()->_namespace + "] Cannot declare child class property '" + thisFieldName +
							"' with same name as parent class property.");
					}
				}
			}
		}
	}
	// Capture the environment
	std::shared_ptr<CEnvironment> previous = currentEnvironment;
	currentEnvironment = currentEnvironment.get()->lookupEnvironment(stmt.get()->name.get()->lexeme, false);
	// Check that the environment was actually found. This must return a value or else cascading failures
	// will occur from subsequent environment value lookups inside the function.
	if (currentEnvironment == nullptr)
	{
		console.get()->error(stmt.get()->name,
			"[typeWizard:0203:" + currentEnvironment.get()->_namespace + "] Critical: Failed to find the environment for function '" + stmt.get()->name.get()->lexeme
			+ "'. Report this bug to the program author.");
		currentEnvironment = previous;
		return;
	}
	// Capture the namespace guard
	currentNamespaces.push_back("class:" + currentEnvironment.get()->_namespace);
	// Visit the properties
	if (stmt.get()->constructor != nullptr)
	{
		stmt.get()->constructor.get()->accept(*this);
	}
	for (std::shared_ptr<CStmt> method : stmt.get()->methods)
	{
		method.get()->accept(*this);
	}
	for (std::shared_ptr<CStmt> _property : stmt.get()->properties)
	{
		_property.get()->accept(*this);
	}
	// Exit the class
	currentEnvironment = previous;
	currentNamespaces.pop_back();
}
// CStmt_Continue
void CTypeWizard::visit(std::shared_ptr<CStmt_Continue> stmt)
{
	// Do Nothing
}
// CStmt_Expression
void CTypeWizard::visit(std::shared_ptr<CStmt_Expression> stmt)
{
	// Simple as
	stmt.get()->expression.get()->acceptTypeWizard(*this);
}
// CStmt_For
void CTypeWizard::visit(std::shared_ptr<CStmt_For> stmt)
{
	// Do not need to type check these, other languages like c++ do not
	// enforce type consistency between the init, condition, and increment
	// statements inside the 'for' statement
	// Only interested in verifying type strength of their contents

	// Capture the environment
	std::shared_ptr<CEnvironment> previous = currentEnvironment;
	currentEnvironment = currentEnvironment.get()->lookupEnvironment(stmt.get()->_namespace, false);
	// Check that the environment was actually found. This must return a value or else cascading failures
		// will occur from subsequent environment value lookups inside the function.
	if (currentEnvironment == nullptr)
	{
		console.get()->error(
			stmt.get()->line, "[typeWizard:0301:" + currentEnvironment.get()->_namespace + "] Failed to find the environment for function '" + stmt.get()->_namespace
			+ "'. Report this bug to the program author.");
		return;
	}
	currentNamespaces.push_back(currentEnvironment.get()->_namespace);
	stmt.get()->initializer.get()->accept(*this);
	stmt.get()->condition.get()->acceptTypeWizard(*this);
	stmt.get()->increment.get()->acceptTypeWizard(*this);
	// Enter into the body environment
	currentEnvironment = currentEnvironment.get()->lookupEnvironment(currentEnvironment.get()->scopes.front().get()->_namespace, false);
	// Check that the environment was actually found. This must return a value or else cascading failures
		// will occur from subsequent environment value lookups inside the function.
	if (currentEnvironment == nullptr)
	{
		console.get()->error(
			stmt.get()->line, "[typeWizard:0302:" + currentEnvironment.get()->_namespace + "] Failed to find the environment for function '" + stmt.get()->_namespace
			+ "'. Report this bug to the program author.");
		return;
	}
	currentNamespaces.push_back(currentEnvironment.get()->_namespace);
	// typeCheck the body
	stmt.get()->body.get()->accept(*this);
	// Exit the body and for environment
	currentNamespaces.pop_back();
	currentNamespaces.pop_back();
	currentEnvironment = previous;
}
// CStmt_Foreach
void CTypeWizard::visit(std::shared_ptr<CStmt_Foreach> stmt)
{
	// Capture the environment
	std::shared_ptr<CEnvironment> previous = currentEnvironment;
	currentEnvironment = currentEnvironment.get()->lookupEnvironment(stmt.get()->_namespace, false);
	// Check that the environment was actually found. This must return a value or else cascading failures
		// will occur from subsequent environment value lookups inside the function.
	if (currentEnvironment == nullptr)
	{
		console.get()->error(
			stmt.get()->line, "[typeWizard:0401:" + currentEnvironment.get()->_namespace + "] Failed to find the environment for function '" + stmt.get()->_namespace
			+ "'. Report this bug to the program author.");
		return;
	}
	currentNamespaces.push_back(currentEnvironment.get()->_namespace);
	std::shared_ptr<CToken> eachType = stmt.get()->typeName;
	std::shared_ptr<CToken> listType = stmt.get()->listExpression.get()->acceptTypeWizard(*this);
	if (!matchContainer(eachType, listType))
	{
		console.get()->error(stmt.get()->line, "[typeWizard:0402:" + currentEnvironment.get()->_namespace + "] Iterator type of '" + eachType.get()->lexeme +
			"' does not match the container type of '" + listType.get()->lexeme + "' in 'foreach' statement.");
	}
	// Enter into the body environment
	currentEnvironment = currentEnvironment.get()->lookupEnvironment(currentEnvironment.get()->scopes.front().get()->_namespace, false);
	// Check that the environment was actually found. This must return a value or else cascading failures
		// will occur from subsequent environment value lookups inside the function.
	if (currentEnvironment == nullptr)
	{
		console.get()->error(
			stmt.get()->line, "[typeWizard:0402:" + currentEnvironment.get()->_namespace + "] Failed to find the environment for function '" + stmt.get()->_namespace
			+ "'. Report this bug to the program author.");
		return;
	}
	currentNamespaces.push_back(currentEnvironment.get()->_namespace);
	// typeCheck the body
	stmt.get()->body.get()->accept(*this);
	// Exit the foreach statement
	currentNamespaces.pop_back();
	currentNamespaces.pop_back();
	currentEnvironment = previous;
}
// CStmt_Function
void CTypeWizard::visit(std::shared_ptr<CStmt_Function> stmt)
{
	if (stmt.get()->isNameDeclarationOnly || stmt.get()->scopeStack.size() != 0)
	{
		// Skip over function definitions that have a scope stack, the resolver
		// has already taken care of 'curing' the empty function declaration and has
		// filled it in with the defined function body. The original declaration was
		// able to be type checked.
		return;
	}
	else
	{
		// Save the currentReturn
		std::shared_ptr<CReturn> previousReturn = currentReturn;
		// Check return type
		if ((stmt.get()->returnType.get()->type != CTokenType::_VOID &&
			stmt.get()->returnType.get()->type != CTokenType::VOID_ARRAY) && stmt.get()->name.get()->type != CTokenType::CONSTRUCTOR)
		{
			// Non-void return expects at least one return statement
			// Set the currentReturn object
			std::vector<int> a = { 1 };
			std::vector<int> c = { 0 };
			currentReturn = std::make_shared<CReturn>(a, c);
			currentReturn.get()->returnCount = 1;
			expectedReturnStack.push_back(stmt.get()->returnType);
		}
		else
		{
			// void return expects no return, so anticipated is 0
			// Set the currentReturn object
			std::vector<int> a = { 0 };
			std::vector<int> c = { 0 };
			currentReturn = std::make_shared<CReturn>(a, c);
			expectedReturnStack.push_back(nullptr);
		}
		// Capture the environment
		std::shared_ptr<CEnvironment> previous = currentEnvironment;
		currentEnvironment = currentEnvironment.get()->lookupEnvironment(stmt.get()->name.get()->lexeme, false);
		currentNamespaces.push_back(stmt.get()->name.get()->lexeme);
		// Check that the environment was actually found. This must return a value or else cascading failures
		// will occur from subsequent environment value lookups inside the function.
		if (currentEnvironment == nullptr)
		{
			console.get()->error(stmt.get()->name,
				"[typeWizard:0501:" + currentEnvironment.get()->_namespace + "] Critical: Failed to find the environment for function '" + stmt.get()->name.get()->lexeme
				+ "'. Report this bug to the program author.");
			return;
		}
		// Type Check the block
		stmt.get()->body.get()->accept(*this);
		// Check that all logic branches have a return statement if required
		if (currentReturn.get()->checkReturns() == false)
		{
			console.get()->error(stmt.get()->name, "[typeWizard:0502:" + currentEnvironment.get()->_namespace + "] Failed to find a 'return' statement for all logic branches.");
		}
		// Exit the function
		currentEnvironment = previous;
		currentReturn = previousReturn;
		expectedReturnStack.pop_back();
		currentNamespaces.pop_back();
	}
}
// CStmt_Namespace
void CTypeWizard::visit(std::shared_ptr<CStmt_Namespace> stmt)
{
	// Set the namespace to "namespace" in order to reject return statements
	// from inside the block.
	std::shared_ptr<CEnvironment> previous = currentEnvironment;
	currentEnvironment = currentEnvironment.get()->lookupEnvironment(stmt.get()->name.get()->lexeme, false);
	// Check that the environment was actually found. This must return a value or else cascading failures
	// will occur from subsequent environment value lookups inside the function.
	if (currentEnvironment == nullptr)
	{
		console.get()->error(stmt.get()->name,
			"[typeWizard:0601:" + currentEnvironment.get()->_namespace + "] Critical: Failed to find the environment for function '" + stmt.get()->name.get()->lexeme
			+ "'. Report this bug to the program author.");
		return;
	}
	currentNamespaces.push_back("namespace");
	// typeCheck the body
	stmt.get()->body.get()->accept(*this);
	// Exit the namespace
	currentNamespaces.pop_back();
	currentEnvironment = previous;
}
// CStmt_If
void CTypeWizard::visit(std::shared_ptr<CStmt_If> stmt)
{
	// Capture the environment
	std::shared_ptr<CEnvironment> previous = currentEnvironment;
	currentEnvironment = currentEnvironment.get()->lookupEnvironment(stmt.get()->_namespace, false);
	// Check that the environment was actually found. This must return a value or else cascading failures
		// will occur from subsequent environment value lookups inside the function.
	if (currentEnvironment == nullptr)
	{
		console.get()->error(
			stmt.get()->line, "[typeWizard:0701:" + currentEnvironment.get()->_namespace + "] Failed to find the environment for function '" + stmt.get()->_namespace
			+ "'. Report this bug to the program author.");
		return;
	}
	currentNamespaces.push_back(currentEnvironment.get()->_namespace);

	// Check each condition expression, which reside in the if/else statement environment
	// The conditions are expressions, which means they may not contain variable declarations
	for (std::shared_ptr<CExpr> expression : stmt.get()->conditionsList)
	{
		std::shared_ptr<CToken> conditionType = expression.get()->acceptTypeWizard(*this);
		if (conditionType.get()->type != CTokenType::BOOL)
		{
			// Note: May get rid of this warning because the interpreter 'isTrue()' function should probably just
			// catch all literal types anyways, and usage of implicit type-cast in conditionals might be common
			// enough that warnings become annoying to users.
			console.get()->warning(stmt.get()->line, "[typeWizard:0702:" + currentEnvironment.get()->_namespace + "] Implicit type-cast from '" + conditionType.get()->lexeme +
				"' to 'boolean', possible loss of data or undefined behavior will occur.");
		}
	}

	// Handle counting of return types for when inside a function
	// Capture currentReturn
	std::shared_ptr<CReturn> previousReturn = currentReturn;
	if (currentReturn != nullptr)
	{
		// Determine number of execution branches possible for the 'if' statement, and create the anticipated/counted vectors
		std::vector<int> a;
		std::vector<int> c;
		size_t hasElse = stmt.get()->elseBranch != nullptr ? 1 : 0;
		for (int i = 0; i < (stmt.get()->executionList.size() + hasElse); i++)
		{
			a.push_back(previousReturn.get()->returnCount);
			c.push_back(0);
		}
		// Set currentReturn & add it to the previousReturn's nestedReturns stack
		currentReturn = std::make_shared<CReturn>(a, c);
		currentReturn.get()->returnCount = previousReturn.get()->returnCount;
		previousReturn.get()->nestedReturns.push_back(currentReturn);
	}

	// Capture the if/else environment
	std::shared_ptr<CEnvironment> ifElseEnv = currentEnvironment;

	for (size_t k = 0; k < stmt.get()->executionList.size(); k++)
	{
		currentEnvironment = currentEnvironment.get()->lookupEnvironment(currentEnvironment.get()->scopes[k].get()->_namespace, false);
		// Check that the environment was actually found. This must return a value or else cascading failures
		// will occur from subsequent environment value lookups inside the function.
		if (currentEnvironment == nullptr)
		{
			console.get()->error(
				stmt.get()->line, "[typeWizard:0703:" + currentEnvironment.get()->_namespace + "] Failed to find the environment for function '" + stmt.get()->_namespace
				+ "'. Report this bug to the program author.");
			return;
		}
		currentNamespaces.push_back(currentEnvironment.get()->_namespace);
		// typeCheck the block
		stmt.get()->executionList[k].get()->accept(*this);
		// Exit the block and clean up
		if (currentReturn != nullptr) { currentReturn.get()->stride++; }
		currentNamespaces.pop_back();
		currentEnvironment = ifElseEnv;
	}
	if (stmt.get()->elseBranch != nullptr)
	{
		currentEnvironment = currentEnvironment.get()->lookupEnvironment(currentEnvironment.get()->scopes.back().get()->_namespace, false);
		// Check that the environment was actually found. This must return a value or else cascading failures
		// will occur from subsequent environment value lookups inside the function.
		if (currentEnvironment == nullptr)
		{
			console.get()->error(
				stmt.get()->line, "[typeWizard:0704:" + currentEnvironment.get()->_namespace + "] Failed to find the environment for function '" + stmt.get()->_namespace
				+ "'. Report this bug to the program author.");
			return;
		}
		currentNamespaces.push_back(currentEnvironment.get()->_namespace);
		stmt.get()->elseBranch.get()->accept(*this);
		currentNamespaces.pop_back();
		currentEnvironment = ifElseEnv;
	}

	// Exit the statement
	currentEnvironment = previous;
	currentNamespaces.pop_back();
	currentReturn = previousReturn;
}
// CStmt_Print
void CTypeWizard::visit(std::shared_ptr<CStmt_Print> stmt)
{
	// typeCheck but do nothing : print accepts all types
	stmt.get()->expression.get()->acceptTypeWizard(*this);
	// Check if the print expression class is an invalid type
	if (dynamic_cast<CExpr_Assign*>(stmt.get()->expression.get()) != nullptr)
	{
		console.get()->error(stmt.get()->line, "[typeWizard:0801:" + currentEnvironment.get()->_namespace + "] Cannot print an 'assignment' expression.");
	}
	return;
}
// CStmt_Return
void CTypeWizard::visit(std::shared_ptr<CStmt_Return> stmt)
{
	if (expectedReturnStack.size() != 0)
	{

		if (currentReturn != nullptr)
		{
			// Increment the counted returns
			currentReturn.get()->countedReturns[currentReturn.get()->stride]++;
			// Throw warning if multiple returns detected
			if (currentReturn.get()->countedReturns[currentReturn.get()->stride] > 1)
			{
				console.get()->warning(stmt.get()->keyword, "[typeWizard:0901:" + currentEnvironment.get()->_namespace + "] Encountered multiple return statements.");
			}
		}
		std::shared_ptr<CToken> returnType = nullptr;
		// Set the returnType if stmt has a value
		if (stmt.get()->value != nullptr)
		{
			returnType = stmt.get()->value.get()->acceptTypeWizard(*this);
		}
		// If the returnStack is not a void return type
		if (expectedReturnStack.back() != nullptr)
		{
			if (returnType == nullptr || !matchTokens(expectedReturnStack.back(), returnType))
			{
				console.get()->error(stmt.get()->keyword, "[typeWizard:0902:" + currentEnvironment.get()->_namespace + "] Expected return type of '" +
					expectedReturnStack.back().get()->lexeme + 
					"' but got value of type '" + 
					returnType.get()->lexeme + "'.");
			}
		}
		// If the returnStack is void type but return statement had a value
		if (expectedReturnStack.back() == nullptr && returnType != nullptr)
		{
			console.get()->error(stmt.get()->keyword, 
				"[typeWizard:0903:" + currentEnvironment.get()->_namespace + "] Function has void return type, value of this return statement will be discarded.");
		}
	}
}
// CStmt_TryCatch
void CTypeWizard::visit(std::shared_ptr<CStmt_TryCatch> stmt)
{
	// Capture the environment
	std::shared_ptr<CEnvironment> previous = currentEnvironment;
	// Enter into the statement environment
	currentEnvironment = currentEnvironment.get()->lookupEnvironment(stmt.get()->_namespace, false);
	// Check that the environment was actually found. This must return a value or else cascading failures
		// will occur from subsequent environment value lookups inside the function.
	if (currentEnvironment == nullptr)
	{
		console.get()->error(
			stmt.get()->line, "[typeWizard:1001:" + currentEnvironment.get()->_namespace + "] Failed to find the environment for function '" + stmt.get()->_namespace
			+ "'. Report this bug to the program author.");
		return;
	}
	currentNamespaces.push_back(currentEnvironment.get()->_namespace);
	std::shared_ptr<CEnvironment> stmtEnv = currentEnvironment;

	// Enter into the try block environment
	currentEnvironment = currentEnvironment.get()->lookupEnvironment(currentEnvironment.get()->scopes.front().get()->_namespace, false);
	// Check that the environment was actually found. This must return a value or else cascading failures
		// will occur from subsequent environment value lookups inside the function.
	if (currentEnvironment == nullptr)
	{
		console.get()->error(
			stmt.get()->line, "[typeWizard:1002:" + currentEnvironment.get()->_namespace + "] Failed to find the environment for function '" + stmt.get()->_namespace
			+ "'. Report this bug to the program author.");
		return;
	}
	currentNamespaces.push_back(currentEnvironment.get()->_namespace);
	// typeCheck the tryBlock
	stmt.get()->tryBlock.get()->accept(*this);
	// Exit tryBlock
	currentEnvironment = stmtEnv;
	currentNamespaces.pop_back();

	// Enter into the catch block environment
	currentEnvironment = currentEnvironment.get()->lookupEnvironment(currentEnvironment.get()->scopes.back().get()->_namespace, false);
	// Check that the environment was actually found. This must return a value or else cascading failures
		// will occur from subsequent environment value lookups inside the function.
	if (currentEnvironment == nullptr)
	{
		console.get()->error(
			stmt.get()->line, "[typeWizard:1003:" + currentEnvironment.get()->_namespace + "] Failed to find the environment for function '" + stmt.get()->_namespace
			+ "'. Report this bug to the program author.");
		return;
	}
	currentNamespaces.push_back(currentEnvironment.get()->_namespace);
	// typeCheck the catchBlock
	stmt.get()->catchBlock.get()->accept(*this);
	// Exit catchBlock
	currentEnvironment = stmtEnv;
	currentNamespaces.pop_back();

	// Exit the statement
	currentNamespaces.pop_back();
	currentEnvironment = previous;
}
// CStmt_Variable
void CTypeWizard::visit(std::shared_ptr<CStmt_Variable> stmt)
{
	if (stmt.get()->initializer == nullptr)
	{
		return;
	}
	else
	{
		std::shared_ptr<CToken> varType = stmt.get()->typeName;
		std::shared_ptr<CToken> assignType = stmt.get()->initializer.get()->acceptTypeWizard(*this);

		if (!matchTokens(varType, assignType))
		{
			console.get()->error(stmt.get()->name,
				"[typeWizard:1101:" + currentEnvironment.get()->_namespace + "] Type violation: Cannot assign type '" + assignType.get()->lexeme +
				"' to variable of type '" + varType.get()->lexeme + "'.");
		}
	}
}
// CStmt_While
void CTypeWizard::visit(std::shared_ptr<CStmt_While> stmt)
{
	// Capture the environment
	std::shared_ptr<CEnvironment> previous = currentEnvironment;
	currentEnvironment = currentEnvironment.get()->lookupEnvironment(stmt.get()->_namespace, false);
	// Check that the environment was actually found. This must return a value or else cascading failures
		// will occur from subsequent environment value lookups inside the function.
	if (currentEnvironment == nullptr)
	{
		console.get()->error(
			stmt.get()->line, "[typeWizard:1201:" + currentEnvironment.get()->_namespace + "] Failed to find the environment for function '" + stmt.get()->_namespace
			+ "'. Report this bug to the program author.");
		return;
	}
	currentNamespaces.push_back(currentEnvironment.get()->_namespace);

	// typeCheck the condition expression
	std::shared_ptr<CToken> conditionType = stmt.get()->condition.get()->acceptTypeWizard(*this);
	if (conditionType == nullptr || conditionType.get()->type != CTokenType::BOOL)
	{
		console.get()->error(conditionType, "[typeWizard:1202:" + currentEnvironment.get()->_namespace + "] Condition expression for 'while' statement must evaluate to '" +
			CTokenTypeNames[(size_t)CTokenType::BOOL] + "' value.");
	}
	// Enter into the body environment
	currentEnvironment = currentEnvironment.get()->lookupEnvironment(currentEnvironment.get()->scopes.front().get()->_namespace, false);
	// Check that the environment was actually found. This must return a value or else cascading failures
		// will occur from subsequent environment value lookups inside the function.
	if (currentEnvironment == nullptr)
	{
		console.get()->error(
			stmt.get()->line, "[typeWizard:1203:" + currentEnvironment.get()->_namespace + "] Failed to find the environment for function '" + stmt.get()->_namespace
			+ "'. Report this bug to the program author.");
		return;
	}
	currentNamespaces.push_back(currentEnvironment.get()->_namespace);
	// typeCheck the body
	stmt.get()->body.get()->accept(*this);
	// Exit the foreach statement
	currentNamespaces.pop_back();
	currentNamespaces.pop_back();
	currentEnvironment = previous;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Visit Expressions
//
////////////////////////////////////////////////////////////////////////////////////////////////

// CExpr_Approximate
std::shared_ptr<CToken> CTypeWizard::visit(std::shared_ptr<CExpr_Approximate> expr)
{
	std::shared_ptr<CToken> left = expr.get()->left.get()->acceptTypeWizard(*this);
	std::shared_ptr<CToken> right = expr.get()->right.get()->acceptTypeWizard(*this);
	std::shared_ptr<CToken> value = expr.get()->value.get()->acceptTypeWizard(*this);
	if (!matchType(left, CTokenType::NUM))
	{
		console.get()->error(left, "[typeWizard:1301:" + currentEnvironment.get()->_namespace + "] Left operand in 'approximate' operation must be object of '" +
			CTokenTypeNames[(size_t)CTokenType::NUM] + "' type.");
	}
	if (!matchType(right, CTokenType::NUM))
	{
		console.get()->error(left, "[typeWizard:1302:" + currentEnvironment.get()->_namespace + "] Right operand in 'approximate' operation must be object of '" +
			CTokenTypeNames[(size_t)CTokenType::NUM] + "' type.");
	}
	if (!matchType(value, CTokenType::NUM))
	{
		console.get()->error(left, "[typeWizard:1303:" + currentEnvironment.get()->_namespace + "] Value comparator in 'approximate' operation must be object of '" +
			CTokenTypeNames[(size_t)CTokenType::NUM] + "' type.");
	}
	return std::make_shared<CToken>(CTokenType::BOOL, left.get()->line);
}
// CExpr_Array
std::shared_ptr<CToken> CTypeWizard::visit(std::shared_ptr<CExpr_Array> expr)
{
	if (expr.get()->values.size() != 0)
	{
		std::shared_ptr<CToken> arrayType = expr.get()->values[0].get()->acceptTypeWizard(*this);
		for (std::shared_ptr<CExpr> expr : expr.get()->values)
		{
			std::shared_ptr<CToken> exprType = expr.get()->acceptTypeWizard(*this);
			if (!matchTokens(arrayType, exprType))
			{
				console.get()->error(arrayType, "[typeWizard:1401:" + currentEnvironment.get()->_namespace + "] Type of '" + exprType.get()->typeString() +
					"' is not equal to the type of the array container type of '" + arrayType.get()->typeString() + "'.");
			}
		}
		return convertToContainer(arrayType);
	}
	// Should be unreachable
	return std::make_shared<CToken>(CTokenType::NIL, expr.get()->line.get()->line);
}
// CExpr_Assign
std::shared_ptr<CToken> CTypeWizard::visit(std::shared_ptr<CExpr_Assign> expr)
{
	std::shared_ptr<CToken> _operator = expr.get()->_operator;
	std::shared_ptr<CToken> left = expr.get()->identifier.get()->acceptTypeWizard(*this);
	std::shared_ptr<CToken> right = expr.get()->value.get()->acceptTypeWizard(*this);
	switch (_operator.get()->type)
	{
	case CTokenType::ASSIGN:
		if (!matchTokens(left, right))
		{
			console.get()->error(left, 
				"[typeWizard:1501:" + currentEnvironment.get()->_namespace + "] Cannot " + _operator.get()->typeString() + " value of type '" + right.get()->typeString() +
				"' to value of type '" + left.get()->typeString() + "'.");;
		}
		break;
	case CTokenType::ASSIGN_ADD:
		switch (left.get()->type)
		{
		case CTokenType::STRING:
			if (right.get()->type != CTokenType::STRING)
			{
				console.get()->error(left,
					"[typeWizard:1502:" + currentEnvironment.get()->_namespace + "] Cannot concatenate value of type '" + right.get()->typeString() +
					"' to string '" + left.get()->typeString() + "'.");;
			}
			break;
		case CTokenType::NUM:
			if (right.get()->type != CTokenType::NUM)
			{
				console.get()->error(left,
					"[typeWizard:1503:" + currentEnvironment.get()->_namespace + "] Cannot " + _operator.get()->typeString() + " value of type '" + right.get()->typeString() +
					"' to number '" + left.get()->typeString() + "'.");;
			}
			break;
		case CTokenType::VECTOR2:
		case CTokenType::VECTOR3:
		case CTokenType::VECTOR4:
			if (!matchTypes(right, { CTokenType::NUM, CTokenType::VECTOR2, CTokenType::VECTOR3, CTokenType::VECTOR4 }))
			{
				console.get()->error(left,
					"[typeWizard:1504:" + currentEnvironment.get()->_namespace + "] Cannot " + _operator.get()->typeString() + " value of type '" + right.get()->typeString() +
					"' to " + left.get()->typeString() + ".");;
			}
			break;
		default:
			console.get()->error(left, 
				"[typeWizard:1505:" + currentEnvironment.get()->_namespace + "] Cannot " + _operator.get()->typeString() + " value of type '" +
				left.get()->typeString() + 
				"'. Assignment target must be number, string, or vector.");
			break;
		}
		
		break;
	case CTokenType::ASSIGN_DIVIDE:
	case CTokenType::ASSIGN_MULTIPLY:
	case CTokenType::ASSIGN_SUBTRACT:
		switch (left.get()->type)
		{
		case CTokenType::NUM:
			if (right.get()->type != CTokenType::NUM)
			{
				console.get()->error(left,
					"[typeWizard:1506:" + currentEnvironment.get()->_namespace + "] Cannot " + _operator.get()->typeString() + " value of type '" + right.get()->typeString() +
					"' to number '" + left.get()->typeString() + "'.");;
			}
			break;
		case CTokenType::VECTOR2:
		case CTokenType::VECTOR3:
		case CTokenType::VECTOR4:
			if (!matchTypes(right, { CTokenType::NUM, CTokenType::VECTOR2, CTokenType::VECTOR3, CTokenType::VECTOR4 }))
			{
				console.get()->error(left,
					"[typeWizard:1507:" + currentEnvironment.get()->_namespace + "] Cannot " + _operator.get()->typeString() + " value of type '" + right.get()->typeString() +
					"' to vector '" + left.get()->typeString() + "'.");;
			}
			break;
		default:
			console.get()->error(left,
				"[typeWizard:1508:" + currentEnvironment.get()->_namespace + "] Cannot " + _operator.get()->typeString() + " to value of type '" +
				left.get()->typeString() +
				"'. Assignment target must be number, string, or vector.");
			break;
		}
		break;

	default:
		break;
	}
	return left;
}
// CExpr_Binary
std::shared_ptr<CToken> CTypeWizard::visit(std::shared_ptr<CExpr_Binary> expr)
{
	bool leftIsNum;
	bool rightIsNum;
	std::shared_ptr<CToken> left = expr.get()->left.get()->acceptTypeWizard(*this);
	std::shared_ptr<CToken> right = expr.get()->right.get()->acceptTypeWizard(*this);
	switch (expr.get()->_operator.get()->type)
	{
	case CTokenType::PLUS:
		if (matchType(left, CTokenType::NUM))
		{
			if (!matchTokens(left, right))
			{
				console.get()->error(left,
					"[typeWizard:1601:" + currentEnvironment.get()->_namespace + "] Cannot '" + expr.get()->_operator.get()->typeString() +
					"' object of type '" + right.get()->typeString() +
					"' to object of 'num' type.");
			}
		}
		else if (matchType(left, CTokenType::STRING))
		{
			if (!matchTokens(left, right))
			{
				console.get()->error(left, 
					"[typeWizard:1602:" + currentEnvironment.get()->_namespace + "] Cannot concatenate object of type '" + right.get()->typeString() +
					"' to object of 'string' type.");
			}
		}
		break;
	case CTokenType::MINUS:
	case CTokenType::DIVIDE:
	case CTokenType::MULTIPLY:
	case CTokenType::MODULO:
	case CTokenType::POWER:
		leftIsNum = matchType(left, CTokenType::NUM);
		rightIsNum = matchType(right, CTokenType::NUM);
		if (leftIsNum == false || rightIsNum == false)
		{
			console.get()->error(left,
				"[typeWizard:1603:" + currentEnvironment.get()->_namespace + "] Cannot '" + expr.get()->_operator.get()->typeString() +
				"' object of type '" + right.get()->typeString() +
				"' to object of '" + left.get()->typeString() + "' type."
				" Both sides of operator must be of 'num' type.");
		}
		break;
	case CTokenType::MORE:
	case CTokenType::MORE_EQUAL:
	case CTokenType::LESS:
	case CTokenType::LESS_EQUAL:
		leftIsNum = matchType(left, CTokenType::NUM);
		rightIsNum = matchType(right, CTokenType::NUM);
		if (leftIsNum == false || rightIsNum == false)
		{
			console.get()->error(left,
				"[typeWizard:1604:" + currentEnvironment.get()->_namespace + "] Cannot '" + expr.get()->_operator.get()->typeString() +
				"' object of type '" + right.get()->typeString() +
				"' to object of '" + left.get()->typeString() + "' type."
				" Both sides of operator must be of 'num' type.");
		}
		return std::make_shared<CToken>(CTokenType::BOOL, expr.get()->_operator.get()->line);
		break;
	case CTokenType::EQUAL:
	case CTokenType::NOT_EQUAL:
		// Defer check to runtime, as the left/right type does not matter in this operator
		return std::make_shared<CToken>(CTokenType::BOOL, expr.get()->_operator.get()->line);
		break;
	default:
		console.get()->error(left, "[typeWizard:1605:" + currentEnvironment.get()->_namespace + "] Encountered unexpected operator type.");
		break;
	}
	
	return left;
}
// CExpr_Call
std::shared_ptr<CToken> CTypeWizard::visit(std::shared_ptr<CExpr_Call> expr)
{
	// Establish the arguments signature
	std::vector<std::shared_ptr<CToken>> argumentTypes;
	if (expr.get()->arguments.size() != 0)
	{
		for (std::shared_ptr<CExpr> argument : expr.get()->arguments)
		{
			argumentTypes.push_back(argument.get()->acceptTypeWizard(*this));
		}
	}
	std::shared_ptr<CObject> callee = nullptr;
	// Get the type of the callee
	std::shared_ptr<CEnvironment> previous = currentEnvironment;
	bool enteredSuperClass = false;
	storeGetKey(expr, previous);
	variableGuardKey = "noexcept";
	std::shared_ptr<CToken> calleeType = expr.get()->callee.get()->acceptTypeWizard(*this);
	variableGuardKey = "";
	// Check if inside a class member function
	if (currentNamespaces.size() != 0 && calleeType.get()->type == CTokenType::NIL)
	{
		std::string thisTypeName = "";
		for (int i = (int)currentNamespaces.size() - 1; i >= 0; i--)
		{
			if (currentNamespaces[(size_t)i].find("class:") != std::string::npos)
			{
				thisTypeName = currentNamespaces[(size_t)i];
				thisTypeName.erase(thisTypeName.begin(), thisTypeName.begin() + 6);
			}
		}
		if (thisTypeName != "")
		{
			std::shared_ptr<CEnvironment> preFetch = previous;
			std::shared_ptr<CEnvironment> fetch = previous.get()->fetchEnvironment(thisTypeName);
			std::shared_ptr<CObject> fetchClass = fetch.get()->enclosing.get()->get(thisTypeName);
			if (fetchClass != nullptr && fetchClass.get()->objType.type == CLiteralTypes::_CClass)
			{
				std::shared_ptr<CToken> superToken = std::get<std::shared_ptr<CClass>>(fetchClass.get()->obj).get()->classDeclaration.get()->superclass;
				while (superToken != nullptr)
				{
					std::shared_ptr<CEnvironment> fetchSuper = previous.get()->fetchEnvironment(superToken.get()->lexeme);
					currentEnvironment = fetchSuper;
					variableGuardKey = "noexcept";
					calleeType = expr.get()->callee.get()->acceptTypeWizard(*this);
					variableGuardKey = "";
					if (calleeType.get()->type != CTokenType::NIL)
					{
						enteredSuperClass = true;
						break;
					}
					currentEnvironment = preFetch;
					std::shared_ptr<CObject> fetchSuperClass = fetchSuper.get()->enclosing.get()->get(fetchSuper.get()->_namespace);
					superToken = std::get<std::shared_ptr<CClass>>(fetchSuperClass.get()->obj).get()->classDeclaration.get()->superclass;
				}
			}
		}
	}
	
	if (calleeType.get()->type == CTokenType::IDENTIFIER ||
		calleeType.get()->type == CTokenType::FUNCTION)
	{
		callee = currentEnvironment.get()->get(calleeType.get()->lexeme);
		// If the callee is valid
		if (callee == nullptr)
		{
			console.get()->error(calleeType, "[typeWizard:1701:" + currentEnvironment.get()->_namespace + "] Cannot find object of name '" + calleeType.get()->typeString() +
				"' in current scope. Check spelling errors and verify that the object is declared.");
			checkGetKey(expr, previous);
			if (enteredSuperClass == true) { currentEnvironment = previous; }
			return std::make_shared<CToken>(CTokenType::NIL, calleeType.get()->line);
		}
		// If callee is of CClass type, this call is a constructor call
		// Also, in this case, the existence of an index operator is ignored
		else if (callee.get()->objType.type == CLiteralTypes::_CClass)
		{
			// Get the object as CClass
			std::shared_ptr<CClass> calleeClass = std::get<std::shared_ptr<CClass>>(callee.get()->obj);
			// Check if the constructor exists
			if (calleeClass.get()->classDeclaration.get()->constructor != nullptr &&
				dynamic_cast<CStmt_Function*>(calleeClass.get()->classDeclaration.get()->constructor.get()) != nullptr)
			{
				std::shared_ptr<CStmt_Function> calleeConstructor = 
					std::dynamic_pointer_cast<CStmt_Function>(calleeClass.get()->classDeclaration.get()->constructor);
				// Verify that the number of arguments match the number of parameters
				if (argumentTypes.size() != calleeConstructor.get()->paramsTypes.size())
				{
					console.get()->error(calleeType,
						"[typeWizard:1702:" + currentEnvironment.get()->_namespace + "] Expected to receive '" + std::to_string(calleeConstructor.get()->paramsTypes.size()) +
						"' arguments for '" + calleeClass.get()->name + "' class constructor but received '" +
						std::to_string(argumentTypes.size()) + "' arguments in call expression.");
				}
				// Verify that the types match
				for (size_t i = 0; i < calleeConstructor.get()->paramsTypes.size(); i++)
				{
					if (i < argumentTypes.size())
					{
						if (!matchTokens(calleeConstructor.get()->paramsTypes[i], argumentTypes[i]))
						{
							console.get()->error(argumentTypes[i], 
								"[typeWizard:1703:" + currentEnvironment.get()->_namespace + "] Argument '" + std::to_string(i) + "' of type '" + argumentTypes[i].get()->typeString() +
								"' in '" + calleeType.get()->lexeme +
								"' constructor call does not match parameter '" + std::to_string(i) + "' of type '" +
								calleeConstructor.get()->paramsTypes[i].get()->typeString() + "'.");
						}
					}
				}
			}
			// No defined constructor exists for the class, callee must now have 0 arguments.
			// Users are warned of uninitialized variables in the parser.
			else
			{
				if (argumentTypes.size() != 0)
				{
					console.get()->error(calleeType, 
						"[typeWizard:1704:" + currentEnvironment.get()->_namespace + "] Class of type '" + calleeClass.get()->name +
						"' has no defined constructor. Cannot call constructor for class '" + calleeClass.get()->name +
						"' with call expression that has '" + std::to_string(argumentTypes.size()) + "' arguments. " + 
						"Remove arguments from call, or define constructor with matching parameters list.");
				}
			}
			// Return type of class object
			checkGetKey(expr, previous);
			if (enteredSuperClass == true) { currentEnvironment = previous; }
			return calleeType;
		}
		// Case where call is a function type
		// Investigate the index associated
		else if (callee.get()->objType.type == CLiteralTypes::_CFunction)
		{
			// Get the object as CFunction
			std::shared_ptr<CStmt_Function> calleeFunction = 
				std::get<std::shared_ptr<CFunction>>(callee.get()->obj).get()->funcDeclaration;
			// Verify that the number of arguments match the number of parameters
			if (argumentTypes.size() != calleeFunction.get()->paramsTypes.size())
			{
				console.get()->error(calleeType,
					"[typeWizard:1705:" + currentEnvironment.get()->_namespace + "] Expected to receive '" + std::to_string(calleeFunction.get()->paramsTypes.size()) +
					"' arguments for '" + calleeFunction.get()->name.get()->lexeme + "' function but received '" +
					std::to_string(argumentTypes.size()) + "' arguments in call expression.");
			}
			// Verify that the types match
			for (size_t i = 0; i < calleeFunction.get()->paramsTypes.size(); i++)
			{
				if (i < argumentTypes.size())
				{
					if (!matchTokens(calleeFunction.get()->paramsTypes[i], argumentTypes[i]))
					{
						console.get()->error(argumentTypes[i],
							"[typeWizard:1706:" + currentEnvironment.get()->_namespace + "] Argument '" + std::to_string(i) + "' of type '" + argumentTypes[i].get()->typeString() +
							"' in '" + calleeType.get()->lexeme +
							"' constructor call does not match parameter '" + std::to_string(i) + "' of type '" +
							calleeFunction.get()->paramsTypes[i].get()->typeString() + "'.");
					}
				}
			}
			// If no index, return function return type
			if (expr.get()->index == nullptr)
			{
				// Return function return type
				checkGetKey(expr, previous);
				if (enteredSuperClass == true) { currentEnvironment = previous; }
				return calleeFunction.get()->returnType;
			}
			// Investigate the index operator
			else if (dynamic_cast<CExpr_Variable*>(expr.get()->index.get()) != nullptr)
			{
				std::shared_ptr<CExpr_Variable> varExpr = std::dynamic_pointer_cast<CExpr_Variable>(expr.get()->index);
				std::shared_ptr<CToken> indexType = varExpr.get()->index.get()->acceptTypeWizard(*this);
				// Verify that the index checkTypes to a number
				if (indexType.get()->type != CTokenType::NUM)
				{
					console.get()->error(calleeType, "[typeWizard:1707:" + currentEnvironment.get()->_namespace + "] Index operator after '" + calleeType.get()->lexeme +
						"' call must evaluate to type of '" + CTokenTypeNames[(size_t)CTokenType::NUM] + "'.");
				}
				std::shared_ptr<CToken> unitType = isArrayType(calleeFunction.get()->returnType);
				if (unitType.get()->type == CTokenType::NIL)
				{
					console.get()->error(calleeType,
						"[typeWizard:1708:" + currentEnvironment.get()->_namespace + "] Cannot retreive array index when function call does not return an array-type object.");
				}
				checkGetKey(expr, previous);
				if (enteredSuperClass == true) { currentEnvironment = previous; }
				return unitType;
			}
			else if (dynamic_cast<CExpr_Unary*>(expr.get()->index.get()) != nullptr)
			{
				std::shared_ptr<CExpr_Unary> unaryExpr = std::dynamic_pointer_cast<CExpr_Unary>(expr.get()->index);
				std::shared_ptr<CToken> _operator = unaryExpr.get()->_operator;
				// Check that the right-expression is a Variable, which should contain an empty name, but have
				// a valid index expression
				if (dynamic_cast<CExpr_Variable*>(unaryExpr.get()->right.get()) != nullptr)
				{
					std::shared_ptr<CExpr_Variable> varExpr = std::dynamic_pointer_cast<CExpr_Variable>(unaryExpr.get()->right);
					std::shared_ptr<CToken> indexType = varExpr.get()->index.get()->acceptTypeWizard(*this);
					// Verify that the index checkTypes to a number
					if (indexType.get()->type != CTokenType::NUM)
					{
						console.get()->error(calleeType, "[typeWizard:1709:" + currentEnvironment.get()->_namespace + "] Index operator after '" + calleeType.get()->lexeme +
							"' call must evaluate to type of '" + CTokenTypeNames[(size_t)CTokenType::NUM] + "'.");
					}
					std::shared_ptr<CToken> unitType = isArrayType(calleeFunction.get()->returnType);
					if (unitType.get()->type == CTokenType::NIL)
					{
						console.get()->error(calleeType,
							"[typeWizard:1710:" + currentEnvironment.get()->_namespace + "] Cannot retreive array index when function call does not return an array-type object.");
					}
					if (unitType.get()->type != CTokenType::NUM)
					{
						console.get()->error(calleeType,
							"[typeWizard:1711:" + currentEnvironment.get()->_namespace + "] Cannot apply '" + _operator.get()->typeString() +
							"' operator to a non 'num' type object.");
					}
					checkGetKey(expr, previous);
					if (enteredSuperClass == true) { currentEnvironment = previous; }
					return unitType;
				}
			}
			else
			{
				console.get()->error(expr.get()->line, "[typeWizard:1712:" + currentEnvironment.get()->_namespace + "] teal - Reached unfinished portion of program.");
				checkGetKey(expr, previous);
				if (enteredSuperClass == true) { currentEnvironment = previous; }
				return calleeType;
			}
			// Should be unreachable
			console.get()->error(expr.get()->line,
				"[typeWizard:1713:" + currentEnvironment.get()->_namespace + "] Encountered unexpected expression type. Report this bug to the program author.");
			checkGetKey(expr, previous);
			if (enteredSuperClass == true) { currentEnvironment = previous; }
			return calleeType;
		}
		else
		{
			console.get()->error(expr.get()->line, "[typeWizard:1714:" + currentEnvironment.get()->_namespace + "] purple - Reached unfinished portion of program.");
			checkGetKey(expr, previous);
			if (enteredSuperClass == true) { currentEnvironment = previous; }
			return calleeType;
		}
	}
	else
	{
		console.get()->error(expr.get()->line, 
			"[typeWizard:1715:" + currentEnvironment.get()->_namespace + "] Encountered call expression following a non 'identifier' type object.");
		checkGetKey(expr, previous);
		if (enteredSuperClass == true) { currentEnvironment = previous; }
		return calleeType;
	}
}
// CExpr_Get
std::shared_ptr<CToken> CTypeWizard::visit(std::shared_ptr<CExpr_Get> expr)
{
	// Get the object type
	std::shared_ptr<CToken> objType = expr.get()->object.get()->acceptTypeWizard(*this);
	if (!matchType(objType, CTokenType::IDENTIFIER))
	{
		console.get()->error(objType, "[typeWizard:1801:" + currentEnvironment.get()->_namespace + "] Cannot use get expression on non-variable object.");
		return objType;
	}

	// Lookup the object value
	std::shared_ptr<CObject> object = currentEnvironment.get()->get(objType.get()->lexeme);
	// If the object is valid
	if (object == nullptr)
	{
		console.get()->error(objType, "[typeWizard:1802:" + currentEnvironment.get()->_namespace + "] Cannot find object of name '" + objType.get()->typeString() +
			"' in current scope. Check spelling errors and verify that the object is declared.");
		return std::make_shared<CToken>(CTokenType::NIL, objType.get()->line);
	}
	// If callee is of CClass type, this call is to a class member function
	// Also, in this case, the existence of an index operator is ignored
	else if (object.get()->objType.type == CLiteralTypes::_CClass)
	{
		// Capture the environment
		std::shared_ptr<CEnvironment> previousEnvironment = currentEnvironment;
		// Get the object as CClass
		std::shared_ptr<CClass> objectClass = std::get<std::shared_ptr<CClass>>(object.get()->obj);
		std::shared_ptr<CClass> superClass;
		std::shared_ptr<CEnvironment> thisClassEnv = currentEnvironment.get()->fetchEnvironment(object.get()->objType.litName);
		std::shared_ptr<CEnvironment> superClassEnv;
		std::vector<std::string> thisFieldNames = thisClassEnv.get()->fetchValueNames();;
		std::vector<std::string> superFieldNames;
		// Check if the class has a parent class
		// Branching logic to find the fieldType depending on whether true
		if (objectClass.get()->classDeclaration.get()->superclass != nullptr)
		{
			// First check the child class fields for a match
			// Enter environment
			currentEnvironment = thisClassEnv;
			// Use the GuardKey to ensure that the fieldType visit always returns an identifier
			variableGuardKey = "class";
			// Get the field type
			lastEnvironment = previousEnvironment;
			std::shared_ptr<CToken> fieldType = expr.get()->field.get()->acceptTypeWizard(*this);
			lastEnvironment = nullptr;
			variableGuardKey = "";
			// Check if successful
			if (!matchType(fieldType, CTokenType::IDENTIFIER))
			{
				if (currentGetExprKey == "")
				{
					// Ensures that the environment is correctly restored
					// currentEnvironment = previousEnvironment;
				}
			}
			else
			{
				std::string fieldMatch = "";
				for (std::string fieldName : thisFieldNames)
				{
					if (fieldName == fieldType.get()->lexeme)
					{
						fieldMatch = fieldName;
						break;
					}
				}
				if (fieldMatch != "")
				{
					// Because the GuardKey earlier prevented fetching the unitType of an array if index != null
					// refetch the type of the field expression without setting the GuardKey, and return it.
					lastEnvironment = previousEnvironment;
					std::shared_ptr<CToken> indexType = expr.get()->field.get()->acceptTypeWizard(*this);
					lastEnvironment = nullptr;
					if (currentGetExprKey == "")
					{
						// Ensures that the environment is correctly restored
						currentEnvironment = previousEnvironment;
					}
					return indexType;
				}
				else
				{
					if (currentGetExprKey == "")
					{
						// Ensures that the environment is correctly restored
						currentEnvironment = previousEnvironment;
					}
				}
			}
			
			// Failed to find fieldType inside child class, now check superClass
			superClass = std::get<std::shared_ptr<CClass>>(
				previousEnvironment.get()->get(
					objectClass.get()->classDeclaration.get()->superclass.get()->lexeme).get()->obj);
			std::shared_ptr<CClass> previousClass;
			do
			{
				previousClass = superClass;
				superClassEnv = currentEnvironment.get()->fetchEnvironment(superClass.get()->name);
				currentEnvironment = superClassEnv;
				// Use the GuardKey to ensure that the fieldType visit always returns an identifier
				variableGuardKey = "class";
				// Get the field type
				lastEnvironment = previousEnvironment;
				std::shared_ptr<CToken> fieldType = expr.get()->field.get()->acceptTypeWizard(*this);
				lastEnvironment = nullptr;
				variableGuardKey = "";
				// Check if successful
				if (!matchType(fieldType, CTokenType::IDENTIFIER)) { }
				else
				{
					std::string fieldMatch = "";
					superFieldNames = getSuperPropertyNames(superClass);
					for (std::string superFieldName : superFieldNames)
					{
						if (superFieldName == fieldType.get()->lexeme)
						{
							fieldMatch = superFieldName;
							break;
						}
					}
					if (fieldMatch != "")
					{
						// Because the GuardKey earlier prevented fetching the unitType of an array if index != null
						// refetch the type of the field expression without setting the GuardKey, and return it.
						std::shared_ptr<CToken> indexType = expr.get()->field.get()->acceptTypeWizard(*this);
						if (currentGetExprKey == "")
						{
							// Ensures that the environment is correctly restored
							currentEnvironment = previousEnvironment;
						}
						return indexType;
					}
				}
				if (superClass.get()->classDeclaration.get()->superclass != nullptr)
				{
					superClass = std::get<std::shared_ptr<CClass>>(
						previousEnvironment.get()->get(
							superClass.get()->classDeclaration.get()->superclass.get()->lexeme).get()->obj);
				}
			} while (previousClass.get()->classDeclaration.get()->superclass != nullptr);

			// If did not exit the do-while loop by return statement, then error occured due to not finding the property name
			console.get()->error(expr.get()->fieldName, "[typeWizard:1803:" + currentEnvironment.get()->_namespace + "] Failed to locate property '" + expr.get()->fieldName.get()->typeString() +
				"' within child class or any parent classes. Check for spelling errors and that the property is defined.");
			if (currentGetExprKey == "")
			{
				// Ensures that the environment is correctly restored
				currentEnvironment = previousEnvironment;
			}
			return std::make_shared<CToken>(CTokenType::NIL, expr.get()->line.get()->line);
		}
		// No superClass block
		else
		{
			// Get the field type
			currentEnvironment = thisClassEnv;
			// Use the GuardKey to ensure that the fieldType visit always returns an identifier
			variableGuardKey = "class";
			lastEnvironment = previousEnvironment;
			std::shared_ptr<CToken> fieldType = expr.get()->field.get()->acceptTypeWizard(*this);
			lastEnvironment = nullptr;
			variableGuardKey = "";
			// Check that the fieldType is valid
			if (!matchType(fieldType, CTokenType::IDENTIFIER))
			{
				console.get()->error(fieldType, "[typeWizard:1804:" + currentEnvironment.get()->_namespace + "] Cannot use get expression on non-variable object."
					" Check spelling errors and that the member property is defined.");
				if (currentGetExprKey == "")
				{
					// Ensures that the environment is correctly restored
					currentEnvironment = previousEnvironment;
				}
				return fieldType;
			}
			std::string fieldMatch = "";
			for (std::string fieldName : thisFieldNames)
			{
				if (fieldName == fieldType.get()->lexeme)
				{
					fieldMatch = fieldName;
					break;
				}
			}
			if (fieldMatch != "")
			{
				// Because the GuardKey earlier prevented fetching the unitType of an array if index != null
				// refetch the type of the field expression without setting the GuardKey, and return it.
				lastEnvironment = previousEnvironment;
				std::shared_ptr<CToken> indexType = expr.get()->field.get()->acceptTypeWizard(*this);
				lastEnvironment = nullptr;
				if (currentGetExprKey == "")
				{
					// Ensures that the environment is correctly restored
					currentEnvironment = previousEnvironment;
				}
				return indexType;
			}
			else
			{
				if (currentGetExprKey == "")
				{
					// Ensures that the environment is correctly restored
					currentEnvironment = previousEnvironment;
				}
				return std::make_shared<CToken>(CTokenType::NIL, fieldType.get()->line);
			}
		}
	}
	console.get()->error(expr.get()->fieldName, "[typeWizard:1805:" + currentEnvironment.get()->_namespace + "] magenta - Encountered unfinished portion of program.");
	return std::make_shared<CToken>(CTokenType::NIL, expr.get()->line.get()->line);
}
// CExpr_Grouping
std::shared_ptr<CToken> CTypeWizard::visit(std::shared_ptr<CExpr_Grouping> expr)
{
	// Simple as
	return expr.get()->expression.get()->acceptTypeWizard(*this);
}
// CExpr_InstanceOf
std::shared_ptr<CToken> CTypeWizard::visit(std::shared_ptr<CExpr_InstanceOf> expr)
{
	// Defer this type check to runtime, as that is the purpose of the operator type
	return std::make_shared<CToken>(CTokenType::BOOL, expr.get()->line.get()->line);
}
// CExpr_Literal
std::shared_ptr<CToken> CTypeWizard::visit(std::shared_ptr<CExpr_Literal> expr)
{
	if (matchType(expr.get()->value, CTokenType::_THIS))
	{
		if (currentNamespaces.size() == 0)
		{
			console.get()->error(expr.get()->value,
				"[typeWizard:1901:" + currentEnvironment.get()->_namespace + "] Cannot use get expression on 'this' object from outside a class member function.");
			return expr.get()->value;
		}
		std::string thisTypeName = "";
		for (int i = (int)currentNamespaces.size() - 1; i >= 0; i--)
		{
			if (currentNamespaces[(size_t)i].find("class:") != std::string::npos)
			{
				thisTypeName = currentNamespaces[(size_t)i];
				thisTypeName.erase(thisTypeName.begin(), thisTypeName.begin() + 6);
			}
		}
		if (thisTypeName == "")
		{
			console.get()->error(expr.get()->value,
				"[typeWizard:1902:" + currentEnvironment.get()->_namespace + "] Cannot use get expression on 'this' object from outside a class member function.");
			return expr.get()->value;
		}
		return std::make_shared<CToken>(CTokenType::IDENTIFIER, thisTypeName, expr.get()->value.get()->line);
	}
	return expr.get()->value;
}
// CExpr_Logical
std::shared_ptr<CToken> CTypeWizard::visit(std::shared_ptr<CExpr_Logical> expr)
{
	std::shared_ptr<CToken> left = expr.get()->left.get()->acceptTypeWizard(*this);
	std::shared_ptr<CToken> right = expr.get()->right.get()->acceptTypeWizard(*this);
	if (!matchType(left, CTokenType::BOOL))
	{
		console.get()->error(left, 
			"[typeWizard:2001:" + currentEnvironment.get()->_namespace + "] Left operand of '" + expr.get()->_operator.get()->typeString() + "' operator must be of '" +
			CTokenTypeNames[(size_t)CTokenType::BOOL] + "' type.");
	}
	if (!matchType(right, CTokenType::BOOL))
	{
		console.get()->error(left,
			"[typeWizard:2002:" + currentEnvironment.get()->_namespace + "] Left operand of '" + expr.get()->_operator.get()->typeString() + "' operator must be of '" +
			CTokenTypeNames[(size_t)CTokenType::BOOL] + "' type.");
	}
	return std::make_shared<CToken>(CTokenType::BOOL, expr.get()->_operator.get()->line);
}
// CExpr_Scope
std::shared_ptr<CToken> CTypeWizard::visit(std::shared_ptr<CExpr_Scope> expr)
{
	if (dynamic_cast<CExpr_Variable*>(expr.get()->right.get()) != nullptr ||
		dynamic_cast<CExpr_Get*>(expr.get()->right.get()) != nullptr || 
		dynamic_cast<CExpr_Call*>(expr.get()->right.get()) != nullptr)
	{
		// No error, received expected class type for the right-hand side of the
		// scope resolution operator.
	}
	else
	{
		console.get()->error(expr.get()->scopeStack.get()->front(), 
			"[typeWizard:2102:" + currentEnvironment.get()->_namespace + "] Scope resolution operators can only applied to a variable, get expression, or call expression.");
	}
	// Capture currentEnvironment
	std::shared_ptr<CEnvironment> previous = currentEnvironment;
	// Find and enter the environment from the scopeStack
	currentEnvironment = currentEnvironment.get()->lookupEnvironment(expr);
	if (currentEnvironment != nullptr)
	{
		// From this environment, typeCheck the var/get/call expression
		std::shared_ptr<CToken> rightType = expr.get()->right.get()->acceptTypeWizard(*this);
		// Reset the environment
		currentEnvironment = previous;
		// Return the type of what was found
		return rightType;
	}
	// Failure
	else
	{
		// Reset the environment
		currentEnvironment = previous;
		console.get()->error(expr.get()->scopeStack.get()->front(),
			"[typeWizard:2103:" + currentEnvironment.get()->_namespace + "] Lookup of object at tail of scope resolution operator failed, check spelling and variable declarations.");
		// Return Nil
		return std::make_shared<CToken>(CTokenType::NIL, expr.get()->scopeStack.get()->front().get()->line);
	}
}
// CExpr_TypeCast
std::shared_ptr<CToken> CTypeWizard::visit(std::shared_ptr<CExpr_TypeCast> expr)
{
	std::shared_ptr<CToken> type = expr.get()->type;
	std::shared_ptr<CToken> right = expr.get()->right.get()->acceptTypeWizard(*this);
	if (!isValidCast(type, right))
	{
		console.get()->error(type, "[typeWizard:2201:" + currentEnvironment.get()->_namespace + "] Cannot cast value of type '" + right.get()->typeString() +
			"' to value of type '" + type.get()->typeString() + "'.");
	}
	return type;
}
// CExpr_Unary
std::shared_ptr<CToken> CTypeWizard::visit(std::shared_ptr<CExpr_Unary> expr)
{
	std::shared_ptr<CToken> rightType = expr.get()->right.get()->acceptTypeWizard(*this);
	switch (expr.get()->_operator.get()->type)
	{
	case CTokenType::FALSIFY:
		if (!matchType(rightType, CTokenType::BOOL))
		{
			console.get()->error(rightType, "[typeWizard:2301:" + currentEnvironment.get()->_namespace + "] Falsify operator '!' must be used on object of '" +
				CTokenTypeNames[(size_t)CTokenType::BOOL] + "' type.");
		}
		break;
	case CTokenType::NEGATE:
		if (!matchType(rightType, CTokenType::NUM))
		{
			console.get()->error(rightType, "[typeWizard:2302:" + currentEnvironment.get()->_namespace + "] Negate operator '-' must be used on object of '" +
				CTokenTypeNames[(size_t)CTokenType::NUM] + "' type.");
		}
		break;
	case CTokenType::INCREMENT:
		if (!matchType(rightType, CTokenType::NUM))
		{
			console.get()->error(rightType, "[typeWizard:2303:" + currentEnvironment.get()->_namespace + "] Increment operator '++' must be used on object of '" +
				CTokenTypeNames[(size_t)CTokenType::NUM] + "' type.");
		}
		break;
	case CTokenType::DECREMENT:
		if (!matchType(rightType, CTokenType::NUM))
		{
			console.get()->error(rightType, "[typeWizard:2304:" + currentEnvironment.get()->_namespace + "] Decrement operator '--' must be used on object of '" +
				CTokenTypeNames[(size_t)CTokenType::NUM] + "' type.");
		}
		break;
	default:
		break;
	}
	return rightType;
}
// CExpr_Variable
std::shared_ptr<CToken> CTypeWizard::visit(std::shared_ptr<CExpr_Variable> expr)
{
	// First verify that the index expression typeChecks to a number object
	// There is no need to check the type of the object at the index, because
	// the array must only contain objects of the same type.
	std::shared_ptr<CToken> indexType = nullptr;
	if (expr.get()->index != nullptr)
	{
		indexGuardKey = "index";
		if (lastEnvironment != nullptr)
		{
			std::shared_ptr<CEnvironment> previous = currentEnvironment;
			currentEnvironment = lastEnvironment;
			indexType = expr.get()->index.get()->acceptTypeWizard(*this);
			currentEnvironment = previous;
		}
		else
		{
			indexType = expr.get()->index.get()->acceptTypeWizard(*this);
		}
		if (!matchType(indexType, CTokenType::NUM))
		{
			console.get()->error(indexType,
				"[typeWizard:2401:" + currentEnvironment.get()->_namespace + "] Expression inside of index operator '[]' must return an object of type '" +
				CTokenTypeNames[(size_t)CTokenType::NUM] + "'.");
		}
		indexGuardKey = "";
	}
	std::shared_ptr<CObject> getVar = nullptr;
	// Next check that the variable resolves, then report back its type
	getVar = currentEnvironment.get()->get(expr.get()->name.get()->lexeme);
	if (getVar == nullptr && checkNamespaces("class:"))
	{
		std::shared_ptr<CEnvironment> classEnv = getNamespaceEnvironment("class:");
		getVar = classEnv.get()->get(expr.get()->name.get()->lexeme);
		if (getVar == nullptr)
		{
			std::shared_ptr<CClass> classObj = 
				std::get<std::shared_ptr<CClass>>(
					classEnv.get()->enclosing.get()->get(classEnv.get()->_namespace).get()->obj);
			std::shared_ptr<CClass> superClass = classObj.get()->superClass;
			while (superClass != nullptr && getVar == nullptr)
			{
				getVar = superClass.get()->classEnv.get()->get(expr.get()->name.get()->lexeme);
				superClass = superClass.get()->superClass;
			}
		}
	}
	// Create the token and return it
	if (getVar == nullptr)
	{
		if (variableGuardKey == "")
		{
			console.get()->error(expr.get()->name,
				"[typeWizard:2402:" + currentEnvironment.get()->_namespace + "] Failed to locate variable named '" + expr.get()->name.get()->lexeme +
				"' in the current environment. Check for spelling errors and that the variable is declared.");
		}
		return std::make_shared<CToken>(CTokenType::NIL, expr.get()->name.get()->line);
	}
	else
	{
		if (variableGuardKey != "" && variableGuardKey != "noexcept" && indexGuardKey == "")
		{
			return std::make_shared<CToken>(CTokenType::IDENTIFIER, getVar.get()->objName, expr.get()->name.get()->line);
		}
		else if (expr.get()->index == nullptr)
		{
			return makeCToken(getVar, expr.get()->name.get()->line);
		}
		else
		{
			return convertToUnit(getVar, expr.get()->name.get()->line);
		}
	}
}
// CExpr_Vector
std::shared_ptr<CToken> CTypeWizard::visit(std::shared_ptr<CExpr_Vector> expr)
{
	CTokenType kind = expr.get()->kind.get()->type;
	std::shared_ptr<CToken> x, y, z, w;
	if (kind == CTokenType::VECTOR2 || kind == CTokenType::VECTOR3 || kind == CTokenType::VECTOR4)
	{
		x = expr.get()->x.get()->acceptTypeWizard(*this);
		y = expr.get()->y.get()->acceptTypeWizard(*this);

		if (!matchType(x, CTokenType::NUM))
		{
			console.get()->error(expr.get()->kind, "[typeWizard:2501:" + currentEnvironment.get()->_namespace + "] Vector value 'x' must be a number value.");
		}
		if (!matchType(y, CTokenType::NUM))
		{
			console.get()->error(expr.get()->kind, "[typeWizard:2502:" + currentEnvironment.get()->_namespace + "] Vector value 'y' must be a number value.");
		}
	}
	if (kind == CTokenType::VECTOR3 || kind == CTokenType::VECTOR4)
	{
		z = expr.get()->z.get()->acceptTypeWizard(*this);
		if (!matchType(z, CTokenType::NUM))
		{
			console.get()->error(expr.get()->kind, "[typeWizard:2503:" + currentEnvironment.get()->_namespace + "] Vector value 'z' must be a number value.");
		}
	}
	if (kind == CTokenType::VECTOR4)
	{
		w = expr.get()->w.get()->acceptTypeWizard(*this);
		if (!matchType(w, CTokenType::NUM))
		{
			console.get()->error(expr.get()->kind, "[typeWizard:2504:" + currentEnvironment.get()->_namespace + "] Vector value 'w' must be a number value.");
		}
	}
	return expr.get()->kind;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// CTypeWizard Utility Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

//
bool CTypeWizard::checkNamespaces(std::string delim)
{
	if (currentNamespaces.size() == 0) { return false; }
	else
	{
		for (int i = (int)currentNamespaces.size() - 1; i >= 0; i--)
		{
			if (currentNamespaces[(size_t)i].find(delim)) { return true; }
		}
	}
	return false;
}
//
std::shared_ptr<CEnvironment> CTypeWizard::getNamespaceEnvironment(std::string delim)
{
	if (currentNamespaces.size() == 0) { return nullptr; }
	else
	{
		for (int i = (int)currentNamespaces.size() - 1; i >= 0; i--)
		{
			if (currentNamespaces[(size_t)i].find(delim) != std::string::npos) 
			{
				size_t find = currentNamespaces[(size_t)i].find(delim);
				std::string name = currentNamespaces[(size_t)i].substr(find + delim.size());
				return currentEnvironment.get()->fetchEnvironment(name);
			}
		}
	}
	return nullptr;
}

//
void CTypeWizard::storeGetKey(std::shared_ptr<CExpr_Call> expr, std::shared_ptr<CEnvironment> environment)
{
	// Check whether this expression's callee is of type CExpr_Get
	// If it is, store the field name as an identifier key, because the get expression
	// will dive into a new environment when it get's visited.
	if (dynamic_cast<CExpr_Get*>(expr.get()->callee.get()) != nullptr)
	{
		if (currentGetExprKey == "")
		{
			currentGetExprKey = 
				std::dynamic_pointer_cast<CExpr_Get>(expr.get()->callee).get()->fieldName.get()->lexeme + environment.get()->_namespace;
			//lastEnvironment = environment;
		}
	}
	else if (dynamic_cast<CExpr_Variable*>(expr.get()->callee.get()) != nullptr)
	{
		if (currentGetExprKey == "")
		{
			currentGetExprKey = 
				std::dynamic_pointer_cast<CExpr_Variable>(expr.get()->callee).get()->name.get()->lexeme + environment.get()->_namespace;
			//lastEnvironment = environment;
		}
	}
}
void CTypeWizard::checkGetKey(std::shared_ptr<CExpr_Call> expr, std::shared_ptr<CEnvironment> previous)
{
	// When the key is encountered again (when the get expression has now fully unwound)
	// Reset the environment
	if (dynamic_cast<CExpr_Get*>(expr.get()->callee.get()) != nullptr)
	{
		if (currentGetExprKey == 
			std::dynamic_pointer_cast<CExpr_Get>(expr.get()->callee).get()->fieldName.get()->lexeme + previous.get()->_namespace)
		{
			currentGetExprKey = "";
			currentEnvironment = previous;
			//lastEnvironment = nullptr;
		}
	}
	else if (dynamic_cast<CExpr_Variable*>(expr.get()->callee.get()) != nullptr)
	{
		if (currentGetExprKey ==
			std::dynamic_pointer_cast<CExpr_Variable>(expr.get()->callee).get()->name.get()->lexeme + previous.get()->_namespace)
		{
			currentGetExprKey = "";
			currentEnvironment = previous;
			//lastEnvironment = nullptr;
		}
	}
}


//
std::vector<std::string> CTypeWizard::getSuperPropertyNames(std::shared_ptr<CObject> superClass)
{
	std::shared_ptr<CEnvironment> superClassEnv = currentEnvironment.get()->fetchEnvironment(superClass.get()->objName);
	std::vector<std::string> superFieldNames = superClassEnv.get()->fetchValueNames();
	std::shared_ptr<CClass> superSuper = std::get<std::shared_ptr<CClass>>(superClass.get()->obj);
	while (superSuper.get()->classDeclaration.get()->superclass != nullptr)
	{
		std::string lookup = superSuper.get()->classDeclaration.get()->superclass.get()->lexeme;
		std::shared_ptr<CEnvironment> superSuperEnv = currentEnvironment.get()->fetchEnvironment(lookup);
		std::vector<std::string> superSuperNames = superSuperEnv.get()->fetchValueNames();
		superFieldNames.insert(superFieldNames.end(), superSuperNames.begin(), superSuperNames.end());
		std::shared_ptr<CObject> objTemp = currentEnvironment.get()->get(lookup);
		superSuper = std::get<std::shared_ptr<CClass>>(objTemp.get()->obj);
	}
	return superFieldNames;
}
//
std::vector<std::string> CTypeWizard::getSuperPropertyNames(std::shared_ptr<CClass> superClass)
{
	std::shared_ptr<CEnvironment> superClassEnv = currentEnvironment.get()->fetchEnvironment(superClass.get()->name);
	std::vector<std::string> superFieldNames = superClassEnv.get()->fetchValueNames();
	return superFieldNames;
}

//
bool CTypeWizard::isValidCast(std::shared_ptr<CToken> type, std::shared_ptr<CToken> query)
{
	// <type>query - Case of query is identifier, which means it is object of class or function type
	if (matchTypes(query, { CTokenType::IDENTIFIER, CTokenType::IDENTIFIER_ARRAY }))
	{
		// Lookup the class environment (must be class)
		std::shared_ptr<CEnvironment> queryEnv = currentEnvironment.get()->fetchEnvironment(query.get()->lexeme).get()->enclosing;
		std::shared_ptr<CObject> queryObj = queryEnv.get()->get(query.get()->lexeme);
		if (queryObj.get()->objType.type != CLiteralTypes::_CClass) { return false; }
		std::shared_ptr<CStmt_Class> queryClass = std::get<std::shared_ptr<CClass>>(queryObj.get()->obj).get()->classDeclaration;
		for (std::shared_ptr<CStmt> classFunc : queryClass.get()->methods)
		{
			if (dynamic_cast<CStmt_Function*>(classFunc.get()) != nullptr)
			{
				std::shared_ptr<CToken> nameToken = std::dynamic_pointer_cast<CStmt_Function>(classFunc).get()->name;
				std::shared_ptr<CToken> returnToken = std::dynamic_pointer_cast<CStmt_Function>(classFunc).get()->returnType;
				if (nameToken.get()->type == CTokenType::CAST)
				{
					if (returnToken.get()->typeString() == type.get()->typeString())
					{
						return true;
					}
				}
			}
		}
	}
	else
	{
		// <type>query
		switch (query.get()->type)
		{
		case CTokenType::NUM:
			if (matchTypes(type, { CTokenType::NUM, CTokenType::BOOL, CTokenType::STRING }))
			{
				return true;
			}
			else { return false; }
			break;
		case CTokenType::STRING:
			if (matchTypes(type, { CTokenType::STRING, CTokenType::BOOL }))
			{
				return true;
			}
			else { return false; }
			break;
		case CTokenType::BOOL:
			if (matchTypes(type, { CTokenType::BOOL, CTokenType::NUM, CTokenType::STRING }))
			{
				return true;
			}
			else { return false; }
			break;
		case CTokenType::VECTOR2:
			if (matchTypes(type, { CTokenType::VECTOR2, CTokenType::STRING, CTokenType::VECTOR3, CTokenType::VECTOR4 }))
			{
				return true;
			}
			else { return false; }
			break;
		case CTokenType::VECTOR3:
			if (matchTypes(type, { CTokenType::VECTOR3, CTokenType::STRING, CTokenType::VECTOR2, CTokenType::VECTOR4 }))
			{
				return true;
			}
			else { return false; }
			break;
		case CTokenType::VECTOR4:
			if (matchTypes(type, { CTokenType::VECTOR4, CTokenType::STRING, CTokenType::VECTOR2, CTokenType::VECTOR3 }))
			{
				return true;
			}
			else { return false; }
			break;
		default:
			return false;
			break;
		}
	}
	return false;
}
//
std::shared_ptr<CToken> CTypeWizard::isArrayType(std::shared_ptr<CToken> type)
{
	switch (type.get()->type)
	{
	case CTokenType::IDENTIFIER_ARRAY:
		return std::make_shared<CToken>(CTokenType::IDENTIFIER, type.get()->lexeme, type.get()->line);
		break;
	case CTokenType::NUM_ARRAY:
		return std::make_shared<CToken>(CTokenType::NUM, type.get()->line);
		break;
	case CTokenType::STRING_ARRAY:
		return std::make_shared<CToken>(CTokenType::STRING, type.get()->line);
		break;
	case CTokenType::BOOL_ARRAY:
		return std::make_shared<CToken>(CTokenType::BOOL, type.get()->line);
		break;
	case CTokenType::VECTOR2_ARRAY:
		return std::make_shared<CToken>(CTokenType::VECTOR2, type.get()->line);
		break;
	case CTokenType::VECTOR3_ARRAY:
		return std::make_shared<CToken>(CTokenType::VECTOR3, type.get()->line);
		break;
	case CTokenType::VECTOR4_ARRAY:
		return std::make_shared<CToken>(CTokenType::VECTOR4, type.get()->line);
		break;
	case CTokenType::VOID_ARRAY:
		return std::make_shared<CToken>(CTokenType::_VOID, type.get()->line);
		break;
	default:
		return std::make_shared<CToken>(CTokenType::NIL, type.get()->line);
	}
}
//
std::shared_ptr<CToken> CTypeWizard::convertToContainer(std::shared_ptr<CToken> unitType)
{
	switch (unitType.get()->type)
	{
	case CTokenType::IDENTIFIER:
		return std::make_shared<CToken>(CTokenType::IDENTIFIER_ARRAY, unitType.get()->lexeme, unitType.get()->line);
		break;
	case CTokenType::NUM:
		return std::make_shared<CToken>(CTokenType::NUM_ARRAY, unitType.get()->line);
		break;
	case CTokenType::STRING:
		return std::make_shared<CToken>(CTokenType::STRING_ARRAY, unitType.get()->line);
		break;
	case CTokenType::BOOL:
		return std::make_shared<CToken>(CTokenType::BOOL_ARRAY, unitType.get()->line);
		break;
	case CTokenType::VECTOR2:
		return std::make_shared<CToken>(CTokenType::VECTOR2_ARRAY, unitType.get()->line);
		break;
	case CTokenType::VECTOR3:
		return std::make_shared<CToken>(CTokenType::VECTOR3_ARRAY, unitType.get()->line);
		break;
	case CTokenType::VECTOR4:
		return std::make_shared<CToken>(CTokenType::VECTOR4_ARRAY, unitType.get()->line);
		break;
	case CTokenType::_VOID:
		return std::make_shared<CToken>(CTokenType::VOID_ARRAY, unitType.get()->line);
		break;
	default:
		return std::make_shared<CToken>(CTokenType::NIL, unitType.get()->line);
	}
}
//
bool CTypeWizard::matchTokens(std::shared_ptr<CToken> left, std::shared_ptr<CToken> right)
{
	// Check for Any type. This is an internal type used for simplifying native function/class calls
	// that allow multiple input types. Common example is for setProperty_byName(string name, any value)
	// Be aware that while powerful, this allows invalid std::get on objects when fetching the value
	// of a function that returns Any
	if (left.get()->type == CTokenType::ANY) { return true; }
	if (right.get()->type == CTokenType::ANY) { return true; }
	if (right.get()->type == CTokenType::ANY_ARRAY) 
	{ 
		switch (left.get()->type)
		{
		case CTokenType::BOOL_ARRAY:
		case CTokenType::NUM_ARRAY:
		case CTokenType::IDENTIFIER_ARRAY:
		case CTokenType::FUNCTION_ARRAY:
		case CTokenType::STRING_ARRAY:
		case CTokenType::VECTOR2_ARRAY:
		case CTokenType::VECTOR3_ARRAY:
		case CTokenType::VECTOR4_ARRAY:
			return true;
		}
		return false;
	}
	// Else is regular function
	switch (left.get()->type)
	{
	case CTokenType::IDENTIFIER:
		if (right.get()->type == CTokenType::IDENTIFIER) 
		{ 
			if (left.get()->lexeme == right.get()->lexeme)
			{
				return true;
			}
			else { return false; }
		}
		else { return false; }
		break;
	case CTokenType::IDENTIFIER_ARRAY:
		if (right.get()->type == CTokenType::IDENTIFIER_ARRAY) 
		{ 
			if (left.get()->lexeme == right.get()->lexeme)
			{
				return true;
			}
			else { return false; }
		}
		else { return false; }
		break;
	case CTokenType::NUM:
		if (right.get()->type == CTokenType::NUM) { return true; }
		else { return false; }
		break;
	case CTokenType::NUM_ARRAY:
		if (right.get()->type == CTokenType::NUM_ARRAY) { return true; }
		else { return false; }
		break;
	case CTokenType::STRING:
		if (right.get()->type == CTokenType::STRING) { return true; }
		else { return false; }
		break;
	case CTokenType::STRING_ARRAY:
		if (right.get()->type == CTokenType::STRING_ARRAY) { return true; }
		else { return false; }
		break;
	case CTokenType::BOOL:
		if (right.get()->type == CTokenType::BOOL) { return true; }
		else { return false; }
		break;
	case CTokenType::BOOL_ARRAY:
		if (right.get()->type == CTokenType::BOOL_ARRAY) { return true; }
		else { return false; }
		break;
	case CTokenType::FUNCTION:
		if (right.get()->type == CTokenType::FUNCTION) { return true; }
		else { return false; }
		break;
	case CTokenType::FUNCTION_ARRAY:
		if (right.get()->type == CTokenType::FUNCTION_ARRAY) { return true; }
		else { return false; }
		break;
	case CTokenType::VECTOR2:
		if (right.get()->type == CTokenType::VECTOR2) { return true; }
		else { return false; }
		break;
	case CTokenType::VECTOR2_ARRAY:
		if (right.get()->type == CTokenType::VECTOR2_ARRAY) { return true; }
		else { return false; }
		break;
	case CTokenType::VECTOR3:
		if (right.get()->type == CTokenType::VECTOR3) { return true; }
		else { return false; }
		break;
	case CTokenType::VECTOR3_ARRAY:
		if (right.get()->type == CTokenType::VECTOR3_ARRAY) { return true; }
		else { return false; }
		break;
	case CTokenType::VECTOR4:
		if (right.get()->type == CTokenType::VECTOR4) { return true; }
		else { return false; }
		break;
	case CTokenType::VECTOR4_ARRAY:
		if (right.get()->type == CTokenType::VECTOR4_ARRAY) { return true; }
		else { return false; }
		break;
	case CTokenType::_THIS:
		if (right.get()->type == CTokenType::_THIS) { return true; }
		else { return false; }
		break;
	default:
		return false;
	}
}
bool CTypeWizard::matchType(std::shared_ptr<CToken> left, CTokenType right)
{
	CTokenType leftType = left.get()->type;
	if (leftType == CTokenType::ANY || right == CTokenType::ANY)
	{
		return true;
	}
	else if (leftType != right)
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool CTypeWizard::matchTypes(std::shared_ptr<CToken> query, std::vector<CTokenType> types)
{
	CTokenType queryType = query.get()->type;
	bool foundMatch = false;
	for (CTokenType check : types)
	{
		if (queryType == check)
		{
			foundMatch = true;
		}
	}
	return foundMatch;
}

//
bool CTypeWizard::matchContainer(std::shared_ptr<CToken> iterator, std::shared_ptr<CToken> container)
{
	switch (iterator.get()->type)
	{
	case CTokenType::IDENTIFIER:
		if (container.get()->type == CTokenType::IDENTIFIER_ARRAY)
		{
			if (iterator.get()->lexeme == container.get()->lexeme)
			{
				return true;
			}
			else { return false; }
		}
		else { return false; }
		break;
	case CTokenType::NUM:
		if (container.get()->type == CTokenType::NUM_ARRAY) { return true; }
		else { return false; }
		break;
	case CTokenType::STRING:
		if (container.get()->type == CTokenType::STRING_ARRAY) { return true; }
		else { return false; }
		break;
	case CTokenType::BOOL:
		if (container.get()->type == CTokenType::BOOL_ARRAY) { return true; }
		else { return false; }
		break;
	case CTokenType::VECTOR2:
		if (container.get()->type == CTokenType::VECTOR2_ARRAY) { return true; }
		else { return false; }
		break;
	case CTokenType::VECTOR3:
		if (container.get()->type == CTokenType::VECTOR3_ARRAY) { return true; }
		else { return false; }
		break;
	case CTokenType::VECTOR4:
		if (container.get()->type == CTokenType::VECTOR4_ARRAY) { return true; }
		else { return false; }
		break;
	case CTokenType::_VOID:
	case CTokenType::VOID_ARRAY:
		return false;
		break;
	default:
		return false;
	}
}

//
std::shared_ptr<CToken> CTypeWizard::makeCToken(std::shared_ptr<CObject> object, int line)
{
	switch (object.get()->objType.type)
	{
	case CLiteralTypes::_CNumber:
		return std::make_shared<CToken>(CTokenType::NUM, line);
		break;
	case CLiteralTypes::_CNumber_Array:
		return std::make_shared<CToken>(CTokenType::NUM_ARRAY, line);
		break;
	case CLiteralTypes::_CString:
		return std::make_shared<CToken>(CTokenType::STRING, line);
		break;
	case CLiteralTypes::_CString_Array:
		return std::make_shared<CToken>(CTokenType::STRING_ARRAY, line);
		break;
	case CLiteralTypes::_CBool:
		return std::make_shared<CToken>(CTokenType::BOOL, line);
		break;
	case CLiteralTypes::_CBool_Array:
		return std::make_shared<CToken>(CTokenType::BOOL_ARRAY, line);
		break;
	case CLiteralTypes::_CFunction:
		return std::make_shared<CToken>(CTokenType::FUNCTION, object.get()->objType.litName, line);
		break;
	case CLiteralTypes::_CFunction_Array:
		return std::make_shared<CToken>(CTokenType::FUNCTION_ARRAY, object.get()->objType.litName, line);
		break;
	case CLiteralTypes::_CInstance:
		return std::make_shared<CToken>(CTokenType::IDENTIFIER, object.get()->objType.litName, line);
		break;
	case CLiteralTypes::_CInstance_Array:
		return std::make_shared<CToken>(CTokenType::IDENTIFIER_ARRAY, object.get()->objType.litName, line);
		break;
	case CLiteralTypes::_CClass:
		return std::make_shared<CToken>(CTokenType::IDENTIFIER, object.get()->objType.litName, line);
		break;
	case CLiteralTypes::_CThis:
		return std::make_shared<CToken>(CTokenType::_THIS, line);
		break;
	case CLiteralTypes::_CVec2:
		return std::make_shared<CToken>(CTokenType::VECTOR2, line);
		break;
	case CLiteralTypes::_CVec2_Array:
		return std::make_shared<CToken>(CTokenType::VECTOR2_ARRAY, line);
		break;
	case CLiteralTypes::_CVec3:
		return std::make_shared<CToken>(CTokenType::VECTOR3, line);
		break;
	case CLiteralTypes::_CVec3_Array:
		return std::make_shared<CToken>(CTokenType::VECTOR3_ARRAY, line);
		break;
	case CLiteralTypes::_CVec4:
		return std::make_shared<CToken>(CTokenType::VECTOR4, line);
		break;
	case CLiteralTypes::_CVec4_Array:
		return std::make_shared<CToken>(CTokenType::VECTOR4_ARRAY, line);
		break;
	default:
		return std::make_shared<CToken>(CTokenType::NIL, line);
		break;
	}
}

//
std::shared_ptr<CToken> CTypeWizard::convertToUnit(std::shared_ptr<CObject> object, int line)
{
	// Note that arrayType is not used, it is only here to use overloading
	switch (object.get()->objType.type)
	{
	case CLiteralTypes::_CNumber_Array:
		return std::make_shared<CToken>(CTokenType::NUM, line);
		break;
	case CLiteralTypes::_CString_Array:
		return std::make_shared<CToken>(CTokenType::STRING, line);
		break;
	case CLiteralTypes::_CBool_Array:
		return std::make_shared<CToken>(CTokenType::BOOL, line);
		break;
	case CLiteralTypes::_CFunction_Array:
		return std::make_shared<CToken>(CTokenType::IDENTIFIER, object.get()->objType.litName, line);
		break;
	case CLiteralTypes::_CInstance_Array:
		return std::make_shared<CToken>(CTokenType::IDENTIFIER, object.get()->objType.litName, line);
		break;
	case CLiteralTypes::_CVec2_Array:
		return std::make_shared<CToken>(CTokenType::VECTOR2, line);
		break;
	case CLiteralTypes::_CVec3_Array:
		return std::make_shared<CToken>(CTokenType::VECTOR3, line);
		break;
	case CLiteralTypes::_CVec4_Array:
		return std::make_shared<CToken>(CTokenType::VECTOR4, line);
		break;
	default:
		console.get()->error((size_t)line, "[typeWizard:" + currentEnvironment.get()->_namespace + "] Cannot fetch index of non 'array' type object.");
		return std::make_shared<CToken>(CTokenType::NIL, line);
		break;
	}
}