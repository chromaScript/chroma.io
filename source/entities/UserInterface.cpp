#include "../include/entities/UserInterface.h"

#include "../include/entities/layers/Canvas.h"
#include "../include/entities/widgets/HorizontalBox.h"
#include "../include/entities/widgets/VerticalBox.h"
#include "../include/entities/widgets/ImageWidget.h"
#include "../include/entities/widgets/Text.h"
#include "../include/entities/widgets/Line.h"
#include "../include/entities/widgets/GradientBox.h"
#include "../include/entities/widgets/Graph_ToolControl.h"
#include "../include/entities/widgets/Noise_ToolControl.h"
#include "../include/cscript/ChromaScript.h"
#include "../include/cscript/CEnvironment.h"
#include "../include/cscript/CInterpreter.h"
#include "../include/cscript/CEnums.h"
#include "../include/clayout/LEnums.h"
#include "../include/cscript/CObject.h"
#include "../include/tool/Toolbox.h"
#include "../include/entities/visuals/Visualizer.h"

#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <deque>
#include <functional>
#include <algorithm>
#include <map>

// Constructors
UI::UI(std::shared_ptr<Application> owner)
{
	// Register the owner for quick access
	this->owner = owner;
	// Initialize the fontHandler
	fontHandler = std::make_shared<Fonts>();
	visualizer = std::make_unique<Visualizer>();
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Canvas Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

bool UI::newDocument(std::string docName, int width, int height, bool setAsActive)
{
	closeDocument("NULL", -1, true, false);
	documents.push_back(std::make_shared<Canvas>(docName, width, height, owner.get()->getCanvasShader()));
	documents.back()->newDataLayer(
		glm::ivec2(width, height), 
		"Background", 
		white, 
		owner.get()->getLayerShader());
	if (setAsActive) 
	{ 
		setCanvas(documents.back()); 
		owner.get()->getCamera()->setMaxZoom(float(width), float(height));
		owner.get()->getCamera()->zoomExtents(glm::ivec2(width, height));
	}
	return true;
}
bool UI::closeDocument(std::string docName, int docID, bool closeActive, bool saveBeforeExit)
{
	bool result = false;
	if (closeActive)
	{
		if (activeCanvas != nullptr)
		{
			result = activeCanvas.get()->close(saveBeforeExit);
			std::vector<std::shared_ptr<Canvas>>::iterator it = documents.begin();
			for (it; it != documents.end(); ++it)
			{
				if (*it == activeCanvas)
				{
					activeCanvas.get()->deleteBuffers();
					activeCanvas.reset();
					break;
				}
			}
			documents.erase(it);
			documents.shrink_to_fit();
		}
		visualizer.get()->clearLayers();
	}
	else
	{

	}
	return result;
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

void UI::sizeWidgetByChildren(Widget* target)
{
	target->setSizeByChildren();
}
void UI::sizeWidgetByParent(Widget* target)
{
	target->setSizeByParent();
}
void UI::updateWidgetLocation(Widget* target)
{
	target->buildWidget();
	target->placeWidget();
	target->checkOverflow();
}
void UI::stepThroughWidgetTree(std::shared_ptr<Widget> treeRoot, bool doTopDown, bool doBottomUp, bool doPlacement)
{
	// Be aware that adding too many calls in this recursive function can significantly impact
	// performance for resizing/moving widgets. Make due with as few functions as necessary
	// in order to avoid displaying widgets incorrectly.

	if (doTopDown == true) { sizeWidgetByParent(treeRoot.get()); }
	if (doPlacement == true) { updateWidgetLocation(treeRoot.get()); }

	for (std::shared_ptr<Widget> child : treeRoot.get()->childWidgets)
	{
		// Additional Preliminary top-level sizeByChildren step so that at least the minimum
		// size of the widget is known before diving further. Without this, child widgets of 'child' that would
		// float right, but have a minimum size lower than 'child's final size, will not get the right potential
		// maxX and maxY values.
		for (std::shared_ptr<Widget> childChild : child.get()->childWidgets)
		{
			sizeWidgetByChildren(child.get());
		}

		// Do stuff before stepping into tree
		if (doTopDown == true) { sizeWidgetByParent(child.get()); }
		if (doPlacement == true) { updateWidgetLocation(child.get()); }

		stepThroughWidgetTree(child, doTopDown, doBottomUp, doPlacement);

		// Do stuff after stepping into tree
		if (doBottomUp == true) { child.get()->resetSize(); sizeWidgetByChildren(child.get()); }
	}

	if (doBottomUp == true) { treeRoot.get()->resetSize(); sizeWidgetByChildren(treeRoot.get()); }
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
	for (std::shared_ptr<Widget> root : rootWidgets)
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
void UI::requestWidgetHierarchyRebuild(std::weak_ptr<Widget> target)
{
	int targetUEID = 0;
	int parentUEID = 0;
	bool isRoot = false;

	if (!target.expired())
	{
		targetUEID = target.lock().get()->getUEID();
		if (!target.lock().get()->parentWidget.expired())
		{
			parentUEID = target.lock().get()->parentWidget.lock().get()->getUEID();
		}
		else { isRoot = true; }

		if (rebuildWidgetIDList.size() == 0)
		{
			if (!isRoot) { rebuildWidgetIDList.push_back(parentUEID); rebuildWidgets.push_back(target.lock().get()->parentWidget); }
			else { rebuildWidgetIDList.push_back(targetUEID); rebuildWidgets.push_back(target); }
		}
		else
		{
			bool didCollide = false;
			if (isRoot)
			{
				for (int i : rebuildWidgetIDList)
				{
					if (i == targetUEID) { didCollide = true; break; }
				}
				if (!didCollide) { rebuildWidgetIDList.push_back(targetUEID); rebuildWidgets.push_back(target); }
			}
			else
			{
				std::vector<int> parentUEIDChain = target.lock().get()->reportParentUEIDChain();
				for (int i : rebuildWidgetIDList)
				{
					if (i == parentUEID) { didCollide = true; break; }
					for (int k : parentUEIDChain)
					{
						if (i == k) { didCollide = true; break; }
					}
				}
				if (!didCollide) { rebuildWidgetIDList.push_back(parentUEID); rebuildWidgets.push_back(target.lock().get()->parentWidget); }
			}
		}
	}
}
void UI::rebuildWidgetHierarchy(std::weak_ptr<Widget> target)
{
	if (!target.expired())
	{
		stepThroughWidgetTree(target.lock(), false, true, false); // 1. BottomUp
		stepThroughWidgetTree(target.lock(), true, false, false); // 2. TopDown
		stepThroughWidgetTree(target.lock(), false, false, true); // 3. Placement
		if (!target.lock()->getRoot().expired())
		{
			target.lock().get()->checkOutofBoundsWidgets(target.lock()->getRoot(), target.lock()->getRoot().lock()->getColliderBox());
		}
	}
}

void UI::clearRebuildRequests()
{
	if (rebuildWidgets.size() == 0) { return; }
	for (std::weak_ptr<Widget> target : rebuildWidgets)
	{
		if (!target.expired())
		{
			rebuildWidgetHierarchy(target);
		}
		target.reset(); // Added**
	}
	rebuildWidgets.clear();
	rebuildWidgets.shrink_to_fit();
	rebuildWidgetIDList.clear();
	rebuildWidgetIDList.shrink_to_fit();
}

std::shared_ptr<Widget> UI::createWidget(
	LTokenType widgetType,
	std::map<std::string, std::string> basicAttribs,
	std::weak_ptr<Widget> parent,
	std::weak_ptr<WidgetStyle> style,
	std::shared_ptr<Shader> shader)
{
	// 4. Switch on elmentName, using element specific constructor
	
	if (widgetType == LTokenType::PANEL)
	{
		return nullptr;
	}
	else if (widgetType == LTokenType::H_BOX)
	{
		return std::make_shared<HorizontalBox>(basicAttribs, parent, style, shader);
	}
	else if (widgetType == LTokenType::V_BOX)
	{
		return std::make_shared<VerticalBox>(basicAttribs, parent, style, shader);
	}
	else if (widgetType == LTokenType::TEXT)
	{
		return std::make_shared<Text>(basicAttribs, parent, style, shader, fontHandler);
	}
	else if (widgetType == LTokenType::IMAGE)
	{
		return std::make_shared<ImageWidget>(basicAttribs, parent, style, shader);
	}
	else if (widgetType == LTokenType::LINE)
	{
		return std::make_shared<Line>(basicAttribs, parent, style, shader, fontHandler);
	}
	else if (widgetType == LTokenType::GRADIENT_BOX)
	{
		return std::make_shared<GradientBox>(basicAttribs, parent, style, shader);
	}
	else if (widgetType == LTokenType::T_GRAPH)
	{
		std::shared_ptr<Widget> returnObj = std::make_shared<Graph_ToolControl>(basicAttribs, parent, style, shader);
		return returnObj;
	}
	else if (widgetType == LTokenType::T_NOISE)
	{
		return std::make_shared<Noise_ToolControl>(basicAttribs, parent, style, shader);
	}
	else
	{
		return nullptr;
	}
	//case LTokenType::C_GRAPH:
	//	return std::make_shared<GradientBox>(basicAttribs, parent, style, shader);
	//case LTokenType::R_GRAPH:
	//	return std::make_shared<GradientBox>(basicAttribs, parent, style, shader);
	//case LTokenType::EDIT_GRADIENT:
	//	return std::make_shared<GradientBox>(basicAttribs, parent, style, shader);
}

std::shared_ptr<Widget> UI::createWidget_fromPrototype(
	std::shared_ptr<Widget> targetWidget, 
	std::string protoID, std::string childID, std::vector<std::string> extraClasses)
{
	std::shared_ptr<Widget> proto = prototypeFactoryTable.at(protoID);
	std::string rootID = targetWidget.get()->getRoot().lock()->rootId;
	std::shared_ptr<Widget> instance = proto.get()->duplicate(proto, targetWidget, rootID, true);
	instance.get()->rootId = "";
	instance.get()->idAttrib = childID;
	std::shared_ptr<CInterpreter> interpreter = owner.get()->scriptConsole.get()->getInterpreter();
	std::shared_ptr<CEnvironment> previousEnvironment = interpreter.get()->currentEnvironment;
	interpreter.get()->setEnvironment(interpreter.get()->getEnvironment(proto.get()->_namespace));
	for (std::string className : extraClasses)
	{
		std::shared_ptr<CObject> classStyle = interpreter.get()->currentEnvironment.get()->get("." + className);
		if (classStyle != nullptr)
		{
			instance.get()->style.mergeStyle(std::get<std::shared_ptr<WidgetStyle>>(classStyle.get()->obj));
		}
	}
	interpreter.get()->setEnvironment(previousEnvironment);
	return instance;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Initialize Interface
//
////////////////////////////////////////////////////////////////////////////////////////////////
inline double fibo(double f)
{
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
		std::filesystem::path layoutPath = splitRegularString(layouts, ",", true);
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
		std::filesystem::path stylePath = splitRegularString(styles, ",", true);
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
		std::filesystem::path scriptPath = splitRegularString(scripts, ",", true);
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

void UI::resetFGBGColor(CColor fg, CColor bg)
{
	updateFGColor(fg, 0, 0);
	updateBGColor(bg, 0, 0);
}
void UI::swapFGBGColor()
{
	CColor currentFG = fgColor;
	CColor currentBG = bgColor;
	updateFGColor(currentBG, 0, 0);
	updateBGColor(currentFG, 0, 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Click Handler Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

bool UI::widgetSweepTest(Input input)
{
	hitWidget.reset();
	std::vector<std::weak_ptr<Widget>> hitWidgets;
	for (size_t i = 0; i < rootWidgets.size(); i++)
	{
		// Check for sweep against hotspots, single depth check
		bool sweepResult = rootWidgets[i].get()->mouseSweep(input.x, input.y);
		if (sweepResult)
		{
			hitWidgets.push_back(rootWidgets[i]);
		}
	}
	if (hitWidgets.size() == 0) { return false; }
	else
	{
		// If mouse-over, do not store as hit
		if (input.button == InputMouseButton::hover)
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

bool UI::widgetHitTest(Input input, bool dragOnly, bool shouldFocus)
{
	bool hitResult = false;
	// Check if there's a stored widget
	if (!hitWidget.expired())
	{
		// First prevent clicks when a popup is blocking
		if (checkBlockingPopupMatch(hitWidget.lock().get()->getRoot())) { return false; }
		// Resolve hit events
		std::deque<Widget*> clickStack;
		std::deque<Widget*> focusStack;
		unsigned int maxZIndex = 0;
		hitResult = hitWidget.lock().get()->mouseHit(&input, dragOnly, shouldFocus, clickStack, focusStack, isZStackActive, maxZIndex);
		clickStack.shrink_to_fit();
		focusStack.shrink_to_fit();
		if (hitResult && isZStackActive)
		{
			hitResult = false;
			// If the ZStack is empty, then the return is simple
			for (int i = (int)clickStack.size() - 1; i >= 0; i--)
			{
				if (clickStack[i]->style.zIndex < maxZIndex) { continue; }
				if (clickStack[i]->selfHit(&input, dragOnly, false, maxZIndex, true)) { hitResult = true; break; }
			}
		}
		// Clean up focus events
		if (isZStackActive && focusStack.size() != 0 && shouldFocus)
		{
			bool focusResult = false;
			// If the ZStack is empty, then the return is simple
			for (int i = 0; i < focusStack.size(); i++)
			//for (int i = (int)focusStack.size() - 1; i >= 0; i--)
			{
				if (focusStack[i]->selfFocus(&input, false, maxZIndex, true)) { focusResult = true; break; }
			}
			if (!didFocusThisClick && !focusResult && shouldFocus)
			{
				hitWidget.lock().get()->selfFocus(&input, false, maxZIndex, false);
				updateFocusWidget(hitWidget);
			}
		}
		else if (!didFocusThisClick && shouldFocus)
		{
			hitWidget.lock().get()->selfFocus(&input, false, maxZIndex, false);
			updateFocusWidget(hitWidget);
		}
		hitWidget.reset();
	}
	// Otherwise, continue
	else
	{
		std::vector<std::weak_ptr<Widget>> hitWidgets;
		for (size_t i = 0; i < rootWidgets.size(); i++)
		{
			// Check for sweep against hotspots, single depth check
			bool sweepResult = rootWidgets[i].get()->mouseSweep(input.x, input.y);
			if (sweepResult)
			{
				hitResult = true;
				hitWidgets.push_back(rootWidgets[i]);
			}
		}
		// First prevent clicks when a popup is blocking
		if (checkBlockingPopupMatch(hitWidgets.back().lock().get()->getRoot())) { return false; }
		// Check for hitResult against the collision
		// Note: For now only check against the last widget in the array which (should)
		// be the top-most (last) rendered widget.
		std::deque<Widget*> clickStack;
		std::deque<Widget*> focusStack;
		unsigned int maxZIndex = 0;
		if (hitWidgets.size() != 0) 
		{
			hitResult = hitWidgets.back().lock().get()->mouseHit(&input, dragOnly, shouldFocus, clickStack, focusStack, isZStackActive, maxZIndex);
			clickStack.shrink_to_fit();
			if (hitResult)
			{
				if (isZStackActive)
				{
					hitResult = false;
					// If the ZStack is empty, then the return is simple
					for (int i = (int)clickStack.size() - 1; i >= 0; i--)
					{
						if (clickStack[i]->style.zIndex < maxZIndex) { continue; }
						if (clickStack[i]->selfHit(&input, dragOnly, false, maxZIndex, true) != 0) { hitResult = true; break; }
					}
				}
			}
		}
		if (hitWidgets.size() != 0)
		{
			// Clean up focus events
			if (isZStackActive && focusStack.size() != 0 && shouldFocus)
			{
				focusStack.shrink_to_fit();
				bool focusResult = false;
				// If the ZStack is empty, then the return is simple
				for (int i = 0; i < focusStack.size(); i++)
				{
					if (focusStack[i]->selfFocus(&input, false, maxZIndex, true)) { focusResult = true; break; }
				}
				if (!didFocusThisClick && !focusResult && shouldFocus)
				{
					hitWidgets.back().lock().get()->selfFocus(&input, false, maxZIndex, false);
					updateFocusWidget(hitWidgets.back());
				}
			}
			else if (!didFocusThisClick && shouldFocus)
			{
				hitWidgets.back().lock().get()->selfFocus(&input, false, maxZIndex, false);
				updateFocusWidget(hitWidgets.back());
			}
		}
	}
	didFocusThisClick = false;
	return hitResult;
}

bool UI::widgetHoverTest(Input* input)
{
	// Otherwise, continue
	std::vector<std::weak_ptr<Widget>> hoveredWidgets;
	for (size_t i = 0; i < rootWidgets.size(); i++)
	{
		// Check for sweep against hotspots, single depth check
		bool sweepResult = rootWidgets[i].get()->mouseSweep(input->x, input->y);
		if (sweepResult)
		{
			hoveredWidgets.push_back(rootWidgets[i]);
		}
	}
	if (hoveredWidgets.size() == 0) { return false; }
	else
	{
		// First prevent clicks when a popup is blocking
		if (checkBlockingPopupMatch(hoveredWidgets.back().lock().get()->getRoot())) { return false; }
		// Check for hitResult against the collision
		// Note: For now only check against the last widget in the array which (should)
		// be the top-most (last) rendered widget.
		// If the hitResult is false (no-hit in event of non-rectangular widget)
		// then check the next widget in the array
		std::deque<Widget*> hoverStack;
		unsigned int maxZIndex = 0;
		bool hitResult = hoveredWidgets.back().lock().get()->mouseHover(input, hoverStack, isZStackActive, maxZIndex);
		hoverStack.shrink_to_fit();
		if (hitResult) 
		{
			// If the ZStack is empty, then the return is simple
			if (!isZStackActive) { return hitResult; }
			for (int i = (int)hoverStack.size() - 1; i >= 0; i--)
			{
				if (hoverStack[i]->selfHover(input, false, maxZIndex, false) != 0) { return hitResult; }
			}
			return hitResult;
		}
		else { return hitResult; }
	}
	return false;
}

bool UI::widgetLeaveTest(Input* input, unsigned int maxZIndex)
{
	bool result = false;
	std::vector<int> eraseKeys;
	for (auto& entry : enteredWidgets)
	{
		if (!entry.second.expired() && entry.second.lock().get()->selfLeave(input, maxZIndex))
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
		// Also remove widget from overWidgets
		if (overWidgets.count(key) == 1) { overWidgets.erase(key); }
	}
	return result;
}

bool UI::widgetOverTest(Input* input, unsigned int maxZIndex)
{
	bool result = false;
	std::vector<int> eraseKeys;
	for (auto& entry : overWidgets)
	{
		if (!entry.second.expired())
		{
			if (entry.second.lock().get()->selfLeave(input, maxZIndex))
			{
				entry.second.reset();
				eraseKeys.push_back(entry.first);
			}
			else
			{
				activeWidget = entry.second;
				owner.get()->scriptConsole.get()->run(entry.second.lock()->callbackMap.at("onmouseover"), "global");
				activeWidget.reset();
				result = true;
			}
		}
		else
		{
			eraseKeys.push_back(entry.first);
		}
	}
	for (int key : eraseKeys)
	{
		overWidgets.erase(key);
	}
	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Widget Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

void UI::addToClassTable(std::string className, std::string rootID)
{
	if (widgetClassTable.count(className) == 0)
	{
		widgetClassTable.insert(std::pair<std::string, std::vector<std::string>>(className, { rootID }));
	}
	else
	{
		bool rootFound = false;
		for (std::string rootIDExisting : widgetClassTable.at(className))
		{
			if (rootIDExisting == rootID) { rootFound = true; }
		}
		if (!rootFound) { widgetClassTable.at(className).push_back(rootID); }
	}
}

// Z Index Handling
void UI::addZIndexEntry(Widget* target, unsigned int zIndex)
{
	if (zIndexMap.count(zIndex) == 1)
	{
		zIndexMap.at(zIndex).stack.push_back(target);
	}
	else
	{
		zIndexMap.insert(std::pair<unsigned int, ZIndexStack>(zIndex, ZIndexStack(zIndex)));
		zIndexMap.at(zIndex).stack.push_back(target);
	}
}

// Handling Input Widget Updates
void UI::putActiveInputWidget(std::weak_ptr<Widget> widget, bool isClear, bool withBlur, int eventType)
{
	// Clear Active Widget
	if (isClear)
	{
		if (!activeInputWidget.expired())
		{
			// Trigger callbacks
			std::shared_ptr<Widget> target = activeInputWidget.lock();
			if (eventType == UI_WEVENT_CANCEL && 
				target->callbackMap.count("oncancel") == 1 && 
				target->callbackMap.at("oncancel").size() != 0)
			{
				owner.get()->getUI().get()->activeWidget = activeInputWidget;
				owner.get()->scriptConsole.get()->run(target->callbackMap.at("oncancel"), target->_namespace);
				owner.get()->getUI().get()->activeWidget.reset();
			}
			else if (eventType == UI_WEVENT_ENTRY &&
				target->callbackMap.count("onentry") == 1 &&
				target->callbackMap.at("onentry").size() != 0)
			{
				owner.get()->getUI().get()->activeWidget = activeInputWidget;
				owner.get()->scriptConsole.get()->run(target->callbackMap.at("onentry"), target->_namespace);
				owner.get()->getUI().get()->activeWidget.reset();
			}
			// Type-Specific Instructions
			if (activeInputWidget.lock().get()->type == LTokenType::LINE)
			{
				std::shared_ptr<Line> lineWidget = std::dynamic_pointer_cast<Line>(target);
				lineWidget.get()->clearSelection();
				lineWidget.get()->setCursorPos(0);
				lineWidget.get()->isActive = false;
			}
			activeInputWidget.reset();
			activeInputType = LTokenType::NIL;
			// Trigger blur callback
			if (withBlur && !focusWidget.expired() && target == focusWidget.lock())
			{
				clearFocusWidget();
			}
		}
		else
		{
			activeInputWidget.reset();
			activeInputType = LTokenType::NIL;
		}
		return;
	}
	// Set Active Widget
	if (!widget.expired() && !isClear)
	{
		if (widget.lock().get()->type == LTokenType::LINE)
		{
			activeInputWidget = widget;
			//
			std::shared_ptr<Line> lineWidget = std::dynamic_pointer_cast<Line>(activeInputWidget.lock());
			activeInputType = lineWidget.get()->type;
			lineWidget.get()->isActive = true;
			lineWidget.get()->valueAttrib = "";
			lineWidget.get()->setCursorPos(0);
		}
	}
}

// Manage resize events
void UI::clearResizeEvents()
{
	if (resizeWidgets.size() == 0) { return; }
	for (std::weak_ptr<Widget> widget : resizeWidgets)
	{
		if (widget.expired()) { continue; }
		std::shared_ptr<Widget> target = widget.lock();
		if (target->callbackMap.count("onresize") == 1 && target->callbackMap.at("onresize").size() != 0)
		{
			owner.get()->getUI().get()->activeWidget = widget;
			owner.get()->scriptConsole.get()->run(target->callbackMap.at("onresize"), target->_namespace);
			owner.get()->getUI().get()->activeWidget.reset();
		}
	}
	resizeWidgets.clear();
	resizeWidgets.shrink_to_fit();
}


// Manage popup widgets
bool UI::putActivePopupWidget(std::weak_ptr<Widget> widget, bool isBlocking, std::shared_ptr<CFunction> escCallback)
{
	bool wasHandled = false;
	// First verify that the callback is valid
	if (escCallback == nullptr || widget.expired()) { return wasHandled; }
	else
	{
		if (escCallback.get()->funcDeclaration.get()->paramsTypes.size() != 0 ||
			escCallback.get()->funcDeclaration.get()->returnType.get()->type != CTokenType::_VOID)
		{
			return wasHandled;
		}
	}
	// Next check that the tag is not duplicate
	bool foundTagMatch = false;
	size_t matchIndex = std::string::npos;
	for (int i = 0; i < popupTags.size(); i++)
	{
		if (popupTags[i] == widget.lock().get()->idAttrib)
		{
			foundTagMatch = true;
			matchIndex = size_t(i);
			break;
		}
	}
	// Set the blocking property
	widget.lock().get()->popupIsBlocking = isBlocking;
	// Insert or rearrange the pop-up order
	if (foundTagMatch)
	{
		// If was duplicate, move the popup to the front of the deque
		// The front-most element in the deque intercepts the escape-key inputs
		popupTags.erase(popupTags.begin() + matchIndex);
		popupWidgets.erase(popupWidgets.begin() + matchIndex);
		popupTags.push_front(widget.lock().get()->idAttrib);
		popupWidgets.push_front(widget);
		wasHandled = isPopupActive = true;
		moveRootToFront(widget.lock().get()->getRoot());
	}
	else
	{
		// If was not a duplicate, insert a new popup
		if (popupEscCallbacks.count(widget) == 0)
		{
			popupEscCallbacks.insert(std::pair<std::weak_ptr<Widget>, std::shared_ptr<CFunction>>(widget, escCallback));
			wasHandled = isPopupActive = true;
			popupTags.push_front(widget.lock().get()->idAttrib);
			popupWidgets.push_back(widget);
			moveRootToFront(widget.lock().get()->getRoot());
		}
	}
	
	return wasHandled;
}

bool UI::clearPopupWidget(std::weak_ptr<Widget> widget)
{
	bool wasHandled = false;

	bool foundTagMatch = false;
	size_t matchIndex = std::string::npos;
	for (int i = 0; i < popupTags.size(); i++)
	{
		if (popupTags[i] == widget.lock().get()->idAttrib)
		{
			foundTagMatch = true;
			matchIndex = size_t(i);
			break;
		}
	}

	if (foundTagMatch)
	{
		// Clear the popup from the callbacks map
		std::weak_ptr<Widget> popup;
		for (auto const& item : popupEscCallbacks)
		{
			if (item.first.lock() == widget.lock()) { popup = item.first; }
		}
		popupEscCallbacks.erase(popup);
		// Clear the tag from the tags list
		popupTags.erase(popupTags.begin() + matchIndex);
		popupWidgets.erase(popupWidgets.begin() + matchIndex);
		if (popupTags.size() == 0)
		{
			isPopupActive = false;
		}
		wasHandled = true;
	}

	if ((popupEscCallbacks.size() == 0 && popupTags.size() != 0) ||
		(popupEscCallbacks.size() != 0 && popupTags.size() == 0))
	{
		std::cout << "UI::ERROR::POPUP_ESC_CALLBACK::SYNC_ERROR" << std::endl;
	}
	return wasHandled;
}

bool UI::sendPopupEscape()
{
	if (popupEscCallbacks.size() == 0) { return false; }
	else
	{
		for (auto const& item : popupEscCallbacks)
		{
			if (!item.first.expired() && item.first.lock().get()->idAttrib == popupTags.front())
			{ 
				std::vector<std::shared_ptr<CObject>> args;
				item.second.get()->call(owner.get()->scriptConsole.get()->getInterpreter(), &args);
				return true;
			}
		}
		return false;
	}
}

bool UI::checkPopupBlocking()
{
	if (!isPopupActive) { return false; }
	for (auto& popup : popupEscCallbacks)
	{
		if (!popup.first.expired() && popup.first.lock().get()->popupIsBlocking == true) { return true; }
	}
	return false;
}

bool UI::checkBlockingPopupMatch(std::weak_ptr<Widget> widget)
{
	if (!isPopupActive || widget.expired()) { return false; }
	int firstBlockingPos = -1; int widgetQueryPos = -1;
	std::string id = widget.lock().get()->idAttrib;
	std::weak_ptr<Widget> weakRoot = widget.lock().get()->getRoot();
	if (weakRoot.expired()) { return false; }
	std::shared_ptr<Widget> strongRoot = widget.lock();
	for (int i = 0; i < popupWidgets.size(); i++)
	{
		if (!popupWidgets[i].expired())
		{
			if (firstBlockingPos == -1 && popupWidgets[i].lock().get()->popupIsBlocking == true)
			{
				firstBlockingPos = i;
			}
			std::weak_ptr<Widget> popupWeak = popupWidgets[i].lock().get()->getRoot();
			if (!popupWeak.expired())
			{ 
				if (popupWeak.lock() == strongRoot) 
				{ 
					widgetQueryPos = i; 
				}
			}
			
		}
		if (firstBlockingPos == -1 && id == popupTags[i]) { widgetQueryPos = i; }
	}
	if (firstBlockingPos == -1) { return false; }
	if (widgetQueryPos != -1 && widgetQueryPos <= firstBlockingPos) { return false; }
	if (widgetQueryPos > firstBlockingPos || (firstBlockingPos != -1 && widgetQueryPos == -1)) { return true; }
	return false;
}

void UI::updateFocusWidget(std::weak_ptr<Widget> target)
{
	if (!target.expired())
	{
		if (!focusWidget.expired())
		{
			if (focusWidget.lock().get() != target.lock().get())
			{
				//std::cout << "UPDATE::FOCUS::WITH-BLUR" << std::endl;
				// Check whether should allow the focusWidget to reset
				if (!interruptBlur)
				{
					focusWidget.lock().get()->selfBlur();
					// On focus change, always fire onentry callback
					if (!activeInputWidget.expired() && 
						target.lock() != activeInputWidget.lock() && 
						focusWidget.lock() == activeInputWidget.lock())
					{
						putActiveInputWidget(activeInputWidget, true, false, UI_WEVENT_ENTRY);
					}
					// Check whether to allow focus change
					if (!interruptFocus)
					{
						focusWidget.reset();
						//std::cout << "UPDATE::FOCUS::CHANGED-TARGET::ID,CLASSES=" <<
						//	target.lock().get()->idAttrib + ", " << stringVecToString(target.lock().get()->classAttribs) << std::endl;
						focusWidget = target;
						moveRootToFront(focusWidget.lock().get()->getRoot());
					}
					else
					{
						interruptFocus = false;
					}
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
			if (!interruptFocus)
			{
				//std::cout << "UPDATE::FOCUS::NEW-TARGET::ID,CLASSES=" <<
				//	target.lock().get()->idAttrib + ", " << stringVecToString(target.lock().get()->classAttribs) << std::endl;
				//std::cout << "UPDATE::FOCUS::CHANGED-TARGET::ID,CLASSES=" <<
				//	target.lock().get()->idAttrib + ", " << stringVecToString(target.lock().get()->classAttribs) << std::endl;
				focusWidget = target;
				moveRootToFront(focusWidget.lock().get()->getRoot());
			}
			else
			{
				interruptFocus = false;
			}
		}
	}
}
void UI::clearFocusWidget()
{
	if (!focusWidget.expired())
	{
		//std::cout << "CLEAR::FOCUS::WITH-BLUR" << std::endl;
		focusWidget.lock().get()->selfBlur();
		if (!activeInputWidget.expired())
		{
			putActiveInputWidget(activeInputWidget, true, false, UI_WEVENT_ENTRY);
		}
	}
	//std::cout << "CLEAR::FOCUS" << std::endl;
	focusWidget.reset();
}

void UI::moveRootToFront(std::weak_ptr<Widget> root)
{
	if (root.expired() || rootWidgets.size() <= 1) { return; }
	else
	{
		size_t position = std::string::npos;
		for (int i = 0; i < rootWidgets.size(); i++)
		{
			if (rootWidgets[i] == root.lock()) { position = size_t(i); break; }
		}
		if (position != std::string::npos && position != rootWidgets.size() - 1)
		{
			std::swap(rootWidgets[rootWidgets.size() - 1], rootWidgets[position]);
		}
	}
}

void UI::checkFocusVisibility()
{
	if (focusWidget.expired()) { return; }
	else
	{
		if (!focusWidget.lock().get()->checkVisibility())
		{
			focusWidget.lock().get()->selfBlur();
			if (!activeInputWidget.expired() && activeInputWidget.lock() == focusWidget.lock())
			{
				putActiveInputWidget(activeInputWidget, true, false, UI_WEVENT_CANCEL);
			}
			focusWidget.reset();
		}
	}
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
	for (std::shared_ptr<Widget> root : rootWidgets)
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
	if (layoutStringTypeMap.count(typeName) == 1)
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
	for (std::shared_ptr<Widget> root : rootWidgets)
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
// Delete Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////
void UI::requestWidgetDeletion(std::shared_ptr<CInterpreter> interpreter, std::string lookup)
{
	std::weak_ptr<Widget> target = getWidgetByID(lookup);
	if (!target.expired())
	{
		deleteWidgets.push_back(target);
	}
}
void UI::clearDeletionRequests()
{
	if (deleteWidgets.size() == 0) { return; }
	for (std::weak_ptr<Widget> target : deleteWidgets)
	{
		if (!target.expired())
		{
			deleteWidget_byID(nullptr, target.lock()->idAttrib);
		}
		target.reset(); // Added**
	}
	deleteWidgets.clear();
	deleteWidgets.shrink_to_fit();
}

bool UI::deleteWidget_byID(std::shared_ptr<CInterpreter> interpreter, std::string lookup)
{
	std::weak_ptr<Widget> target = getWidgetByID(lookup);
	if (!target.expired())
	{
		std::weak_ptr<Widget> tarParent = target.lock()->parentWidget;
		bool isRoot = target.lock()->isRoot;
		bool result = target.lock()->deleteWidget();
		std::string tarID = target.lock()->idAttrib;
		if (isRoot)
		{
			std::vector<std::shared_ptr<Widget>>::iterator it = rootWidgets.begin();
			for (it; it != rootWidgets.end(); ++it)
			{
				if (*it == target.lock()) { break; }
			}
			rootWidgets.erase(it);
			rootWidgets.shrink_to_fit();
		}
		else if (!tarParent.expired())
		{
			std::vector<std::shared_ptr<Widget>>::iterator it = tarParent.lock()->childWidgets.begin();
			for (it; it != tarParent.lock()->childWidgets.end(); ++it)
			{
				if (*it == target.lock()) { break; }
			}
			tarParent.lock()->childWidgets.erase(it);
			tarParent.lock()->childWidgets.shrink_to_fit();
			if (tarID != "" && widgetIDTable.count(tarID) == 1)
			{
				widgetIDTable.erase(tarID);
			}
			std::weak_ptr<Widget> rebuildTarget = tarParent;
			int rebuildCount = 0;
			while (!rebuildTarget.expired() && !rebuildTarget.lock()->parentWidget.expired() && rebuildCount <= 2)
			{
				rebuildTarget = rebuildTarget.lock()->parentWidget; rebuildCount++;
			}
			
			requestWidgetHierarchyRebuild(rebuildTarget);
			return true;
		}
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Sorting Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

bool UI::sortTargetWidgetChildren(std::shared_ptr<CInterpreter> interpreter,
	std::string lookup, std::string sortValue, std::string macroName, bool setVisible)
{
	if (macroName == "effectsOrdering") { return sortChildren_effectsOrdering(interpreter, lookup, sortValue, setVisible); }
	return false;
}

bool UI::sortChildren_effectsOrdering(std::shared_ptr<CInterpreter> interpreter,
	std::string lookup, std::string sortValue, bool setVisible)
{
	std::weak_ptr<Widget> target = getWidgetByID(lookup);
	if (target.expired()) { return false; }
	if (owner.get()->toolbox.get()->checkActiveTool() && 
		!owner.get()->toolbox.get()->getActiveTool().get()->checkInterestMask(TSetType::effects)) { 
		return false; 
	}
	std::vector<std::string> listCurrent;
	for (std::shared_ptr<Widget> child : target.lock()->childWidgets)
	{
		if (sortValue == "id") { listCurrent.push_back(child.get()->idAttrib); }
		else if (sortValue == "name") { listCurrent.push_back(child.get()->nameAttrib); }
		else if (sortValue == "value") { listCurrent.push_back(child.get()->valueAttrib); }
		else { listCurrent.push_back("null"); }
		if (listCurrent.back() == "null" || listCurrent.back() == "") { return false; }
	}
	int childCount = (int)listCurrent.size();
	TSet_Effects* effects = owner.get()->toolbox.get()->getActiveTool().get()->getEffects();
	effects->updateEffectsOrdering(false);
	std::vector<int> effectsCurrent = effects->getOrdering_vec();
	int pos = 0;
	for (int order : effectsCurrent)
	{
		if (order == 0) { continue; }
		if (pos >= childCount) { break; }
		std::string targetChildID = sortEffectsOrdering_matchValue(
			&listCurrent, 
			effects->intToFX_string(effectsCurrent[pos]));
		if (targetChildID == "") { return false; }
		bool result = target.lock()->updateChildOrder(targetChildID, pos, false, true, setVisible);
		std::vector<std::string>::iterator strIterator = listCurrent.begin();
		for (strIterator; strIterator != listCurrent.end(); ++strIterator)
		{
			if (targetChildID == *strIterator) { break; }
		}
		listCurrent.erase(strIterator); listCurrent.shrink_to_fit(); pos++;
		if (!result) { return false; }
	}
	if (listCurrent.size() > 0)
	{
		for (std::string li : listCurrent)
		{
			std::weak_ptr<Widget> child = getWidgetByID(li);
			if (!child.expired())
			{
				child.lock()->setProperty(interpreter, "visibility", std::make_shared<CObject>(std::string("hidden")));
			}
		}
	}
	return true;
}

std::string UI::sortEffectsOrdering_matchValue(std::vector<std::string>* widgetValues, std::string fxSearchStr)
{
	std::string outID = "";
	std::string searchStr = fxSearchStr;
	if (searchStr.size() > 5) { searchStr = searchStr.substr(0, 5); }
	for (std::string value : *widgetValues)
	{
		std::string lower = value; stringToLower(lower);
		if (lower.find(searchStr) != std::string::npos) { outID = value; break; }
	}
	return outID;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Render Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

void UI::drawWidgets()
{
	if (rootWidgets.size() == 0) { return; }
	else
	{
		//for (std::shared_ptr<Widget> obj : rootWidgets)
		for (int i = 0; i < rootWidgets.size(); i++)
		{
			//obj.get()->draw(owner.get()->getCamera()->getShaderTransform());
			rootWidgets[i].get()->draw(owner.get()->getCamera()->getShaderTransform());
		}
		drawZStack();
	}
}

void UI::drawZStack()
{
	if (zIndexMap.size() == 0) { isZStackActive = false; return; }
	ShaderTransform* xform = owner.get()->getCamera()->getShaderTransform();
	bool didZIndexDraw = false;
	for (auto& zindex : zIndexMap)
	{
		bool result = zindex.second.drawStack(xform);
		zindex.second.clearStack();
		if (result) { didZIndexDraw = true; }
	}
	if (!didZIndexDraw)
	{
		zIndexMap.clear();
		isZStackActive = false;
	}
	else
	{
		isZStackActive = true;
	}
}