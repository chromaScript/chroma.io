#include "../include/tool/Toolbox.h"
#include "../include/CustomCursor.h"
#include "../include/methods/IOClasses.h"
#include "../include/input/keys.h"

#include "../include/entities/UserInterface.h"
#include "../include/cscript/CEnums.h"
#include "../include/cscript/CCallable.h"
#include "../include/cscript/CInterpreter.h"
#include "../include/cscript/CObject.h"
#include "../include/methods/MethodType.h"

#include "../include/ctoolfile/ChromaToolFile.h"

#ifndef APPLICATION_H
#include "include/Application.h"
#endif

#include <iostream>
#include <map>
#include <iostream>
#include <memory>

extern std::shared_ptr<Application> chromaIO;

Toolbox::Toolbox()
{
	pointer = std::make_shared<CustomCursor>("./assets/textures/T_cursorPointer16_D.png", CursorType::pointer, CursorJustification::upperLeftCorner, "Pointer");
	hand = std::make_shared<CustomCursor>("./assets/textures/T_cursorHand16_D.png", CursorType::hand, CursorJustification::upperCenter, "Hand");
	grab = std::make_shared<CustomCursor>("./assets/textures/T_cursorGrab16_D.png", CursorType::grab, CursorJustification::upperCenter, "Grab");
	rotate = std::make_shared<CustomCursor>("./assets/textures/T_cursorRotate16_D.png", CursorType::rotate, CursorJustification::upperLeftCorner, "Rotate");
	zoomScrub = std::make_shared<CustomCursor>("./assets/textures/T_cursorZoomScrub16_D.png", CursorType::zoomscrub, CursorJustification::upperLeftCorner, "Zoom");
	zoomIn = std::make_shared<CustomCursor>("./assets/textures/T_cursorZoomIn16_D.png", CursorType::zoomIn, CursorJustification::upperLeftCorner, "Zoom In");
	zoomOut = std::make_shared<CustomCursor>("./assets/textures/T_cursorZoomOut16_D.png", CursorType::zoomIn, CursorJustification::upperLeftCorner, "Zoom Out");
	brush = std::make_shared<CustomCursor>("./assets/textures/T_cursorBrush16_D.png", CursorType::brush, CursorJustification::upperLeftCorner, "Brush");
	dropper = std::make_shared<CustomCursor>("./assets/textures/T_cursorDropper16_D.png", CursorType::dropper, CursorJustification::lowerLeftCorner, "Dropper");
	circleSmall = std::make_shared<CustomCursor>("./assets/textures/T_cursorCircleSmall16_D.png", CursorType::circleSmall, CursorJustification::center, "Circle Small");
}

// Cursor Functions
std::shared_ptr<CustomCursor> Toolbox::getCursor(CursorType name)
{
	switch (name)
	{
	case CursorType::pointer:
		return pointer;
	case CursorType::hand:
		return hand;
	case CursorType::grab:
		return grab;
	case CursorType::rotate:
		return rotate;
	case CursorType::zoomscrub:
		return zoomScrub;
	case CursorType::zoomIn:
		return zoomIn;
	case CursorType::zoomOut:
		return zoomOut;
	case CursorType::brush:
		return brush;
	case CursorType::dropper:
		return dropper;
	case CursorType::circleSmall:
		return circleSmall;
	default:
		return pointer;
	}
}

void Toolbox::initializeTools(bool checkExisting, bool createNew)
{
	if (checkExisting)
	{
		bool result = chromaIO.get()->toolFileConsole.get()->loadFiles_default(chromaIO.get()->toolbox);
		//bool result = true;
		// Result will return true if the application doesn't find any tool files to load. In this scenario
		// it will currently load the default tools instead.

		if (result == true)
		{
			createDefaultTools();
		}
		return;
	}
	else if (checkExisting == false && createNew == true)
	{
		// Load default tools
		createDefaultTools();
		return;
	}
	else
	{
		// Read tool presets from config file
		createDefaultTools();
		return;
	}
}
bool Toolbox::exportTools(bool exportAll, std::vector<int> selectedToolIDs, bool overwrite, bool useDefaultRoot, std::filesystem::path rootDir)
{
	bool result = false;
	if (exportAll)
	{
		result = chromaIO.get()->toolFileConsole.get()->writeFiles_defualt(tools, overwrite, rootDir, useDefaultRoot);
	}
	else
	{
		// Export selected tools
	}
	return result;
}

