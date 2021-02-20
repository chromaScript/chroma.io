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