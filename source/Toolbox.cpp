#include "include/Toolbox.h"
#include "include/CustomCursor.h"
#include "include/IOClasses.h"
#include "include/keys.h"

#include "include/structs.h"
#include "include/entities/UserInterface.h"
#include "include/cscript/CEnums.h"
#include "include/cscript/CCallable.h"
#include "include/cscript/CInterpreter.h"
#include "include/cscript/CObject.h"

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
	pointer = std::make_shared<CustomCursor>("./assets/textures/T_cursorPointer16_D.png", CURSOR_POINTER, CURSOR_ULCORNER);
	hand = std::make_shared<CustomCursor>("./assets/textures/T_cursorHand16_D.png", CURSOR_HAND, CURSOR_UCENTER);
	grab = std::make_shared<CustomCursor>("./assets/textures/T_cursorGrab16_D.png", CURSOR_GRAB, CURSOR_UCENTER);
	rotate = std::make_shared<CustomCursor>("./assets/textures/T_cursorRotate16_D.png", CURSOR_ROTATE, CURSOR_ULCORNER);
	zoomScrub = std::make_shared<CustomCursor>("./assets/textures/T_cursorZoomScrub16_D.png", CURSOR_ZOOMSCRUB, CURSOR_ULCORNER);
	zoomIn = std::make_shared<CustomCursor>("./assets/textures/T_cursorZoomIn16_D.png", CURSOR_ZOOMIN, CURSOR_ULCORNER);
	zoomOut = std::make_shared<CustomCursor>("./assets/textures/T_cursorZoomOut16_D.png", CURSOR_ZOOMOUT, CURSOR_ULCORNER);
	brush = std::make_shared<CustomCursor>("./assets/textures/T_cursorBrush16_D.png", CURSOR_BRUSH, CURSOR_ULCORNER);
	dropper = std::make_shared<CustomCursor>("./assets/textures/T_cursorDropper16_D.png", CURSOR_DROPPER, CURSOR_LLCORNER);
	circleSmall = std::make_shared<CustomCursor>("./assets/textures/T_cursorCircleSmall16_D.png", CURSOR_CIRCLESMALL, CURSOR_CENTER);
}

// Cursor Functions
std::shared_ptr<CustomCursor> Toolbox::getCursor(int name)
{
	switch (name)
	{
	case CURSOR_POINTER:
		return pointer;
	case CURSOR_HAND:
		return hand;
	case CURSOR_GRAB:
		return grab;
	case CURSOR_ROTATE:
		return rotate;
	case CURSOR_ZOOMSCRUB:
		return zoomScrub;
	case CURSOR_ZOOMIN:
		return zoomIn;
	case CURSOR_ZOOMOUT:
		return zoomOut;
	case CURSOR_BRUSH:
		return brush;
	case CURSOR_DROPPER:
		return dropper;
	case CURSOR_CIRCLESMALL:
		return circleSmall;
	default:
		return pointer;
	}
}

void Toolbox::initializeTools(bool isNew)
{
	if (isNew == true)
	{
		// Load default tools
		createDefaultTools();
	}
	else
	{
		// Read tool presets from config file
	}
}

bool Toolbox::createCustomTool(
	std::string cursorUp, std::string cursorDown,
	int toolID, std::string toolName,
	std::string inputMethod, std::string controlScheme, std::string outputMethod,
	int modBit, int glfwKey)
{
	int cursorUpEnum = convertCursorName(stringToLower(cursorUp));
	int cursorDownEnum = convertCursorName(stringToLower(cursorDown));
	int inputID = convertIOStringToMacro(1, inputMethod);
	int controlID = convertIOStringToMacro(3, controlScheme);
	TSetType controlEnum = convertControlMacro(controlID);
	int outputID = convertIOStringToMacro(2, outputMethod);
	int keybind = createKeySig(modBit, glfwKey);

	if (!chromaIO.get()->isValidKeybind_tool(modBit, glfwKey)) { return false; }
	if (!checkValidIOCombination(nullptr, inputID, outputID)) { return false; }
	if (!checkValidControlScheme(nullptr, inputID, controlID)) { return false; }

	tools.emplace_back(
		std::make_shared<Tool>(getCursor(cursorUpEnum), getCursor(cursorDownEnum), toolID, toolName,
			inputID, outputID,
			controlEnum,
			keybind, shared_from_this()));
	tools.back().get()->initializeTool(controlEnum);
	
	return true;
}