bool Toolbox::createCustomTool(
	std::string cursorUp, std::string cursorDown,
	int toolID, std::string toolName,
	std::string inputMethod, std::string controlScheme, std::string outputMethod,
	Keybind keybind)
{
	CursorType cursorUpEnum = convertCursorName(stringToLower(cursorUp));
	CursorType cursorDownEnum = convertCursorName(stringToLower(cursorDown));
	MethodType inputID = convertIOStringToMacro(1, inputMethod);
	MethodType controlID = convertIOStringToMacro(3, controlScheme);
	TSetType controlEnum = convertControlMacro(controlID);
	MethodType outputID = convertIOStringToMacro(2, outputMethod);

	glm::bvec3 result = glm::bvec3(true, true, true);
	
	if (!chromaIO.get()->isValidKeybind_tool(keybind))
	{ 
		result.x = false; 
	}
	if (!checkValidIOCombination(nullptr, inputID, outputID)) 
	{ 
		result.y = false; 
	}
	if (!checkValidControlScheme(nullptr, inputID, controlID)) 
	{ 
		result.z = false; 
	}
	if (result.x == false || result.y == false || result.z == false)
	{
		return false;
	}

	tools.emplace_back(
		std::make_shared<Tool>(
			getCursor(cursorUpEnum), getCursor(cursorDownEnum), toolID, toolName,
			inputID, outputID,
			controlEnum,
			keybind, shared_from_this()));
	tools.back().get()->initializeTool(controlEnum);
	
	return true;
}

