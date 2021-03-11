#ifndef WIDGETINTERFACE_H
#define WIDGETINTERFACE_H

#include "Widget.h"
#include "../cscript/CForward.h"

#include <memory>

class WidgetInterface
{
public:
	WidgetInterface() {}
	void attachTextInterface(Widget* widget);
	std::shared_ptr<CStmt> createTextClickEvent();
};

#endif