#include "../include/cscript/CCallable.h"
#include "../include/cscript/CCallable_Lib.h"

#include "../include/cscript/ChromaScript.h"
#include "../include/cscript/CForward.h"
#include "../include/cscript/CLiteral.h"
#include "../include/cscript/CExpr.h"
#include "../include/cscript/CStmt.h"
#include "../include/cscript/CEnvironment.h"
#include "../include/cscript/CInterpreter.h"
#include "../include/cscript/CEnums.h"
#include "../include/clayout/LEnums.h"
#include "../include/cscript/CObject.h"
#include "../include/cscript/CToken.h"
#include "../include/entities/WidgetStyle.h"

#include "../include/Application.h"
#include "../include/entities/UserInterface.h"
#include "../include/Toolbox.h"
#include "../include/entities/Widget.h"
#include "../include/structs.h"
#include "../include/keys.h"
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
// Internal Built-In Widget Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

//
//
// CInt_TextClick
CInt_TextClick::CInt_TextClick(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui)
{
	this->ui = ui;
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CInt_TextClick;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	std::vector<std::string> paramsNames;
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "@textClick", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CInt_TextClick::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	if (!ui.get()->activeWidget.expired())
	{
		// Text Input widgets automatically receive focus, even in the absense of a
		// valid callback. If a focus callback exists, then it will have been triggered
		// before reaching this point.
		ui.get()->updateFocusWidget(ui.get()->activeWidget);
		// Update the active input widget, which will signal to the program to swap
		// the input stream over to text.
		ui.get()->putActiveInputWidget(ui.get()->activeWidget, false, false, UI_WEVENT_NULL);
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CInt_TextClick::toString() { return "<int @textClick>"; }

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

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Standard Library Global Widget Operator Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

// CStd_fSetActiveProperty
CStd_fSetProperty::CStd_fSetProperty(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui)
{
	this->ui = ui;
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fSetProperty;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::ANY, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("name");
	paramsNames.push_back("value");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "setProperty", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fSetProperty::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string name = std::get<std::string>(args[0].get()->obj);
	if (!ui.get()->activeWidget.expired())
	{
		ui.get()->activeWidget.lock().get()->setProperty(interpreter, name, args[1]);
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_fSetProperty::toString() { return "<std setProperty>"; }

//
//
// CStd_fGetProperty
CStd_fGetProperty::CStd_fGetProperty(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui)
{
	this->ui = ui;
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fGetProperty;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("name");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::ANY, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "getProperty", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fGetProperty::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Get property name
	std::string name = std::get<std::string>(args[0].get()->obj);
	if (!ui.get()->activeWidget.expired())
	{
		return ui.get()->activeWidget.lock().get()->getProperty(interpreter, name);
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_fGetProperty::toString() { return "<std getProperty>"; }

//
//
// CStd_fSetChildProperty_byID
CStd_fSetChildProperty_byID::CStd_fSetChildProperty_byID(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui)
{
	this->ui = ui;
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fSetChildProperty_byID;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::ANY, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("id");
	paramsNames.push_back("name");
	paramsNames.push_back("value");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "setChildProperty_byID", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fSetChildProperty_byID::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string id = std::get<std::string>(args[0].get()->obj);
	std::string name = std::get<std::string>(args[1].get()->obj);
	if (!ui.get()->activeWidget.expired())
	{
		ui.get()->activeWidget.lock().get()->setChildProperty(0, interpreter, id, name, args[2]);
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_fSetChildProperty_byID::toString() { return "<std setChildProperty_byID>"; }

//
//
// CStd_fSetChildProperty_byType
CStd_fSetChildProperty_byType::CStd_fSetChildProperty_byType(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui)
{
	this->ui = ui;
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fSetChildProperty_byType;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::ANY, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("type");
	paramsNames.push_back("name");
	paramsNames.push_back("value");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "setChildProperty_byType", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fSetChildProperty_byType::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string typeName = std::get<std::string>(args[0].get()->obj);
	LTokenType type = LTokenType::NIL;
	if (layoutTypeMap.count(typeName) == 1) { type = layoutTypeMap.at(typeName); }
	std::string name = std::get<std::string>(args[1].get()->obj);
	if (!ui.get()->activeWidget.expired())
	{
		std::shared_ptr<std::vector<std::weak_ptr<Widget>>> bucket =
			std::make_shared<std::vector<std::weak_ptr<Widget>>>();
		ui.get()->activeWidget.lock().get()->getChild_byType(bucket, type, "");
		for (std::weak_ptr<Widget> item : *bucket)
		{
			if (!item.expired())
			{
				item.lock().get()->setProperty(interpreter, name, args[2]);
			}
		}
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_fSetChildProperty_byType::toString() { return "<std setChildProperty_byType>"; }

//
//
// CStd_fSetChildProperty_byClass
CStd_fSetChildProperty_byClass::CStd_fSetChildProperty_byClass(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui)
{
	this->ui = ui;
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fSetChildProperty_byClass;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::ANY, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("class");
	paramsNames.push_back("name");
	paramsNames.push_back("value");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "setChildProperty_byClass", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fSetChildProperty_byClass::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string className = std::get<std::string>(args[0].get()->obj);
	std::string name = std::get<std::string>(args[1].get()->obj);
	if (!ui.get()->activeWidget.expired())
	{
		std::shared_ptr<std::vector<std::weak_ptr<Widget>>> bucket = 
			std::make_shared<std::vector<std::weak_ptr<Widget>>>();
		ui.get()->activeWidget.lock().get()->getChild_byClass(bucket, className, "");
		for (std::weak_ptr<Widget> item : *bucket)
		{
			if (!item.expired())
			{
				item.lock().get()->setProperty(interpreter, name, args[2]);
			}
		}
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_fSetChildProperty_byClass::toString() { return "<std setChildProperty_byClass>"; }

//
//
// CStd_fSetChildProperty_byName
CStd_fSetChildProperty_byName::CStd_fSetChildProperty_byName(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui)
{
	this->ui = ui;
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fSetChildProperty_byName;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::ANY, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("nameID");
	paramsNames.push_back("name");
	paramsNames.push_back("value");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "setChildProperty_byClass", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fSetChildProperty_byName::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string nameName = std::get<std::string>(args[0].get()->obj);
	std::string name = std::get<std::string>(args[1].get()->obj);
	if (!ui.get()->activeWidget.expired())
	{
		ui.get()->activeWidget.lock().get()->setChildProperty(1, interpreter, nameName, name, args[2]);
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_fSetChildProperty_byName::toString() { return "<std setChildProperty_byName>"; }

//
//
// CStd_fSetClassProperty
CStd_fSetClassProperty::CStd_fSetClassProperty(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui)
{
	this->ui = ui;
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fSetClassProperty;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::ANY, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("class");
	paramsNames.push_back("name");
	paramsNames.push_back("value");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "setClassProperty", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fSetClassProperty::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string className = std::get<std::string>(args[0].get()->obj);
	std::string name = std::get<std::string>(args[1].get()->obj);

	// Call into the UI
	ui.get()->setPropertyByClass(interpreter, className, "", name, args[2]);

	return std::make_shared<CObject>(nullptr);
}
std::string CStd_fSetClassProperty::toString() { return "<std setClassProperty>"; }

//
//
// CStd_fSetClassProperty_excludingID
CStd_fSetClassProperty_excludingID::CStd_fSetClassProperty_excludingID(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui)
{
	this->ui = ui;
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fSetClassProperty_excludingID;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::ANY, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("class");
	paramsNames.push_back("exclusion");
	paramsNames.push_back("name");
	paramsNames.push_back("value");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "setClassProperty_excludingID", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fSetClassProperty_excludingID::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string className = std::get<std::string>(args[0].get()->obj);
	std::string exclusion = std::get<std::string>(args[1].get()->obj);
	std::string name = std::get<std::string>(args[2].get()->obj);
	if (!ui.get()->activeWidget.expired())
	{
		ui.get()->setPropertyByClass(interpreter, className, exclusion, name, args[3]);
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_fSetClassProperty_excludingID::toString() { return "<std setClassProperty_excludingID>"; }

//
//
// CStd_fSetClassProperty_excludingActive
CStd_fSetClassProperty_excludingActive::CStd_fSetClassProperty_excludingActive(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui)
{
	this->ui = ui;
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fSetClassProperty_excludingActive;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::ANY, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("class");
	paramsNames.push_back("name");
	paramsNames.push_back("value");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "setClassProperty_excludingActive", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fSetClassProperty_excludingActive::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string className = std::get<std::string>(args[0].get()->obj);
	std::weak_ptr<Widget> exclusion = ui.get()->activeWidget;
	std::string name = std::get<std::string>(args[1].get()->obj);
	if (!ui.get()->activeWidget.expired())
	{
		ui.get()->setPropertyByClass(interpreter, className, exclusion, name, args[2]);
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_fSetClassProperty_excludingActive::toString() { return "<std setClassProperty_excludingActive>"; }

//
//
// CStd_fResetProperty
CStd_fResetProperty::CStd_fResetProperty(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui)
{
	this->ui = ui;
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fResetProperty;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("name");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "resetProperty", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fResetProperty::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string name = std::get<std::string>(args[0].get()->obj);
	if (!ui.get()->activeWidget.expired())
	{
		ui.get()->activeWidget.lock().get()->resetProperty(interpreter, name);
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_fResetProperty::toString() { return "<std resetProperty>"; }

//
//
// CStd_fResetClassProperty
CStd_fResetClassProperty::CStd_fResetClassProperty(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui)
{
	this->ui = ui;
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fResetClassProperty;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("class");
	paramsNames.push_back("name");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "resetClassProperty", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fResetClassProperty::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string className = std::get<std::string>(args[0].get()->obj);
	std::string name = std::get<std::string>(args[1].get()->obj);
	if (!ui.get()->activeWidget.expired())
	{
		ui.get()->resetPropertyByClass(interpreter, className, name);
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_fResetClassProperty::toString() { return "<std resetClassProperty>"; }

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Standard Library Classes
//
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
//
// CStd_App
//
////////////////////////////////////////////////////////////////////////////////////////////////

CStd_cApp::CStd_cApp(std::shared_ptr<CEnvironment> classEnv, std::shared_ptr<Application> app)
{
	this->name = "App";
	this->type = CCallableTypes::_CStd_cApp;
	this->classEnv = classEnv;
	this->superClass = nullptr;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CStmt>> methods;
	this->classEnv.get()->define(
		"exit", 
		std::make_shared<CObject>(CCallableTypes::_CStd_cfExit, classEnv.get()->lookupEnvironment("exit", true)));
	this->classEnv.get()->define(
		"saveBMP",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfSaveBMP, classEnv.get()->lookupEnvironment("saveBMP", true)));
	this->classEnv.get()->define(
		"getWindowSize",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfGetWindowSize, classEnv.get()->lookupEnvironment("getWindowSize", true)));
	this->classEnv.get()->define(
		"minimize", 
		std::make_shared<CObject>(CCallableTypes::_CStd_cfMinimize, classEnv.get()->lookupEnvironment("minimize", true)));
	this->classEnv.get()->define(
		"maximize", 
		std::make_shared<CObject>(CCallableTypes::_CStd_cfMaximize, classEnv.get()->lookupEnvironment("maximize", true)));
	this->classEnv.get()->define(
		"setCursor", 
		std::make_shared<CObject>(CCallableTypes::_CStd_cfSetCursor, classEnv.get()->lookupEnvironment("setCursor", true)));
	this->classEnv.get()->define(
		"isValidKeybind_tool",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfIsValidKeybind_Tool, classEnv.get()->lookupEnvironment("isValidKeybind_tool", true)));
	for (auto const& item : this->classEnv.get()->values)
	{
		std::shared_ptr<CStmt> func = std::get<std::shared_ptr<CFunction>>(item.second.get()->obj).get()->funcDeclaration;
		methods.push_back(func);
	}

	// Build the properties
	std::vector<std::shared_ptr<CStmt>> properties;
	// Raw App Literal Object (Hidden using @)
	this->classEnv.get()->define("@appObj", std::make_shared<CObject>(app));

	// Native 'Ui' Wrapper Class Object - Use secure "global" get to ensure good get
	std::shared_ptr<CClass> uiClassTemplate = std::get<std::shared_ptr<CClass>>(classEnv.get()->get({ "global" }, "Ui").get()->obj);
	this->classEnv.get()->define("ui", std::make_shared<CObject>(classEnv, "ui", uiClassTemplate));
	// Native 'Toolbox' Wrapper Class Object - Use secure "global" get to ensure good get
	std::shared_ptr<CClass> toolboxClassTemplate = std::get<std::shared_ptr<CClass>>(classEnv.get()->get({ "global" }, "Toolbox").get()->obj);
	this->classEnv.get()->define("toolbox", std::make_shared<CObject>(classEnv, "toolbox", toolboxClassTemplate));

	// Construct the pseudo-CStmt_Class object
	this->classDeclaration = std::make_shared<CStmt_Class>(
		scopeStack,	// scopeStack (empty)
		std::make_shared<CToken>(CTokenType::IDENTIFIER, this->name, -1),	// name
		nullptr,	// superClass
		false,		// isDeclarationOnly
		nullptr,	// constructor
		methods,	// methods
		properties	// properties
		);
}
std::string CStd_cApp::toString() { return name; }

////////////////////////////////////////////////////////////////////////////////////////////////
//
// App Class Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

//
//
// CStd_cfExit
CStd_cfExit::CStd_cfExit(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfExit;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	std::vector<std::string> paramsNames;
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "exit", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfExit::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Fetch the appObj, always has the same value name
	std::get<std::shared_ptr<Application>>(
		interpreter.get()->currentEnvironment.get()->values.at("@appObj").get()->obj).get()->closeApplication();
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfExit::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfSaveBMP
CStd_cfSaveBMP::CStd_cfSaveBMP(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfSaveBMP;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	std::vector<std::string> paramsNames;
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "saveBMP", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfSaveBMP::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Fetch the appObj, always has the same value name
	std::get<std::shared_ptr<Application>>(
		interpreter.get()->currentEnvironment.get()->values.at("@appObj").get()->obj).get()->renderCanvas_toFile();
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfSaveBMP::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfGetWindowSize
CStd_cfGetWindowSize::CStd_cfGetWindowSize(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfGetWindowSize;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	std::vector<std::string> paramsNames;
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::NUM_ARRAY, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "getWindowSize", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfGetWindowSize::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Fetch the appObj, always has the same value name
	Application* app = std::get<std::shared_ptr<Application>>(
		interpreter.get()->currentEnvironment.get()->values.at("@appObj").get()->obj).get();
	// Create the return object
	std::vector<std::shared_ptr<CObject>> returnList;
	returnList.emplace_back(std::make_shared<CObject>((double)app->getWindowWidth()));
	returnList.emplace_back(std::make_shared<CObject>((double)app->getWindowHeight()));
	return std::make_shared<CObject>(CLiteralTypes::_CNumber_Array, std::make_shared<std::vector<std::shared_ptr<CObject>>>(returnList));
}
std::string CStd_cfGetWindowSize::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfMaximize
CStd_cfMaximize::CStd_cfMaximize(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfExit;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	std::vector<std::string> paramsNames;
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "exit", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfMaximize::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Fetch the appObj, always has the same value name
	std::get<std::shared_ptr<Application>>(
		interpreter.get()->currentEnvironment.get()->values.at("@appObj").get()->obj).get()->maximizeApplication();
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfMaximize::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfMinimize
CStd_cfMinimize::CStd_cfMinimize(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfExit;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	std::vector<std::string> paramsNames;
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "exit", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfMinimize::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Fetch the appObj, always has the same value name
	std::get<std::shared_ptr<Application>>(
		interpreter.get()->currentEnvironment.get()->values.at("@appObj").get()->obj).get()->minimizeApplication();
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfMinimize::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfSetCursor
CStd_cfSetCursor::CStd_cfSetCursor(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfExit;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("name");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "setCursor", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfSetCursor::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string name = std::get<std::string>(args[0].get()->obj);
	// Fetch the appObj, always has the same value name
	std::get<std::shared_ptr<Application>>(
		interpreter.get()->currentEnvironment.get()->values.at("@appObj").get()->obj).get()->setCursor(convertCursorName(name));
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfSetCursor::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfIsValidKeybind_Tool
CStd_cfIsValidKeybind_Tool::CStd_cfIsValidKeybind_Tool(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfIsValidKeybind_Tool;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("modKeyBit");
	paramsNames.push_back("glfwKey");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::BOOL, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "isValidKeybind_tool", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfIsValidKeybind_Tool::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	int modKey = (int)std::get<double>(args[0].get()->obj);
	int glfwKey = (int)std::get<double>(args[1].get()->obj);
	// Fetch the appObj, always has the same value name
	bool result = std::get<std::shared_ptr<Application>>(
		interpreter.get()->currentEnvironment.get()->values.at("@appObj").get()->obj).get()->isValidKeybind_tool(modKey, glfwKey);
	return std::make_shared<CObject>(result);
}
std::string CStd_cfIsValidKeybind_Tool::toString() { return funcDeclaration.get()->name.get()->lexeme; }


////////////////////////////////////////////////////////////////////////////////////////////////
//
// CStd_Ui
//
////////////////////////////////////////////////////////////////////////////////////////////////

CStd_cUi::CStd_cUi(std::shared_ptr<CEnvironment> classEnv, std::shared_ptr<Application> app)
{
	this->name = "Ui";
	this->type = CCallableTypes::_CStd_cUi;
	this->classEnv = classEnv;
	this->superClass = nullptr;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CStmt>> methods;
	this->classEnv.get()->define(
		"moveRootToFront",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfMoveRootToFront, classEnv.get()->lookupEnvironment("moveRootToFront", true)));
	this->classEnv.get()->define(
		"preventBlurCallback",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfPreventBlurCallback, classEnv.get()->lookupEnvironment("preventBlurCallback", true)));
	this->classEnv.get()->define(
		"preventFocusCallback",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfPreventFocusCallback, classEnv.get()->lookupEnvironment("preventFocusCallback", true)));
	this->classEnv.get()->define(
		"setActivePopup",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfSetActivePopup, classEnv.get()->lookupEnvironment("setActivePopup", true)));
	this->classEnv.get()->define(
		"clearPopup",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfClearPopup, classEnv.get()->lookupEnvironment("clearPopup", true)));
	this->classEnv.get()->define(
		"setFocus_byID",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfSetFocus_byID, classEnv.get()->lookupEnvironment("setFocus_byID", true)));
	this->classEnv.get()->define(
		"clearFocus",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfClearFocus, classEnv.get()->lookupEnvironment("clearFocus", true)));
	this->classEnv.get()->define(
		"getWidget_byID",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfGetWidget_byID, classEnv.get()->lookupEnvironment("getWidget_byID", true)));
	this->classEnv.get()->define(
		"setFGColor_HSL",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfSetFGColor_HSL, classEnv.get()->lookupEnvironment("setFGColor_HSL", true)));
	this->classEnv.get()->define(
		"setBGColor_HSL",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfSetBGColor_HSL, classEnv.get()->lookupEnvironment("setBGColor_HSL", true)));
	for (auto const& item : this->classEnv.get()->values)
	{
		std::shared_ptr<CStmt> func = std::get<std::shared_ptr<CFunction>>(item.second.get()->obj).get()->funcDeclaration;
		methods.push_back(func);
	}

	// Build the properties
	std::vector<std::shared_ptr<CStmt>> properties;
	// Raw UI Literal Object (Hidden using @)
	this->classEnv.get()->define("@uiObj", std::make_shared<CObject>(app.get()->getUI()));

	// Construct the pseudo-CStmt_Class object
	this->classDeclaration = std::make_shared<CStmt_Class>(
		scopeStack,	// scopeStack (empty)
		std::make_shared<CToken>(CTokenType::IDENTIFIER, this->name, -1),	// name
		nullptr,	// superClass
		false,		// isDeclarationOnly
		nullptr,	// constructor
		methods,	// methods
		properties	// properties
		);
}
std::string CStd_cUi::toString() { return name; }

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Ui Class Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

//
//
// CStd_cfMoveRootToFront
CStd_cfMoveRootToFront::CStd_cfMoveRootToFront(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfMoveRootToFront;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("id");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "moveRootToFront", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfMoveRootToFront::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string & Arg 1 as Function
	std::string id = std::get<std::string>(args[0].get()->obj);
	// Check that the widget id is valid
	UI* ui = std::get<std::shared_ptr<UI>>(
		interpreter.get()->currentEnvironment.get()->values.at("@uiObj").get()->obj).get();
	std::weak_ptr<Widget> widget = ui->getWidgetByID(id);
	if (!widget.expired())
	{
		ui->moveRootToFront(widget.lock().get()->getRoot());
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfMoveRootToFront::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfSetActivePopup
CStd_cfSetActivePopup::CStd_cfSetActivePopup(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfSetActivePopup;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::BOOL, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::FUNCTION, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("id");
	paramsNames.push_back("isBlocking");
	paramsNames.push_back("escapeCallback");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "setActivePopup", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfSetActivePopup::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string & Arg 1 as Function
	std::string id = std::get<std::string>(args[0].get()->obj);
	bool isBlocking = std::get<bool>(args[1].get()->obj);
	std::shared_ptr<CFunction> callback = std::get<std::shared_ptr<CFunction>>(args[2].get()->obj);
	// Check that the widget id is valid
	UI* ui = std::get<std::shared_ptr<UI>>(
		interpreter.get()->currentEnvironment.get()->values.at("@uiObj").get()->obj).get();
	std::weak_ptr<Widget> widget = ui->getWidgetByID(id);
	if (!widget.expired())
	{
		ui->putActivePopupWidget(widget, isBlocking, callback);
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfSetActivePopup::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfClearPopup
CStd_cfClearPopup::CStd_cfClearPopup(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfClearPopup;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("id");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "clearPopup", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfClearPopup::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string id = std::get<std::string>(args[0].get()->obj);
	// Get the UI
	UI* ui = std::get<std::shared_ptr<UI>>(
		interpreter.get()->currentEnvironment.get()->values.at("@uiObj").get()->obj).get();
	std::weak_ptr<Widget> widget = ui->getWidgetByID(id);
	if (!widget.expired())
	{
		ui->clearPopupWidget(widget);
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfClearPopup::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfPreventBlurCallback
CStd_cfPreventBlurCallback::CStd_cfPreventBlurCallback(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfSetFocus_byID;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	std::vector<std::string> paramsNames;
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "preventBlurCallback", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfPreventBlurCallback::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Fetch the UI
	std::weak_ptr<UI> ui = std::get<std::shared_ptr<UI>>(
		interpreter.get()->currentEnvironment.get()->values.at("@uiObj").get()->obj);
	if (!ui.expired())
	{
		ui.lock().get()->interruptBlur = true;
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfPreventBlurCallback::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfPreventFocusCallback
CStd_cfPreventFocusCallback::CStd_cfPreventFocusCallback(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfPreventFocusCallback;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	std::vector<std::string> paramsNames;
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "preventFocusCallback", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfPreventFocusCallback::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Fetch the UI
	std::weak_ptr<UI> ui = std::get<std::shared_ptr<UI>>(
		interpreter.get()->currentEnvironment.get()->values.at("@uiObj").get()->obj);
	if (!ui.expired())
	{
		ui.lock().get()->interruptFocus = true;
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfPreventFocusCallback::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfSetFocus_byID
CStd_cfSetFocus_byID::CStd_cfSetFocus_byID(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfSetFocus_byID;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("id");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "setFocus_byID", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfSetFocus_byID::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string id = std::get<std::string>(args[0].get()->obj);
	// Call getWidget_byID on the UI
	std::weak_ptr<UI> ui = std::get<std::shared_ptr<UI>>(
		interpreter.get()->currentEnvironment.get()->values.at("@uiObj").get()->obj);
	std::weak_ptr<Widget> widget = ui.lock().get()->getWidgetByID(id);
	if (!widget.expired())
	{
		ui.lock().get()->updateFocusWidget(widget);
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfSetFocus_byID::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfClearFocus
CStd_cfClearFocus::CStd_cfClearFocus(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfClearFocus;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	std::vector<std::string> paramsNames;
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "clearFocus", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfClearFocus::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Call getWidget_byID on the UI
	std::weak_ptr<UI> ui = std::get<std::shared_ptr<UI>>(
		interpreter.get()->currentEnvironment.get()->values.at("@uiObj").get()->obj);
	ui.lock().get()->clearFocusWidget();
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfClearFocus::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfGetWidget_byID
CStd_cfGetWidget_byID::CStd_cfGetWidget_byID(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fSetChildProperty_byID;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("id");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "Widget", -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "setChildProperty_byID", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfGetWidget_byID::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string id = std::get<std::string>(args[0].get()->obj);
	// Call getWidget_byID on the UI
	std::weak_ptr<Widget> widget = std::get<std::shared_ptr<UI>>(
		interpreter.get()->currentEnvironment.get()->values.at("@uiObj").get()->obj).get()->getWidgetByID(id);
	if (!widget.expired())
	{
		std::vector<std::shared_ptr<CObject>> instanceArgs;
		std::shared_ptr<CObject> callee = interpreter.get()->currentEnvironment.get()->get({ "global" }, "Widget");
		instanceArgs.push_back(callee);
		instanceArgs.push_back(args[0]);
		std::shared_ptr<CObject> returnObj = std::get<std::shared_ptr<CClass>>(callee.get()->obj).get()->call(interpreter, &instanceArgs);
		std::get<std::shared_ptr<CInstance>>(returnObj.get()->obj).get()->instanceEnv.get()->get("@widgetObj").get()->obj = widget;
		return returnObj;
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfGetWidget_byID::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfSetFGColor_HSL
CStd_cfSetFGColor_HSL::CStd_cfSetFGColor_HSL(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfSetFGColor_HSL;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("hueDegree");
	paramsNames.push_back("satPercent");
	paramsNames.push_back("valPercent");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "setFGColor_HSL", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfSetFGColor_HSL::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Set the values
	double degree = abs(std::get<double>(args[0].get()->obj));
	float satP = clampf(abs((float)std::get<double>(args[1].get()->obj)), 0.0f, 1.0f);
	float valP = clampf(abs((float)std::get<double>(args[2].get()->obj)), 0.0f, 1.0f);
	CColor hsl = HSL_toRGB(glm::dvec3(degree / 360.0, satP, valP));
	std::get<std::shared_ptr<UI>>(
		interpreter.get()->currentEnvironment.get()->values.at("@uiObj").get()->obj).get()->updateFGColor(hsl, 0, 0);
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfSetFGColor_HSL::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfSetBGColor_HSL
CStd_cfSetBGColor_HSL::CStd_cfSetBGColor_HSL(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfSetBGColor_HSL;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("hueDegree");
	paramsNames.push_back("satPercent");
	paramsNames.push_back("valPercent");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "Widget", -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "setBGColor_HSL", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfSetBGColor_HSL::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Set the values
	double degree = abs(std::get<double>(args[0].get()->obj));
	float satP = clampf(abs((float)std::get<double>(args[1].get()->obj)), 0.0f, 1.0f);
	float valP = clampf(abs((float)std::get<double>(args[2].get()->obj)), 0.0f, 1.0f);
	CColor hsl = HSL_toRGB(glm::dvec3(degree / 360.0, satP, valP));
	std::get<std::shared_ptr<UI>>(
		interpreter.get()->currentEnvironment.get()->values.at("@uiObj").get()->obj).get()->updateBGColor(hsl, 0, 0);
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfSetBGColor_HSL::toString() { return funcDeclaration.get()->name.get()->lexeme; }

////////////////////////////////////////////////////////////////////////////////////////////////
//
// CStd_Document
//
////////////////////////////////////////////////////////////////////////////////////////////////

CStd_cDocument::CStd_cDocument(std::shared_ptr<CEnvironment> classEnv, std::shared_ptr<Application> app)
{
	//this->classDeclaration = classDeclaration;
	//this->name = name;
	//this->type = type;
	//this->classEnv = classEnv;
	//this->superClass = superClass;
}
std::string CStd_cDocument::toString() { return name; }

////////////////////////////////////////////////////////////////////////////////////////////////
//
// CStd_cToolbox
//
////////////////////////////////////////////////////////////////////////////////////////////////

CStd_cToolbox::CStd_cToolbox(std::shared_ptr<CEnvironment> classEnv, std::shared_ptr<Application> app)
{
	this->name = "Toolbox";
	this->type = CCallableTypes::_CStd_cToolbox;
	this->classEnv = classEnv;
	this->superClass = nullptr;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CStmt>> methods;
	this->classEnv.get()->define(
		"checkValidIOCombination",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfCheckValidIOCombination, classEnv.get()->lookupEnvironment("checkValidIOCombination", true)));
	this->classEnv.get()->define(
		"checkValidControlScheme",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfCheckValidControlScheme, classEnv.get()->lookupEnvironment("checkValidControlScheme", true)));
	this->classEnv.get()->define(
		"createNewTool",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfCreateNewTool, classEnv.get()->lookupEnvironment("createNewTool", true)));
	this->classEnv.get()->define(
		"checkActiveToolSettingsMask",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfCheckActiveToolSettingsMask, classEnv.get()->lookupEnvironment("checkActiveToolSettingsMask", true)));
	this->classEnv.get()->define(
		"setActiveToolProp",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfSetActiveToolProp, classEnv.get()->lookupEnvironment("setActiveToolProp", true)));
	this->classEnv.get()->define(
		"getActiveToolProp",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfGetActiveToolProp, classEnv.get()->lookupEnvironment("getActiveToolProp", true)));
	for (auto const& item : this->classEnv.get()->values)
	{
		std::shared_ptr<CStmt> func = std::get<std::shared_ptr<CFunction>>(item.second.get()->obj).get()->funcDeclaration;
		methods.push_back(func);
	}

	// Build the properties
	std::vector<std::shared_ptr<CStmt>> properties;
	// Raw UI Literal Object (Hidden using @)
	this->classEnv.get()->define("@toolboxObj", std::make_shared<CObject>(app.get()->getToolbox()));

	// Construct the pseudo-CStmt_Class object
	this->classDeclaration = std::make_shared<CStmt_Class>(
		scopeStack,	// scopeStack (empty)
		std::make_shared<CToken>(CTokenType::IDENTIFIER, this->name, -1),	// name
		nullptr,	// superClass
		false,		// isDeclarationOnly
		nullptr,	// constructor
		methods,	// methods
		properties	// properties
		);
}
std::string CStd_cToolbox::toString() { return name; }

////////////////////////////////////////////////////////////////////////////////////////////////
//
// CStd_cToolbox Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

//
//
// CStd_cfCheckValidIOCombination
CStd_cfCheckValidIOCombination::CStd_cfCheckValidIOCombination(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfCheckValidIOCombination;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("inputMethod");
	paramsNames.push_back("outputMethod");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::BOOL, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "checkValidIOCombination", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfCheckValidIOCombination::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	std::string inputName = std::get<std::string>(args[0].get()->obj);
	std::string outputName = std::get<std::string>(args[1].get()->obj);
	// Set the values
	bool result = std::get<std::shared_ptr<Toolbox>>(
		interpreter.get()->currentEnvironment.get()->values.at("@toolboxObj").get()->obj).get()->checkValidIOCombination(
			interpreter, inputName, outputName);
	return std::make_shared<CObject>(result);
}
std::string CStd_cfCheckValidIOCombination::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfCheckValidControlScheme
CStd_cfCheckValidControlScheme::CStd_cfCheckValidControlScheme(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfCheckValidControlScheme;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("inputMethod");
	paramsNames.push_back("controlScheme");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::BOOL, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "checkValidControlScheme", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfCheckValidControlScheme::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	std::string inputName = std::get<std::string>(args[0].get()->obj);
	std::string controlScheme = std::get<std::string>(args[1].get()->obj);
	// Set the values
	bool result = std::get<std::shared_ptr<Toolbox>>(
		interpreter.get()->currentEnvironment.get()->values.at("@toolboxObj").get()->obj).get()->checkValidControlScheme(
			interpreter, inputName, controlScheme);
	return std::make_shared<CObject>(result);
}
std::string CStd_cfCheckValidControlScheme::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfCreateNewTool
CStd_cfCreateNewTool::CStd_cfCreateNewTool(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfCreateNewTool;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("cursorHover");
	paramsNames.push_back("cursorPress");
	paramsNames.push_back("toolID");
	paramsNames.push_back("toolName");
	paramsNames.push_back("inputMethod");
	paramsNames.push_back("controlScheme");
	paramsNames.push_back("outputMethod");
	paramsNames.push_back("keybind_modBit");
	paramsNames.push_back("keybind_glfwKey");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::BOOL, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "createNewTool", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfCreateNewTool::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	std::string cursorHover = std::get<std::string>(args[0].get()->obj);
	std::string cursorPress = std::get<std::string>(args[1].get()->obj);
	int toolID = (int)std::get<double>(args[2].get()->obj);
	std::string toolName = std::get<std::string>(args[3].get()->obj);
	std::string inputMethod = std::get<std::string>(args[4].get()->obj);
	std::string controlScheme = std::get<std::string>(args[5].get()->obj);
	std::string outputMethod = std::get<std::string>(args[6].get()->obj);
	int keybind_modBit = (int)std::get<double>(args[7].get()->obj);
	int keybind_glfwKey = (int)std::get<double>(args[8].get()->obj);
	// Set the values
	bool result = std::get<std::shared_ptr<Toolbox>>(
		interpreter.get()->currentEnvironment.get()->values.at("@toolboxObj").get()->obj).get()->createCustomTool(
			cursorHover, cursorPress, toolID, toolName, inputMethod, 
			controlScheme, outputMethod, keybind_modBit, keybind_glfwKey);
	if (!result)
	{
		interpreter.get()->console.get()->error("");
	}
	return std::make_shared<CObject>(result);
}
std::string CStd_cfCreateNewTool::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfCheckActiveToolSettingsMask
CStd_cfCheckActiveToolSettingsMask::CStd_cfCheckActiveToolSettingsMask(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfCheckActiveToolSettingsMask;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("maskName");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::BOOL, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "checkActiveToolSettingsMask", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfCheckActiveToolSettingsMask::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	std::string settingsMaskName = std::get<std::string>(args[0].get()->obj);
	// Set the values
	bool result = std::get<std::shared_ptr<Toolbox>>(
		interpreter.get()->currentEnvironment.get()->values.at("@toolboxObj").get()->obj).get()->getActiveTool().get()->checkInterestMask(
			interpreter, settingsMaskName);
	return std::make_shared<CObject>(result);
}
std::string CStd_cfCheckActiveToolSettingsMask::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfSetActiveToolProp
CStd_cfSetActiveToolProp::CStd_cfSetActiveToolProp(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfSetActiveToolProp;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::ANY, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("settingKeySig");
	paramsNames.push_back("value");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "setActiveToolProp", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfSetActiveToolProp::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	int sig = (int)std::get<double>(args[0].get()->obj);
	// Set the values
	std::get<std::shared_ptr<Toolbox>>(
		interpreter.get()->currentEnvironment.get()->values.at("@toolboxObj").get()->obj).get()->getActiveTool().get()->putProperty(
			interpreter, sig, args[1], false);
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfSetActiveToolProp::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfGetActiveToolProp
CStd_cfGetActiveToolProp::CStd_cfGetActiveToolProp(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfGetActiveToolProp;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("settingKeySig");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::ANY, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "getActiveToolProp", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfGetActiveToolProp::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	int sig = (int)std::get<double>(args[0].get()->obj);
	// Set the values
	std::shared_ptr<CObject> returnObj = std::get<std::shared_ptr<Toolbox>>(
		interpreter.get()->currentEnvironment.get()->values.at("@toolboxObj").get()->obj).get()->getActiveTool().get()->putProperty(
			interpreter, sig, nullptr, true);
	return returnObj;
}
std::string CStd_cfGetActiveToolProp::toString() { return funcDeclaration.get()->name.get()->lexeme; }

////////////////////////////////////////////////////////////////////////////////////////////////
//
// CStd_Widget 
//
////////////////////////////////////////////////////////////////////////////////////////////////

CStd_cWidget::CStd_cWidget(std::shared_ptr<CEnvironment> classEnv)
{
	this->name = "Widget";
	this->type = CCallableTypes::_CStd_cWidget;
	this->classEnv = classEnv;
	this->superClass = nullptr;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	// Build the methods
	std::vector<std::shared_ptr<CStmt>> methods;
	this->classEnv.get()->define(
		"setProperty", 
		std::make_shared<CObject>(CCallableTypes::_CStd_cfSetProperty, classEnv.get()->lookupEnvironment("setProperty", true)));
	this->classEnv.get()->define(
		"getProperty",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfGetProperty, classEnv.get()->lookupEnvironment("getProperty", true)));
	this->classEnv.get()->define(
		"setPosition", 
		std::make_shared<CObject>(CCallableTypes::_CStd_cfSetPosition, classEnv.get()->lookupEnvironment("setPosition", true)));
	for (auto const& item : this->classEnv.get()->values)
	{
		std::shared_ptr<CStmt> func = std::get<std::shared_ptr<CFunction>>(item.second.get()->obj).get()->funcDeclaration;
		methods.push_back(func);
	}
	
	// Create properties
	std::vector<std::shared_ptr<CStmt>> properties;
	// Raw Empty Widget Literal Object (Hidden using @) - Set this when returning a widget
	this->classEnv.get()->define("@widgetObj", std::make_shared<CObject>(CLiteral(CLiteralTypes::_CWidget, this->name), "@widgetObj"));
	
	// Create the pseudo-CStmt_Class
	this->classDeclaration = std::make_shared<CStmt_Class>(
		scopeStack,	// scopeStack (empty)
		std::make_shared<CToken>(CTokenType::IDENTIFIER, this->name, -1),	// name
		nullptr,	// superClass
		false,		// isDeclarationOnly
		nullptr,	// constructor
		methods,	// methods
		properties	// properties
		);
}
std::string CStd_cWidget::toString() { return name; }

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Widget Class Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

//
//
// CStd_cfSetProperty
CStd_cfSetProperty::CStd_cfSetProperty(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fSetProperty;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::ANY, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("name");
	paramsNames.push_back("value");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "setProperty", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfSetProperty::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string name = std::get<std::string>(args[0].get()->obj);
	// Call setProperty on the widgetObj
	std::get<std::weak_ptr<Widget>>(
		interpreter.get()->currentEnvironment.get()->values.at("@widgetObj").get()->obj).lock().get()->setProperty(interpreter, name, args[1]);
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfSetProperty::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfGetProperty
CStd_cfGetProperty::CStd_cfGetProperty(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fSetProperty;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("name");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::ANY, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "getProperty", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfGetProperty::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string name = std::get<std::string>(args[0].get()->obj);
	// Call getProperty on the widgetObj
	return std::get<std::weak_ptr<Widget>>(
		interpreter.get()->currentEnvironment.get()->values.at("@widgetObj").get()->obj).lock().get()->getProperty(interpreter, name);
}
std::string CStd_cfGetProperty::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfSetPosition
CStd_cfSetPosition::CStd_cfSetPosition(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfSetPosition;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("x");
	paramsNames.push_back("y");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "setPosition", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfSetPosition::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	int x = (int)std::get<double>(args[0].get()->obj);
	int y = (int)std::get<double>(args[1].get()->obj);
	// Call setProperty on the widgetObj
	std::get<std::weak_ptr<Widget>>(
		interpreter.get()->currentEnvironment.get()->values.at("@widgetObj").get()->obj).lock().get()->setLocation(x, y);
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfSetPosition::toString() { return funcDeclaration.get()->name.get()->lexeme; }

////////////////////////////////////////////////////////////////////////////////////////////////
//
// CStd_Style
//
////////////////////////////////////////////////////////////////////////////////////////////////

// CStd_Style
CStd_cStyle::CStd_cStyle(std::shared_ptr<CEnvironment> classEnv)
{
	this->name = "Style";
	this->type = CCallableTypes::_CStd_cStyle;
	this->classEnv = classEnv;
	this->superClass = nullptr;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CStmt>> methods;
	this->classEnv.get()->define("setProperty_byName", std::make_shared<CObject>(CCallableTypes::_CStd_cfSetProperty_byName, this->classEnv));
	this->classEnv.get()->define("getProperty_byName", std::make_shared<CObject>(CCallableTypes::_CStd_cfGetProperty_byName, this->classEnv));
	for (auto const& item : this->classEnv.get()->values)
	{
		std::shared_ptr<CStmt> func = std::get<std::shared_ptr<CFunction>>(item.second.get()->obj).get()->funcDeclaration;
		methods.push_back(func);
	}

	// Build the properties
	std::vector<std::shared_ptr<CStmt>> properties;
	// Raw Style Literal Object (Hidden using @)
	this->classEnv.get()->define("@styleObj", std::make_shared<CObject>(std::make_shared<WidgetStyle>()));
	this->classDeclaration = std::make_shared<CStmt_Class>(
		scopeStack,	// scopeStack (empty)
		std::make_shared<CToken>(CTokenType::IDENTIFIER, this->name, -1),	// name
		nullptr,	// superClass
		false,		// isDeclarationOnly
		nullptr,	// constructor
		methods,	// methods
		properties	// properties
		);
}
std::string CStd_cStyle::toString() { return name; }

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Style Class Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

// CStd_cfSetProperty_byName
CStd_cfSetProperty_byName::CStd_cfSetProperty_byName(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfSetProperty_byName;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::ANY, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("name");
	paramsNames.push_back("value");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "setProperty_byName", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfSetProperty_byName::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string name = std::get<std::string>(args[0].get()->obj);
	// Fetch the styleObj, always has the same value name
	std::shared_ptr<WidgetStyle> thisStyle =
		std::get<std::shared_ptr<WidgetStyle>>(
			interpreter.get()->currentEnvironment.get()->values.at("@styleObj").get()->obj);
	thisStyle.get()->getSetProperty(interpreter, name, args[1], false);
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfSetProperty_byName::toString() { return funcDeclaration.get()->name.get()->lexeme; }

// CStd_cfGetProperty_byName
CStd_cfGetProperty_byName::CStd_cfGetProperty_byName(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfGetProperty_byName;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("name");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::ANY, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "getProperty_byName", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfGetProperty_byName::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Get property name
	std::string name = std::get<std::string>(args[0].get()->obj);
	// Fetch the styleObj, always has the same value name
	std::shared_ptr<WidgetStyle> thisStyle = 
		std::get<std::shared_ptr<WidgetStyle>>(
			interpreter.get()->currentEnvironment.get()->values.at("@styleObj").get()->obj);
	return thisStyle.get()->getSetProperty(interpreter, name, nullptr, true);
}
std::string CStd_cfGetProperty_byName::toString() { return funcDeclaration.get()->name.get()->lexeme; }