void Toolbox::createDefaultTools()
{
	std::vector<std::string> tags = {""};
	// Later this will be read using a config file, likely XML or some human-readable format for easy editing
	tools.emplace_back(
		std::make_shared<Tool>(pointer, pointer, 0, "Pointer",
			MethodType::in_noInput, MethodType::out_noOutput,
			TSetType::usedefault,
			Keybind(InputKey::f1, InputModKey::none), shared_from_this(),
			tags, "Default Tools", "Default", "08.15.21", "chromaTinker"));
	tools.back().get()->initializeTool(TSetType::usedefault);
	//
	tools.emplace_back(
		std::make_shared<Tool>(hand, grab, 2, "Pan Camera", 
			MethodType::in_pan, MethodType::out_pan,
			TSetType::pan,
			Keybind(InputKey::h, InputModKey::none), shared_from_this()));
	tools.back().get()->initializeTool(TSetType::pan);
	//
	tools.emplace_back(
		std::make_shared<Tool>(rotate, rotate, 3, "Rotate Camera", 
			MethodType::in_rotate, MethodType::out_rotate,
			TSetType::rotate,
			Keybind(InputKey::r, InputModKey::none), shared_from_this(),
			tags, "Default Tools", "Default", "08.15.21", "chromaTinker"));
	tools.back().get()->initializeTool(TSetType::rotate);
	//
	tools.emplace_back(
		std::make_shared<Tool>(zoomIn, zoomOut, 4, "Zoom Camera", 
			MethodType::in_zoom, MethodType::out_zoom,
			TSetType::zoom,
			Keybind(InputKey::z, InputModKey::none), shared_from_this()));
	tools.back().get()->initializeTool(TSetType::zoom);
	//
	tools.emplace_back(
		std::make_shared<Tool>(brush, circleSmall, 12, "Brush",
			MethodType::in_draw, MethodType::out_stroke,
			TSetType::continuous,
			Keybind(InputKey::b, InputModKey::none), shared_from_this(),
			tags, "Default Tools", "Default", "08.15.21", "chromaTinker"));
	tools.back().get()->initializeTool(TSetType::continuous);
	/*//
	tools.emplace_back(
		std::make_shared<Tool>(brush, circleSmall, DT_STROKE_LASSO, "Lasso",
			MethodType::in_lasso, MethodType::out_stroke,
			TSetType::continuous,
			INPUT_KEY_L, shared_from_this(),
			tags, "Default Tools", "Default", "08.15.21", "chromaTinker"));
	tools.back().get()->initializeTool(TSetType::continuous);
	*///
	tools.emplace_back(
		std::make_shared<Tool>(brush, circleSmall, 28, "Rectangle Brush",
			MethodType::in_polygon, MethodType::out_stroke,
			TSetType::onepoint,
			Keybind(InputKey::w, InputModKey::none), shared_from_this(),
			tags, "Default Tools", "Default", "08.15.21", "chromaTinker"));
	tools.back().get()->initializeTool(TSetType::onepoint);
	//
	tools.emplace_back(
		std::make_shared<Tool>(hand, circleSmall, 43, "Shape-Line Brush",
			MethodType::in_shapeLine, MethodType::out_stroke,
			TSetType::drag,
			Keybind(InputKey::t, InputModKey::none), shared_from_this(),
			tags, "Default Tools", "Default", "08.15.21", "chromaTinker"));
	tools.back().get()->initializeTool(TSetType::drag);
	//
	tools.emplace_back(
		std::make_shared<Tool>(hand, circleSmall, 44, "Shape-Draw Brush",
			MethodType::in_shapeDraw, MethodType::out_stroke,
			TSetType::continuous,
			Keybind(InputKey::k, InputModKey::none), shared_from_this(),
			tags, "Default Tools", "Default", "08.15.21", "chromaTinker"));
	tools.back().get()->initializeTool(TSetType::continuous);
	//
	tools.emplace_back(
		std::make_shared<Tool>(hand, circleSmall, 54, "Drag-Shape Field Brush",
			MethodType::in_shapeField, MethodType::out_stroke,
			TSetType::drag,
			Keybind(InputKey::q, InputModKey::none), shared_from_this(),
			tags, "Default Tools", "Default", "08.15.21", "chromaTinker"));
	tools.back().get()->initializeTool(TSetType::drag);
	//
	tools.emplace_back(
		std::make_shared<Tool>(hand, circleSmall, 55, "Box-Shape Field Brush",
			MethodType::in_shapeField, MethodType::out_stroke,
			TSetType::onepoint,
			Keybind(InputKey::s, InputModKey::none), shared_from_this(),
			tags, "Default Tools", "Default", "08.15.21", "chromaTinker"));
	tools.back().get()->initializeTool(TSetType::onepoint);
	//
	tools.emplace_back(
		std::make_shared<Tool>(brush, circleSmall, 56, "Vortex Brush",
			MethodType::in_vortex, MethodType::out_stroke,
			TSetType::continuous,
			Keybind(InputKey::e, InputModKey::none), shared_from_this(),
			tags, "Default Tools", "Default", "08.15.21", "chromaTinker"));
	tools.back().get()->initializeTool(TSetType::continuous);
	//
	tools.emplace_back(
		std::make_shared<Tool>(brush, circleSmall, 57, "Radial Field Brush",
			MethodType::in_vortex, MethodType::out_stroke,
			TSetType::drag,
			Keybind(InputKey::o, InputModKey::none), shared_from_this(),
			tags, "Default Tools", "Default", "08.15.21", "chromaTinker"));
	tools.back().get()->initializeTool(TSetType::drag);
	//
	tools.emplace_back(
		std::make_shared<Tool>(brush, circleSmall, 58, "Rake Brush",
			MethodType::in_rake, MethodType::out_stroke,
			TSetType::continuous,
			Keybind(InputKey::a, InputModKey::none), shared_from_this(),
			tags, "Default Tools", "Default", "08.15.21", "chromaTinker"));
	tools.back().get()->initializeTool(TSetType::continuous);
	//
	tools.emplace_back(
		std::make_shared<Tool>(brush, circleSmall, 64, "Fan Brush",
			MethodType::in_fan, MethodType::out_stroke,
			TSetType::continuous,
			Keybind(InputKey::f, InputModKey::none), shared_from_this(),
			tags, "Default Tools", "Default", "08.15.21", "chromaTinker"));
	tools.back().get()->initializeTool(TSetType::continuous);
	tools.back().get()->getContinuousControl()->anchorSpacing = 12.5f;
	//
	tools.emplace_back(
		std::make_shared<Tool>(brush, circleSmall, 65, "Fan Airbrush",
			MethodType::in_fan, MethodType::out_stroke,
			TSetType::continuous,
			Keybind(InputKey::f, InputModKey::shift), shared_from_this(),
			tags, "Default Tools", "Default", "08.15.21", "chromaTinker"));
	tools.back().get()->initializeTool(TSetType::continuous);
	//
	tools.emplace_back(
		std::make_shared<Tool>(dropper, dropper, 73, "Color Picker",
			MethodType::in_sampler, MethodType::out_sampler,
			TSetType::sampler,
			Keybind(InputKey::i, InputModKey::none), shared_from_this(),
			tags, "Default Tools", "Default", "08.15.21", "chromaTinker"));
	tools.back().get()->initializeTool(TSetType::sampler);
	//
	tools.emplace_back(
		std::make_shared<Tool>(brush, circleSmall, 74, "Spline Brush",
			MethodType::in_spline, MethodType::out_stroke,
			TSetType::continuous,
			Keybind(InputKey::y, InputModKey::none), shared_from_this(),
			tags, "Default Tools", "Default", "08.15.21", "chromaTinker"));
	tools.back().get()->initializeTool(TSetType::continuous);
	//
	/* // Tool is broken for now
	tools.emplace_back(
		std::make_shared<Tool>(hand, hand, DT_STROKE_POLYLINE, "Poly Line",
			MethodType::in_polyLine, MethodType::out_stroke,
			TSetType::drag,
			INPUT_KEY_M, shared_from_this()));
	tools.back().get()->initializeTool(TSetType::drag);
	//*/
	setActiveTool_byID(0);
	std::cout << "TOOLBOX::CREATEDEFAULTTOOLS::ARRAY-SIZE::" << tools.size() << std::endl;
}
int Toolbox::checkToolHotkeys(Keybind keybind)
{
	// Note: Assumes single use hotkeys for tools (Later can allow for cycling between tools with same key)
	// Note: Returns the index from tools array, not the tool ID number, must use setActiveTool_byIndex following this.
	int len = static_cast<int>(tools.size());
	for (int i = 0; i < len; i++)
	{
		if (compareKeybind(tools[i].get()->keybind, keybind, false))
		{
			std::cout << "TOOLBOX::CHECKHOTKEYS::KEYMATCH=" << keybindToString(keybind) << "::TOOLID::=" << tools[i]->id << std::endl;
			return i;
		}
	}
	return -1;
}
void Toolbox::setActiveTool_byID(int id)
{
	int len = static_cast<int>(tools.size());
	for (int i = 0; i < len; i++)
	{
		if (tools[i].get()->id == id)
		{
			activeTool = tools[i];
			toolSwitchCallback();
			std::cout << "TOOLBOX::SETACTIVETOOL::NAME=" << activeTool.get()->name << std::endl;
			break;
		}
	}
	if (activeTool == nullptr && tools.size() != 0) { setActiveTool_byID(tools.front().get()->id); }
}
void Toolbox::setActiveTool_byIndex(int i)
{
	activeTool = tools[i].get()->activate(chromaIO.get()->ui.get()->getColors(), chromaIO.get()->ui);
	toolSwitchCallback();
	std::cout << "TOOLBOX::SETACTIVETOOL::NAME=" << activeTool.get()->name << std::endl;
}
void Toolbox::toolSwitchCallback()
{
	if (toolSwitchCallbacks.size() != 0)
	{
		std::vector<std::shared_ptr<CObject>> args = { };
		for (auto const& item : toolSwitchCallbacks)
		{
			item.second.get()->call(chromaIO.get()->scriptConsole.get()->getInterpreter(), &args);
			chromaIO.get()->scriptConsole.get()->cleanEngine();
		}
	}
}
void Toolbox::notifyColorChange(CColor input, bool isBG)
{
	if (activeTool == nullptr) { return; }
	if (!isBG)
	{
		activeTool.get()->setFGColor(input, false);
	}
	else
	{
		activeTool.get()->setBGColor(input, false);
	}
}
void Toolbox::notifyCursorChange(bool isDown, bool updateSavedCursor)
{
	if (isDown) 
	{ 
		chromaIO.get()->ui.get()->updateCursorImage(activeTool.get()->cursorDown);
		if (updateSavedCursor) { chromaIO.get()->ui.get()->savedCursor = activeTool.get()->cursorDown; }
	}
	else 
	{ 
		chromaIO.get()->ui.get()->updateCursorImage(activeTool.get()->cursorUp); 
		if (updateSavedCursor) { chromaIO.get()->ui.get()->savedCursor = activeTool.get()->cursorUp; }
	}
}

