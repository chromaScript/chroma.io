#include "../include/cscript/CInterpreter.h"
#include "../include/cscript/CEnvironment.h"
#include "../include/cscript/ChromaScript.h"
#include "../include/cscript/CForward.h"
#include "../include/cscript/CExpr.h"
#include "../include/cscript/CStmt.h"
#include "../include/cscript/CEnums.h"
#include "../include/cscript/CToken.h"
#include "../include/cscript/CObject.h"
#include "../include/cscript/CCallable.h"
#include "../include/cscript/CCallable_Lib.h"
#include "../include/entities/WidgetStyle.h"

#include <string>
#include <vector>
#include <exception>

//
CInterpreter::CInterpreter()
{

}

void CInterpreter::initialize(std::shared_ptr<ChromaScript> console)
{
	this->console = console;
}

//
void CInterpreter::interpret(std::vector<std::shared_ptr<CStmt>> statements, std::shared_ptr<CEnvironment> entryEnv)
{
	setEnvironment(entryEnv);
	for (std::shared_ptr<CStmt> stmt : statements)
	{
		execute(stmt);
	}
	setEnvironment(console.get()->global);
}
std::shared_ptr<CObject> CInterpreter::interpretLine(std::vector<std::shared_ptr<CStmt>> statements, std::shared_ptr<CEnvironment> entryEnv)
{
	setEnvironment(entryEnv);
	std::shared_ptr<CObject> returnObj;
	if (statements.size() != 0 && dynamic_cast<CStmt_Expression*>(statements[0].get()) != nullptr)
	{
		std::shared_ptr<CStmt_Expression> expr = std::dynamic_pointer_cast<CStmt_Expression>(statements[0]);
		returnObj = expr.get()->expression.get()->acceptInterpreter(*this);
	}
	else
	{
		console.get()->error("[interpreter:0001] Cannot execute single line command that is not an expression.");
		returnObj = std::make_shared<CObject>(nullptr);
	}
	setEnvironment(nullptr);
	return returnObj;
}

