#include "../../include/entities/widgets/WidgetInterface.h"

#include "../../include/entities/widgets/Widget.h"
#include "../../include/cscript/CCallable.h"
#include "../../include/cscript/CInterpreter.h"
#include "../../include/cscript/CToken.h"
#include "../../include/cscript/CStmt.h"
#include "../../include/cscript/CExpr.h"

#include <memory>
#include <map>
#include <vector>
#include <string>
#include <deque>

void WidgetInterface::attachTextInterface(Widget* widget)
{
	widget->callbackMap.insert(std::pair<std::string, std::vector<std::shared_ptr<CStmt>>>(
		"onclick", 
		{ createTextClickEvent() }));
}

std::shared_ptr<CStmt> WidgetInterface::createTextClickEvent()
{
	// Make the token for the lookup
	std::shared_ptr<CToken> line = std::make_shared<CToken>(CTokenType::IDENTIFIER, "@textClick", -1);
	// Craft the pseudo-expression
	std::vector<std::shared_ptr<CExpr>> args = {};
	std::shared_ptr<CExpr> callee = std::make_shared<CExpr_Variable>(line);
	std::shared_ptr<CExpr> call = std::make_shared<CExpr_Call>(callee, args, nullptr, line);
	// Return the callback binding
	return std::make_shared<CStmt_Expression>(call, line);
}

void WidgetInterface::attachGraphInterface(Widget* widget, WidgetInterfaceNames graphInterface)
{
	if (graphInterface == WidgetInterfaceNames::TGraph_Main)
	{
		widget->callbackMap.insert(std::pair<std::string, std::vector<std::shared_ptr<CStmt>>>(
			"ondragstart",
			{ createGraphEvent(WidgetInterfaceNames::TGraph_Main_dragStart) }));
	}
	else if (graphInterface == WidgetInterfaceNames::TGraph_TopSlider)
	{
		widget->callbackMap.insert(std::pair<std::string, std::vector<std::shared_ptr<CStmt>>>(
			"ondragstart",
			{ createGraphEvent(WidgetInterfaceNames::TGraph_TopSlider_dragStart) }));
		widget->callbackMap.insert(std::pair<std::string, std::vector<std::shared_ptr<CStmt>>>(
			"ondrag",
			{ createGraphEvent(WidgetInterfaceNames::TGraph_TopSlider_drag) }));
		widget->callbackMap.insert(std::pair<std::string, std::vector<std::shared_ptr<CStmt>>>(
			"onmouseover",
			{ createGraphEvent(WidgetInterfaceNames::TGraph_Slider_mouseover) }));
		widget->callbackMap.insert(std::pair<std::string, std::vector<std::shared_ptr<CStmt>>>(
			"onmouseleave",
			{ createGraphEvent(WidgetInterfaceNames::TGraph_Slider_mouseleave) }));
	}
	else if (graphInterface == WidgetInterfaceNames::TGraph_BottomSlider)
	{
		widget->callbackMap.insert(std::pair<std::string, std::vector<std::shared_ptr<CStmt>>>(
			"ondragstart",
			{ createGraphEvent(WidgetInterfaceNames::TGraph_BottomSlider_dragStart) }));
		widget->callbackMap.insert(std::pair<std::string, std::vector<std::shared_ptr<CStmt>>>(
			"ondrag",
			{ createGraphEvent(WidgetInterfaceNames::TGraph_BottomSlider_drag) }));
		widget->callbackMap.insert(std::pair<std::string, std::vector<std::shared_ptr<CStmt>>>(
			"onmouseover",
			{ createGraphEvent(WidgetInterfaceNames::TGraph_Slider_mouseover) }));
		widget->callbackMap.insert(std::pair<std::string, std::vector<std::shared_ptr<CStmt>>>(
			"onmouseleave",
			{ createGraphEvent(WidgetInterfaceNames::TGraph_Slider_mouseleave) }));
	}
	else if (graphInterface == WidgetInterfaceNames::TGraph_Handle)
	{
		widget->callbackMap.insert(std::pair<std::string, std::vector<std::shared_ptr<CStmt>>>(
			"ondragstart",
			{ createGraphEvent(WidgetInterfaceNames::TGraph_Handle_dragStart) }));
		widget->callbackMap.insert(std::pair<std::string, std::vector<std::shared_ptr<CStmt>>>(
			"ondrag",
			{ createGraphEvent(WidgetInterfaceNames::TGraph_Handle_drag) }));
		widget->callbackMap.insert(std::pair<std::string, std::vector<std::shared_ptr<CStmt>>>(
			"onmouseover",
			{ createGraphEvent(WidgetInterfaceNames::TGraph_Handle_mouseover) }));
		widget->callbackMap.insert(std::pair<std::string, std::vector<std::shared_ptr<CStmt>>>(
			"onmouseleave",
			{ createGraphEvent(WidgetInterfaceNames::TGraph_Handle_mouseleave) }));
	}
}

