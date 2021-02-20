#include "include/Tool.h"

#include "include/structs.h"
#include "include/CustomCursor.h"
#include "include/Toolbox.h"
#include "include/methods/InputMethod.h"
#include "include/methods/OutputMethod.h"
#include "include/IOClasses.h"
#include "include/entities/UserInterface.h"
#include "include/cscript/CObject.h"
#include "include/cscript/CInterpreter.h"

#include "include/ToolSettings.h"

#include <string>
#include <iostream>
#include <functional>
#include <memory>

// Construct as New
Tool::Tool(
	std::shared_ptr<CustomCursor> cursorUp,
	std::shared_ptr<CustomCursor> cursorDown,
	int id, std::string n,
	int inID, int outID,
	TSetType controlScheme,
	int key, std::shared_ptr<Toolbox> owner)
{
	this->cursorUp = cursorUp;
	this->cursorDown = cursorDown;
	this->id = id;
	this->name = n;
	this->keySig = key;
	this->owner = owner;
	this->inID = inID;
	this->outID = outID;
	//createMethods(inID, outID);
	//buildSettings(input.get()->interestMask, output.get()->interestMask);
	std::cout << "TOOL::CONSTRUCTOR::REGISTERED::ID-" << id << "-::NAME-" << name << std::endl;
}
Tool::~Tool()
{
	std::cout << "TOOL::DESTRUCTOR::TOOL_DESTROYED::ID-" << id << "-::NAME-" << name << std::endl;
}
void Tool::initializeTool(TSetType controlScheme)
{
	createMethods(controlScheme);
	buildSettings();
}

void Tool::createMethods(TSetType controlScheme)
{
	// Set InputMethod
	switch (inID)
	{
	case IN_NO_INPUT:
		input = std::make_unique<In_NoInput>(IN_NO_INPUT, controlScheme, shared_from_this());
		break;
	case IN_PAN:
		input = std::make_unique<In_Pan>(IN_PAN, controlScheme, shared_from_this());
		break;
	case IN_ZOOM:
		input = std::make_unique<In_Zoom>(IN_ZOOM, controlScheme, shared_from_this());
		break;
	case IN_ROTATE:
		input = std::make_unique<In_Rotate>(IN_ROTATE, controlScheme, shared_from_this());
		break;
	case IN_DRAW:
		input = std::make_unique<In_Draw>(IN_DRAW, controlScheme, shared_from_this());
		break;
	case IN_POINT:
		input = std::make_unique<In_Point>(IN_POINT, controlScheme, shared_from_this());
		break;
	case IN_SAMPLER:
		input = std::make_unique<In_Sampler>(IN_SAMPLER, controlScheme, shared_from_this());
		break;
	case IN_SHAPELINE:
		input = std::make_unique<In_ShapeLine>(IN_SHAPELINE, controlScheme, shared_from_this());
		break;
	case IN_SHAPEDRAW:
		input = std::make_unique<In_ShapeDraw>(IN_SHAPEDRAW, controlScheme, shared_from_this());
		break;
	case IN_RECTANGLE:
		break;
	case IN_ELLIPSE:
		break;
	case IN_NGON:
		break;
	case IN_LASSO:
		input = std::make_unique<In_Lasso>(IN_LASSO, controlScheme, shared_from_this());
		break;
	case IN_POLYGON:
		input = std::make_unique<In_Polygon>(IN_POLYGON, controlScheme, shared_from_this());
		break;
	case IN_SHAPEFIELD:
		input = std::make_unique<In_ShapeField>(IN_SHAPEFIELD, controlScheme, shared_from_this());
		break;
	case IN_VORTEX:
		input = std::make_unique<In_Vortex>(IN_VORTEX, controlScheme, shared_from_this());
		break;
	case IN_RAKE:
		input = std::make_unique<In_Rake>(IN_RAKE, controlScheme, shared_from_this());
		break;
	case IN_FAN:
		input = std::make_unique<In_Fan>(IN_FAN, controlScheme, shared_from_this());
		break;
	case IN_POLYLINE:
		input = std::make_unique<In_PolyLine>(IN_POLYLINE, controlScheme, shared_from_this());
		break;
	default:
		input = std::make_unique<In_NoInput>(IN_NO_INPUT, controlScheme, shared_from_this());
	}
	// Set OutputMethod
	switch (outID)
	{
	case OUT_NO_OUTPUT:
		output = std::make_unique<Out_NoOutput>(OUT_NO_OUTPUT, shared_from_this());
		break;
	case OUT_CAMERAPAN:
		output = std::make_unique<Out_PanCamera>(OUT_CAMERAPAN, shared_from_this());
		break;
	case OUT_CAMERAROTATE:
		output = std::make_unique<Out_RotateCamera>(OUT_CAMERAROTATE, shared_from_this());
		break;
	case OUT_CAMERAZOOM:
		output = std::make_unique<Out_ZoomCamera>(OUT_CAMERAZOOM, shared_from_this());
		break;
	case OUT_STROKE:
		output = std::make_unique<Out_Stroke>(OUT_STROKE, shared_from_this());
		break;
	case OUT_SHAPE:
		break;
	case OUT_GRADIENT:
		break;
	case OUT_FILL:
		break;
	case OUT_SELECTION:
		break;
	case OUT_SAMPLER:
		output = std::make_unique<Out_Sampler>(OUT_SAMPLER, shared_from_this());
		break;
	default:
		output = std::make_unique<Out_NoOutput>(OUT_NO_OUTPUT, shared_from_this());
	}
}