//
void CInterpreter::execute(std::shared_ptr<CStmt> stmt)
{
	stmt.get()->accept(*this);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Visit Statements
//
////////////////////////////////////////////////////////////////////////////////////////////////

// CStmt_Block
void CInterpreter::visit(std::shared_ptr<CStmt_Block> stmt)
{
	std::shared_ptr<CEnvironment> previous = nullptr;
	// Check for anonymous block statement
	if (currentNamespaces.size() == 0 || (stmt.get()->_namespace != "" && stmt.get()->_namespace != currentNamespaces.back()))
	{
		// Enter anonymous block
		previous = currentEnvironment;
		// Should always return, already checked
		currentEnvironment = currentEnvironment.get()->lookupEnvironment(stmt.get()->_namespace, false);
		currentNamespaces.push_back(stmt.get()->_namespace);
	}

	for (std::shared_ptr<CStmt> statement : stmt.get()->statements)
	{
		// Check if should exit
		if (foundReturn || foundBreak || foundContinue)
		{
			if (foundReturn)
			{
				if (checkNamespaces("function:")) { break; }
				else { foundReturn = false; }
			}
			else if (foundBreak)
			{
				if (checkNamespaces("loop:")) { break; }
				else { foundBreak = false; }
			}
			else
			{
				if (checkNamespaces("loop:")) { break; }
				else { foundContinue = false; }
			}
		}
		if (!shouldExit) { statement.get()->accept(*this); }
		else { break; }
	}

	// If was anonymous block, exit environment
	if (previous != nullptr)
	{
		currentNamespaces.pop_back();
		currentEnvironment = previous;
	}
}
// CStmt_Break
void CInterpreter::visit(std::shared_ptr<CStmt_Break> stmt)
{
	// Simple as
	foundBreak = true;
}
// CStmt_Class
void CInterpreter::visit(std::shared_ptr<CStmt_Class> stmt)
{
	// Get the class environment
	std::shared_ptr<CEnvironment> previous = currentEnvironment;
	currentEnvironment = currentEnvironment.get()->fetchEnvironment(stmt.get()->name.get()->lexeme);
	currentNamespaces.push_back("class:" + stmt.get()->name.get()->lexeme);
	// interpret the property statements
	// so that instances of this class will
	// already have the property values defined.
	for (std::shared_ptr<CStmt> _property : stmt.get()->properties)
	{
		_property.get()->accept(*this);
	}
	// Exit the environment
	currentNamespaces.pop_back();
	currentEnvironment = previous;
}
// CStmt_Continue
void CInterpreter::visit(std::shared_ptr<CStmt_Continue> stmt)
{
	// Simple as
	foundContinue = true;
}
// CStmt_Expression
void CInterpreter::visit(std::shared_ptr<CStmt_Expression> stmt)
{
	// Simple as
	stmt.get()->expression.get()->acceptInterpreter(*this);
}
// CStmt_For
void CInterpreter::visit(std::shared_ptr<CStmt_For> stmt)
{
	// Capture the environment
	std::shared_ptr<CEnvironment> previous = currentEnvironment;
	currentEnvironment = currentEnvironment.get()->lookupEnvironment(stmt.get()->_namespace, false);
	currentNamespaces.push_back("loop:" + currentEnvironment.get()->_namespace);
	// Run the initializer statement
	stmt.get()->initializer.get()->accept(*this);
	// Run the condition
	std::shared_ptr<CObject> condition = stmt.get()->condition.get()->acceptInterpreter(*this);
	// Store the outer body
	std::shared_ptr<CEnvironment> outerBody = currentEnvironment;
	int loopCount = 0;
	foundBreak = false;
	foundContinue = false;
	while (std::get<bool>(condition.get()->obj) == true)
	{
		loopCount++;
		// Enter into the body environment
		currentEnvironment =
			currentEnvironment.get()->lookupEnvironment(
				currentEnvironment.get()->scopes.front().get()->_namespace, false).get()->virtualize("", nullptr);
		currentNamespaces.push_back(currentEnvironment.get()->_namespace);
		stmt.get()->body.get()->accept(*this);
		currentEnvironment = outerBody;
		currentNamespaces.pop_back();
		if (foundBreak) { break; }
		stmt.get()->increment.get()->acceptInterpreter(*this);
		condition = stmt.get()->condition.get()->acceptInterpreter(*this);
		if (loopCount > 255)
		{
			console.get()->warning(stmt.get()->line,
				"[interpreter:0101] Loop-counter exceeded 255, "
				"force-exited 'while' loop to prevent program execution from hanging.");
			break;
		}
	}
	foundBreak = false;
	foundContinue = false;
	currentNamespaces.pop_back();
	currentEnvironment = previous;
}
// CStmt_Foreach
void CInterpreter::visit(std::shared_ptr<CStmt_Foreach> stmt)
{
	// Capture the environment
	std::shared_ptr<CEnvironment> previous = currentEnvironment;
	currentEnvironment = currentEnvironment.get()->lookupEnvironment(stmt.get()->_namespace, false);
	currentNamespaces.push_back("loop:" + currentEnvironment.get()->_namespace);
	// Store the outer body
	std::shared_ptr<CEnvironment> outerBody = currentEnvironment;
	// Get the obj, then dereference the vector ptr
	std::shared_ptr<CObject> obj = stmt.get()->listExpression.get()->acceptInterpreter(*this);
	std::vector<std::shared_ptr<CObject>> list = 
		*std::get<std::shared_ptr<std::vector<std::shared_ptr<CObject>>>>(obj.get()->obj);
	// Get the iterator object
	std::shared_ptr<CObject> iterator = currentEnvironment.get()->get(stmt.get()->iteratorName.get()->lexeme);
	int loopCount = 0;
	foundBreak = false;
	foundContinue = false;
	for (std::shared_ptr<CObject> item : list)
	{
		loopCount++;
		iterator.get()->obj = console.get()->copyObj(item, iterator.get()->objName).get()->obj;
		// Enter into the body environment
		currentEnvironment =
			currentEnvironment.get()->lookupEnvironment(
				currentEnvironment.get()->scopes.front().get()->_namespace, false).get()->virtualize("", nullptr);
		currentNamespaces.push_back(currentEnvironment.get()->_namespace);
		// interpret the body
		stmt.get()->body.get()->accept(*this);
		currentEnvironment = outerBody;
		currentNamespaces.pop_back();
		if (foundBreak) { break; }
		if (loopCount > 255)
		{
			console.get()->warning(stmt.get()->line,
				"[interpreter:0201] Loop-counter exceeded 255, "
				"force-exited 'while' loop to prevent program execution from hanging.");
			break;
		}
	}
	foundBreak = false;
	foundContinue = false;
	currentNamespaces.pop_back();
	currentEnvironment = previous;
}
// CStmt_Function
void CInterpreter::visit(std::shared_ptr<CStmt_Function> stmt)
{
	// Do not need to run or evaluate function declarations
}
// CStmt_Namespace
void CInterpreter::visit(std::shared_ptr<CStmt_Namespace> stmt)
{
	// Capture the environment
	std::shared_ptr<CEnvironment> previous = currentEnvironment;
	currentEnvironment = currentEnvironment.get()->lookupEnvironment(stmt.get()->name.get()->lexeme, false);
	currentNamespaces.push_back("namespace:" + currentEnvironment.get()->_namespace);
	// interpret the body
	stmt.get()->body.get()->accept(*this);
	// Exit the namespace
	currentNamespaces.pop_back();
	currentEnvironment = previous;
}
// CStmt_If
void CInterpreter::visit(std::shared_ptr<CStmt_If> stmt)
{
	// Capture the environment
	std::shared_ptr<CEnvironment> previous = currentEnvironment;
	currentEnvironment = currentEnvironment.get()->lookupEnvironment(stmt.get()->_namespace, false);
	currentNamespaces.push_back(currentEnvironment.get()->_namespace);
	// Capture the if/else environment
	std::shared_ptr<CEnvironment> ifElseEnv = currentEnvironment;
	// interpret the statements
	bool foundTruth = false;
	for (size_t k = 0; k < stmt.get()->executionList.size(); k++)
	{
		std::shared_ptr<CObject> condition = stmt.get()->conditionsList[k].get()->acceptInterpreter(*this);
		if (std::get<bool>(condition.get()->obj) == true)
		{
			// Enter the environment
			currentEnvironment = currentEnvironment.get()->lookupEnvironment(currentEnvironment.get()->scopes[k].get()->_namespace, false);
			currentNamespaces.push_back(currentEnvironment.get()->_namespace);
			// interpret the block
			stmt.get()->executionList[k].get()->accept(*this);
			// Exit and clean up
			currentNamespaces.pop_back();
			currentEnvironment = ifElseEnv;
			foundTruth = true;
			break;
		}
	}
	if (stmt.get()->elseBranch != nullptr && foundTruth == false)
	{
		// Enter the environment
		currentEnvironment = ifElseEnv;
		currentEnvironment = currentEnvironment.get()->lookupEnvironment(currentEnvironment.get()->scopes.back().get()->_namespace, false);
		currentNamespaces.push_back(currentEnvironment.get()->_namespace);
		// interpret the block
		stmt.get()->elseBranch.get()->accept(*this);
		// Exit
		currentNamespaces.pop_back();
		currentEnvironment = ifElseEnv;
	}
	currentEnvironment = previous;
	currentNamespaces.pop_back();
}
// CStmt_Print
void CInterpreter::visit(std::shared_ptr<CStmt_Print> stmt)
{
	console.get()->printObj(stmt.get()->expression.get()->acceptInterpreter(*this));
}
// CStmt_Return
void CInterpreter::visit(std::shared_ptr<CStmt_Return> stmt)
{
	foundReturn = true;
	if (checkNamespaces("void:")) { return; }
	else
	{
		currentFunctions.back().get()->returnValue = stmt.get()->value.get()->acceptInterpreter(*this);
	}
}
// CStmt_TryCatch
void CInterpreter::visit(std::shared_ptr<CStmt_TryCatch> stmt)
{
	// Capture the environment
	std::shared_ptr<CEnvironment> previous = currentEnvironment;
	// Enter into the statement environment
	currentEnvironment = currentEnvironment.get()->lookupEnvironment(stmt.get()->_namespace, false);
	currentNamespaces.push_back(currentEnvironment.get()->_namespace);
	std::shared_ptr<CEnvironment> stmtEnv = currentEnvironment;

	// Enter into the try block environment
	currentEnvironment = currentEnvironment.get()->lookupEnvironment(
		currentEnvironment.get()->scopes.front().get()->_namespace, false).get()->virtualize("", nullptr);
	currentNamespaces.push_back("try:" + currentEnvironment.get()->_namespace);
	// try the tryBlock inside c++ try/catch
	try
	{
		// interpret tryBlock
		stmt.get()->tryBlock.get()->accept(*this);
		// Exit tryBlock
		currentEnvironment = stmtEnv;
		currentNamespaces.pop_back();
	}
	catch (...)
	{
		// Exit tryBlock - Encountered exception!
		currentEnvironment = stmtEnv;
		currentNamespaces.pop_back();
		// Enter into the catch block environment
		currentEnvironment = currentEnvironment.get()->lookupEnvironment(
			currentEnvironment.get()->scopes.back().get()->_namespace, false).get()->virtualize("", nullptr);
		currentNamespaces.push_back("catch:" + currentEnvironment.get()->_namespace);
		// interpret catchBlock
		stmt.get()->catchBlock.get()->accept(*this);
		// Exit catchBlock
		currentEnvironment = stmtEnv;
		currentNamespaces.pop_back();
	}
	// Exit the statement
	currentNamespaces.pop_back();
	currentEnvironment = previous;
}
// CStmt_Variable
void CInterpreter::visit(std::shared_ptr<CStmt_Variable> stmt)
{
	if (stmt.get()->initializer != nullptr)
	{
		currentAssignee = stmt.get()->name.get()->lexeme;
		std::shared_ptr<CObject> var = currentEnvironment.get()->get(stmt.get()->name.get()->lexeme);
		std::shared_ptr<CObject> assignment = stmt.get()->initializer.get()->acceptInterpreter(*this);
		if (assignment.get()->objType.type == CLiteralTypes::_CNil)
		{
			return;
		}
		var.get()->obj = assignment.get()->obj;
		currentAssignee = "";
	}
}
// CStmt_While
void CInterpreter::visit(std::shared_ptr<CStmt_While> stmt)
{
	// Capture the environment
	std::shared_ptr<CEnvironment> previous = currentEnvironment;
	// Enter the outer body environment - Should always return, already checked
	currentEnvironment = currentEnvironment.get()->lookupEnvironment(stmt.get()->_namespace, false);
	currentNamespaces.push_back("loop:" + currentEnvironment.get()->_namespace);
	// Check the condition
	std::shared_ptr<CObject> condition = stmt.get()->condition.get()->acceptInterpreter(*this);
	// Store the outer body
	std::shared_ptr<CEnvironment> outerBody = currentEnvironment;
	int loopCount = 0;
	foundBreak = false;
	foundContinue = false;
	while (std::get<bool>(condition.get()->obj) == true)
	{
		loopCount++;
		// Enter into the body environment
		currentEnvironment = 
			currentEnvironment.get()->lookupEnvironment(
				currentEnvironment.get()->scopes.front().get()->_namespace, false).get()->virtualize("", nullptr);
		currentNamespaces.push_back(currentEnvironment.get()->_namespace);
		stmt.get()->body.get()->accept(*this);
		currentEnvironment = outerBody;
		currentNamespaces.pop_back();
		if (foundBreak) { break; }
		condition = stmt.get()->condition.get()->acceptInterpreter(*this);
		if (loopCount > 255)
		{
			console.get()->warning(stmt.get()->line, 
				"[interpreter:0301] Loop-counter exceeded 255, "
				"force-exited 'while' loop to prevent program execution from hanging.");
			break;
		}
	}
	foundBreak = false;
	foundContinue = false;
	currentNamespaces.pop_back();
	currentEnvironment = previous;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Visit Expressions
//
////////////////////////////////////////////////////////////////////////////////////////////////

// CExpr_Approximate
std::shared_ptr<CObject> CInterpreter::visit(std::shared_ptr<CExpr_Approximate> expr)
{
	// Can assume that each of these will return a number
	double left = std::get<double>(expr.get()->left.get()->acceptInterpreter(*this).get()->obj);
	double right = std::get<double>(expr.get()->right.get()->acceptInterpreter(*this).get()->obj);
	double value = std::get<double>(expr.get()->value.get()->acceptInterpreter(*this).get()->obj);
	if (abs(left - right) <= value)
	{
		return std::make_shared<CObject>(true);
	}
	else
	{
		return std::make_shared<CObject>(false);
	}
}
// CExpr_Array
std::shared_ptr<CObject> CInterpreter::visit(std::shared_ptr<CExpr_Array> stmt)
{
	return makeArrayObj(stmt.get()->values);
}
// CExpr_Assign
std::shared_ptr<CObject> CInterpreter::visit(std::shared_ptr<CExpr_Assign> expr)
{
	std::shared_ptr<CToken> _operator = expr.get()->_operator;
	std::shared_ptr<CObject> left = expr.get()->identifier.get()->acceptInterpreter(*this);
	std::shared_ptr<CObject> right = expr.get()->value.get()->acceptInterpreter(*this);
	switch (_operator.get()->type)
	{
	case CTokenType::ASSIGN:
		return assign(left, right); break;
	case CTokenType::ASSIGN_ADD:
		return assign(left, add(left, right));
		left.get()->obj = add(left, right).get()->obj; return left; break;
	case CTokenType::ASSIGN_DIVIDE:
		return assign(left, divide(left, right));
		left.get()->obj = divide(left, right).get()->obj; return left; break;
	case CTokenType::ASSIGN_MULTIPLY:
		return assign(left, multiply(left, right));
		left.get()->obj = multiply(left, right).get()->obj; return left; break;
	case CTokenType::ASSIGN_SUBTRACT:
		return assign(left, subtract(left, right));
		left.get()->obj = subtract(left, right).get()->obj; return left; break;
	}
	// Should be unreachable
	return left;
}
// CExpr_Binary
std::shared_ptr<CObject> CInterpreter::visit(std::shared_ptr<CExpr_Binary> expr)
{
	// Fetch each side of the operator
	std::shared_ptr<CObject> left = expr.get()->left.get()->acceptInterpreter(*this);
	std::shared_ptr<CObject> right = expr.get()->right.get()->acceptInterpreter(*this);
	// Switch on operator type using helper functions
	// The left side of the operator is always the returned result
	switch (expr.get()->_operator.get()->type)
	{
	case CTokenType::PLUS:
		return add(left, right); break;
	case CTokenType::MINUS:
		return subtract(left, right); break;
	case CTokenType::DIVIDE:
		return divide(left, right); break;
	case CTokenType::MULTIPLY:
		return multiply(left, right); break;
	case CTokenType::MODULO:
		return modulo(left, right); break;
	case CTokenType::POWER:
		return power(left, right); break;
	case CTokenType::MORE:
		return isMore(left, right); break;
	case CTokenType::MORE_EQUAL:
		return isMoreEqual(left, right); break;
	case CTokenType::LESS:
		return isLess(left, right); break;
	case CTokenType::LESS_EQUAL:
		return isLessEqual(left, right); break;
	case CTokenType::EQUAL:
		return isEqual(left, right); break;
	case CTokenType::NOT_EQUAL:
		return falsify(isEqual(left, right)); break;
	default:
		return left; // Unreachable - typeWizard already accounts for this and error's it
	}
}
// CExpr_Call
std::shared_ptr<CObject> CInterpreter::visit(std::shared_ptr<CExpr_Call> expr)
{
	std::shared_ptr<CObject> callee;
	foundReturn = false;
	if (currentThisInstance != nullptr)
	{
		std::shared_ptr<CEnvironment> previous = currentEnvironment;
		currentEnvironment = currentThisInstance;
		callee = expr.get()->callee.get()->acceptInterpreter(*this);
		currentEnvironment = previous;
		//currentThisInstance = nullptr;
	}
	else
	{
		callee = expr.get()->callee.get()->acceptInterpreter(*this);
	}
	// Get the arguments
	std::shared_ptr<CEnvironment> previousInstance = currentThisInstance;
	std::vector<std::shared_ptr<CObject>> arguments;
	for (std::shared_ptr<CExpr> argument : expr.get()->arguments)
	{
		arguments.push_back(argument.get()->acceptInterpreter(*this));
	}
	currentThisInstance = previousInstance;
	// Capture the environment
	std::shared_ptr<CEnvironment> previous = currentEnvironment;
	// Check if calling a class constructor
	if (callee.get()->objType.type == CLiteralTypes::_CClass)
	{
		// Get the object as CClass
		std::shared_ptr<CClass> calleeClass = std::get<std::shared_ptr<CClass>>(callee.get()->obj);
		// Append the calleeClass and assignee as riders to the call arguments
		// Must be in this order
		arguments.push_back(callee);
		arguments.push_back(std::make_shared<CObject>(currentAssignee));
		// Return result of call to the calleeClass. Calls constructor from within this.
		return calleeClass.get()->call(shared_from_this(), &arguments);
	}
	else if (callee.get()->objType.type == CLiteralTypes::_CFunction)
	{
		std::shared_ptr<CObject> returnObj = nullptr;
		std::shared_ptr<CFunction> function = std::get<std::shared_ptr<CFunction>>(callee.get()->obj);
		if (currentThisInstance != nullptr)
		{
			currentEnvironment = currentThisInstance;
			returnObj = function.get()->call(shared_from_this(), &arguments);
			currentEnvironment = previous;
			currentThisInstance = nullptr;
		}
		else
		{
			returnObj = function.get()->call(shared_from_this(), &arguments);
		}
		currentEnvironment.get()->returnValue = nullptr;
		foundReturn = false;
		if (expr.get()->index != nullptr)
		{
			std::shared_ptr<CObject> indexVal = expr.get()->index.get()->acceptInterpreter(*this);
			size_t i = (size_t)std::get<double>(indexVal.get()->obj);
			std::vector<std::shared_ptr<CObject>> arrayObj =
				*std::get<std::shared_ptr<std::vector<std::shared_ptr<CObject>>>>(returnObj.get()->obj);
			if (i >= arrayObj.size())
			{
				console.get()->error("[interpreter:0403] Array Index out of bounds at '" + std::to_string(expr.get()->line.get()->line) + "'.");
			}
			else
			{
				return arrayObj[i];
			}
		}
		return returnObj;
	}
	else
	{
		console.get()->error("[interpreter:0401] navyblue - Encountered unfinished area of program.");
	}
	return nullptr;
}
// CExpr_Get
std::shared_ptr<CObject> CInterpreter::visit(std::shared_ptr<CExpr_Get> expr)
{
	// Get the object
	std::shared_ptr<CObject> object = expr.get()->object.get()->acceptInterpreter(*this);
	if (object.get()->objType.type == CLiteralTypes::_CInstance)
	{
		// Convert to instance object
		std::shared_ptr<CInstance> instance = std::get<std::shared_ptr<CInstance>>(object.get()->obj);
		// Capture the environment
		std::shared_ptr<CEnvironment> previous = currentEnvironment;
		currentEnvironment = instance.get()->instanceEnv;
		currentThisInstance = instance.get()->instanceEnv;
		// interpret the field
		std::shared_ptr<CObject> returnObj = expr.get()->field.get()->acceptInterpreter(*this);
		// Exit and return the result
		currentEnvironment = previous;
		return returnObj;
	}
	else if (object.get()->objType.type == CLiteralTypes::_CInstance_Array)
	{
		console.get()->error("[interpreter:0501] orange - Encountered unfinished area of program.");
	}
	else
	{
		console.get()->error("[interpreter:0502] tangarine - Encountered unfinished area of program.");
	}
	return nullptr;
}
// CExpr_Grouping
std::shared_ptr<CObject> CInterpreter::visit(std::shared_ptr<CExpr_Grouping> expr)
{
	// Simple as
	return expr.get()->expression.get()->acceptInterpreter(*this);
}
// CExpr_InstanceOf
std::shared_ptr<CObject> CInterpreter::visit(std::shared_ptr<CExpr_InstanceOf> expr)
{
	std::shared_ptr<CObject> query = expr.get()->right.get()->acceptInterpreter(*this);
	std::shared_ptr<CToken> type = expr.get()->type;
	return matchType(type, query.get()->objType);
}
// CExpr_Literal
std::shared_ptr<CObject> CInterpreter::visit(std::shared_ptr<CExpr_Literal> expr)
{
	switch (expr.get()->value.get()->type)
	{
	case CTokenType::NUM:
		return std::make_shared<CObject>(std::stod(expr.get()->value.get()->lexeme));
		break;
	case CTokenType::STRING:
		return std::make_shared<CObject>(expr.get()->value.get()->lexeme);
		break;
	case CTokenType::BOOL:
		if (expr.get()->value.get()->lexeme == "true")
		{
			return std::make_shared<CObject>(true);
		}
		return std::make_shared<CObject>(false);
		break;
	case CTokenType::_THIS:
		return currentThisInstance.get()->enclosing.get()->get(currentThisInstance.get()->_namespace);
		break;
	default:
		return std::make_shared<CObject>(nullptr);
		break;
	}
}
// CExpr_Logical
std::shared_ptr<CObject> CInterpreter::visit(std::shared_ptr<CExpr_Logical> expr)
{
	// Fetch each side of the operator
	std::shared_ptr<CObject> left = expr.get()->left.get()->acceptInterpreter(*this);
	std::shared_ptr<CObject> right = expr.get()->right.get()->acceptInterpreter(*this);
	// Switch on operator type using helper functions
	switch (expr.get()->_operator.get()->type)
	{
	case CTokenType::LOGIC_OR:
		return logicalOr(left, right);
		break;
	case CTokenType::LOGIC_AND:
		return logicalAnd(left, right);
		break;
	default:
		return std::make_shared<CObject>(nullptr);
		break;
	}
}
// CExpr_Scope
std::shared_ptr<CObject> CInterpreter::visit(std::shared_ptr<CExpr_Scope> expr)
{
	// Capture currentEnvironment
	std::shared_ptr<CEnvironment> previous = currentEnvironment;
	// Find and enter the environment from the scopeStack
	currentEnvironment = currentEnvironment.get()->lookupEnvironment(expr);
	// From this environment, typeCheck the var/get/call expression
	std::shared_ptr<CObject> right = expr.get()->right.get()->acceptInterpreter(*this);
	// Reset the environment
	currentEnvironment = previous;
	// Return the type of what was found
	return right;
}
// CExpr_TypeCast
std::shared_ptr<CObject> CInterpreter::visit(std::shared_ptr<CExpr_TypeCast> expr)
{
	std::shared_ptr<CObject> right = expr.get()->right.get()->acceptInterpreter(*this);
	if (right.get()->objType.type == CLiteralTypes::_CInstance)
	{
		std::shared_ptr<CInstance> instance = std::get<std::shared_ptr<CInstance>>(right.get()->obj);
		std::shared_ptr<CObject> castObj = 
			instance.get()->instanceEnv.get()->get(expr.get()->type.get()->typeString());
		std::shared_ptr<CFunction> castFunc = std::get<std::shared_ptr<CFunction>>(castObj.get()->obj);
		// Capture the environment
		std::shared_ptr<CEnvironment> previous = currentEnvironment;
		// Set up the return
		foundReturn = false;
		std::vector<std::shared_ptr<CObject>> arguments;
		std::shared_ptr<CObject> returnObj = nullptr;
		currentEnvironment = currentThisInstance = instance.get()->instanceEnv;
		returnObj = castFunc.get()->call(shared_from_this(), &arguments);
		currentEnvironment = previous;
		currentThisInstance = nullptr;
		currentEnvironment.get()->returnValue = nullptr;
		foundReturn = false;
		return returnObj;
	}
	else if (right.get()->objType.type == CLiteralTypes::_CInstance_Array)
	{
		console.get()->error("[interpreter:1101] ultramarine - Encountered unfinished area of program.");
	}
	else
	{
		console.get()->error("[interpreter:1101] violet - Encountered unfinished area of program.");
	}
	return nullptr;
}
// CExpr_Unary
std::shared_ptr<CObject> CInterpreter::visit(std::shared_ptr<CExpr_Unary> expr)
{
	// Fetch the value
	std::shared_ptr<CObject> right = expr.get()->right.get()->acceptInterpreter(*this);
	// Safely send to operator function -
	// falsify must always be bool
	// negate, increment, decrement are always num
	switch (expr.get()->_operator.get()->type)
	{
	case CTokenType::FALSIFY:
		return falsify(right); break;
	case CTokenType::NEGATE:
		return negate(right); break;
	case CTokenType::INCREMENT:
		std::get<double>(right.get()->obj) = std::get<double>(increment(right).get()->obj);
		return right; break;
	case CTokenType::DECREMENT:
		std::get<double>(right.get()->obj) = std::get<double>(decrement(right).get()->obj);
		return right; break;
	default:
		return right; break;
	}
	
}
// CExpr_Variable
std::shared_ptr<CObject> CInterpreter::visit(std::shared_ptr<CExpr_Variable> expr)
{
	std::shared_ptr<CObject> getVar = currentEnvironment.get()->get(expr.get()->name.get()->lexeme);
	if (expr.get()->index == nullptr) { return getVar; }
	else if (getVar == nullptr && expr.get()->name.get()->type == CTokenType::PAREN_RIGHT)
	{
		return expr.get()->index.get()->acceptInterpreter(*this);
	}
	else
	{
		size_t index = (size_t)std::get<double>(expr.get()->index.get()->acceptInterpreter(*this).get()->obj);
		return std::get<std::shared_ptr<std::vector<std::shared_ptr<CObject>>>>(getVar.get()->obj).get()->at(index);
	}
}
// CExpr_Vector
std::shared_ptr<CObject> CInterpreter::visit(std::shared_ptr<CExpr_Vector> expr)
{
	CTokenType kind = expr.get()->kind.get()->type;
	std::shared_ptr<CObject> vec;
	double x, y, z, w;
	switch (kind)
	{
	case CTokenType::VECTOR2:
		x = std::get<double>(expr.get()->x.get()->acceptInterpreter(*this).get()->obj);
		y = std::get<double>(expr.get()->y.get()->acceptInterpreter(*this).get()->obj);
		vec = std::make_shared<CObject>(glm::dvec2(x, y));
		break;
	case CTokenType::VECTOR3:
		x = std::get<double>(expr.get()->x.get()->acceptInterpreter(*this).get()->obj);
		y = std::get<double>(expr.get()->y.get()->acceptInterpreter(*this).get()->obj);
		z = std::get<double>(expr.get()->z.get()->acceptInterpreter(*this).get()->obj);
		vec = std::make_shared<CObject>(glm::dvec3(x, y, z));
		break;
	case CTokenType::VECTOR4:
		x = std::get<double>(expr.get()->x.get()->acceptInterpreter(*this).get()->obj);
		y = std::get<double>(expr.get()->y.get()->acceptInterpreter(*this).get()->obj);
		z = std::get<double>(expr.get()->z.get()->acceptInterpreter(*this).get()->obj);
		w = std::get<double>(expr.get()->w.get()->acceptInterpreter(*this).get()->obj);
		vec = std::make_shared<CObject>(glm::dvec4(x, y, z, w));
		break;
	}
	return vec;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// CInterpreter Utility Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

//
bool CInterpreter::isTrue(std::shared_ptr<CObject> object)
{
	return false;
}

//
bool CInterpreter::checkNamespaces(std::string delim)
{
	if (currentNamespaces.size() == 0) { return false; }
	else
	{
		for (int i = (int)currentNamespaces.size() - 1; i >= 0; i--)
		{
			if (currentNamespaces[(size_t)i].find(delim) != std::string::npos) { return true; }
		}
	}
	return false;
}
//
//
void CInterpreter::storeGetKey(std::shared_ptr<CExpr_Call> expr, std::shared_ptr<CEnvironment> environment)
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
		}
	}
	else if (dynamic_cast<CExpr_Variable*>(expr.get()->callee.get()) != nullptr)
	{
		if (currentGetExprKey == "")
		{
			currentGetExprKey =
				std::dynamic_pointer_cast<CExpr_Variable>(expr.get()->callee).get()->name.get()->lexeme + environment.get()->_namespace;
		}
	}
}
void CInterpreter::checkGetKey(std::shared_ptr<CExpr_Call> expr, std::shared_ptr<CEnvironment> previous)
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
		}
	}
	else if (dynamic_cast<CExpr_Variable*>(expr.get()->callee.get()) != nullptr)
	{
		if (currentGetExprKey ==
			std::dynamic_pointer_cast<CExpr_Variable>(expr.get()->callee).get()->name.get()->lexeme + previous.get()->_namespace)
		{
			currentGetExprKey = "";
			currentEnvironment = previous;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// CInterpreter Make Array Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

// Make Arrays
std::shared_ptr<CObject> CInterpreter::makeArrayObj(std::vector<std::shared_ptr<CExpr>> expressions)
{
	// Arrays are generic CObject containers (ie. nested Objects)
	// This allows for permanence of objects in an array
	if (expressions.size() != 0)
	{
		std::vector<std::shared_ptr<CObject>> arr; 
		std::string lastAsignee = currentAssignee;
		currentAssignee = "";
		for (std::shared_ptr<CExpr> expr : expressions)
		{
			
			arr.push_back(expr.get()->acceptInterpreter(*this));
		}
		currentAssignee = lastAsignee;
		switch (arr[0].get()->objType.type)
		{
		case CLiteralTypes::_CInstance:
			return std::make_shared<CObject>(
				CLiteralTypes::_CInstance_Array,
				std::make_shared<std::vector<std::shared_ptr<CObject>>>(arr));
			break;
		case CLiteralTypes::_CNumber:
			return std::make_shared<CObject>(
				CLiteralTypes::_CNumber_Array, 
				std::make_shared<std::vector<std::shared_ptr<CObject>>>(arr));
			break;
		case CLiteralTypes::_CBool:
			return std::make_shared<CObject>(
				CLiteralTypes::_CBool_Array,
				std::make_shared<std::vector<std::shared_ptr<CObject>>>(arr));
			break;
		case CLiteralTypes::_CString:
			return std::make_shared<CObject>(
				CLiteralTypes::_CString_Array,
				std::make_shared<std::vector<std::shared_ptr<CObject>>>(arr));
			break;
		case CLiteralTypes::_CFunction:
			return std::make_shared<CObject>(
				CLiteralTypes::_CFunction_Array,
				std::make_shared<std::vector<std::shared_ptr<CObject>>>(arr));
			break;
		case CLiteralTypes::_CVec2:
			return std::make_shared<CObject>(
				CLiteralTypes::_CVec2_Array,
				std::make_shared<std::vector<std::shared_ptr<CObject>>>(arr));
			break;
		case CLiteralTypes::_CVec3:
			return std::make_shared<CObject>(
				CLiteralTypes::_CVec3_Array,
				std::make_shared<std::vector<std::shared_ptr<CObject>>>(arr));
			break;
		case CLiteralTypes::_CVec4:
			return std::make_shared<CObject>(
				CLiteralTypes::_CVec4_Array,
				std::make_shared<std::vector<std::shared_ptr<CObject>>>(arr));
			break;
		default:
			console.get()->error("[interpreter:0601] skyblue - Encountered unfinished area of program.");
			return std::make_shared<CObject>(nullptr); break;
		}
	}
	else
	{
		return std::make_shared<CObject>(nullptr);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// CInterpreter Binary/Unary Operator Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

// Binary Assign
std::shared_ptr<CObject> CInterpreter::assign(std::shared_ptr<CObject> left, std::shared_ptr<CObject> right)
{
	// For Certain Data Types, Make sure to copy the data without
	// changing the pointer address
	if (left.get()->objType.type == CLiteralTypes::_CStyle)
	{
		*std::get<std::shared_ptr<WidgetStyle>>(left.get()->obj) = *std::get<std::shared_ptr<WidgetStyle>>(console.get()->copyObj(right, right.get()->objName).get()->obj);
		return left;
	}
	else
	{
		switch (left.get()->objType.type)
		{
		case CLiteralTypes::_CString:
		case CLiteralTypes::_CBool:
		case CLiteralTypes::_CNumber:
		case CLiteralTypes::_CString_Array:
		case CLiteralTypes::_CBool_Array:
		case CLiteralTypes::_CNumber_Array:
		case CLiteralTypes::_CVec2:
		case CLiteralTypes::_CVec3:
		case CLiteralTypes::_CVec4:
		case CLiteralTypes::_CVec2_Array:
		case CLiteralTypes::_CVec3_Array:
		case CLiteralTypes::_CVec4_Array:
			left.get()->obj = console.get()->copyObj(right, right.get()->objName).get()->obj;
			return left;
		case CLiteralTypes::_CFunction:
		case CLiteralTypes::_CFunction_Array:
			// Warning: May fail to correctly update function environments, not sure how this behaves
			left.get()->obj = console.get()->copyObj(right, right.get()->objName).get()->obj;
			return left;
		case CLiteralTypes::_CInstance:
			//
		case CLiteralTypes::_CInstance_Array:
			left.get()->obj = console.get()->copyObj(right, right.get()->objName).get()->obj;
			return left;
		case CLiteralTypes::_CStyle:

		case CLiteralTypes::_CWidget:
			// Warning: May result in undefined behavior currently.
			left.get()->obj = console.get()->copyObj(right, right.get()->objName).get()->obj;
			return left;
		case CLiteralTypes::_CUi:
			// Do not copy the ui!
			return left;
		case CLiteralTypes::_CApp:
			// Do not copy the application!
			return left;
		case CLiteralTypes::_CNil:
		default:
			return std::make_shared<CObject>(nullptr);
		}
	}
}

// Binary Add
std::shared_ptr<CObject> CInterpreter::add(std::shared_ptr<CObject> left, std::shared_ptr<CObject> right)
{
	switch (left.get()->objType.type)
	{
	case CLiteralTypes::_CString:
		return std::make_shared<CObject>(std::get<std::string>(left.get()->obj) + std::get<std::string>(right.get()->obj));
		break;
	case CLiteralTypes::_CNumber:
		return std::make_shared<CObject>(std::get<double>(left.get()->obj) + std::get<double>(right.get()->obj));
		break;
	default:
		return std::make_shared<CObject>(nullptr);
	}
}
// Binary Subtract
std::shared_ptr<CObject> CInterpreter::subtract(std::shared_ptr<CObject> left, std::shared_ptr<CObject> right)
{
	switch (left.get()->objType.type)
	{
	case CLiteralTypes::_CNumber:
		return std::make_shared<CObject>(std::get<double>(left.get()->obj) - std::get<double>(right.get()->obj));
		break;
	default:
		return std::make_shared<CObject>(nullptr);
	}
}
// Binary Multiply
std::shared_ptr<CObject> CInterpreter::multiply(std::shared_ptr<CObject> left, std::shared_ptr<CObject> right)
{
	switch (left.get()->objType.type)
	{
	case CLiteralTypes::_CNumber:
		return std::make_shared<CObject>(std::get<double>(left.get()->obj) * std::get<double>(right.get()->obj));
		break;
	default:
		return std::make_shared<CObject>(nullptr);
	}
}
// Binary Divide
std::shared_ptr<CObject> CInterpreter::divide(std::shared_ptr<CObject> left, std::shared_ptr<CObject> right)
{
	switch (left.get()->objType.type)
	{
	case CLiteralTypes::_CNumber:
		return std::make_shared<CObject>(std::get<double>(left.get()->obj) / std::get<double>(right.get()->obj));
		break;
	default:
		return std::make_shared<CObject>(nullptr);
	}
}
// Binary Modulo
std::shared_ptr<CObject> CInterpreter::modulo(std::shared_ptr<CObject> left, std::shared_ptr<CObject> right)
{
	switch (left.get()->objType.type)
	{
	case CLiteralTypes::_CNumber:
		return std::make_shared<CObject>(double((int)std::get<double>(left.get()->obj) % (int)std::get<double>(right.get()->obj)));
		break;
	default:
		return std::make_shared<CObject>(nullptr);
	}
}
// Binary Power
std::shared_ptr<CObject> CInterpreter::power(std::shared_ptr<CObject> left, std::shared_ptr<CObject> right)
{
	switch (left.get()->objType.type)
	{
	case CLiteralTypes::_CNumber:
		return std::make_shared<CObject>(pow(std::get<double>(left.get()->obj), std::get<double>(right.get()->obj)));
		break;
	default:
		return std::make_shared<CObject>(nullptr);
	}
}
// Unary Falsify
std::shared_ptr<CObject> CInterpreter::falsify(std::shared_ptr<CObject> right)
{
	return std::make_shared<CObject>(!std::get<bool>(right.get()->obj));
}
// Unary Negate
std::shared_ptr<CObject> CInterpreter::negate(std::shared_ptr<CObject> right)
{
	return std::make_shared<CObject>(-std::get<double>(right.get()->obj));
}
// Unary Increment
std::shared_ptr<CObject> CInterpreter::increment(std::shared_ptr<CObject> right)
{
	return std::make_shared<CObject>(std::get<double>(right.get()->obj) + 1);
}
// Unary Decrement
std::shared_ptr<CObject> CInterpreter::decrement(std::shared_ptr<CObject> right)
{
	return std::make_shared<CObject>(std::get<double>(right.get()->obj) - 1);
}
// Binary Logical Or
std::shared_ptr<CObject> CInterpreter::logicalOr(std::shared_ptr<CObject> left, std::shared_ptr<CObject> right)
{
	if (std::get<bool>(left.get()->obj) || std::get<bool>(right.get()->obj))
	{
		return std::make_shared<CObject>(true);
	}
	else { return std::make_shared<CObject>(false); }
}
// Binary Logical And
std::shared_ptr<CObject> CInterpreter::logicalAnd(std::shared_ptr<CObject> left, std::shared_ptr<CObject> right)
{
	if (std::get<bool>(left.get()->obj) && std::get<bool>(right.get()->obj))
	{
		return std::make_shared<CObject>(true);
	}
	else { return std::make_shared<CObject>(false); }
}
// Binary Equal // Not-Equal
std::shared_ptr<CObject> CInterpreter::isEqual(std::shared_ptr<CObject> left, std::shared_ptr<CObject> right)
{
	if (left.get()->objType.type == right.get()->objType.type)
	{
		switch (left.get()->objType.type)
		{
		case CLiteralTypes::_CBool:
			if (std::get<bool>(left.get()->obj) == std::get<bool>(right.get()->obj))
			{
				return std::make_shared<CObject>(true);
			}
			else { return std::make_shared<CObject>(false); }
			break;
		case CLiteralTypes::_CNumber:
			if (std::get<double>(left.get()->obj) == std::get<double>(right.get()->obj))
			{
				return std::make_shared<CObject>(true);
			}
			else { return std::make_shared<CObject>(false); }
			break;

		case CLiteralTypes::_CString:
			if (std::get<std::string>(left.get()->obj) == std::get<std::string>(right.get()->obj))
			{
				return std::make_shared<CObject>(true);
			}
			else { return std::make_shared<CObject>(false); }
			break;
		case CLiteralTypes::_CInstance:
			if (left.get()->objName == right.get()->objName)
			{
				return std::make_shared<CObject>(true);
			}
			else { return std::make_shared<CObject>(false); }
			break;
		default:
			console.get()->error("[interpreter:0701] magenta - Encountered unfinished area of program.");
			return std::make_shared<CObject>(false);
		}
	}
	else
	{
		return std::make_shared<CObject>(false);
	}
}
// Binary More
std::shared_ptr<CObject> CInterpreter::isMore(std::shared_ptr<CObject> left, std::shared_ptr<CObject> right)
{
	if (std::get<double>(left.get()->obj) > std::get<double>(right.get()->obj))
	{
		return std::make_shared<CObject>(true);
	}
	else { return std::make_shared<CObject>(false); }
}
// Binary More Equal
std::shared_ptr<CObject> CInterpreter::isMoreEqual(std::shared_ptr<CObject> left, std::shared_ptr<CObject> right)
{
	if (std::get<double>(left.get()->obj) >= std::get<double>(right.get()->obj))
	{
		return std::make_shared<CObject>(true);
	}
	else { return std::make_shared<CObject>(false); }
}
// Binary Less
std::shared_ptr<CObject> CInterpreter::isLess(std::shared_ptr<CObject> left, std::shared_ptr<CObject> right)
{
	if (std::get<double>(left.get()->obj) < std::get<double>(right.get()->obj))
	{
		return std::make_shared<CObject>(true);
	}
	else { return std::make_shared<CObject>(false); }
}
// Binary Less Equal
std::shared_ptr<CObject> CInterpreter::isLessEqual(std::shared_ptr<CObject> left, std::shared_ptr<CObject> right)
{
	if (std::get<double>(left.get()->obj) <= std::get<double>(right.get()->obj))
	{
		return std::make_shared<CObject>(true);
	}
	else { return std::make_shared<CObject>(false); }
}

// Unary InstanceOf
std::shared_ptr<CObject> CInterpreter::matchType(std::shared_ptr<CToken> type, CLiteral query)
{
	CLiteralTypes litType = tokenType_ToLiteralType(type.get()->type);
	if (litType == CLiteralTypes::_CInstance || litType == CLiteralTypes::_CInstance_Array)
	{
		if (type.get()->lexeme == query.litName) { return  std::make_shared<CObject>(true); }
		return std::make_shared<CObject>(false);
	}
	else
	{
		if (litType == query.type) { return std::make_shared<CObject>(true); }
		else { return std::make_shared<CObject>(false); }
	}
}
CLiteralTypes CInterpreter::tokenType_ToLiteralType(CTokenType type)
{
	switch (type)
	{
	case CTokenType::IDENTIFIER:
		return CLiteralTypes::_CInstance; break;
	case CTokenType::IDENTIFIER_ARRAY:
		return CLiteralTypes::_CInstance_Array; break;
	case CTokenType::BOOL:
		return CLiteralTypes::_CBool; break;
	case CTokenType::STRING:
		return CLiteralTypes::_CString; break;
	case CTokenType::NUM:
		return CLiteralTypes::_CNumber; break;
	case CTokenType::VECTOR2:
		return CLiteralTypes::_CVec2; break;
	case CTokenType::VECTOR3:
		return CLiteralTypes::_CVec3; break;
	case CTokenType::VECTOR4:
		return CLiteralTypes::_CVec4; break;
	case CTokenType::BOOL_ARRAY:
		return CLiteralTypes::_CBool_Array; break;
	case CTokenType::STRING_ARRAY:
		return CLiteralTypes::_CString_Array; break;
	case CTokenType::NUM_ARRAY:
		return CLiteralTypes::_CNumber_Array; break;
	case CTokenType::VECTOR2_ARRAY:
		return CLiteralTypes::_CVec2_Array; break;
	case CTokenType::VECTOR3_ARRAY:
		return CLiteralTypes::_CVec3_Array; break;
	case CTokenType::VECTOR4_ARRAY:
		return CLiteralTypes::_CVec4_Array; break;
	default:
		console.get()->error("[interpreter:0901] blue - Encountered unfinished area of program.");
		return CLiteralTypes::_CNil; break;
	}
}