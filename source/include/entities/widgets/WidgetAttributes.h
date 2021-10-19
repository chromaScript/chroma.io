#ifndef WIDGETATTRIBUTES_H
#define WIDGETATTRIBUTES_H

#include <string>
#include <vector>

struct WidgetAttributes
{
	std::string id = "";
	std::vector<std::string> classes;
	std::string name = "";
};

#endif