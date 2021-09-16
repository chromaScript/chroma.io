#include "../../include/cscript/CCallable.h"
#include "../../include/cscript/CCallable_Lib.h"
#include "../../include/cscript/lib/CCLib_App.h"

#include "../../include/cscript/ChromaScript.h"
#include "../../include/cscript/CForward.h"
#include "../../include/cscript/CLiteral.h"
#include "../../include/cscript/CExpr.h"
#include "../../include/cscript/CStmt.h"
#include "../../include/cscript/CEnvironment.h"
#include "../../include/cscript/CInterpreter.h"
#include "../../include/cscript/CEnums.h"
#include "../../include/clayout/LEnums.h"
#include "../../include/cscript/CObject.h"
#include "../../include/cscript/CToken.h"
#include "../../include/entities/WidgetStyle.h"

#include "../../include/Application.h"
#include "../../include/entities/UserInterface.h"
#include "../../include/Toolbox.h"
#include "../../include/entities/Widget.h"
#include "../../include/structs.h"
#include "../../include/keys.h"
#include <glm.hpp>

#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <iostream>
#include <variant>
#include <map>

extern std::shared_ptr<Application> chromaIO;

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Standard Library Utility Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

//
//
// CStd_Clock
CStd_fClock::CStd_fClock(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fClock;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	std::vector<std::string> paramsNames;
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::NUM, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "clock", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fClock::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	return std::make_shared<CObject>(glfwGetTime());
}
std::string CStd_fClock::toString() { return "<std clock>"; }

//
//
// CStd_fVecSize
CStd_fVecSize::CStd_fVecSize(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fVecSize;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::ANY, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("object");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::NUM, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "vecSize", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fVecSize::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	CLiteralTypes objType = args[0].get()->objType.type;
	size_t vecSize = 0;
	switch (objType)
	{
	case CLiteralTypes::_CBool:
	case CLiteralTypes::_CNumber:
	case CLiteralTypes::_CClass:
	case CLiteralTypes::_CInstance:
	case CLiteralTypes::_CFunction:
	case CLiteralTypes::_CThis:
	case CLiteralTypes::_CVec2:
	case CLiteralTypes::_CVec3:
	case CLiteralTypes::_CVec4:
	case CLiteralTypes::_CString:
	case CLiteralTypes::_CApp:
	case CLiteralTypes::_CUi:
	case CLiteralTypes::_CToolbox:
	case CLiteralTypes::_CWidget:
	case CLiteralTypes::_CStyle:
		vecSize = 1;
		break;
	case CLiteralTypes::_CInstance_Array:
	case CLiteralTypes::_CFunction_Array:
	case CLiteralTypes::_CNumber_Array:
	case CLiteralTypes::_CVec2_Array:
	case CLiteralTypes::_CVec3_Array:
	case CLiteralTypes::_CVec4_Array:
	case CLiteralTypes::_CString_Array:
	case CLiteralTypes::_CBool_Array:
		vecSize = std::get<std::shared_ptr<std::vector<std::shared_ptr<CObject>>>>(args[0].get()->obj).get()->size();
	}
	return std::make_shared<CObject>(double(vecSize));
}
std::string CStd_fVecSize::toString() { return "<std vecSize>"; }

//
//
// CStd_fToString
CStd_fToString::CStd_fToString(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fToString;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::ANY, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("object");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::STRING, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "toString", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fToString::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	return std::make_shared<CObject>(interpreter.get()->console.get()->toString(args[0]));
}
std::string CStd_fToString::toString() { return "<std toString>"; }

//
//
// CStd_fToNum
CStd_fToNum::CStd_fToNum(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fToNum;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::ANY, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("object");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::NUM, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "toNum", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fToNum::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	return std::make_shared<CObject>(interpreter.get()->console.get()->toNum(args[0]));
}
std::string CStd_fToNum::toString() { return "<std toNum>"; }

//
//
// CStd_fKeyToString
CStd_fKeyToString::CStd_fKeyToString(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fKeyToString;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("modKeyBit");
	paramsNames.push_back("glfwKey");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::STRING, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "keyToString", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fKeyToString::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Get the arguments
	int modKeyBit = (int)std::get<double>(args[0].get()->obj);
	int glfwKey = (int)std::get<double>(args[1].get()->obj);
	// Build the string using the global function
	return std::make_shared<CObject>(keySigToString((glfwKey * 10) + modKeyBit));
}
std::string CStd_fKeyToString::toString() { return "<std keyToString>"; }