std::shared_ptr<Tool> Toolbox::getActiveTool() { return activeTool; }
bool Toolbox::checkActiveTool() { if (activeTool == nullptr) { return false; } return true; }
std::shared_ptr<Tool> Toolbox::getTool_lastCreated()
{
	return tools.back();
}

// Callback Functions
bool Toolbox::bindCallback(std::shared_ptr<CInterpreter> interpreter,
	CCallbackType callType, std::string callerID,
	std::shared_ptr<CFunction> callFunc)
{
	// First do type checking (Some callbacks share signatures, so save on space)
	bool validSignature = true;
	std::vector<std::shared_ptr<CToken>> paramTypes;
	switch (callType)
	{
	case CCallbackType::toolSwitch:
		paramTypes = callFunc.get()->funcDeclaration.get()->paramsTypes;
		if (paramTypes.size() != 0) { validSignature = false; }
		break;
	case CCallbackType::colorpickStart:
	case CCallbackType::colorpickMove:
	case CCallbackType::colorpickEnd:
		// scriptSignature - void function(num xpos, num ypos)
		paramTypes = callFunc.get()->funcDeclaration.get()->paramsTypes;
		if (paramTypes.size() != 2) { validSignature = false; }
		if (callFunc.get()->funcDeclaration.get()->returnType.get()->type != CTokenType::_VOID) { validSignature = false; }
		if (paramTypes[0].get()->type != CTokenType::NUM) { validSignature = false; }
		if (paramTypes[1].get()->type != CTokenType::NUM) { validSignature = false; }
		break;
	}
	if (!validSignature)
	{
		interpreter.get()->console.get()->error("[toolbox:0101] Callback function supplied for '" + CCallbackNames[size_t(callType)] +
			"' does not match the required signature. Check the documentation for callback signatures.");
		return false;
	}
	// Then if there were no errors, bind the callback
	switch (callType)
	{
	case CCallbackType::toolSwitch:
		toolSwitchCallbacks.insert(std::pair<std::string, std::shared_ptr<CFunction>>(callerID, callFunc));
		break;
	case CCallbackType::colorpickStart:
		colorpickStartCallbacks.insert(std::pair<std::string, std::shared_ptr<CFunction>>(callerID, callFunc));
		break;
	case CCallbackType::colorpickMove:
		colorpickMoveCallbacks.insert(std::pair<std::string, std::shared_ptr<CFunction>>(callerID, callFunc));
		break;
	case CCallbackType::colorpickEnd:
		colorpickEndCallbacks.insert(std::pair<std::string, std::shared_ptr<CFunction>>(callerID, callFunc));
		break;
	}
	return true;
}

