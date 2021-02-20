#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "Fonts.h"
#include "Canvas.h"
#include "Widget.h"
#include "../CustomCursor.h"
#include "../Application.h"
#include "../cscript/CObject.h"

class WidgetStyle;
class CInterpreter;

#include <vector>
#include <string>
#include <map>
#include <filesystem>
#include <functional>

class UI
{
private:
	std::shared_ptr<Application> owner; // Note: Change to weak_ptr

	// Canvas Variables
	std::vector<std::shared_ptr<Canvas>> documents; // Note: Change to smart pointers
	std::shared_ptr<Canvas> activeCanvas;

	// Font variables
	std::shared_ptr<Fonts> fontHandler;

	// Builder Variables
	std::vector<std::shared_ptr<WidgetStyle>> styles;
	std::vector<std::shared_ptr<WPlugin>> plugins;
	std::vector<std::filesystem::path> pluginPaths;

protected:
public:
	// Color Variables
	CColor fgColor = black;
	CColor bgColor = white;

	// ID Tables
	// In form of <"id/class", "owning root id">
	// Providies a means by which to quickly lookup the root widget to search
	// in for getWidgetById
	// Must manage both of these when deleting/creating widgets during runtime
	// Note: At a later date, may upgrade these to store a widget pointer instead
	// Note: Also not using these initially for lookup. Will develop later for
	// optimization, current use is .layout resolver
	std::map<std::string, std::string> widgetIDTable;

	// ClassTable uses a vector for the "owning root id"(s), the class is
	// a unique key, and lookup is done by querying every root id in the value
	std::map<std::string, std::vector<std::string>> widgetClassTable;

// Array of root Widgets
	std::vector<std::shared_ptr<Widget>> widgets;

// Held hitWidget
	// Used for storing the innermost widget that received a click event
	// When the click is eventually fired, it bubbles from that widget up
	std::weak_ptr<Widget> hitWidget;

// Entered Widgets
	std::map<int, std::weak_ptr<Widget>> enteredWidgets;

// Active Widget
	// Used to act as a 'this' pointer for global functions that act on the
	// active widget. Gets set/reset before/after calling a bound callback function
	std::weak_ptr<Widget> activeWidget;

// Drag Widget
	// Used to store the currently dragged widget
	std::weak_ptr<Widget> dragWidget;

// Clicked Widget
	// Used to store which widget received initial click input
	// Will call this widget's onrelease callback when the mouse is released
	bool clickReleaseGate = false;
	std::weak_ptr<Widget> clickedWidget;

// Focus Widget
	// Used to store the last clicked widget until another click event occurs
	// Unlike clickedWidget, this does not reset on mouseRelease
	// When the focus widget changes, then the onblur callback is called on the
	// focusWidget before it is reset or changed
	bool didFocusThisClick = false;
	std::weak_ptr<Widget> focusWidget;

// Prototype Factory Table
	// In form of <"id", WidgetObject>
	// Stores the widget trees of every prototype widget for creating copies from
	// When a copy is made, a unique "id" for the prototype passed as param to addWidgetFromPrototype()
	// which is appended for every id attribute in the tree, and those
	// are then added to the widgetID/ClassTable(s)
	std::map<std::string, std::shared_ptr<Widget>> prototypeFactoryTable;

// Callback Arrays
	// Various callback functions that get triggered when certain program events occur.
	// The callbacks are used to notify plugins of when the program state
	// changes and they need to refresh their data. Typically the callback supplies the
	// data as parameters into the function callback.

	// Color Update Callbacks - void function(num array rgba, num xpos, num ypos, bool isBG)
	// Gets called every time updateF/BGColor is called. Does not fire if the originating source
	// disabled notifications.
	std::map<std::string, std::shared_ptr<CFunction>> fgColorCallbacks;
	std::map<std::string, std::shared_ptr<CFunction>> bgColorCallbacks;

// Saved Cursor
	std::shared_ptr<CustomCursor> savedCursor = nullptr;
	std::shared_ptr<CustomCursor> currentCursor = nullptr;