void Toolbox::createDefaultTools()
{
	// Later this will be read using a config file, likely XML or some human-readable format for easy editing
	tools.emplace_back(
		std::make_shared<Tool>(pointer, pointer, DT_NO_TOOL, "Pointer",
			IN_NO_INPUT, OUT_NO_OUTPUT,
			TSetType::continuous,
			INPUT_KEY_F1, shared_from_this())); 
	tools.back().get()->initializeTool(TSetType::continuous);
	//
	tools.emplace_back(
		std::make_shared<Tool>(hand, grab, DT_PAN, "Pan Camera", 
			IN_PAN, OUT_CAMERAPAN,
			TSetType::continuous,
			INPUT_KEY_H, shared_from_this()));
	tools.back().get()->initializeTool(TSetType::continuous);
	//
	tools.emplace_back(
		std::make_shared<Tool>(rotate, rotate, DT_ROTATE, "Rotate Camera", 
			IN_ROTATE, OUT_CAMERAROTATE,
			TSetType::continuous,
			INPUT_KEY_R, shared_from_this()));
	tools.back().get()->initializeTool(TSetType::continuous);
	//
	tools.emplace_back(
		std::make_shared<Tool>(zoomIn, zoomOut, DT_ZOOM_SCRUB, "Zoom Camera", 
			IN_ZOOM, OUT_CAMERAZOOM,
			TSetType::continuous,
			INPUT_KEY_Z, shared_from_this()));
	tools.back().get()->initializeTool(TSetType::continuous);
	//
	tools.emplace_back(
		std::make_shared<Tool>(brush, circleSmall, DT_BRUSH, "Brush",
			IN_DRAW, OUT_STROKE,
			TSetType::continuous,
			INPUT_KEY_B, shared_from_this()));
	tools.back().get()->initializeTool(TSetType::continuous);
	//
	tools.emplace_back(
		std::make_shared<Tool>(brush, circleSmall, DT_STROKE_LASSO, "Lasso",
			IN_LASSO, OUT_STROKE,
			TSetType::continuous,
			INPUT_KEY_L, shared_from_this()));
	tools.back().get()->initializeTool(TSetType::continuous);
	//
	tools.emplace_back(
		std::make_shared<Tool>(brush, circleSmall, DT_STROKE_RECTANGLE, "Rectangle Brush",
			IN_POLYGON, OUT_STROKE,
			TSetType::onepoint,
			INPUT_KEY_W, shared_from_this()));
	tools.back().get()->initializeTool(TSetType::onepoint);
	//
	tools.emplace_back(
		std::make_shared<Tool>(hand, circleSmall, DT_STROKE_SHAPELINE, "Shape-Line Brush",
			IN_SHAPELINE, OUT_STROKE,
			TSetType::drag,
			INPUT_KEY_T, shared_from_this()));
	tools.back().get()->initializeTool(TSetType::drag);
	//
	tools.emplace_back(
		std::make_shared<Tool>(hand, circleSmall, DT_STROKE_SHAPELINE, "Shape-Draw Brush",
			IN_SHAPEDRAW, OUT_STROKE,
			TSetType::continuous,
			INPUT_KEY_K, shared_from_this()));
	tools.back().get()->initializeTool(TSetType::continuous);
	//
	tools.emplace_back(
		std::make_shared<Tool>(hand, circleSmall, DT_STROKE_SHAPEFIELDPOINTS, "Drag-Shape Field Brush",
			IN_SHAPEFIELD, OUT_STROKE,
			TSetType::drag,
			INPUT_KEY_Q, shared_from_this()));
	tools.back().get()->initializeTool(TSetType::drag);
	//
	tools.emplace_back(
		std::make_shared<Tool>(hand, circleSmall, DT_STROKE_SHAPEFIELDPOINTS, "Box-Shape Field Brush",
			IN_SHAPEFIELD, OUT_STROKE,
			TSetType::onepoint,
			INPUT_KEY_D, shared_from_this()));
	tools.back().get()->initializeTool(TSetType::onepoint);
	//
	tools.emplace_back(
		std::make_shared<Tool>(brush, circleSmall, DT_STROKE_VORTEX, "Vortex Brush",
			IN_VORTEX, OUT_STROKE,
			TSetType::continuous,
			INPUT_KEY_E, shared_from_this()));
	tools.back().get()->initializeTool(TSetType::continuous);
	//
	tools.emplace_back(
		std::make_shared<Tool>(brush, circleSmall, DT_STROKE_VORTEX, "Radial Field Brush",
			IN_VORTEX, OUT_STROKE,
			TSetType::drag,
			INPUT_KEY_O, shared_from_this()));
	tools.back().get()->initializeTool(TSetType::drag);
	//
	tools.emplace_back(
		std::make_shared<Tool>(hand, circleSmall, DT_STROKE_SCANLINE, "Scanline Brush",
			IN_RAKE, OUT_STROKE,
			TSetType::continuous,
			INPUT_KEY_S, shared_from_this()));
	tools.back().get()->initializeTool(TSetType::continuous);
	tools.back().get()->getRake()->pushToCanvasEdge = true;
	//
	tools.emplace_back(
		std::make_shared<Tool>(brush, circleSmall, DT_STROKE_RAKE, "Rake Brush",
			IN_RAKE, OUT_STROKE,
			TSetType::continuous,
			INPUT_KEY_A, shared_from_this()));
	tools.back().get()->initializeTool(TSetType::continuous);
	//
	tools.emplace_back(
		std::make_shared<Tool>(brush, circleSmall, DT_STROKE_FAN, "Fan Brush",
			IN_FAN, OUT_STROKE,
			TSetType::continuous,
			INPUT_KEY_F, shared_from_this()));
	tools.back().get()->initializeTool(TSetType::continuous);
	//
	tools.emplace_back(
		std::make_shared<Tool>(brush, circleSmall, DT_STROKE_FAN, "Fan Airbrush",
			IN_FAN, OUT_STROKE,
			TSetType::continuous,
			INPUT_KEY_F_SHIFT, shared_from_this()));
	tools.back().get()->initializeTool(TSetType::continuous);
	tools.back().get()->getFan()->airbrushMode = true;
	//
	tools.emplace_back(
		std::make_shared<Tool>(dropper, dropper, DT_COLORPICKER, "Color Picker",
			IN_SAMPLER, OUT_SAMPLER,
			TSetType::continuous,
			INPUT_KEY_I, shared_from_this()));
	tools.back().get()->initializeTool(TSetType::continuous);
	//
	/* // Tool is broken for now
	tools.emplace_back(
		std::make_shared<Tool>(hand, hand, DT_STROKE_POLYLINE, "Poly Line",
			IN_POLYLINE, OUT_STROKE,
			TSetType::drag,
			INPUT_KEY_M, shared_from_this()));
	tools.back().get()->initializeTool(TSetType::drag);
	//*/
	setActiveTool_byID(DT_NO_TOOL);
	std::cout << "TOOLBOX::CREATEDEFAULTTOOLS::ARRAY-SIZE::" << tools.size() << std::endl;
}
int Toolbox::checkToolHotkeys(int sig)
{
	// Note: Assumes single use hotkeys for tools (Later can allow for cycling between tools with same key)
	// Note: Returns the index from tools array, not the tool ID number, must use setActiveTool_byIndex following this.
	int len = static_cast<int>(tools.size());
	for (int i = 0; i < len; i++)
	{
		if (tools[i].get()->keySig == sig)
		{
			std::cout << "TOOLBOX::CHECKHOTKEYS::KEYMATCH=" << sig << "::TOOLID::=" << tools[i]->id << std::endl;
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

std::shared_ptr<Tool> Toolbox::getActiveTool() { return activeTool; }

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
	int id = activeTool.get()->output.get()->getID();
	std::shared_ptr<CObject> xArg = std::make_shared<CObject>(xpos);
	std::shared_ptr<CObject> yArg = std::make_shared<CObject>(ypos);
	std::vector<std::shared_ptr<CObject>> args = { xArg, yArg };
	if (id == OUT_SAMPLER)
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
	int id = activeTool.get()->output.get()->getID();
	std::shared_ptr<CObject> xArg = std::make_shared<CObject>(xpos);
	std::shared_ptr<CObject> yArg = std::make_shared<CObject>(ypos);
	std::vector<std::shared_ptr<CObject>> args = { xArg, yArg };
	if (id == OUT_SAMPLER)
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
	int id = activeTool.get()->output.get()->getID();
	std::shared_ptr<CObject> xArg = std::make_shared<CObject>(xpos);
	std::shared_ptr<CObject> yArg = std::make_shared<CObject>(ypos);
	std::vector<std::shared_ptr<CObject>> args = { xArg, yArg };
	if (id == OUT_SAMPLER)
	{
		for (auto const& item : colorpickEndCallbacks)
		{
			item.second.get()->call(interpreter, &args);
		}
		return;
	}
}

// Input Functions
int Toolbox::sendClick(Application* sender, MouseEvent dat)
{
	return activeTool.get()->input.get()->click(sender, dat);
}
int Toolbox::sendMove(Application* sender, MouseEvent dat)
{
	return activeTool.get()->input.get()->move(sender, dat);
}
void Toolbox::sendPreview(Application* sender)
{
	activeTool.get()->output.get()->preview(sender, &activeTool.get()->input.get()->fragData);
}
void Toolbox::sendFinialize(Application* sender)
{
	activeTool.get()->output.get()->finalize(sender, &activeTool.get()->input.get()->fragData);
	activeTool.get()->output.get()->postprocess(sender, &activeTool.get()->input.get()->fragData);
}

// Validation Functions
bool Toolbox::checkValidIOCombination(std::shared_ptr<CInterpreter> interpreter, std::string inputName, std::string outputName)
{
	int input = convertIOStringToMacro(1, inputName);
	int output = convertIOStringToMacro(2, outputName);
	return checkValidIOCombination(interpreter, input, output);
}

bool Toolbox::checkValidIOCombination(std::shared_ptr<CInterpreter> interpreter, int inputMacro, int outputMacro)
{
	if (inputMacro == -1 || outputMacro == -1) { return false; }
	if (inputMacro == 0 || outputMacro == 0) { return true; }

	switch (inputMacro)
	{
	case IN_DRAG: if (outputMacro == OUT_STROKE) { return true; } break;
	case IN_ROTATE: if (outputMacro == OUT_CAMERAROTATE) { return true; } break;
	case IN_ZOOM: if (outputMacro == OUT_CAMERAZOOM) { return true; } break;
	case IN_DRAW: if (outputMacro == OUT_STROKE) { return true; } break;
	case IN_LINE: if (outputMacro == OUT_STROKE) { return true; } break;
	case IN_LASSO: if (outputMacro == OUT_STROKE) { return true; } break;
	case IN_POLYGON: if (outputMacro == OUT_STROKE) { return true; } break;
	case IN_PAN: if (outputMacro == OUT_CAMERAPAN) { return true; } break;
	case IN_POINT: if (outputMacro == OUT_STROKE) { return true; } break;
	case IN_SAMPLER: if (outputMacro == OUT_SAMPLER) { return true; } break;
	case IN_SHAPEFIELD: if (outputMacro == OUT_STROKE) { return true; } break;
	case IN_VORTEX: if (outputMacro == OUT_STROKE) { return true; } break;
	case IN_RAKE: if (outputMacro == OUT_STROKE) { return true; } break;
	case IN_FAN: if (outputMacro == OUT_STROKE) { return true; } break;
	case IN_SHAPELINE: if (outputMacro == OUT_STROKE) { return true; } break;
	case IN_SHAPEDRAW: if (outputMacro == OUT_STROKE) { return true; } break;
	case IN_POLYLINE: if (outputMacro == OUT_STROKE) { return true; } break;
	}

	return false;
}

bool Toolbox::checkValidControlScheme(std::shared_ptr<CInterpreter> interpreter, std::string inputName, std::string controlScheme)
{
	int input = convertIOStringToMacro(1, inputName);
	int control = convertIOStringToMacro(3, controlScheme);
	return checkValidControlScheme(interpreter, input, control);
}

bool Toolbox::checkValidControlScheme(std::shared_ptr<CInterpreter> interpreter, int inputMacro, int controlMacro)
{
	if (inputMacro == -1 || controlMacro == -1) { return false; }
	if (inputMacro == 0) { return true; }

	switch (inputMacro)
	{
	case IN_DRAG: if (controlMacro == CONTROL_CONTINUOUS) { return true; } break;
	case IN_ROTATE: if (controlMacro == CONTROL_DEFAULT) { return true; } break;
	case IN_ZOOM: if (controlMacro == CONTROL_DEFAULT) { return true; } break;
	case IN_DRAW: if (controlMacro == CONTROL_CONTINUOUS) { return true; } break;
	case IN_LINE: if (controlMacro == CONTROL_DRAG) { return true; } break;
	case IN_LASSO: if (controlMacro == CONTROL_CONTINUOUS) { return true; } break;
	case IN_POLYGON: if (controlMacro == CONTROL_ONEPOINT) { return true; } break;
	case IN_PAN: if (controlMacro == CONTROL_DEFAULT) { return true; } break;
	case IN_POINT: if (controlMacro == CONTROL_CONTINUOUS) { return true; } break;
	case IN_SAMPLER: if (controlMacro == CONTROL_DEFAULT) { return true; } break;
	case IN_SHAPEFIELD: if (controlMacro == CONTROL_ONEPOINT || controlMacro == CONTROL_DRAG) { return true; } break;
	case IN_VORTEX: if (controlMacro == CONTROL_CONTINUOUS || controlMacro == CONTROL_DRAG) { return true; } break;
	case IN_RAKE: if (controlMacro == CONTROL_CONTINUOUS) { return true; } break;
	case IN_FAN: if (controlMacro == CONTROL_CONTINUOUS) { return true; } break;
	case IN_SHAPELINE: if (controlMacro == CONTROL_ONEPOINT || controlMacro == CONTROL_DRAG) { return true; } break;
	case IN_SHAPEDRAW: if (controlMacro == CONTROL_CONTINUOUS) { return true; } break;
	case IN_POLYLINE: if (controlMacro == CONTROL_DRAG) { return true; } break;
	}

	return false;
}

int Toolbox::convertIOStringToMacro(int kind, std::string name)
{
	std::string nameFixed = name;
	nameFixed.erase(remove_if(nameFixed.begin(), nameFixed.end(), isspace), nameFixed.end());
	stringToLower(nameFixed);
	if (kind == 1) // INPUT METHOD
	{
		if (nameFixed == "noinputMacro") { return IN_NO_INPUT; }
		if (nameFixed == "drag") { return IN_DRAG; }
		if (nameFixed == "rotate" || nameFixed == "inrotate" || nameFixed == "rotatecamera" || nameFixed == "camerarotate") { return IN_ROTATE; }
		if (nameFixed == "zoom" || nameFixed == "inzoom" || nameFixed == "zoomcamera" || nameFixed == "camerazoom") { return IN_ZOOM; }
		if (nameFixed == "draw") { return IN_DRAW; }
		if (nameFixed == "line") { return IN_LINE; }
		if (nameFixed == "lasso") { return IN_LASSO; }
		if (nameFixed == "polygon") { return IN_POLYGON; }
		if (nameFixed == "pan" || nameFixed == "inpan" || nameFixed == "pancamera" || nameFixed == "camerapan") { return IN_PAN; }
		if (nameFixed == "point") { return IN_POINT; }
		if (nameFixed == "sampler" || nameFixed == "eyedropper" || nameFixed == "colorpicker") { return IN_SAMPLER; }
		if (nameFixed == "shapefield") { return IN_SHAPEFIELD; }
		if (nameFixed == "vortex") { return IN_VORTEX; }
		if (nameFixed == "rake") { return IN_RAKE; }
		if (nameFixed == "fan") { return IN_FAN; }
		if (nameFixed == "shapeline") { return IN_SHAPELINE; }
		if (nameFixed == "shapedraw") { return IN_SHAPEDRAW; }
		if (nameFixed == "polyline") { return IN_POLYLINE; }
	}
	else if (kind == 2) // OUTPUT METHOD
	{
		if (nameFixed == "nooutput") { return OUT_NO_OUTPUT; }
		if (nameFixed == "pan" || nameFixed == "outpan" || nameFixed == "pancamera" || nameFixed == "camerapan") { return OUT_CAMERAPAN; }
		if (nameFixed == "rotate" || nameFixed == "outrotate" || nameFixed == "rotatecamera" || nameFixed == "camerarotate") { return OUT_CAMERAROTATE; }
		if (nameFixed == "zoom" || nameFixed == "outzoom" || nameFixed == "zoomcamera" || nameFixed == "camerazoom") { return OUT_CAMERAZOOM; }
		if (nameFixed == "stroke" || nameFixed == "brush") { return OUT_STROKE; }
		if (nameFixed == "shape") { return OUT_SHAPE; }
		if (nameFixed == "gradient") { return OUT_GRADIENT; }
		if (nameFixed == "fill") { return OUT_FILL; }
		if (nameFixed == "sampler" || nameFixed == "eyedropper" || nameFixed == "colorpicker") { return OUT_SAMPLER; }
		if (nameFixed == "selection") { return OUT_SELECTION; }
	}
	else if (kind == 3) // CONTROL SCHEME
	{
		if (nameFixed == "default" || nameFixed == "usedefault") { return CONTROL_DEFAULT; }
		if (nameFixed == "continuous") { return CONTROL_CONTINUOUS; }
		if (nameFixed == "drag") { return CONTROL_DRAG; }
		if (nameFixed == "onepoint") { return CONTROL_ONEPOINT; }
		if (nameFixed == "twopoint") { return CONTROL_TWOPOINT; }
		if (nameFixed == "threepoint") { return CONTROL_THREEPOINT; }
		if (nameFixed == "fourpoint") { return CONTROL_FOURPOINT; }
	}
	else { return -1; }
}

TSetType Toolbox::convertControlMacro(int macro)
{
	switch (macro)
	{
	case CONTROL_DEFAULT: return TSetType::usedefault;
	case CONTROL_CONTINUOUS: return TSetType::continuous;
	case CONTROL_DRAG: return TSetType::drag;
	case CONTROL_ONEPOINT: return TSetType::onepoint;
	case CONTROL_TWOPOINT: return TSetType::twopoint;
	case CONTROL_THREEPOINT: return TSetType::threepoint;
	case CONTROL_FOURPOINT: return TSetType::fourpoint;
	default:
		return TSetType::usedefault;
	}
}