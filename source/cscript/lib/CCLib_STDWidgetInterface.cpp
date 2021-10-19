#include "../../include/cscript/CCallable.h"
#include "../../include/cscript/CCallable_Lib.h"
#include "../../include/cscript/lib/CCLib_STDWidgetInterface.h"

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
#include "../../include/entities/widgets/WidgetStyle.h"
#include "../../include/entities/widgets/Graph_ToolControl.h"

#include "../../include/Application.h"
#include "../../include/entities/UserInterface.h"
#include "../../include/tool/Toolbox.h"
#include "../../include/entities/widgets/Widget.h"
#include "../../include/input/keys.h"
#include <glm.hpp>

#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <iostream>
#include <variant>
#include <map>
#include <memory>

extern std::shared_ptr<Application> chromaIO;

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Line/Paragraph Built In Functions
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
std::string CInt_TextClick::toString() { return funcDeclaration.get()->name.get()->lexeme; }

////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGraph Built In Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

//
//
// CInt_TGraph_Hover

CInt_TGraph_Hover::CInt_TGraph_Hover(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui)
{
	this->ui = ui;
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CInt_TGraph_Drag;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("hoverSelector");
	paramsNames.push_back("targetType");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "@TGraph_Hover", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CInt_TGraph_Hover::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	std::shared_ptr<CEnvironment> previous = interpreter.get()->currentEnvironment;

	interpreter.get()->setEnvironment(interpreter.get()->getEnvironment("global"));
	std::shared_ptr<CEnvironment> current = interpreter.get()->currentEnvironment;

	if (!ui.get()->activeWidget.expired())
	{
		int hoverSelect = (int)std::get<double>(args[0].get()->obj);
		int targetType = (int)std::get<double>(args[1].get()->obj);
		if (hoverSelect == 1)
		{
			ui.get()->activeWidget.lock()->setProperty(interpreter, "background-color", std::make_shared<CObject>(glm::dvec4(0.62, 0.62, 0.62, 1.0)));
		}
		else if (hoverSelect == 0)
		{
			if (targetType == 1) 
			{
				ui.get()->activeWidget.lock()->setProperty(interpreter, "background-color", std::make_shared<CObject>(glm::dvec4(0.89, 0.89, 0.89, 1.0)));
			}
			else if (targetType == 0)
			{
				if (chromaIO.get()->getModKeys() == InputModKey::ctrl)
				{
					ui.get()->activeWidget.lock()->setProperty(interpreter, "background-color", std::make_shared<CObject>(glm::dvec4(0.94, 0.32, 0.32, 1.0)));
				}
				else
				{
					ui.get()->activeWidget.lock()->setProperty(interpreter, "background-color", std::make_shared<CObject>(glm::dvec4(0.89, 0.89, 0.89, 1.0)));
				}
			}
		}
	}
	interpreter.get()->currentEnvironment = previous;
	return std::make_shared<CObject>(nullptr);
}
std::string CInt_TGraph_Hover::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CInt_TGraph_DragStart