// Create New Settings
void Tool::buildSettings()
{
	for (TSetType mask : input.get()->interestMask)
	{
		std::vector<TSetType> outputMask = output.get()->interestMask;
		switch (mask)
		{
		case TSetType::basic:
			if (matchMask(TSetType::basic, outputMask))
			{
				settings.insert(std::pair<TSetType, std::unique_ptr<ToolSettings>>(TSetType::basic, std::make_unique<TSet_Basic>()));
			}
			break;
		case TSetType::image:
			if (matchMask(TSetType::image, outputMask))
			{
				settings.insert(std::pair<TSetType, std::unique_ptr<ToolSettings>>(TSetType::image, std::make_unique<TSet_Image>()));
			}
			break;
		case TSetType::polygon:
			if (matchMask(TSetType::polygon, outputMask))
			{
				settings.insert(std::pair<TSetType, std::unique_ptr<ToolSettings>>(TSetType::polygon, std::make_unique<TSet_Polygon>()));
			}
			break;
		case TSetType::alpha:
			if (matchMask(TSetType::alpha, outputMask))
			{
				settings.insert(std::pair<TSetType, std::unique_ptr<ToolSettings>>(TSetType::alpha, std::make_unique<TSet_Alpha>()));
			}
			break;
		case TSetType::smoothing:
			if (matchMask(TSetType::smoothing, outputMask))
			{
				settings.insert(std::pair<TSetType, std::unique_ptr<ToolSettings>>(TSetType::smoothing, std::make_unique<TSet_Smoothing>()));
			}
			break;
		case TSetType::pan:
			if (matchMask(TSetType::pan, outputMask))
			{
				settings.insert(std::pair<TSetType, std::unique_ptr<ToolSettings>>(TSetType::pan, std::make_unique<TSet_Pan>()));
			}
			break;
		case TSetType::rotate:
			if (matchMask(TSetType::rotate, outputMask))
			{
				settings.insert(std::pair<TSetType, std::unique_ptr<ToolSettings>>(TSetType::rotate, std::make_unique<TSet_Rotate>()));
			}
			break;
		case TSetType::zoom:
			if (matchMask(TSetType::zoom, outputMask))
			{
				settings.insert(std::pair<TSetType, std::unique_ptr<ToolSettings>>(TSetType::zoom, std::make_unique<TSet_Zoom>()));
			}
			break;
		case TSetType::field:
			if (matchMask(TSetType::field, outputMask))
			{
				settings.insert(std::pair<TSetType, std::unique_ptr<ToolSettings>>(TSetType::field, std::make_unique<TSet_Field>()));
			}
			break;
		case TSetType::vortex:
			if (matchMask(TSetType::vortex, outputMask))
			{
				settings.insert(std::pair<TSetType, std::unique_ptr<ToolSettings>>(TSetType::vortex, std::make_unique<TSet_Vortex>()));
			}
			break;
		case TSetType::rake:
			if (matchMask(TSetType::rake, outputMask))
			{
				settings.insert(std::pair<TSetType, std::unique_ptr<ToolSettings>>(TSetType::rake, std::make_unique<TSet_Rake>()));
			}
			break;
		case TSetType::fan:
			if (matchMask(TSetType::fan, outputMask))
			{
				settings.insert(std::pair<TSetType, std::unique_ptr<ToolSettings>>(TSetType::fan, std::make_unique<TSet_Fan>()));
			}
			break;
		case TSetType::polyline:
			if (matchMask(TSetType::polyline, outputMask))
			{
				settings.insert(std::pair<TSetType, std::unique_ptr<ToolSettings>>(TSetType::polyline, std::make_unique<TSet_PolyLine>()));
			}
			break;
		case TSetType::sampler:
			settings.insert(std::pair<TSetType, std::unique_ptr<ToolSettings>>(TSetType::sampler, std::make_unique<TSet_Sampler>()));
			break;
		}
	}
	switch (input.get()->controlScheme)
	{
	case TSetType::continuous:
		settings.insert(std::pair<TSetType, std::unique_ptr<ToolSettings>>(TSetType::continuous, std::make_unique<TSet_ContinuousControl>()));
		break;
	case TSetType::drag:
		settings.insert(std::pair<TSetType, std::unique_ptr<ToolSettings>>(TSetType::drag, std::make_unique<TSet_DragControl>()));
		break;
	case TSetType::onepoint:
		settings.insert(std::pair<TSetType, std::unique_ptr<ToolSettings>>(TSetType::onepoint, std::make_unique<TSet_OnePointControl>()));
		break;
	case TSetType::twopoint:
		settings.insert(std::pair<TSetType, std::unique_ptr<ToolSettings>>(TSetType::twopoint, std::make_unique<TSet_TwoPointControl>()));
		break;
	case TSetType::threepoint:
		settings.insert(std::pair<TSetType, std::unique_ptr<ToolSettings>>(TSetType::threepoint, std::make_unique<TSet_ThreePointControl>()));
		break;
	default:
		break;
	}
}
bool Tool::matchMask(TSetType query, std::vector<TSetType> interestMask)
{
	for (TSetType mask : interestMask)
	{
		if (mask == query) { return true; }
	}
	return false;
}