	// Widget Variables
	bool interruptBlur = false;

	// Constructors
	UI(std::shared_ptr<Application>);

	// Utility Functions
	void stepThroughWidgetTree(std::shared_ptr<Widget> treeRoot, bool doTopDown, bool doBottomUp, bool doPlacement);
	void sizeWidgetByChildren(std::shared_ptr<Widget> target);
	void sizeWidgetByParent(std::shared_ptr<Widget> target);
	void updateWidgetLocation(std::shared_ptr<Widget> target);
	int loadFont(std::filesystem::path fontPath);
	// Builders
	std::shared_ptr<Widget> createWidget(
		LTokenType widgetType,
		std::map<std::string, std::string> basicAttribs,
		std::weak_ptr<Widget> parent,
		std::weak_ptr<WidgetStyle> style,
		std::shared_ptr<Shader> shader);
	void initializeInterface();
	void buildAllWidgetTrees();
	void buildWidgetHierarchy();
	void rebuildWidgetHierarchy(std::weak_ptr<Widget> target);
	// Plugin Header Files (.plugin)
	void findPlugins();
	void loadPlugins();
	// Plugin Header Files (.plugin) - organized data list
	void parsePluginString(std::string fileString, 
		std::string* plugName, std::string* plugVersion, std::string* plugAbout, 
		std::string* _namespace,
		std::vector<std::filesystem::path>* layoutP, 
		std::vector<std::filesystem::path>* styleP, 
		std::vector<std::filesystem::path>* scriptPaths);

	// Callback Binding Functions
	bool bindCallback(std::shared_ptr<CInterpreter> interpreter, 
		CCallbackType callType, std::string callerID,
		std::shared_ptr<CFunction> callFunc);

	// Color Functions
	std::pair<CColor, CColor> getColors() { return std::pair<CColor, CColor>(fgColor, bgColor); }
	void updateFGColor(CColor color, int xpos, int ypos);
	void updateBGColor(CColor color, int xpos, int ypos);

	// Canvas Functions
	void newDocument(int width, int height, bool setAsActive);
	void setCanvas(std::shared_ptr<Canvas> c);
	bool canvasHitTest(glm::vec3 worldPos);
	std::shared_ptr<Canvas> getCanvas();

	// Widget Variables
	void updateFocusWidget(std::weak_ptr<Widget> target);
	void clearFocusWidget();
	void resetPropertyByClass(std::shared_ptr<CInterpreter> interpreter,
		std::string className,
		std::string propertyName);
	void setPropertyByClass(std::shared_ptr<CInterpreter> interpreter,
		std::string className, std::string idExclusion,
		std::string propertyName, std::shared_ptr<CObject> value);
	void setPropertyByClass(std::shared_ptr<CInterpreter> interpreter,
		std::string className, std::weak_ptr<Widget> widgetExclusion,
		std::string propertyName, std::shared_ptr<CObject> value);
	// Widget Lookup
	std::weak_ptr<Widget> getWidgetByID(std::string id);
	std::vector<std::weak_ptr<Widget>> getWidgetsByClass(std::string className, std::string idExclusion);
	std::vector<std::weak_ptr<Widget>> getWidgetsByType(std::string typeName, std::string idExclusion);
	std::weak_ptr<Widget> getRootWidgetByID(std::string rootID);

	// Cursor Functions
	bool widgetSweepTest(MouseEvent input);
	bool widgetHitTest(MouseEvent input, bool dragOnly);
	bool widgetFocusTest(MouseEvent input);
	bool widgetHoverTest(MouseEvent input);
	bool widgetLeaveTest(MouseEvent input);
	void updateCursorImage(std::shared_ptr<CustomCursor> cursor);
	std::shared_ptr<CustomCursor> getCursor();

	// Render Functions
	void drawWidgets();
};

#endif