// Switch on output type in order to permit custom tools to also send callback notifications
void Toolbox::startCallback(std::shared_ptr<CInterpreter> interpreter, double xpos, double ypos)
{
	MethodType id = activeTool.get()->output.get()->getID();
	std::shared_ptr<CObject> xArg = std::make_shared<CObject>(xpos);
	std::shared_ptr<CObject> yArg = std::make_shared<CObject>(ypos);
	std::vector<std::shared_ptr<CObject>> args = { xArg, yArg };
	if (id == MethodType::out_sampler)
	{
		for (auto const& item : colorpickStartCallbacks)
		{
			item.second.get()->call(interpreter, &args);
		}
		return;
	}
}
void Toolbox::moveCallback(std::shared_ptr<CInterpreter> interpreter, double xpos, double ypos)
{
	MethodType id = activeTool.get()->output.get()->getID();
	std::shared_ptr<CObject> xArg = std::make_shared<CObject>(xpos);
	std::shared_ptr<CObject> yArg = std::make_shared<CObject>(ypos);
	std::vector<std::shared_ptr<CObject>> args = { xArg, yArg };
	if (id == MethodType::out_sampler)
	{
		for (auto const& item : colorpickMoveCallbacks)
		{
			item.second.get()->call(interpreter, &args);
		}
		return;
	}
}
void Toolbox::endCallback(std::shared_ptr<CInterpreter> interpreter, double xpos, double ypos)
{
	MethodType id = activeTool.get()->output.get()->getID();
	std::shared_ptr<CObject> xArg = std::make_shared<CObject>(xpos);
	std::shared_ptr<CObject> yArg = std::make_shared<CObject>(ypos);
	std::vector<std::shared_ptr<CObject>> args = { xArg, yArg };
	if (id == MethodType::out_sampler)
	{
		for (auto const& item : colorpickEndCallbacks)
		{
			item.second.get()->call(interpreter, &args);
		}
		return;
	}
}