// Activate Tool
std::shared_ptr<Tool> Tool::activate(std::pair<CColor, CColor> colors, std::shared_ptr<UI> ui)
{
	// Gather the activation results, checking if the color is locked for FG/BG
	std::pair<bool, CColor> fgResult = setFGColor(colors.first, true);
	std::pair<bool, CColor> bgResult = setBGColor(colors.second, true);
	// If the colors were locked, then push the savedColor back up to UI which will notify
	// widgets that the color has changed again.
	if (fgResult.first) { ui.get()->updateFGColor(fgResult.second, 0, 0); }
	if (bgResult.first) { ui.get()->updateBGColor(bgResult.second, 0, 0); }
	return shared_from_this();
}

// Quick Settings Fetchers
TSet_ContinuousControl* Tool::getContinuousControl()
{
	if (settings.count(TSetType::continuous) == 1)
	{
		return dynamic_cast<TSet_ContinuousControl*>(settings.at(TSetType::continuous).get());
	}
	return nullptr;
}
TSet_DragControl* Tool::getDragControl()
{
	if (settings.count(TSetType::drag) == 1)
	{
		return dynamic_cast<TSet_DragControl*>(settings.at(TSetType::drag).get());
	}
	return nullptr;
}
TSet_OnePointControl* Tool::getOnePointControl()
{
	if (settings.count(TSetType::onepoint) == 1)
	{
		return dynamic_cast<TSet_OnePointControl*>(settings.at(TSetType::onepoint).get());
	}
	return nullptr;
}
TSet_TwoPointControl* Tool::getTwoPointControl()
{
	if (settings.count(TSetType::twopoint) == 1)
	{
		return dynamic_cast<TSet_TwoPointControl*>(settings.at(TSetType::twopoint).get());
	}
	return nullptr;
}
TSet_ThreePointControl* Tool::getThreePointControl()
{
	if (settings.count(TSetType::threepoint) == 1)
	{
		return dynamic_cast<TSet_ThreePointControl*>(settings.at(TSetType::threepoint).get());
	}
	return nullptr;
}
//
//
TSet_Sampler* Tool::getSampler()
{
	if (settings.count(TSetType::sampler) == 1)
	{
		return dynamic_cast<TSet_Sampler*>(settings.at(TSetType::sampler).get());
	}
	return nullptr;
}
TSet_Pan* Tool::getPan()
{
	if (settings.count(TSetType::pan) == 1)
	{
		return dynamic_cast<TSet_Pan*>(settings.at(TSetType::pan).get());
	}
	return nullptr;
}
TSet_Zoom* Tool::getZoom()
{
	if (settings.count(TSetType::zoom) == 1)
	{
		return dynamic_cast<TSet_Zoom*>(settings.at(TSetType::zoom).get());
	}
	return nullptr;
}
TSet_Rotate* Tool::getRotate()
{
	if (settings.count(TSetType::rotate) == 1)
	{
		return dynamic_cast<TSet_Rotate*>(settings.at(TSetType::rotate).get());
	}
	return nullptr;
}
TSet_Basic* Tool::getBasic()
{
	if (settings.count(TSetType::basic) == 1)
	{
		return dynamic_cast<TSet_Basic*>(settings.at(TSetType::basic).get());
	}
	return nullptr;
}
TSet_Smoothing* Tool::getSmoothing()
{
	if (settings.count(TSetType::smoothing) == 1)
	{
		return dynamic_cast<TSet_Smoothing*>(settings.at(TSetType::smoothing).get());
	}
	return nullptr;
}
TSet_Image* Tool::getImage()
{
	if (settings.count(TSetType::image) == 1)
	{
		return dynamic_cast<TSet_Image*>(settings.at(TSetType::image).get());
	}
	return nullptr;
}
TSet_Polygon* Tool::getPolygon()
{
	if (settings.count(TSetType::polygon) == 1)
	{
		return dynamic_cast<TSet_Polygon*>(settings.at(TSetType::polygon).get());
	}
	return nullptr;
}
TSet_Field* Tool::getField()
{
	if (settings.count(TSetType::field) == 1)
	{
		return dynamic_cast<TSet_Field*>(settings.at(TSetType::field).get());
	}
	return nullptr;
}
TSet_Alpha* Tool::getAlpha()
{
	if (settings.count(TSetType::alpha) == 1)
	{
		return dynamic_cast<TSet_Alpha*>(settings.at(TSetType::alpha).get());
	}
	return nullptr;
}
TSet_Fan* Tool::getFan()
{
	if (settings.count(TSetType::fan) == 1)
	{
		return dynamic_cast<TSet_Fan*>(settings.at(TSetType::fan).get());
	}
	return nullptr;
}
TSet_Vortex* Tool::getVortex()
{
	if (settings.count(TSetType::vortex) == 1)
	{
		return dynamic_cast<TSet_Vortex*>(settings.at(TSetType::vortex).get());
	}
	return nullptr;
}
TSet_Rake* Tool::getRake()
{
	if (settings.count(TSetType::rake) == 1)
	{
		return dynamic_cast<TSet_Rake*>(settings.at(TSetType::rake).get());
	}
	return nullptr;
}
TSet_PolyLine* Tool::getPolyLine()
{
	if (settings.count(TSetType::polyline) == 1)
	{
		return dynamic_cast<TSet_PolyLine*>(settings.at(TSetType::polyline).get());
	}
	return nullptr;
}