//
//
// CStd_fMakeKeySig
CStd_fMakeKeySig::CStd_fMakeKeySig(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fMakeKeySig;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("modKeyBit");
	paramsNames.push_back("glfwKey");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::STRING, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "makeKeySig", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fMakeKeySig::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Get the arguments
	int modKeyBit = (int)std::get<double>(args[0].get()->obj);
	int glfwKey = (int)std::get<double>(args[1].get()->obj);
	// Build the key Sig using the global function
	return std::make_shared<CObject>((double)createKeySig(modKeyBit, glfwKey));
}
std::string CStd_fMakeKeySig::toString() { return "<std makeKeySig>"; }

//
//
// cStd_fBindTimerEvent
CStd_fBindTimerEvent::CStd_fBindTimerEvent(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<Application> app)
{
	this->app = app;
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fBindTimerEvent;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::FUNCTION, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("timerID");
	paramsNames.push_back("seconds");
	paramsNames.push_back("repeat");
	paramsNames.push_back("callback");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "bindTimerEvent", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fBindTimerEvent::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Copy the arguments
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	std::string id = std::get<std::string>(args[0].get()->obj);
	double duration = std::get<double>(args[1].get()->obj);
	int repeat = (int)std::get<double>(args[2].get()->obj);
	if (repeat <= -2) { repeat = -1; }
	std::shared_ptr<CObject> funcObj = args[3];
	if (funcObj.get()->objType.type == CLiteralTypes::_CFunction)
	{
		bool fizzle = false;
		std::shared_ptr<CFunction> funcCall = std::get<std::shared_ptr<CFunction>>(funcObj.get()->obj);
		if (funcCall.get()->funcDeclaration.get()->returnType.get()->type != CTokenType::_VOID)
		{
			fizzle = true;
			interpreter.get()->console.get()->error(
				funcCall.get()->funcDeclaration.get()->returnType,
				"[interpreter:7401] Callback function '" + funcCall.get()->funcDeclaration.get()->name.get()->lexeme +
				"' return type provided for callback 'bindTimerEvent' must be of 'void' return type.");
		}
		if (funcCall.get()->funcDeclaration.get()->paramsTypes.size() != 1 ||
			funcCall.get()->funcDeclaration.get()->paramsTypes[0].get()->type != CTokenType::NUM)
		{
			fizzle = true;
			interpreter.get()->console.get()->error(
				funcCall.get()->funcDeclaration.get()->returnType,
				"[interpreter:7401] Callback function '" + funcCall.get()->funcDeclaration.get()->name.get()->lexeme +
				"' argument signature provided for callback 'bindTimerEvent' must be 1 argument of 'num' type.");
		}
		if (!fizzle)
		{
			app.get()->bindTimerCallback(id, duration, repeat, funcCall);
		}
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_fBindTimerEvent::toString() { return "<std bindTimerEvent>"; }

//
//
// cStd_fCancelTimerEvent
CStd_fCancelTimerEvent::CStd_fCancelTimerEvent(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<Application> app)
{
	this->app = app;
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fCancelTimerEvent;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("timerID");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "cancelTimerEvent", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fCancelTimerEvent::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Copy the arguments
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Get the id of the timer to attempt canceling
	std::string id = std::get<std::string>(args[0].get()->obj);
	// Send the cancel input
	app.get()->cancelTimerCallback(id);

	return std::make_shared<CObject>(nullptr);
}
std::string CStd_fCancelTimerEvent::toString() { return "<std cancelTimerEvent>"; }

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Standard Library Collision Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

//
//
// CStd_fGetMousePos
CStd_fGetMousePos::CStd_fGetMousePos(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<Application> app)
{
	this->app = app;
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fGetMousePos;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	std::vector<std::string> paramsNames;
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::NUM_ARRAY, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "getMousePos", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fGetMousePos::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Copy the arguments
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	MousePosition mousePos = app.get()->getMousePosition(true);
	std::vector<std::shared_ptr<CObject>> returnValue;
	std::shared_ptr<CObject> xpos = std::make_shared<CObject>(mousePos.x);
	std::shared_ptr<CObject> ypos = std::make_shared<CObject>(mousePos.y);
	returnValue.push_back(xpos); returnValue.push_back(ypos);
	std::shared_ptr<std::vector<std::shared_ptr<CObject>>> outValue = std::make_shared<std::vector<std::shared_ptr<CObject>>>(returnValue);
	return std::make_shared<CObject>(CLiteralTypes::_CNumber_Array, outValue);
}
std::string CStd_fGetMousePos::toString() { return "<std getMousePos>"; }

//
//
// CStd_fGetMousePos_relative
CStd_fGetMousePos_relative::CStd_fGetMousePos_relative(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<Application> app)
{
	this->app = app;
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fGetMousePos_relative;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	std::vector<std::string> paramsNames;
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::NUM_ARRAY, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "getMousePos", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fGetMousePos_relative::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Copy the arguments
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	MousePosition mousePos = app.get()->getMousePosition(true);
	glm::ivec2 widgetLoc = glm::ivec2(0);
	if (!app.get()->ui.get()->activeWidget.expired())
	{
		widgetLoc = app.get()->ui.get()->activeWidget.lock().get()->getScreenLocation(0, 0, false);
	}
	mousePos.x = mousePos.x - widgetLoc.x;
	mousePos.y = mousePos.y - widgetLoc.y;
	std::vector<std::shared_ptr<CObject>> returnValue;
	std::shared_ptr<CObject> xpos = std::make_shared<CObject>(mousePos.x);
	std::shared_ptr<CObject> ypos = std::make_shared<CObject>(mousePos.y);
	returnValue.push_back(xpos); returnValue.push_back(ypos);
	std::shared_ptr<std::vector<std::shared_ptr<CObject>>> outValue = std::make_shared<std::vector<std::shared_ptr<CObject>>>(returnValue);
	return std::make_shared<CObject>(CLiteralTypes::_CNumber_Array, outValue);
}
std::string CStd_fGetMousePos_relative::toString() { return "<std getMousePos_relative>"; }

//
//
// CStd_fHitTestWidget_byID
CStd_fHitTestWidget_byID::CStd_fHitTestWidget_byID(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui)
{
	this->ui = ui;
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fHitTestWidget_byID;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("id");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::BOOL, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "hitTestWidget_byID", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fHitTestWidget_byID::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Copy the arguments
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Get the id of the timer to attempt canceling
	std::string id = std::get<std::string>(args[0].get()->obj);
	if (ui.get()->widgetIDTable.count(id) == 1)
	{
		std::weak_ptr<Widget> root = ui.get()->getRootWidgetByID(ui.get()->widgetIDTable.at(id));
		if (!root.expired())
		{
			std::weak_ptr<Widget> fetch = root.lock().get()->getChild_byID(id);
			if (!fetch.expired())
			{
				MousePosition pos = chromaIO.get()->getMousePosition(true);
				if (fetch.lock().get()->checkPointCollision_self(glm::ivec2(pos.x, pos.y)))
				{
					return std::make_shared<CObject>(true);
				}
			}
		}
	}
	return std::make_shared<CObject>(false);
}
std::string CStd_fHitTestWidget_byID::toString() { return "<std hitTestWidget_byID>"; }

//
//
// CStd_fSweepTestRoot_byID
CStd_fSweepTestRoot_byID::CStd_fSweepTestRoot_byID(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui)
{
	this->ui = ui;
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fSweepTestRoot_byID;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("id");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::BOOL, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "sweepTestRoot_byID", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fSweepTestRoot_byID::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Copy the arguments
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Get the id of the timer to attempt canceling
	std::string rootName = std::get<std::string>(args[0].get()->obj);
	std::weak_ptr<Widget> root = ui.get()->getRootWidgetByID(ui.get()->widgetIDTable.at(rootName));
	if (!root.expired())
	{
		MousePosition pos = chromaIO.get()->getMousePosition(true);
		return std::make_shared<CObject>(root.lock().get()->mouseSweep(pos.x, pos.y));
	}
	return std::make_shared<CObject>(false);
}
std::string CStd_fSweepTestRoot_byID::toString() { return "<std sweepTestRoot_byID>"; }

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Standard Library Math Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

//
//
// Cstd_Fibonacci
CStd_fFibonacci::CStd_fFibonacci(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fFibonacci;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("f");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::NUM, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "fibonacci", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fFibonacci::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Copy the arguments
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	double t = fib(std::get<double>(args[0].get()->obj));
	return std::make_shared<CObject>(t);
}
double CStd_fFibonacci::fib(double f)
{
	if (f <= 2)
	{
		return f;
	}
	return fib(f - 2) + fib(f - 1);
}
std::string CStd_fFibonacci::toString() { return "<std fibonacci>"; }

//
//
// CStd_fLineIntersect2D
CStd_fLineIntersect2D::CStd_fLineIntersect2D(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fLineIntersect2D;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM_ARRAY, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM_ARRAY, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("line1");
	paramsNames.push_back("line2");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::NUM_ARRAY, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "lineIntersect2D", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fLineIntersect2D::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Copy the arguments
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Set the clamp values
	std::vector<std::shared_ptr<CObject>> line1Obj = *std::get<std::shared_ptr<std::vector<std::shared_ptr<CObject>>>>(args[0].get()->obj);
	std::vector<std::shared_ptr<CObject>> line2Obj = *std::get<std::shared_ptr<std::vector<std::shared_ptr<CObject>>>>(args[1].get()->obj);
	if (line1Obj.size() != 4 || line2Obj.size() != 4) { return std::make_shared<CObject>(nullptr); }
	glm::vec4 line1 = glm::vec4(0.0f);
	glm::vec4 line2 = glm::vec4(0.0f);
	for (int i = 0; i < 4; i++)
	{
		switch (i)
		{
		case 0: line1.x = (float)std::get<double>(line1Obj[i].get()->obj); line2.x = (float)std::get<double>(line2Obj[i].get()->obj); break;
		case 1: line1.y = (float)std::get<double>(line1Obj[i].get()->obj); line2.y = (float)std::get<double>(line2Obj[i].get()->obj); break;
		case 2: line1.z = (float)std::get<double>(line1Obj[i].get()->obj); line2.z = (float)std::get<double>(line2Obj[i].get()->obj); break;
		case 3: line1.w = (float)std::get<double>(line1Obj[i].get()->obj); line2.w = (float)std::get<double>(line2Obj[i].get()->obj); break;
		}
	}
	glm::vec2 intersect = lineIntersect2D(line1, line2);
	std::vector<std::shared_ptr<CObject>> returnVector = { std::make_shared<CObject>((double)intersect.x), std::make_shared<CObject>((double)intersect.y) };
	std::shared_ptr<std::vector<std::shared_ptr<CObject>>> returnObj = std::make_shared<std::vector<std::shared_ptr<CObject>>>(returnVector);
	return std::make_shared<CObject>(CLiteralTypes::_CNumber_Array, returnObj);
}
std::string CStd_fLineIntersect2D::toString() { return "<std lineIntersect2D>"; }

//
//
// CStd_fClamp
CStd_fClamp::CStd_fClamp(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fClamp;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("value");
	paramsNames.push_back("min");
	paramsNames.push_back("max");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::NUM, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "clamp", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fClamp::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Copy the arguments
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Set the clamp values
	double value = std::get<double>(args[0].get()->obj);
	double min = std::get<double>(args[1].get()->obj);
	double max = std::get<double>(args[2].get()->obj);
	return std::make_shared<CObject>(clampf((float)value, (float)min, (float)max));
}
std::string CStd_fClamp::toString() { return "<std clamp>"; }

//
//
// CStd_fFloor
CStd_fFloor::CStd_fFloor(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fFloor;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("value");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::NUM, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "floor", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fFloor::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Copy the arguments
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Set the clamp values
	double value = std::get<double>(args[0].get()->obj);
	return std::make_shared<CObject>(floor((float)value));
}
std::string CStd_fFloor::toString() { return "<std clamp>"; }

//
//
// CStd_fLerp
CStd_fLerp::CStd_fLerp(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fLerp;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("t");
	paramsNames.push_back("a");
	paramsNames.push_back("b");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::NUM, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "lerp", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fLerp::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Copy the arguments
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Set the clamp values
	double t = std::get<double>(args[0].get()->obj);
	double a = std::get<double>(args[1].get()->obj);
	double b = std::get<double>(args[2].get()->obj);
	return std::make_shared<CObject>(lerpf((float)a, (float)b, (float)t));
}
std::string CStd_fLerp::toString() { return "<std lerp>"; }

//
//
// CStd_fRGB_toHSL
CStd_fRGB_toHSL::CStd_fRGB_toHSL(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fRGB_toHSL;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM_ARRAY, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("rgbColor");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::NUM_ARRAY, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "RGB_toHSL", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fRGB_toHSL::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Copy the arguments
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Set the clamp values
	std::vector<std::shared_ptr<CObject>> numArr =
		*std::get<std::shared_ptr<std::vector<std::shared_ptr<CObject>>>>(args[0].get()->obj);

	double r = clampf(abs((float)std::get<double>(numArr[0].get()->obj)), 0.0f, 1.0f);
	double g = clampf(abs((float)std::get<double>(numArr[1].get()->obj)), 0.0f, 1.0f);
	double b = clampf(abs((float)std::get<double>(numArr[2].get()->obj)), 0.0f, 1.0f);
	CColor_hsl hsl = RGB_toHSL(glm::dvec3(r, g, b));
	double a = 1.0f;
	std::vector<std::shared_ptr<CObject>> hslArr = {
		std::make_shared<CObject>(hsl.h),
		std::make_shared<CObject>(hsl.s),
		std::make_shared<CObject>(hsl.l),
		std::make_shared<CObject>(a) };
	std::shared_ptr<std::vector<std::shared_ptr<CObject>>> hslOut = std::make_shared<std::vector<std::shared_ptr<CObject>>>(hslArr);
	return std::make_shared<CObject>(CLiteralTypes::_CNumber_Array, hslOut);
}
std::string CStd_fRGB_toHSL::toString() { return "<std HSL_toRGB>"; }

//
//
// CStd_fHSL_toRGB
CStd_fHSL_toRGB::CStd_fHSL_toRGB(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fHSL_toRGB;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("degree");
	paramsNames.push_back("satPercent");
	paramsNames.push_back("valPercent");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::NUM_ARRAY, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "HSL_toRGB", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fHSL_toRGB::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Copy the arguments
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Set the clamp values
	double degree = abs(std::get<double>(args[0].get()->obj));
	float satP = clampf(abs((float)std::get<double>(args[1].get()->obj)), 0.0f, 1.0f);
	float valP = clampf(abs((float)std::get<double>(args[2].get()->obj)), 0.0f, 1.0f);
	CColor hsl = HSL_toRGB(glm::dvec3(degree / 360.0, satP, valP));
	double a = 1.0f;
	std::vector<std::shared_ptr<CObject>> rgba = {
		std::make_shared<CObject>(hsl.r),
		std::make_shared<CObject>(hsl.g),
		std::make_shared<CObject>(hsl.b),
		std::make_shared<CObject>(a) };
	std::shared_ptr<std::vector<std::shared_ptr<CObject>>> rgbaOut = std::make_shared<std::vector<std::shared_ptr<CObject>>>(rgba);
	return std::make_shared<CObject>(CLiteralTypes::_CNumber_Array, rgbaOut);
}
std::string CStd_fHSL_toRGB::toString() { return "<std HSL_toRGB>"; }

//
//
// CStd_fHueByDegree_HSL
CStd_fHueByDegree_HSL::CStd_fHueByDegree_HSL(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fHueByDegree_HSL;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("degree");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::NUM_ARRAY, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "hueByDegree_HSL", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fHueByDegree_HSL::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Copy the arguments
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Set the clamp values
	double degree = abs(std::get<double>(args[0].get()->obj));
	CColor hsl = HSL_toRGB(glm::dvec3(degree / 360.0, 1.0, 0.5));
	double a = 1.0f;
	std::vector<std::shared_ptr<CObject>> rgba = {
		std::make_shared<CObject>(hsl.r),
		std::make_shared<CObject>(hsl.g),
		std::make_shared<CObject>(hsl.b),
		std::make_shared<CObject>(a) };
	std::shared_ptr<std::vector<std::shared_ptr<CObject>>> rgbaOut = std::make_shared<std::vector<std::shared_ptr<CObject>>>(rgba);
	return std::make_shared<CObject>(CLiteralTypes::_CNumber_Array, rgbaOut);
}
std::string CStd_fHueByDegree_HSL::toString() { return "<std hueByDegree_HSL>"; }

//
//
// CStd_fSaturationByPercentDegree_HSL
CStd_fSaturationByPercentDegree_HSL::CStd_fSaturationByPercentDegree_HSL(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fSaturationByPercentDegree_HSL;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("percent");
	paramsNames.push_back("degree");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::NUM_ARRAY, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "saturationByPercentDegree_HSL", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fSaturationByPercentDegree_HSL::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Copy the arguments
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Set the values
	float percent = clampf(abs((float)std::get<double>(args[0].get()->obj)), 0.0f, 1.0f);
	double degree = abs(std::get<double>(args[1].get()->obj));
	CColor hsl = HSL_toRGB(glm::dvec3(degree / 360.0, 1.0, 0.5 + double(0.5 * (double)percent)));
	double a = 1.0f;
	std::vector<std::shared_ptr<CObject>> rgba = {
		std::make_shared<CObject>(hsl.r),
		std::make_shared<CObject>(hsl.g),
		std::make_shared<CObject>(hsl.b),
		std::make_shared<CObject>(a) };
	std::shared_ptr<std::vector<std::shared_ptr<CObject>>> rgbaOut = std::make_shared<std::vector<std::shared_ptr<CObject>>>(rgba);
	return std::make_shared<CObject>(CLiteralTypes::_CNumber_Array, rgbaOut);
}
std::string CStd_fSaturationByPercentDegree_HSL::toString() { return "<std saturationByPercentDegree_HSL>"; }

//
//
// CStd_fValueByPercent_HSL
CStd_fValueByPercent_HSL::CStd_fValueByPercent_HSL(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fValueByPercent_HSL;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("percent");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::NUM_ARRAY, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "valueByPercent_HSL", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fValueByPercent_HSL::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Copy the arguments
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Set the clamp values
	double percent = abs(std::get<double>(args[0].get()->obj));
	CColor hsl = percentValue_toRGB(percent);
	double a = 1.0f;
	std::vector<std::shared_ptr<CObject>> rgba = {
		std::make_shared<CObject>(hsl.r),
		std::make_shared<CObject>(hsl.g),
		std::make_shared<CObject>(hsl.b),
		std::make_shared<CObject>(a) };
	std::shared_ptr<std::vector<std::shared_ptr<CObject>>> rgbaOut = std::make_shared<std::vector<std::shared_ptr<CObject>>>(rgba);
	return std::make_shared<CObject>(CLiteralTypes::_CNumber_Array, rgbaOut);
}
std::string CStd_fValueByPercent_HSL::toString() { return "<std valueByPercent_HSL>"; }

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Standard Library Callback Binding Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

//
//
// CStd_fBindCallbackEvent
CStd_fBindCallbackEvent::CStd_fBindCallbackEvent(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<Application> app)
{
	this->app = app;
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fBindCallbackEvent;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::FUNCTION, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("callbackName");
	paramsNames.push_back("callerID");
	paramsNames.push_back("callback");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "bindCallbackEvent", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fBindCallbackEvent::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Copy the arguments
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	std::string callName = std::get<std::string>(args[0].get()->obj);
	std::string callerID = std::get<std::string>(args[1].get()->obj);
	if (callerID == "") { callerID = funcDeclaration.get()->name.get()->lexeme; }
	std::shared_ptr<CObject> funcObj = args[2];
	CCallbackType type = stringToCCallbackType(callName);
	if (type == CCallbackType::null)
	{
		interpreter.get()->console.get()->error("[interpreter:7501] Callback bind ID '" + callName + "' does not exist.");
		return std::make_shared<CObject>(nullptr);
	}
	if (funcObj.get()->objType.type == CLiteralTypes::_CFunction)
	{
		bool fizzle = false;
		std::shared_ptr<CFunction> funcCall = std::get<std::shared_ptr<CFunction>>(funcObj.get()->obj);
		// Dispatch to the callee in charge of the callback trigger for type checking
		switch (type)
		{
		case CCallbackType::updateFGColor:
		case CCallbackType::updateBGColor:
			fizzle = app.get()->ui.get()->bindCallback(interpreter, type, callerID, funcCall);
			break;
		case CCallbackType::colorpickStart:
		case CCallbackType::colorpickMove:
		case CCallbackType::colorpickEnd:
		case CCallbackType::toolSwitch:
			fizzle = app.get()->toolbox.get()->bindCallback(interpreter, type, callerID, funcCall);
			break;
		case CCallbackType::storedEventListener:
		case CCallbackType::keyListenerAll:
		case CCallbackType::keyListenerAll_blocking:
		case CCallbackType::keyListener:
		case CCallbackType::keyListener_blocking:
		case CCallbackType::canvasRightClickPress:
			fizzle = app.get()->bindCallback(interpreter, type, callerID, funcCall);
			break;
		}
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_fBindCallbackEvent::toString() { return "<std bindCallbackEvent>"; }

//
//
// CStd_fTriggerStoredEvent
CStd_fTriggerStoredEvent::CStd_fTriggerStoredEvent(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<Application> app)
{
	this->app = app;
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fTriggerStoredEvent;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("callbackName");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::BOOL, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "triggerStoredEvent", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fTriggerStoredEvent::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Copy the arguments
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	std::string callName = std::get<std::string>(args[0].get()->obj);

	bool result = app.get()->triggerStoredEventCallback(callName);

	return std::make_shared<CObject>(result);
}
std::string CStd_fTriggerStoredEvent::toString() { return "<std triggerStoredEvent>"; }