// Graph Events
std::shared_ptr<CStmt> WidgetInterface::createGraphEvent(WidgetInterfaceNames interfaceSelector)
{
	int select = 0;
	switch (interfaceSelector)
	{
	case WidgetInterfaceNames::TGraph_Main_dragStart:
	case WidgetInterfaceNames::TGraph_TopSlider_dragStart:
	case WidgetInterfaceNames::TGraph_TopSlider_drag:
	case WidgetInterfaceNames::TGraph_BottomSlider_dragStart:
	case WidgetInterfaceNames::TGraph_BottomSlider_drag:
	case WidgetInterfaceNames::TGraph_Handle_dragStart:
	case WidgetInterfaceNames::TGraph_Handle_drag:
		select = 1; break;
	case WidgetInterfaceNames::TGraph_Handle_mouseover:
	case WidgetInterfaceNames::TGraph_Handle_mouseleave:
	case WidgetInterfaceNames::TGraph_Slider_mouseover:
	case WidgetInterfaceNames::TGraph_Slider_mouseleave:
		select = 2; break;
	default: select = 0;
	}
	if (select == 1)
	{
		int dragSelector = 0;
		int targetType = 0;
		switch (interfaceSelector)
		{
		case WidgetInterfaceNames::TGraph_Main_dragStart: dragSelector = 0; targetType = 2; break;
		case WidgetInterfaceNames::TGraph_TopSlider_dragStart: dragSelector = 0; targetType = 0; break;
		case WidgetInterfaceNames::TGraph_TopSlider_drag: dragSelector = 1; targetType = 0; break;
		case WidgetInterfaceNames::TGraph_BottomSlider_dragStart: dragSelector = 0; targetType = 1; break;
		case WidgetInterfaceNames::TGraph_BottomSlider_drag: dragSelector = 1; targetType = 1; break;
		case WidgetInterfaceNames::TGraph_Handle_dragStart: dragSelector = 0; targetType = 3; break;
		case WidgetInterfaceNames::TGraph_Handle_drag: dragSelector = 1; targetType = 3; break;
		}
		// Make the token for the lookup
		std::shared_ptr<CToken> line = std::make_shared<CToken>(CTokenType::IDENTIFIER, "@TGraph_Drag", -1);
		// Craft the pseudo-expression
		std::vector<std::shared_ptr<CExpr>> args = {};
		args.push_back(std::make_shared<CExpr_Literal>(std::make_shared<CToken>(CTokenType::NUM, std::to_string(dragSelector), -1)));
		args.push_back(std::make_shared<CExpr_Literal>(std::make_shared<CToken>(CTokenType::NUM, std::to_string(targetType), -1)));
		std::shared_ptr<CExpr> callee = std::make_shared<CExpr_Variable>(line);
		std::shared_ptr<CExpr> call = std::make_shared<CExpr_Call>(callee, args, nullptr, line);
		// Return the callback binding
		return std::make_shared<CStmt_Expression>(call, line);
	}
	else if (select == 2)
	{
		int hoverSelector = 0;
		int targetType = 0;
		switch (interfaceSelector)
		{
		case WidgetInterfaceNames::TGraph_Handle_mouseover: hoverSelector = 0; targetType = 0; break;
		case WidgetInterfaceNames::TGraph_Handle_mouseleave: hoverSelector = 1; targetType = 0; break;
		case WidgetInterfaceNames::TGraph_Slider_mouseover: hoverSelector = 0; targetType = 1; break;
		case WidgetInterfaceNames::TGraph_Slider_mouseleave: hoverSelector = 1; targetType = 1; break;
		}
		// Make the token for the lookup
		std::shared_ptr<CToken> line = std::make_shared<CToken>(CTokenType::IDENTIFIER, "@TGraph_Hover", -1);
		// Craft the pseudo-expression
		std::vector<std::shared_ptr<CExpr>> args = {};
		args.push_back(std::make_shared<CExpr_Literal>(std::make_shared<CToken>(CTokenType::NUM, std::to_string(hoverSelector), -1)));
		args.push_back(std::make_shared<CExpr_Literal>(std::make_shared<CToken>(CTokenType::NUM, std::to_string(targetType), -1)));
		std::shared_ptr<CExpr> callee = std::make_shared<CExpr_Variable>(line);
		std::shared_ptr<CExpr> call = std::make_shared<CExpr_Call>(callee, args, nullptr, line);
		// Return the callback binding
		return std::make_shared<CStmt_Expression>(call, line);
	}
}