// Get Colors
CColor Tool::getFGColor()
{
	if (settings.count(TSetType::basic) == 1)
	{
		return dynamic_cast<TSet_Basic*>(settings.at(TSetType::basic).get())->currentFGColor;
	}
	return black;
}
CColor Tool::getBGColor()
{
	if (settings.count(TSetType::basic) == 1)
	{
		return dynamic_cast<TSet_Basic*>(settings.at(TSetType::basic).get())->currentBGColor;
	}
	return white;
}

// Set Colors
std::pair<bool, CColor> Tool::setFGColor(CColor color, bool isActivation)
{
	if (settings.count(TSetType::basic) == 1)
	{
		TSet_Basic* setPtr = dynamic_cast<TSet_Basic*>(settings.at(TSetType::basic).get());
		setPtr->currentFGColor = color;
		// If the FG is unlocked, then update the savedColor as well
		if (!setPtr->lockFGColor)
		{
			setPtr->savedFGColor = color;
		}
		// Else if this is an activation as well, reset the currentColor to savedColor
		else if (isActivation && setPtr->lockFGColor)
		{
			setPtr->currentFGColor = setPtr->savedFGColor;
		}
		// Return the lock state & resulting currentColor
		return std::pair<bool, CColor>(setPtr->lockFGColor, setPtr->currentFGColor);
	}
	// If this tool is uninterested in basic, return false/defaultFG
	return std::pair<bool, CColor>(false, black);
}