// Input Functions
InputHandlerFlag Toolbox::sendClick(Application* sender, Input dat)
{
	return activeTool.get()->input.get()->click(sender, dat);
}
InputHandlerFlag Toolbox::sendMove(Application* sender, Input dat)
{
	return activeTool.get()->input.get()->move(sender, dat);
}
InputHandlerFlag Toolbox::sendKey(Application* sender, Input dat, Keybind key, InputAction action, InputModKey modKeys)
{
	return activeTool.get()->input.get()->key(sender, dat, key, action, modKeys);
}
void Toolbox::sendPreview(Application* sender, VertexData* vertexData, InputHandlerFlag action)
{
	activeTool.get()->output.get()->preview(sender, vertexData, action);
}
void Toolbox::sendFinialize(Application* sender)
{
	activeTool.get()->output.get()->finalize(sender, &activeTool.get()->input.get()->fragData);
	activeTool.get()->output.get()->postprocess(sender, &activeTool.get()->input.get()->fragData);
}

// Validation Functions
bool Toolbox::checkValidIOCombination(std::shared_ptr<CInterpreter> interpreter, std::string inputName, std::string outputName)
{
	MethodType input = convertIOStringToMacro(1, inputName);
	MethodType output = convertIOStringToMacro(2, outputName);
	return checkValidIOCombination(interpreter, input, output);
}

bool Toolbox::checkValidIOCombination(std::shared_ptr<CInterpreter> interpreter, MethodType inputMacro, MethodType outputMacro)
{
	if (inputMacro == MethodType::null || outputMacro == MethodType::null) { return false; }
	if (inputMacro == MethodType::in_noInput || outputMacro == MethodType::out_noOutput) { return true; }

	switch (inputMacro)
	{
	case MethodType::in_rotate: if (outputMacro == MethodType::out_rotate) { return true; } break;
	case MethodType::in_zoom: if (outputMacro == MethodType::out_zoom) { return true; } break;
	case MethodType::in_draw: if (outputMacro == MethodType::out_stroke) { return true; } break;
	case MethodType::in_line: if (outputMacro == MethodType::out_stroke) { return true; } break;
	case MethodType::in_lasso: if (outputMacro == MethodType::out_stroke) { return true; } break;
	case MethodType::in_polygon: if (outputMacro == MethodType::out_stroke) { return true; } break;
	case MethodType::in_pan: if (outputMacro == MethodType::out_pan) { return true; } break;
	case MethodType::in_point: if (outputMacro == MethodType::out_stroke) { return true; } break;
	case MethodType::in_sampler: if (outputMacro == MethodType::out_sampler) { return true; } break;
	case MethodType::in_shapeField: if (outputMacro == MethodType::out_stroke) { return true; } break;
	case MethodType::in_spline: if (outputMacro == MethodType::out_stroke) { return true; } break;
	case MethodType::in_vortex: if (outputMacro == MethodType::out_stroke) { return true; } break;
	case MethodType::in_rake: if (outputMacro == MethodType::out_stroke) { return true; } break;
	case MethodType::in_fan: if (outputMacro == MethodType::out_stroke) { return true; } break;
	case MethodType::in_shapeLine: if (outputMacro == MethodType::out_stroke) { return true; } break;
	case MethodType::in_shapeDraw: if (outputMacro == MethodType::out_stroke) { return true; } break;
	case MethodType::in_polyLine: if (outputMacro == MethodType::out_stroke) { return true; } break;
	}

	return false;
}

