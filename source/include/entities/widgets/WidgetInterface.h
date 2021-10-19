#ifndef WIDGETINTERFACE_H
#define WIDGETINTERFACE_H

#include "Widget.h"
#include "../../cscript/CForward.h"

#include <memory>

enum class WidgetInterfaceNames
{
	TGraph_Main,
	TGraph_Main_dragStart,
	TGraph_Main_drag,
	TGraph_Main_dragEnd,
	TGraph_TopSlider,
	TGraph_TopSlider_dragStart,
	TGraph_TopSlider_drag,
	TGraph_TopSlider_dragEnd,
	TGraph_BottomSlider,
	TGraph_BottomSlider_dragStart,
	TGraph_BottomSlider_drag,
	TGraph_BottomSlider_dragEnd,
	TGraph_Slider_mouseover,
	TGraph_Slider_mouseleave,
	TGraph_Handle,
	TGraph_Handle_dragStart,
	TGraph_Handle_drag,
	TGraph_Handle_dragEnd,
	TGraph_Handle_mouseover,
	TGraph_Handle_mouseleave,
};

class WidgetInterface
{
public:
	WidgetInterface() {}
	void attachTextInterface(Widget* widget);
	void attachGraphInterface(Widget* widget, WidgetInterfaceNames graphInterface);
	std::shared_ptr<CStmt> createTextClickEvent();
	std::shared_ptr<CStmt> createGraphEvent(WidgetInterfaceNames interfaceSelector);
};

#endif