std::pair<bool, CColor> Tool::setBGColor(CColor color, bool isActivation)
{
	if (settings.count(TSetType::basic) == 1)
	{
		TSet_Basic* setPtr = dynamic_cast<TSet_Basic*>(settings.at(TSetType::basic).get());
		setPtr->currentBGColor = color;
		// If the FG is unlocked, then update the savedColor as well
		if (!setPtr->lockBGColor)
		{
			setPtr->savedBGColor = color;
		}
		// Else if this is an activation as well, reset the currentColor to savedColor
		else if (isActivation && setPtr->lockFGColor)
		{
			setPtr->currentBGColor = setPtr->savedBGColor;
		}
		// Return the lock state & resulting currentColor
		return std::pair<bool, CColor>(setPtr->lockBGColor, setPtr->currentBGColor);
	}
	// If this tool is uninterested in basic, return false/defaultBG
	return std::pair<bool, CColor>(false, white);
}

// Settings Functions
bool Tool::checkInterestMask(std::shared_ptr<CInterpreter> interpreter, std::string name)
{
	TSetType query = stringToTSetType(name);
	// Return false if the settings name was invalid
	if (query == TSetType::null) { return false; }
	else 
	{
		bool inputMatch = false, outputMatch = false;
		switch (query)
		{
		case TSetType::drag:
		case TSetType::continuous:
		case TSetType::onepoint:
		case TSetType::twopoint:
		case TSetType::threepoint:
			for (TSetType mask : input.get()->interestMask)
			{
				if (mask == query) { return true; }
			}
			break;
		default:
			for (TSetType mask : input.get()->interestMask)
			{
				if (mask == query) { inputMatch = true; }
			}
			for (TSetType mask : output.get()->interestMask)
			{
				if (mask == query) { outputMatch = true; }
			}
			if (inputMatch && outputMatch) { return true; }
			else { return false; }
		}
		return false; 
	}
}

std::shared_ptr<CObject> Tool::putProperty(
	std::shared_ptr<CInterpreter> interpreter, int settingsSig, std::shared_ptr<CObject> object, bool isGet)
{
	int coreSelect = settingsSig / 100;
	switch (coreSelect)
	{
	case 0: break;
	case 1: break;
	case 2: break;
	case 3: break;
	case 4:
		if (settings.count(TSetType::continuous))
		{
			return getContinuousControl()->putProperty(interpreter, shared_from_this(), settingsSig, object, isGet);
		}
		else { return std::make_shared<CObject>(nullptr); }
		break;
	case 5: break;
	case 6: break;
	case 7: break;
	case 8: break;
	case 20: break;
	case 21: break;
	case 22:
		if (settings.count(TSetType::image)) 
		{
			return getImage()->putProperty(interpreter, shared_from_this(), settingsSig, object, isGet); 
		}
		else { return std::make_shared<CObject>(nullptr); }
		break;
	case 23: break;
	case 24: break;
	case 25: break;
	case 26: break;
	case 27: break;
	case 28:
		if (settings.count(TSetType::alpha))
		{
			return getAlpha()->putProperty(interpreter, shared_from_this(), settingsSig, object, isGet);
		}
		else { return std::make_shared<CObject>(nullptr); }
		break;
	case 29: break;
	case 30: break;
	case 31: break;
	case 32: break;
	case 33: break;
	case 34: break;
	case 35: break;
	case 36: break;
	case 37: break;
	case 38: break;
	case 48: break;
	case 49: break;
	case 50: break;
	case 61: break;
	case 62: break;
	case 63: break;
	case 70: break;
	case 76: break;
	case 82: break;
	case 91: break;
	case 98: break;
	case 99: break;
	}
	return std::make_shared<CObject>(nullptr);
}