bool Toolbox::checkValidControlScheme(std::shared_ptr<CInterpreter> interpreter, std::string inputName, std::string controlScheme)
{
	MethodType input = convertIOStringToMacro(1, inputName);
	MethodType control = convertIOStringToMacro(3, controlScheme);
	return checkValidControlScheme(interpreter, input, control);
}

bool Toolbox::checkValidControlScheme(std::shared_ptr<CInterpreter> interpreter, MethodType inputMacro, MethodType controlMacro)
{
	if (inputMacro == MethodType::null || controlMacro == MethodType::null) { return false; }
	if (inputMacro == MethodType::in_noInput) { return true; }

	switch (inputMacro)
	{
	case MethodType::in_rotate: if (controlMacro == MethodType::control_rotate) { return true; } break;
	case MethodType::in_zoom: if (controlMacro == MethodType::control_zoom) { return true; } break;
	case MethodType::in_pan: if (controlMacro == MethodType::control_pan) { return true; } break;
	case MethodType::in_sampler: if (controlMacro == MethodType::control_sampler) { return true; } break;
		//
	case MethodType::in_draw: if (controlMacro == MethodType::control_continuous) { return true; } break;
	case MethodType::in_line: if (controlMacro == MethodType::control_drag) { return true; } break;
	case MethodType::in_lasso: if (controlMacro == MethodType::control_continuous) { return true; } break;
	case MethodType::in_polygon: if (controlMacro == MethodType::control_onepoint) { return true; } break;
	case MethodType::in_point: if (controlMacro == MethodType::control_continuous) { return true; } break;
	case MethodType::in_shapeField: if (controlMacro == MethodType::control_onepoint || controlMacro == MethodType::control_drag) { return true; } break;
	case MethodType::in_vortex: if (controlMacro == MethodType::control_continuous || controlMacro == MethodType::control_drag) { return true; } break;
	case MethodType::in_rake: if (controlMacro == MethodType::control_continuous) { return true; } break;
	case MethodType::in_fan: if (controlMacro == MethodType::control_continuous) { return true; } break;
	case MethodType::in_shapeLine: if (controlMacro == MethodType::control_onepoint || controlMacro == MethodType::control_drag) { return true; } break;
	case MethodType::in_shapeDraw: if (controlMacro == MethodType::control_continuous) { return true; } break;
	case MethodType::in_spline: if (controlMacro == MethodType::control_continuous) { return true; } break;
	case MethodType::in_polyLine: if (controlMacro == MethodType::control_drag) { return true; } break;
	}

	return false;
}