CInt_TGraph_Drag::CInt_TGraph_Drag(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui)
{
	this->ui = ui;
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CInt_TGraph_Drag;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("dragSelector");
	paramsNames.push_back("targetType");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "@TGraph_Drag", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CInt_TGraph_Drag::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	std::shared_ptr<CEnvironment> previous = interpreter.get()->currentEnvironment;

	interpreter.get()->setEnvironment(interpreter.get()->getEnvironment("global"));
	std::shared_ptr<CEnvironment> current = interpreter.get()->currentEnvironment;

	if (!ui.get()->activeWidget.expired())
	{
		std::weak_ptr<Widget> active = ui.get()->activeWidget;
		int dragSelect = (int)std::get<double>(args[0].get()->obj);
		int targetType = (int)std::get<double>(args[1].get()->obj);
		if (targetType == 0 || targetType == 1)
		{
			if (dragSelect == 0)
			{
				current.get()->get("@TGraph_currentMaxLeft").get()->obj =
					ui.get()->activeWidget.lock()->parentWidget.lock()->getProperty(interpreter, "size-X").get()->obj;
				current.get()->get("@TGraph_currentSliderWidth").get()->obj =
					ui.get()->activeWidget.lock()->getProperty(interpreter, "size-X").get()->obj;
			}
			if (dragSelect == 1)
			{
				int maxLeft = int(std::get<double>(ui.get()->activeWidget.lock()->parentWidget.lock()->getProperty(interpreter, "size-X").get()->obj));
				int sliderWidth = int(std::get<double>(ui.get()->activeWidget.lock()->getProperty(interpreter, "size-X").get()->obj));
				Input mousePos = ui.get()->owner.get()->getMousePosition(true);
				glm::ivec2 widgetLoc = glm::ivec2(0);
				if (!ui.get()->activeWidget.expired())
				{
					widgetLoc = ui.get()->activeWidget.lock()->parentWidget.lock().get()->getScreenLocation(0, 0, false);
				}
				mousePos.x = mousePos.x - widgetLoc.x;
				mousePos.y = mousePos.y - widgetLoc.y;
				int xLoc = (int)clampf((float)mousePos.x, -(sliderWidth / 2.0f), maxLeft - (sliderWidth / 2.0f));
				ui.get()->activeWidget.lock()->setProperty(interpreter, "left", std::make_shared<CObject>(double(xLoc)));
			}
		}
		else if (targetType == 2)
		{
			if (dragSelect == 0)
			{
				if (blockNewHandle == true) { blockNewHandle = false; }
				else if (active.lock()->type == LTokenType::T_GRAPH && 
					std::dynamic_pointer_cast<Graph_ToolControl>(active.lock()).get()->canAddHandle())
				{
					Input mousePos = ui.get()->owner.get()->getMousePosition(true);
					glm::ivec2 widgetLoc = glm::ivec2(0);
					if (!ui.get()->activeWidget.expired())
					{
						widgetLoc = ui.get()->activeWidget.lock()->getScreenLocation(0, 0, false);
					}
					mousePos.x = mousePos.x - widgetLoc.x;
					mousePos.y = mousePos.y - widgetLoc.y;
					int sliderWidth = (int)std::dynamic_pointer_cast<Graph_ToolControl>(active.lock()).get()->handleStyle.get()->minX;
					activeDragTarget = std::dynamic_pointer_cast<Graph_ToolControl>(
						active.lock())->addGraphHandleWidget(ui, int((float)mousePos.y - (sliderWidth / 2.0f)), int((float)mousePos.x - (sliderWidth / 2.0f)), true);
					printStr("UI::GRAPH_TOOLCONTROL::ADD_HANDLE_WIDGET");
					// Set the drag values
					current.get()->get("@TGraph_currentMaxLeft").get()->obj =
						ui.get()->activeWidget.lock()->getProperty(interpreter, "size-X").get()->obj;
					current.get()->get("@TGraph_currentMaxTop").get()->obj =
						ui.get()->activeWidget.lock()->getProperty(interpreter, "size-Y").get()->obj;
					current.get()->get("@TGraph_currentSliderWidth").get()->obj =
						activeDragTarget.lock()->getProperty(interpreter, "size-X").get()->obj;
					// Force swap the dragWidget
					ui.get()->dragWidget = activeDragTarget;
					ui.get()->owner.get()->setDragStart();
				}
			}
		}
		else if (targetType == 3)
		{
			if (chromaIO.get()->getModKeys() == InputModKey::ctrl)
			{
				if (!active.lock()->parentWidget.expired() && active.lock()->parentWidget.lock()->type == LTokenType::T_GRAPH)
				{
					std::dynamic_pointer_cast<Graph_ToolControl>(active.lock()->parentWidget.lock())->removeHandleWidget(ui, active.lock()->genIndexA, true);
					blockNewHandle = true;
				}
			}
			else if (dragSelect == 0)
			{
				current.get()->get("@TGraph_currentMaxLeft").get()->obj =
					ui.get()->activeWidget.lock()->parentWidget.lock()->getProperty(interpreter, "size-X").get()->obj;
				current.get()->get("@TGraph_currentMaxTop").get()->obj =
					ui.get()->activeWidget.lock()->parentWidget.lock()->getProperty(interpreter, "size-Y").get()->obj;
				current.get()->get("@TGraph_currentSliderWidth").get()->obj =
					ui.get()->activeWidget.lock()->getProperty(interpreter, "size-X").get()->obj;
			}
			else if (dragSelect == 1)
			{
				int maxLeft = int(std::get<double>(current.get()->get("@TGraph_currentMaxLeft").get()->obj));
				int maxTop = int(std::get<double>(current.get()->get("@TGraph_currentMaxTop").get()->obj));
				int sliderWidth = int(std::get<double>(current.get()->get("@TGraph_currentSliderWidth").get()->obj));
				Input mousePos = ui.get()->owner.get()->getMousePosition(true);
				glm::ivec2 widgetLoc = glm::ivec2(0);
				if (!ui.get()->activeWidget.expired())
				{
					widgetLoc = ui.get()->activeWidget.lock()->parentWidget.lock().get()->getScreenLocation(0, 0, false);
				}
				mousePos.x = mousePos.x - widgetLoc.x;
				mousePos.y = mousePos.y - widgetLoc.y;
				int xLoc = (int)clampf((float)mousePos.x - (sliderWidth / 2.0f), -(sliderWidth / 2.0f), maxLeft - (sliderWidth / 2.0f));
				int yLoc = (int)clampf((float)mousePos.y - (sliderWidth / 2.0f), -(sliderWidth / 2.0f), maxTop - (sliderWidth / 2.0f));
				ui.get()->activeWidget.lock()->setProperty(interpreter, "left", std::make_shared<CObject>(double(xLoc)));
				ui.get()->activeWidget.lock()->setProperty(interpreter, "top", std::make_shared<CObject>(double(yLoc)));
			}
		}
		if (!active.expired())
		{
			if (active.lock()->type == LTokenType::T_GRAPH)
			{
				std::dynamic_pointer_cast<Graph_ToolControl>(active.lock()).get()->updateGraphData(ui);
			}
			else if (!active.lock()->parentWidget.expired() && active.lock()->parentWidget.lock()->type == LTokenType::T_GRAPH)
			{
				std::dynamic_pointer_cast<Graph_ToolControl>(active.lock()->parentWidget.lock()).get()->updateGraphData(ui);
			}
		}
	}
	interpreter.get()->currentEnvironment = previous;
	return std::make_shared<CObject>(nullptr);
}
std::string CInt_TGraph_Drag::toString() { return funcDeclaration.get()->name.get()->lexeme; }