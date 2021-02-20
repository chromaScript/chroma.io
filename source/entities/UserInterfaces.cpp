#include "../include/entities/UserInterface.h"

#include "../include/entities/Canvas.h"
#include "../include/entities/Root.h"
#include "../include/entities/HorizontalBox.h"
#include "../include/entities/VerticalBox.h"
#include "../include/entities/Image.h"
#include "../include/entities/Text.h"
#include "../include/cscript/ChromaScript.h"
#include "../include/cscript/CEnvironment.h"
#include "../include/clayout/LEnums.h"
#include "../include/cscript/CObject.h"

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

// Canvas Functions
void UI::addCanvas(Canvas* c, bool doSet)
{
	documents.push_back(c);
	if (doSet == true)
	{
		if (doSet == true) { setCanvas(documents[documents.size() - 1]); }
	}
}
void UI::setCanvas(Canvas* c)
{
	canvas = c;
}
Canvas* UI::getCanvas()
{
	return canvas;
}

// Cursor Functions
void UI::updateCursorImage(CustomCursor* c)
{
	glfwSetCursor(owner.get()->getWindow(), c->getCursor());
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
	if (doTopDown == true) { sizeWidgetByParent(treeRoot); }
	if (doPlacement == true) { updateWidgetLocation(treeRoot); }
	
	for (std::shared_ptr<Widget> child : treeRoot.get()->childWidgets)
	{
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
		stepThroughWidgetTree(root, false, true, false);
		stepThroughWidgetTree(root, true, false, false);
		stepThroughWidgetTree(root, false, false, true);
		root.get()->checkOutofBoundsWidgets(root);
	}
}

// Build WidgetHierarchy
void UI::buildWidgetHierarchy()
{

}

// Rebuild WidgetHierarchy
void UI::rebuildWidgetHierarchy()
{

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
	case LTokenType::PROTO:
	case LTokenType::ROOT:
		return std::make_shared<Root>(basicAttribs, parent, style, shader);
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
		std::cout << "\nAPPLICATION::UI::LOAD_PLUGIN::SCRIPTS::NAME=" << plugin.get()->name << std::endl;
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
		std::cout << "\nAPPLICATION::UI::LOAD_PLUGIN::STYLES::NAME=" << plugin.get()->name << std::endl;
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
		std::cout << "\nAPPLICATION::UI::LOAD_PLUGIN::STYLES::NAME=" << plugin.get()->name << std::endl;
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

bool UI::widgetHitTest(MouseEvent input)
{
	// Check if there's a stored widget
	if (!hitWidget.expired())
	{
		if (hitWidget.lock().get()->mouseHit(input)) { hitWidget.reset(); return true; }
		hitWidget.reset();
	}
	// Otherwise, continue
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
		// Check for hitResult against the collision
		// Note: For now only check against the last widget in the array which (should)
		// be the top-most (last) rendered widget.
		// If the hitResult is false (no-hit in event of non-rectangular widget)
		// then check the next widget in the array
		bool hitResult = hitWidgets.back().lock().get()->mouseHit(input);
		if (hitResult) { return true; }
		else { return false; }
	}
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