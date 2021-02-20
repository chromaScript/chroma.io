#include "../include/entities/UserInterface.h"

#include "../include/entities/Canvas.h"
#include "../include/entities/HorizontalBox.h"
#include "../include/entities/VerticalBox.h"
#include "../include/entities/Image.h"
#include "../include/entities/Text.h"
#include "../include/entities/GradientBox.h"
#include "../include/cscript/ChromaScript.h"
#include "../include/cscript/CEnvironment.h"
#include "../include/cscript/CInterpreter.h"
#include "../include/cscript/CEnums.h"
#include "../include/clayout/LEnums.h"
#include "../include/cscript/CObject.h"
#include "../include/Toolbox.h"

#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <functional>
#include <map>

// Constructors
UI::UI(std::shared_ptr<Application> owner)
{
	// Register the owner for quick access
	this->owner = owner;
	// Initialize the fontHandler
	fontHandler = std::make_shared<Fonts>();
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Canvas Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

void UI::newDocument(int width, int height, bool setAsActive)
{
	documents.push_back(std::make_shared<Canvas>(width, height, owner.get()->getCanvasShader()));
	if (setAsActive) { setCanvas(documents.back()); }
}
void UI::setCanvas(std::shared_ptr<Canvas> canvas)
{
	activeCanvas = canvas;
}
std::shared_ptr<Canvas> UI::getCanvas()
{
	return activeCanvas;
}
bool UI::canvasHitTest(glm::vec3 worldPos)
{
	Bounds canvasBox = activeCanvas.get()->getTransform().boundBox;
	if (worldPos.x >= 0 && worldPos.x <= canvasBox.x2)
	{
		if (worldPos.y <= 0 && worldPos.y >= -canvasBox.y2)
		{
			return true;
		}
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Cursor Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

void UI::updateCursorImage(std::shared_ptr<CustomCursor> cursor)
{
	currentCursor = cursor;
	glfwSetCursor(owner.get()->getWindow(), cursor.get()->cursor);
}
std::shared_ptr<CustomCursor> UI::getCursor()
{
	return currentCursor;
}


////////////////////////////////////////////////////////////////////////////////////////////////
//
// Widget Utility Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

void UI::sizeWidgetByChildren(std::shared_ptr<Widget> target)
{
	target.get()->setSizeByChildren();
}
void UI::sizeWidgetByParent(std::shared_ptr<Widget> target)
{
	target.get()->setSizeByParent();
}
void UI::updateWidgetLocation(std::shared_ptr<Widget> target)
{
	target.get()->buildWidget();
	target.get()->placeWidget();
}
void UI::stepThroughWidgetTree(std::shared_ptr<Widget> treeRoot, bool doTopDown, bool doBottomUp, bool doPlacement)
{
	// Be aware that adding too many calls in this recursive function can significantly impact
	// performance for resizing/moving widgets. Make due with as few functions as necessary
	// in order to avoid displaying widgets incorrectly.

	if (doTopDown == true) { sizeWidgetByParent(treeRoot); }
	if (doPlacement == true) { updateWidgetLocation(treeRoot); }

	for (std::shared_ptr<Widget> child : treeRoot.get()->childWidgets)
	{
		// Additional Preliminary top-level sizeByChildren step so that at least the minimum
		// size of the widget is known before diving further. Without this, child widgets of 'child' that would
		// float right, but have a minimum size lower than 'child's final size, will not get the right potential
		// maxX and maxY values.
		for (std::shared_ptr<Widget> childChild : child.get()->childWidgets)
		{
			sizeWidgetByChildren(child);
		}

		// Do stuff before stepping into tree
		if (doTopDown == true) { sizeWidgetByParent(child); }
		if (doPlacement == true) { updateWidgetLocation(child); }

		stepThroughWidgetTree(child, doTopDown, doBottomUp, doPlacement);

		// Do stuff after stepping into tree
		if (doBottomUp == true) { sizeWidgetByChildren(child); }
	}

	if (doBottomUp == true) { sizeWidgetByChildren(treeRoot); }
	return;
}

int UI::loadFont(std::filesystem::path fontPath)
{
	return fontHandler.get()->loadFontFromFile(fontPath);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Widget Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

// Build Widget Hierarchy
void UI::buildAllWidgetTrees()
{
	// 1. Each of the Widgets in this->widgets is the root of an individual plugin hierarchy tree
	// For each of these tree roots, build the widget tree (sizeDown, sizeUp, then place)
	for (std::shared_ptr<Widget> root : widgets)
	{
						// TopDown, BottomUp, Placement
		stepThroughWidgetTree(root, false, true, false); // 1. BottomUp
		stepThroughWidgetTree(root, true, false, false); // 2. TopDown
		stepThroughWidgetTree(root, false, false, true); // 3. Placement
		root.get()->checkOutofBoundsWidgets(root, root.get()->getColliderBox());
	}
}

// Build WidgetHierarchy
void UI::buildWidgetHierarchy()
{

}

// Rebuild WidgetHierarchy
void UI::rebuildWidgetHierarchy(std::weak_ptr<Widget> target)
{
	if (!target.expired())
	{
		stepThroughWidgetTree(target.lock(), false, true, false); // 1. BottomUp
		stepThroughWidgetTree(target.lock(), true, false, false); // 2. TopDown
		stepThroughWidgetTree(target.lock(), false, false, true); // 3. Placement
		target.lock().get()->checkOutofBoundsWidgets(target.lock(), target.lock().get()->getColliderBox());
	}
}

std::shared_ptr<Widget> UI::createWidget(
	LTokenType widgetType,
	std::map<std::string, std::string> basicAttribs,
	std::weak_ptr<Widget> parent,
	std::weak_ptr<WidgetStyle> style,
	std::shared_ptr<Shader> shader)
{
	// 4. Switch on elmentName, using element specific constructor
	switch (widgetType)
	{
	case LTokenType::PANEL:
		return nullptr;
	case LTokenType::H_BOX:
		return std::make_shared<HorizontalBox>(basicAttribs, parent, style, shader);
	case LTokenType::V_BOX:
		return std::make_shared<VerticalBox>(basicAttribs, parent, style, shader);
	case LTokenType::IMAGE:
		return std::make_shared<Image>(basicAttribs, parent, style, shader);
	case LTokenType::TEXT:
		return std::make_shared<Text>(basicAttribs, parent, style, shader, fontHandler);
	case LTokenType::GRADIENT_BOX:
		return std::make_shared<GradientBox>(basicAttribs, parent, style, shader);
	default:
		return nullptr;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Initialize Interface
//
////////////////////////////////////////////////////////////////////////////////////////////////
inline double fibo(double f)
{
	std::cout << f << std::endl;
	if (f <= 2)
	{
		return f;
	}
	return fibo(f - 2) + fibo(f - 1);
}
void UI::initializeInterface()
{
	// Make path to root directory for the default config files
	std::filesystem::path root = std::filesystem::current_path();
	root /= std::filesystem::path("config");
	// First, load the default style classes (Required, failure to identify the file results in abort)
	double startTime = owner.get()->getTime();
	owner.get()->scriptConsole.get()->ping(startTime, " APPLICATION::UI::DEFAULT_SCRIPTS::LOADING\n");
	if (std::filesystem::exists(root) && std::filesystem::is_directory(root))
	{
		std::vector<std::filesystem::path> globalScriptPaths;
		for (auto const& entry : std::filesystem::recursive_directory_iterator(root))
		{
			if (std::filesystem::is_regular_file(entry) && entry.path().extension() == ".cscript")
			{
				globalScriptPaths.push_back(entry);
			}
		}
		owner.get()->scriptConsole.get()->entry(globalScriptPaths, "global", root);
	}
	owner.get()->scriptConsole.get()->ping(owner.get()->getTime(),
		" APPLICATION::UI::DEFAULT_SCRIPTS::FINISHED\n"
		"\tSTART::= " + std::to_string(startTime));
	std::cout << "\nAPPLICATION::UI::DEFAULT_STYLES=\n";
	// New method of style loading via chromaStyle class
	// Allow both build methods to run until finished and ready to plug everything together
	startTime = owner.get()->getTime();
	owner.get()->scriptConsole.get()->ping(startTime, " APPLICATION::UI::DEFAULT_STYLES::LOADING\n");
	if (std::filesystem::exists(root) && std::filesystem::is_directory(root))
	{
		std::vector<std::filesystem::path> globalStylePaths;
		for (auto const& entry : std::filesystem::recursive_directory_iterator(root))
		{
			if (std::filesystem::is_regular_file(entry) && entry.path().extension() == ".style")
			{
				globalStylePaths.push_back(entry);
			}
		}
		owner.get()->styleConsole.get()->entry(globalStylePaths, "global", root);
	}
	owner.get()->scriptConsole.get()->ping(owner.get()->getTime(),
		" APPLICATION::UI::DEFAULT_STYLES::FINISHED\n"
		"\tSTART::= " + std::to_string(startTime));
	std::cout << "\nAPPLICATION::UI::LOAD_PLUGINS\n\n";
	// Now find and load the included and user-created plugins
	findPlugins();
	loadPlugins();
	buildAllWidgetTrees();
	owner.get()->styleDataDump(styles);
	std::cout << "\n";
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Load Plugin Sequence
//
////////////////////////////////////////////////////////////////////////////////////////////////

// Find and create WPlugins from .plugin files inside /plugins folder
void UI::findPlugins()
{
	std::filesystem::path root = std::filesystem::current_path();
	root /= std::filesystem::path("plugins");
	// Iterate over the /plugins folder location to find potential plugin locations
	if (std::filesystem::exists(root) && std::filesystem::is_directory(root))
	{
		for (auto const& entry : std::filesystem::recursive_directory_iterator(root))
		{
			if (std::filesystem::is_regular_file(entry) && entry.path().extension() == ".plugin")
			{
				pluginPaths.push_back(entry.path());
				std::cout << "APPLICATION::UI::PLUGIN::LOCATED=" << pluginPaths.back() << std::endl;
			}
		}
	}
	// Read the .plugin files to strings, and then parse into new WPlugin's
	if (pluginPaths.size() != 0)
	{
		for (std::filesystem::path& path : pluginPaths)
		{
			std::string name, version, author, _namespace;
			std::filesystem::path pluginPath = path.parent_path();
			std::vector<std::filesystem::path> scriptPaths, layoutPaths, stylePaths;
			// Create a string and read the pluginPath into it
			std::string pluginString;
			readFileToString(pluginString, path);
			// Parse the plugin string to it's component parts
			parsePluginString(pluginString, &name, &version, &author, &_namespace, &layoutPaths, &stylePaths, &scriptPaths);
			_namespace.erase(remove_if(_namespace.begin(), _namespace.end(), isspace), _namespace.end());
			// Create/Add a new WPlugin
			plugins.emplace_back(std::make_shared<WPlugin>(
				name, version, author, _namespace,
				pluginPath, layoutPaths, stylePaths, scriptPaths));
		}
		owner.get()->pluginDataDump(plugins);
	}
	else
	{
		std::cout << "APPLICATION::UI::PLUGINS::NO_PLUGINS_FOUND" << std::endl;
	}
}
// Load the plugins and their style/layout/script sheets
void UI::loadPlugins()
{
	if (plugins.size() == 0)
	{
		std::cout << "APPLICATION::UI::PLUGINS::NO_PLUGINS_LOADED" << std::endl;
		return;
	}

	// For each plugin created from findPlugins(), 
	// First collect all script paths into map,
	// Then send the map to the scriptConsole for compilation
	std::map<std::filesystem::path, std::shared_ptr<WPlugin>> pathMap;
	for (std::shared_ptr<WPlugin> plugin : plugins)
	{
		// Make new environment (critical)
		owner.get()->scriptConsole.get()->global.get()->lookupEnvironment(plugin.get()->script_namespace, true);
		// Create the root directory (Note: Plugins must have their .plugin file in the root directory of the plugin)
		std::filesystem::path pluginRoot = plugin.get()->plugin_path;
		// For each script in the plugin, get the path and run it
		for (std::filesystem::path getPath : plugin.get()->script_paths)
		{
			std::filesystem::path scriptP = pluginRoot / getPath;
			pathMap.insert(std::pair<std::filesystem::path, std::shared_ptr<WPlugin>>(scriptP, plugin));

		}
	}
	owner.get()->scriptConsole.get()->entry(pathMap);

	// For each plugin created from findPlugins(), 
	// Collect all style paths into map,
	// Then send the map to the styleConsole for compilation
	pathMap.clear();
	for (std::shared_ptr<WPlugin> plugin : plugins)
	{
		// Create the root directory (Note: Plugins must have their .plugin file in the root directory of the plugin)
		std::filesystem::path pluginRoot = plugin.get()->plugin_path;
		// For each script in the plugin, get the path and run it
		for (std::filesystem::path getPath : plugin.get()->style_paths)
		{
			std::filesystem::path styleP = pluginRoot / getPath;
			pathMap.insert(std::pair<std::filesystem::path, std::shared_ptr<WPlugin>>(styleP, plugin));
		}
	}
	owner.get()->styleConsole.get()->entry(pathMap);

	// For each plugin created from findPlugins(), 
	// Collect all layout paths into map,
	// Then send the map to the layoutConsole for compilation
	pathMap.clear();
	for (std::shared_ptr<WPlugin> plugin : plugins)
	{
		// Create the root directory (Note: Plugins must have their .plugin file in the root directory of the plugin)
		std::filesystem::path pluginRoot = plugin.get()->plugin_path;
		// For each script in the plugin, get the path and run it
		for (std::filesystem::path getPath : plugin.get()->layout_paths)
		{
			std::filesystem::path layoutP = pluginRoot / getPath;
			pathMap.insert(std::pair<std::filesystem::path, std::shared_ptr<WPlugin>>(layoutP, plugin));
		}
	}
	owner.get()->layoutConsole.get()->entry(pathMap);
	owner.get()->scriptConsole.get()->ping(owner.get()->getTime(), " APPLICATION::UI::PLUGINS::FINISHED\n");
	return;
	// 4. Cross Register Plugins with eachother using the Relevant Header Properties
	// Place into Menu, hotkey, etc.
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// .PLUGIN PARSING FUNCTIONS
//
////////////////////////////////////////////////////////////////////////////////////////////////

// Parse through .plugin (custom data formatting) file string
void UI::parsePluginString(std::string fileString, std::string* plugName, std::string* plugVersion,
	std::string* plugAbout, std::string* _namespace,
	std::vector<std::filesystem::path>* layoutPaths,
	std::vector<std::filesystem::path>* stylePaths,
	std::vector<std::filesystem::path>* scriptPaths)
{
	std::string modString = fileString;
	modString.erase(std::remove(modString.begin(), modString.end(), '\n'), modString.end());
	*plugName = sliceRegularString(modString, "<NAME=", ">");
	*plugVersion = sliceRegularString(modString, "<VERSION=", ">");
	*plugAbout = sliceRegularString(modString, "<ABOUT=", ">");
	*_namespace = sliceRegularString(modString, "<NAMESPACE=", ">");
	// Get layoutpaths
	std::string layouts = sliceRegularString(modString, "<LAYOUT_PATH=", ">");
	layouts.erase(remove_if(layouts.begin(), layouts.end(), isspace), layouts.end());
	while (layouts.find(",") != std::string::npos)
	{
		std::filesystem::path layoutPath = splitRegularString(layouts, ",");
		if (!layoutPath.empty() && layoutPath.extension() == ".layout")
		{
			layoutPaths->emplace_back(layoutPath);
		}
	}
	std::filesystem::path layoutPath = layouts;
	if (!layoutPath.empty() && layoutPath.extension() == ".layout")
	{
		layoutPaths->emplace_back(layoutPath);
	}
	// Get style paths
	std::string styles = sliceRegularString(modString, "<STYLE_PATH=", ">");
	styles.erase(remove_if(styles.begin(), styles.end(), isspace), styles.end());
	while (styles.find(",") != std::string::npos)
	{
		std::filesystem::path stylePath = splitRegularString(styles, ",");
		if (!stylePath.empty() && stylePath.extension() == ".style")
		{
			stylePaths->emplace_back(stylePath);
		}
	}
	std::filesystem::path stylePath = styles;
	if (!stylePath.empty() && stylePath.extension() == ".style")
	{
		stylePaths->emplace_back(stylePath);
	}
	// Get script paths
	std::string scripts = sliceRegularString(modString, "<SCRIPT_PATH=", ">");
	scripts.erase(remove_if(scripts.begin(), scripts.end(), isspace), scripts.end());
	while (scripts.find(",") != std::string::npos)
	{
		std::filesystem::path scriptPath = splitRegularString(scripts, ",");
		if (!scriptPath.empty() && scriptPath.extension() == ".cscript")
		{
			scriptPaths->emplace_back(scriptPath);
		}
	}
	std::filesystem::path scriptPath = scripts;
	if (!scriptPath.empty() && scriptPath.extension() == ".cscript")
	{
		scriptPaths->emplace_back(scriptPath);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Callback Binding Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

bool UI::bindCallback(std::shared_ptr<CInterpreter> interpreter,
	CCallbackType callType, std::string callerID,
	std::shared_ptr<CFunction> callFunc)
{
	// First do type checking (Some callbacks share signatures, so save on space)
	bool validSignature = true;
	switch (callType)
	{
	case CCallbackType::updateFGColor:
	case CCallbackType::updateBGColor:
		// scriptSignature - void function(num array rgba, bool isBG)
		std::vector<std::shared_ptr<CToken>> paramTypes = callFunc.get()->funcDeclaration.get()->paramsTypes;
		if (paramTypes.size() != 2) { validSignature = false; }
		if (callFunc.get()->funcDeclaration.get()->returnType.get()->type != CTokenType::_VOID) { validSignature = false; }
		if (paramTypes[0].get()->type != CTokenType::NUM_ARRAY) { validSignature = false; }
		if (paramTypes[1].get()->type != CTokenType::BOOL) { validSignature = false; }
		break;
	}
	if (!validSignature) 
	{
		interpreter.get()->console.get()->error("[ui:0101] Callback function supplied for '" + CCallbackNames[size_t(callType)] +
			"' does not match the required signature. Check the documentation for callback signatures.");
		return false; 
	}
	// Then if there were no errors, bind the callback
	switch (callType)
	{
	case CCallbackType::updateFGColor:
		fgColorCallbacks.insert(std::pair<std::string, std::shared_ptr<CFunction>>(callerID, callFunc));
		break;
	case CCallbackType::updateBGColor:
		bgColorCallbacks.insert(std::pair<std::string, std::shared_ptr<CFunction>>(callerID, callFunc));
		break;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Color Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

void UI::updateFGColor(CColor color, int xpos, int ypos)
{
	// Set the color
	fgColor = color;
	// Notify widgets that are interested in this callback
	if (fgColorCallbacks.size() != 0)
	{
		// Build The Arguments
		std::vector<std::shared_ptr<CObject>> args;
		std::vector<std::shared_ptr<CObject>> colorArg = 
			std::vector<std::shared_ptr<CObject>>({
				std::make_shared<CObject>((double)color.r),
				std::make_shared<CObject>((double)color.g),
				std::make_shared<CObject>((double)color.b),
				std::make_shared<CObject>((double)1.0)
				});
		args.push_back(std::make_shared<CObject>(CLiteralTypes::_CNumber_Array, std::make_shared<std::vector<std::shared_ptr<CObject>>>(colorArg)));
		args.push_back(std::make_shared<CObject>(false));
		// Call the functions
		for (auto const& item : fgColorCallbacks)
		{
			item.second.get()->call(owner.get()->scriptConsole.get()->getInterpreter(), &args);
		}
	}
	// Tell the active tool about it
	owner.get()->toolbox.get()->notifyColorChange(color, false);
	// Print debugging
	//std::cout << "APP::UI::UPDATE_FG_COLOR=" << color.makeString() << std::endl;
}

void UI::updateBGColor(CColor color, int xpos, int ypos)
{
	// Set the color
	bgColor = color;
	// Notify widgets that are interested in this callback
	if (bgColorCallbacks.size() != 0)
	{
		// Build The Arguments
		std::vector<std::shared_ptr<CObject>> args;
		std::vector<std::shared_ptr<CObject>> colorArg =
			std::vector<std::shared_ptr<CObject>>({
				std::make_shared<CObject>((double)color.r),
				std::make_shared<CObject>((double)color.g),
				std::make_shared<CObject>((double)color.b),
				std::make_shared<CObject>((double)1.0)
				});
		args.push_back(std::make_shared<CObject>(CLiteralTypes::_CNumber_Array, std::make_shared<std::vector<std::shared_ptr<CObject>>>(colorArg)));
		args.push_back(std::make_shared<CObject>(true));
		// Call the functions
		for (auto const& item : fgColorCallbacks)
		{
			item.second.get()->call(owner.get()->scriptConsole.get()->getInterpreter(), &args);
		}
	}
	// Tell the active tool about it
	owner.get()->toolbox.get()->notifyColorChange(color, true);
	// Print debugging
	//std::cout << "APP::UI::UPDATE_BG_COLOR=" << color.makeString() << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Click Handler Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

bool UI::widgetSweepTest(MouseEvent input)
{
	hitWidget.reset();
	std::vector<std::weak_ptr<Widget>> hitWidgets;
	for (size_t i = 0; i < widgets.size(); i++)
	{
		// Check for sweep against hotspots, single depth check
		bool sweepResult = widgets[i].get()->mouseSweep(input.x, input.y);
		if (sweepResult)
		{
			hitWidgets.push_back(widgets[i]);
		}
	}
	if (hitWidgets.size() == 0) { return false; }
	else
	{
		// If mouse-over, do not store as hit
		if (input.button == UI_MOUSE_OVER)
		{
			hitWidgets.clear();
			return true;
		}
		// Else, store as hit
		else
		{
			hitWidget = hitWidgets.back();
			return true;
		}
	}
}

bool UI::widgetHitTest(MouseEvent input, bool dragOnly)
{
	bool result = false;
	// Check if there's a stored widget
	if (!hitWidget.expired())
	{
		if (hitWidget.lock().get()->mouseHit(input, dragOnly)) 
		{ 
			result = true;
		}
		if (!didFocusThisClick)
		{
			hitWidget.lock().get()->selfFocus(input);
			updateFocusWidget(hitWidget);
		}
		hitWidget.reset();
	}
	// Otherwise, continue
	else
	{
		std::vector<std::weak_ptr<Widget>> hitWidgets;
		for (size_t i = 0; i < widgets.size(); i++)
		{
			// Check for sweep against hotspots, single depth check
			bool sweepResult = widgets[i].get()->mouseSweep(input.x, input.y);
			if (sweepResult)
			{
				result = true;
				hitWidgets.push_back(widgets[i]);
			}
		}
		// Check for hitResult against the collision
		// Note: For now only check against the last widget in the array which (should)
		// be the top-most (last) rendered widget.
		if (hitWidgets.size() != 0) { result = hitWidgets.back().lock().get()->mouseHit(input, dragOnly); }
		if (hitWidgets.size() != 0 && !didFocusThisClick)
		{
			hitWidgets.back().lock().get()->selfFocus(input);
			updateFocusWidget(hitWidgets.back());
		}
	}
	didFocusThisClick = false;
	return result;
}

bool UI::widgetHoverTest(MouseEvent input)
{
	// Otherwise, continue
	std::vector<std::weak_ptr<Widget>> hoveredWidgets;
	for (size_t i = 0; i < widgets.size(); i++)
	{
		// Check for sweep against hotspots, single depth check
		bool sweepResult = widgets[i].get()->mouseSweep(input.x, input.y);
		if (sweepResult)
		{
			hoveredWidgets.push_back(widgets[i]);
		}
	}
	if (hoveredWidgets.size() == 0) { return false; }
	else
	{
		// Check for hitResult against the collision
		// Note: For now only check against the last widget in the array which (should)
		// be the top-most (last) rendered widget.
		// If the hitResult is false (no-hit in event of non-rectangular widget)
		// then check the next widget in the array
		bool hitResult = hoveredWidgets.back().lock().get()->mouseHover(input);
		if (hitResult) { return true; }
		else { return false; }
	}
	return false;
}

bool UI::widgetLeaveTest(MouseEvent input)
{
	bool result = false;
	std::vector<int> eraseKeys;
	for (auto& entry : enteredWidgets)
	{
		if (!entry.second.expired() && entry.second.lock().get()->selfLeave(input))
		{
			entry.second.reset();
			eraseKeys.push_back(entry.first);
			result = true;
		}
		else
		{
			if (entry.second.expired()) { eraseKeys.push_back(entry.first); }
		}
	}
	for (int key : eraseKeys)
	{
		enteredWidgets.erase(key);
	}
	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Widget Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

void UI::updateFocusWidget(std::weak_ptr<Widget> target)
{
	if (!target.expired())
	{
		if (!focusWidget.expired())
		{
			if (focusWidget.lock().get() != target.lock().get())
			{
				//std::cout << "UPDATE::FOCUS::WITH-BLUR" << std::endl;
				focusWidget.lock().get()->selfBlur();
				if (!interruptBlur)
				{
					focusWidget.reset();
					//std::cout << "UPDATE::FOCUS::CHANGED-TARGET::ID,CLASSES=" <<
					//	target.lock().get()->idAttrib + ", " << stringVecToString(target.lock().get()->classAttribs) << std::endl;
					focusWidget = target;
				}
				else
				{
					interruptBlur = false;
					//std::cout << "UPDATE::FOCUS::BLUR_INTERRUPTED=" <<
					//	target.lock().get()->idAttrib + ", " << stringVecToString(target.lock().get()->classAttribs) << std::endl;
				}
			}
		}
		else
		{
			//std::cout << "UPDATE::FOCUS::NEW-TARGET::ID,CLASSES=" <<
			//	target.lock().get()->idAttrib + ", " << stringVecToString(target.lock().get()->classAttribs) << std::endl;
			focusWidget = target;
		}
	}
}
void UI::clearFocusWidget()
{
	if (!focusWidget.expired())
	{
		//std::cout << "CLEAR::FOCUS::WITH-BLUR" << std::endl;
		focusWidget.lock().get()->selfBlur();
	}
	//std::cout << "CLEAR::FOCUS" << std::endl;
	focusWidget.reset();
}

void UI::resetPropertyByClass(std::shared_ptr<CInterpreter> interpreter,
	std::string className,
	std::string propertyName)
{
	std::vector<std::weak_ptr<Widget>> selection = getWidgetsByClass(className, "");
	if (selection.size() != 0)
	{
		for (std::weak_ptr<Widget> widget : selection)
		{
			if (!widget.expired())
			{
				widget.lock().get()->resetProperty(interpreter, propertyName);
			}
		}
	}
}

void UI::setPropertyByClass(std::shared_ptr<CInterpreter> interpreter,
	std::string className, std::string idExclusion, 
	std::string propertyName, std::shared_ptr<CObject> value)
{
	std::vector<std::weak_ptr<Widget>> selection = getWidgetsByClass(className, idExclusion);
	
	if (selection.size() != 0)
	{
		for (std::weak_ptr<Widget> widget : selection)
		{
			if (!widget.expired())
			{
				widget.lock().get()->setProperty(interpreter, propertyName, value);
			}
		}
	}
}

void UI::setPropertyByClass(std::shared_ptr<CInterpreter> interpreter,
	std::string className, std::weak_ptr<Widget> widgetExclusion,
	std::string propertyName, std::shared_ptr<CObject> value)
{
	std::vector<std::weak_ptr<Widget>> selection = getWidgetsByClass(className, "");
	std::shared_ptr<Widget> exclusion = (widgetExclusion.expired()) ? nullptr : widgetExclusion.lock();

	if (selection.size() != 0)
	{
		for (std::weak_ptr<Widget> widget : selection)
		{
			if (!widget.expired() && exclusion != nullptr && widget.lock() != exclusion)
			{
				widget.lock().get()->setProperty(interpreter, propertyName, value);
			}
		}
	}
}

// Widget Lookup
std::weak_ptr<Widget> UI::getWidgetByID(std::string id)
{
	std::weak_ptr<Widget> null;
	for (std::shared_ptr<Widget> root : widgets)
	{
		if (root.get()->rootId == id) { return root; }
	}
	if (widgetIDTable.count(id) == 1)
	{
		return getRootWidgetByID(widgetIDTable.at(id)).lock().get()->getChild_byID(id);
	}
	return null;
}

std::vector<std::weak_ptr<Widget>> UI::getWidgetsByClass(std::string className, std::string idExclusion)
{
	std::shared_ptr<std::vector<std::weak_ptr<Widget>>> out = std::make_shared<std::vector<std::weak_ptr<Widget>>>();
	if (widgetClassTable.count(className) == 1)
	{
		for (std::string rootID : widgetClassTable.at(className))
		{
			std::weak_ptr<Widget> root = getRootWidgetByID(rootID);
			if (!root.expired())
			{
				root.lock().get()->getChild_byClass(out, className, idExclusion);
			}
		}
	}
	return *out;
}

std::vector<std::weak_ptr<Widget>> UI::getWidgetsByType(std::string typeName, std::string idExclusion)
{
	// Warning: Unfinished function body
	std::shared_ptr<std::vector<std::weak_ptr<Widget>>> out = std::make_shared<std::vector<std::weak_ptr<Widget>>>();
	if (layoutTypeMap.count(typeName) == 1)
	{
		/*
		for (std::string rootID : widgetClassTable.at(className))
		{
			std::weak_ptr<Widget> root = getRootWidgetByID(rootID);
			if (!root.expired())
			{
				root.lock().get()->getChild_byClass(out, className, idExclusion);
			}
		}
		*/
	}
	return *out;
}

std::weak_ptr<Widget> UI::getRootWidgetByID(std::string rootID)
{
	std::weak_ptr<Widget> null;
	for (std::shared_ptr<Widget> root : widgets)
	{
		if (root.get()->rootId == rootID)
		{
			return root;
		}
	}
	return null;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Render Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

void UI::drawWidgets()
{
	if (widgets.size() == 0) { return; }
	else
	{
		for (std::shared_ptr<Widget> obj : widgets)
		{
			obj.get()->draw(owner.get()->getCamera()->getShaderTransform());
		}
	}
}