MethodType Toolbox::convertIOStringToMacro(int kind, std::string name)
{
	std::string nameFixed = name;
	nameFixed.erase(remove_if(nameFixed.begin(), nameFixed.end(), isspace), nameFixed.end());
	stringToLower(nameFixed);
	if (kind == 1) // INPUT METHOD
	{
		if (nameFixed == "noinputMacro" || nameFixed == "noinput") { return MethodType::in_noInput; }
		if (nameFixed == "rotate" || nameFixed == "inrotate" || nameFixed == "rotatecamera" || nameFixed == "camerarotate") { return MethodType::in_rotate; }
		if (nameFixed == "zoom" || nameFixed == "inzoom" || nameFixed == "zoomcamera" || nameFixed == "camerazoom") { return MethodType::in_zoom; }
		if (nameFixed == "draw") { return MethodType::in_draw; }
		if (nameFixed == "line") { return MethodType::in_line; }
		if (nameFixed == "lasso") { return MethodType::in_lasso; }
		if (nameFixed == "polygon") { return MethodType::in_polygon; }
		if (nameFixed == "pan" || nameFixed == "inpan" || nameFixed == "pancamera" || nameFixed == "camerapan") { return MethodType::in_pan; }
		if (nameFixed == "point") { return MethodType::in_point; }
		if (nameFixed == "sampler" || nameFixed == "eyedropper" || nameFixed == "colorpicker") { return MethodType::in_sampler; }
		if (nameFixed == "shapefield") { return MethodType::in_shapeField; }
		if (nameFixed == "vortex") { return MethodType::in_vortex; }
		if (nameFixed == "rake") { return MethodType::in_rake; }
		if (nameFixed == "fan") { return MethodType::in_fan; }
		if (nameFixed == "shapeline") { return MethodType::in_shapeLine; }
		if (nameFixed == "shapedraw") { return MethodType::in_shapeDraw; }
		if (nameFixed == "spline") { return MethodType::in_spline; }
		if (nameFixed == "polyline") { return MethodType::in_polyLine; }
	}
	else if (kind == 2) // OUTPUT METHOD
	{
		if (nameFixed == "nooutputMacro" || nameFixed == "nooutput") { return MethodType::out_noOutput; }
		if (nameFixed == "pan" || nameFixed == "outpan" || nameFixed == "pancamera" || nameFixed == "camerapan") { return MethodType::out_pan; }
		if (nameFixed == "rotate" || nameFixed == "outrotate" || nameFixed == "rotatecamera" || nameFixed == "camerarotate") { return MethodType::out_rotate; }
		if (nameFixed == "zoom" || nameFixed == "outzoom" || nameFixed == "zoomcamera" || nameFixed == "camerazoom") { return MethodType::out_zoom; }
		if (nameFixed == "stroke" || nameFixed == "brush") { return MethodType::out_stroke; }
		if (nameFixed == "shape") { return MethodType::out_shape; }
		if (nameFixed == "gradient") { return MethodType::out_gradient; }
		if (nameFixed == "fill") { return MethodType::out_fill; }
		if (nameFixed == "sampler" || nameFixed == "eyedropper" || nameFixed == "colorpicker" || nameFixed == "colorsample") { return MethodType::out_sampler; }
		if (nameFixed == "selection") { return MethodType::out_selection; }
	}
	else if (kind == 3) // CONTROL SCHEME
	{
		if (nameFixed == "default" || nameFixed == "usedefault" || nameFixed == "null") { return MethodType::control_default; }
		if (nameFixed == "continuous") { return MethodType::control_continuous; }
		if (nameFixed == "drag") { return MethodType::control_drag; }
		if (nameFixed == "onepoint") { return MethodType::control_onepoint; }
		if (nameFixed == "twopoint") { return MethodType::control_twopoint; }
		if (nameFixed == "threepoint") { return MethodType::control_threepoint; }
		if (nameFixed == "fourpoint") { return MethodType::control_fourpoint; }
		if (nameFixed == "sampler" || nameFixed == "eyedropper" || nameFixed == "colorpicker" || nameFixed == "colorsample") { return MethodType::control_sampler; }
		if (nameFixed == "pan" || nameFixed == "outpan" || nameFixed == "pancamera" || nameFixed == "camerapan") { return MethodType::control_pan; }
		if (nameFixed == "rotate" || nameFixed == "outrotate" || nameFixed == "rotatecamera" || nameFixed == "camerarotate") { return MethodType::control_rotate; }
		if (nameFixed == "zoom" || nameFixed == "outzoom" || nameFixed == "zoomcamera" || nameFixed == "camerazoom") { return MethodType::control_zoom; }
	}
	else { return MethodType::null; }
	return MethodType::null;
}

TSetType Toolbox::convertControlMacro(MethodType macro)
{
	switch (macro)
	{
	case MethodType::control_default: return TSetType::usedefault;
	case MethodType::control_continuous: return TSetType::continuous;
	case MethodType::control_drag: return TSetType::drag;
	case MethodType::control_onepoint: return TSetType::onepoint;
	case MethodType::control_twopoint: return TSetType::twopoint;
	case MethodType::control_threepoint: return TSetType::threepoint;
	case MethodType::control_fourpoint: return TSetType::fourpoint;
	case MethodType::control_sampler: return TSetType::sampler;
	case MethodType::control_pan: return TSetType::pan;
	case MethodType::control_rotate: return TSetType::rotate;
	case MethodType::control_zoom: return TSetType::zoom;
	default:
		return TSetType::usedefault;
	}
}