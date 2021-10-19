#include "../include/tool/Tool.h"

#include "../include/CustomCursor.h"
#include "../include/tool/Toolbox.h"
#include "../include/methods/InputMethod.h"
#include "../include/methods/OutputMethod.h"
#include "../include/methods/IOClasses.h"
#include "../include/entities/UserInterface.h"
#include "../include/cscript/CObject.h"
#include "../include/cscript/CInterpreter.h"

#include "../include/tool/ToolSettings.h"
#include "../include/tool/toolSettings/effects/TSet_Effects.h"
#include "../include/methods/MethodType.h"

#include <string>
#include <iostream>
#include <functional>
#include <memory>

// Construct as New
Tool::Tool(
	std::shared_ptr<CustomCursor> cursorUp,
	std::shared_ptr<CustomCursor> cursorDown,
	int id, std::string n,
	MethodType inID, MethodType outID,
	TSetType controlScheme,
	Keybind keybind, std::shared_ptr<Toolbox> owner)
{
	this->cursorUp = cursorUp;
	this->cursorDown = cursorDown;
	this->id = id;
	this->name = n;
	this->keybind = keybind;
	this->owner = owner;
	this->inID = inID;
	this->outID = outID;
	std::cout << "TOOL::CONSTRUCTOR::REGISTERED::ID-" << id << "-::NAME-" << name << std::endl;
}
Tool::Tool(
	std::shared_ptr<CustomCursor> cursorUp,
	std::shared_ptr<CustomCursor> cursorDown,
	int id, std::string n,
	MethodType inID, MethodType outID,
	TSetType controlScheme,
	Keybind keybind, std::shared_ptr<Toolbox> owner,
	std::vector<std::string> tags, std::string category, std::string type, std::string date, std::string author)
{
	this->cursorUp = cursorUp;
	this->cursorDown = cursorDown;
	this->id = id;
	this->name = n;
	this->keybind = keybind;
	this->owner = owner;
	this->inID = inID;
	this->outID = outID;
	this->tags = tags;
	this->category = category;
	this->toolType = type;
	this->date = date;
	this->author = author;
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
	case MethodType::in_noInput:
		input = std::make_unique<In_NoInput>(MethodType::in_noInput, controlScheme, shared_from_this());
		break;
	case MethodType::in_pan:
		input = std::make_unique<In_Pan>(MethodType::in_pan, controlScheme, shared_from_this());
		break;
	case MethodType::in_zoom:
		input = std::make_unique<In_Zoom>(MethodType::in_zoom, controlScheme, shared_from_this());
		break;
	case MethodType::in_rotate:
		input = std::make_unique<In_Rotate>(MethodType::in_rotate, controlScheme, shared_from_this());
		break;
	case MethodType::in_draw:
		input = std::make_unique<In_Draw>(MethodType::in_draw, controlScheme, shared_from_this());
		break;
	case MethodType::in_point:
		input = std::make_unique<In_Point>(MethodType::in_point, controlScheme, shared_from_this());
		break;
	case MethodType::in_sampler:
		input = std::make_unique<In_Sampler>(MethodType::in_sampler, controlScheme, shared_from_this());
		break;
	case MethodType::in_shapeLine:
		input = std::make_unique<In_ShapeLine>(MethodType::in_shapeLine, controlScheme, shared_from_this());
		break;
	case MethodType::in_shapeDraw:
		input = std::make_unique<In_ShapeDraw>(MethodType::in_shapeDraw, controlScheme, shared_from_this());
		break;
	case MethodType::in_lasso:
		input = std::make_unique<In_Lasso>(MethodType::in_lasso, controlScheme, shared_from_this());
		break;
	case MethodType::in_polygon:
		input = std::make_unique<In_Polygon>(MethodType::in_polygon, controlScheme, shared_from_this());
		break;
	case MethodType::in_shapeField:
		input = std::make_unique<In_ShapeField>(MethodType::in_shapeField, controlScheme, shared_from_this());
		break;
	case MethodType::in_spline:
		input = std::make_unique<In_Spline>(MethodType::in_spline, controlScheme, shared_from_this());
		break;
	case MethodType::in_vortex:
		input = std::make_unique<In_Vortex>(MethodType::in_vortex, controlScheme, shared_from_this());
		break;
	case MethodType::in_rake:
		input = std::make_unique<In_Rake>(MethodType::in_rake, controlScheme, shared_from_this());
		break;
	case MethodType::in_fan:
		input = std::make_unique<In_Fan>(MethodType::in_fan, controlScheme, shared_from_this());
		break;
	case MethodType::in_polyLine:
		input = std::make_unique<In_PolyLine>(MethodType::in_polyLine, controlScheme, shared_from_this());
		break;
	default:
		input = std::make_unique<In_NoInput>(MethodType::in_noInput, controlScheme, shared_from_this());
	}
	// Set OutputMethod
	switch (outID)
	{
	case MethodType::out_noOutput:
		output = std::make_unique<Out_NoOutput>(MethodType::out_noOutput, shared_from_this());
		break;
	case MethodType::out_pan:
		output = std::make_unique<Out_PanCamera>(MethodType::out_pan, shared_from_this());
		break;
	case MethodType::out_rotate:
		output = std::make_unique<Out_RotateCamera>(MethodType::out_rotate, shared_from_this());
		break;
	case MethodType::out_zoom:
		output = std::make_unique<Out_ZoomCamera>(MethodType::out_zoom, shared_from_this());
		break;
	case MethodType::out_stroke:
		output = std::make_unique<Out_Stroke>(MethodType::out_stroke, shared_from_this());
		break;
	case MethodType::out_shape:
		break;
	case MethodType::out_gradient:
		break;
	case MethodType::out_fill:
		break;
	case MethodType::out_selection:
		break;
	case MethodType::out_sampler:
		output = std::make_unique<Out_Sampler>(MethodType::out_sampler, shared_from_this());
		break;
	default:
		output = std::make_unique<Out_NoOutput>(MethodType::out_noOutput, shared_from_this());
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
		case TSetType::color:
			if (matchMask(TSetType::color, outputMask))
			{
				settings.insert(std::pair<TSetType, std::unique_ptr<ToolSettings>>(TSetType::color, std::make_unique<TSet_Color>()));
			}
			break;
		case TSetType::effects:
			if (matchMask(TSetType::effects, outputMask))
			{
				settings.insert(std::pair<TSetType, std::unique_ptr<ToolSettings>>(TSetType::effects, std::make_unique<TSet_Effects>()));
			}
			break;
		case TSetType::character:
			if (matchMask(TSetType::character, outputMask))
			{
				settings.insert(std::pair<TSetType, std::unique_ptr<ToolSettings>>(TSetType::character, std::make_unique<TSet_Character>()));
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
		case TSetType::gradient:
			if (matchMask(TSetType::gradient, outputMask))
			{
				settings.insert(std::pair<TSetType, std::unique_ptr<ToolSettings>>(TSetType::gradient, std::make_unique<TSet_Gradient>()));
			}
			break;
		case TSetType::fill:
			if (matchMask(TSetType::fill, outputMask))
			{
				settings.insert(std::pair<TSetType, std::unique_ptr<ToolSettings>>(TSetType::fill, std::make_unique<TSet_Fill>()));
			}
			break;
		case TSetType::shader:
			if (matchMask(TSetType::shader, outputMask))
			{
				settings.insert(std::pair<TSetType, std::unique_ptr<ToolSettings>>(TSetType::shader, std::make_unique<TSet_Shader>()));
			}
			break;
		case TSetType::filter:
			if (matchMask(TSetType::filter, outputMask))
			{
				settings.insert(std::pair<TSetType, std::unique_ptr<ToolSettings>>(TSetType::filter, std::make_unique<TSet_Filter>()));
			}
			break;
		case TSetType::antiAliasing:
			if (matchMask(TSetType::antiAliasing, outputMask))
			{
				settings.insert(std::pair<TSetType, std::unique_ptr<ToolSettings>>(TSetType::antiAliasing, std::make_unique<TSet_AntiAliasing>()));
			}
			break;
		case TSetType::pattern:
			if (matchMask(TSetType::pattern, outputMask))
			{
				settings.insert(std::pair<TSetType, std::unique_ptr<ToolSettings>>(TSetType::pattern, std::make_unique<TSet_Pattern>()));
			}
			break;
	
		case TSetType::sampler:
			if (matchMask(TSetType::sampler, outputMask))
			{
				settings.insert(std::pair<TSetType, std::unique_ptr<ToolSettings>>(TSetType::sampler, std::make_unique<TSet_Sampler>()));
			}
			break;
		case TSetType::scatter:
			if (matchMask(TSetType::scatter, outputMask))
			{
				settings.insert(std::pair<TSetType, std::unique_ptr<ToolSettings>>(TSetType::scatter, std::make_unique<TSet_Scatter>()));
			}
			break;
		case TSetType::spline:
			if (matchMask(TSetType::spline, outputMask))
			{
				settings.insert(std::pair<TSetType, std::unique_ptr<ToolSettings>>(TSetType::spline, std::make_unique<TSet_Spline>()));
			}
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

float Tool::getSize()
{
	float out = 0.0f;
	if (checkInterestMask(TSetType::image))
	{
		out = getImage()->tipSize;
	}
	return out;
}
float Tool::getTrueSpacing()
{
	float out = 1.0f;
	if (checkInterestMask(TSetType::image))
	{
		out = getImage()->trueSpacing;
	}
	return out;
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

bool Tool::sendInput(Keybind input, InputModKey activeModKey)
{
	if (inID == MethodType::in_spline && checkInterestMask(TSetType::spline))
	{
		Keybind splineCaptureKey = getSpline()->captureKey;
		if (compareModKey(input, splineCaptureKey, false) || compareModKey(input, splineCaptureKey, true))
		{
			dynamic_cast<In_Spline*>(this->input.get())->beginRecording();
			return true;
		}
	}
	return false;
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
TSet_Blend* Tool::getBlend()
{
	if (settings.count(TSetType::blend) == 1)
	{
		return dynamic_cast<TSet_Blend*>(settings.at(TSetType::blend).get());
	}
	return nullptr;
}
TSet_Character* Tool::getCharacter()
{
	if (settings.count(TSetType::character) == 1)
	{
		return dynamic_cast<TSet_Character*>(settings.at(TSetType::character).get());
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
TSet_PolyLine* Tool::getPolyLine()
{
	if (settings.count(TSetType::polyline) == 1)
	{
		return dynamic_cast<TSet_PolyLine*>(settings.at(TSetType::polyline).get());
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
TSet_Color* Tool::getColor()
{
	if (settings.count(TSetType::color) == 1)
	{
		return dynamic_cast<TSet_Color*>(settings.at(TSetType::color).get());
	}
	return nullptr;
}
TSet_Scatter* Tool::getScatter()
{
	if (settings.count(TSetType::scatter) == 1)
	{
		return dynamic_cast<TSet_Scatter*>(settings.at(TSetType::scatter).get());
	}
	return nullptr;
}
TSet_Spline* Tool::getSpline()
{
	if (settings.count(TSetType::spline) == 1)
	{
		return dynamic_cast<TSet_Spline*>(settings.at(TSetType::spline).get());
	}
	return nullptr;
}
TSet_Effects* Tool::getEffects()
{
	if (settings.count(TSetType::effects) == 1)
	{
		return dynamic_cast<TSet_Effects*>(settings.at(TSetType::effects).get());
	}
	return nullptr;
}
TSet_Vector* Tool::getVector()
{
	if (settings.count(TSetType::vector) == 1)
	{
		return dynamic_cast<TSet_Vector*>(settings.at(TSetType::vector).get());
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
TSet_Gradient* Tool::getGradient()
{
	if (settings.count(TSetType::gradient) == 1)
	{
		return dynamic_cast<TSet_Gradient*>(settings.at(TSetType::gradient).get());
	}
	return nullptr;
}
TSet_Fill* Tool::getFill()
{
	if (settings.count(TSetType::fill) == 1)
	{
		return dynamic_cast<TSet_Fill*>(settings.at(TSetType::fill).get());
	}
	return nullptr;
}
TSet_Shader* Tool::getShader()
{
	if (settings.count(TSetType::shader) == 1)
	{
		return dynamic_cast<TSet_Shader*>(settings.at(TSetType::shader).get());
	}
	return nullptr;
}
TSet_Filter* Tool::getFilter()
{
	if (settings.count(TSetType::filter) == 1)
	{
		return dynamic_cast<TSet_Filter*>(settings.at(TSetType::filter).get());
	}
	return nullptr;
}
TSet_AntiAliasing* Tool::getAntiAliasing()
{
	if (settings.count(TSetType::antiAliasing) == 1)
	{
		return dynamic_cast<TSet_AntiAliasing*>(settings.at(TSetType::antiAliasing).get());
	}
	return nullptr;
}
TSet_Pattern* Tool::getPattern()
{
	if (settings.count(TSetType::pattern) == 1)
	{
		return dynamic_cast<TSet_Pattern*>(settings.at(TSetType::pattern).get());
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
	return checkInterestMask(query);
}
bool Tool::checkInterestMask(TSetType interest)
{
	// Return false if the settings name was invalid
	if (interest == TSetType::null) { return false; }
	else
	{
		bool inputMatch = false, outputMatch = false;
		switch (interest)
		{
		case TSetType::drag:
		case TSetType::continuous:
		case TSetType::onepoint:
		case TSetType::twopoint:
		case TSetType::threepoint:
			if (input->controlScheme == interest) { return true; }
			break;
		default:
			for (TSetType mask : input.get()->interestMask)
			{
				if (mask == interest) { inputMatch = true; }
			}
			for (TSetType mask : output.get()->interestMask)
			{
				if (mask == interest) { outputMatch = true; }
			}
			if (inputMatch && outputMatch) { return true; }
			else { return false; }
		}
		return false;
	}
}

ToolSettings* Tool::getSetting(int settingID, int subSigID)
{
	int coreSelect = settingID / 100;

	switch (coreSelect)
	{
	case 0:
		// Meta-Data Settings
		return nullptr;
		break;
	case 1:
		if (settings.count(TSetType::pan)) { return getPan(); }
		else { return nullptr; }
		break;
	case 2:
		if (settings.count(TSetType::zoom)) { return getZoom(); }
		else { return nullptr; }
		break;
	case 3:
		if (settings.count(TSetType::rotate)) { return getRotate(); }
		else { return nullptr; }
		break;
	case 4:
		if (settings.count(TSetType::continuous)) { return getContinuousControl(); }
		else { return nullptr; }
		break;
	case 5:
		if (settings.count(TSetType::drag)) { return getDragControl(); }
		else { return nullptr; }
		break;
	case 6:
		if (settings.count(TSetType::onepoint)) { return getOnePointControl(); }
		else { return nullptr; }
		break;
	case 7:
		if (settings.count(TSetType::twopoint)) { return getTwoPointControl(); }
		else { return nullptr; }
		break;
	case 8:
		if (settings.count(TSetType::threepoint)) { return getThreePointControl(); }
		else { return nullptr; }
		break;
	case 9:
		if (settings.count(TSetType::sampler)) { return getSampler(); }
		else { return nullptr; }
		break;
	case 20:
		if (settings.count(TSetType::blend)) { return getBlend(); }
		else { return nullptr; }
		break;
	case 21:
		if (settings.count(TSetType::basic)) { return getBasic(); }
		else { return nullptr; }
		break;
	case 22:
		if (settings.count(TSetType::image)) { return getImage(); }
		else { return nullptr; }
		break;
	case 23:
		if (settings.count(TSetType::character)) { return getCharacter(); }
		else { return nullptr; }
		break;
	case 24: break;
	case 25: break;
	case 26:
		if (settings.count(TSetType::scatter)) { return getScatter(); }
		else { return nullptr; }
		break;
	case 27:
		if (settings.count(TSetType::color)) { return getColor(); }
		else { return nullptr; }
		break;
	case 28:
		if (settings.count(TSetType::alpha)) { return getAlpha(); }
		else { return nullptr; }
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
	case 50:
		if (settings.count(TSetType::vector)) { return getVector(); }
		else { return nullptr; }
		break;
	case 51:
		if (settings.count(TSetType::polygon)) { return getPolygon(); }
		else { return nullptr; }
		break;
	case 52:
		if (settings.count(TSetType::polyline)) { return getPolyLine(); }
		else { return nullptr; }
		break;
	case 53:
		// getMesh
	case 54:
		if (settings.count(TSetType::vortex)) { return getVortex(); }
		else { return nullptr; }
		break;
	case 55:
		if (settings.count(TSetType::fan)) { return getFan(); }
		else { return nullptr; }
		break;
	case 56:
		if (settings.count(TSetType::rake)) { return getRake(); }
		else { return nullptr; }
		break;
	case 57: break;
	case 58: break;
	case 59: break;
	case 60:
		if (settings.count(TSetType::field)) { return getField(); }
		else { return nullptr; }
		break;
	case 61: break;
	case 62: break;
	case 63: break;
	case 70: break;
	case 71:
		if (settings.count(TSetType::spline)) { return getSpline(); }
		else { return nullptr; }
		break;
	case 76: break;
	case 80:
		if (settings.count(TSetType::effects)) { return getEffects(); }
		else { return nullptr; }
		break;
	case 81:
	case 82:
	case 83:
	case 84:
	case 85:
	case 86:
	case 87:
	case 88:
	case 89:
	case 90:
	case 91:
	case 95:
		break;
	case 98: break;
	case 99: break;
	}
	return nullptr;
}

std::shared_ptr<CObject> Tool::putInterest(
	std::shared_ptr<CInterpreter> interpreter, TSetType interest, std::shared_ptr<CObject> object, bool isGet)
{
	switch (interest)
	{
	case TSetType::metadata:
		// Meta-Data Settings - Cannot enable/disable
		return std::make_shared<CObject>(true);
		break;
	case TSetType::zoom:
		if (settings.count(TSetType::zoom))
		{
			if (!isGet) { getZoom()->isEnabled = std::get<bool>(object.get()->obj); }
			return std::make_shared<CObject>(getZoom()->isEnabled);
		}
		else { return std::make_shared<CObject>(nullptr); }
		break;
	case TSetType::image:
		if (settings.count(TSetType::image))
		{
			if (!isGet) { getImage()->isEnabled = std::get<bool>(object.get()->obj); }
			return std::make_shared<CObject>(getImage()->isEnabled);
		}
		else { return std::make_shared<CObject>(nullptr); }
		break;
	case TSetType::character:
		if (settings.count(TSetType::character))
		{
			if (!isGet) { getCharacter()->isEnabled = std::get<bool>(object.get()->obj); }
			return std::make_shared<CObject>(getCharacter()->isEnabled);
		}
		else { return std::make_shared<CObject>(nullptr); }
		break;
	case TSetType::scatter: 
		if (settings.count(TSetType::scatter))
		{
			if (!isGet) { getScatter()->isEnabled = std::get<bool>(object.get()->obj); }
			return std::make_shared<CObject>(getScatter()->isEnabled);
		}
		else { return std::make_shared<CObject>(nullptr); }
		break;
	case TSetType::color:
		if (settings.count(TSetType::color))
		{
			if (!isGet) { getColor()->isEnabled = std::get<bool>(object.get()->obj); }
			return std::make_shared<CObject>(getColor()->isEnabled);
		}
		else { return std::make_shared<CObject>(nullptr); }
		break;
	case TSetType::alpha:
		if (settings.count(TSetType::alpha))
		{
			if (!isGet) { getAlpha()->isEnabled = std::get<bool>(object.get()->obj); }
			return std::make_shared<CObject>(getAlpha()->isEnabled);
		}
		else { return std::make_shared<CObject>(nullptr); }
		break;
	case TSetType::effects:
		if (settings.count(TSetType::effects))
		{
			if (!isGet) { getEffects()->isEnabled = std::get<bool>(object.get()->obj); }
			return std::make_shared<CObject>(getEffects()->isEnabled);
		}
		else { return std::make_shared<CObject>(nullptr); }
		break;
	}
	return std::make_shared<CObject>(nullptr);
}

std::shared_ptr<CObject> Tool::putProperty(
	std::shared_ptr<CInterpreter> interpreter, int settingsSig, 
	int subSig, std::shared_ptr<CObject> object, bool isGet, 
	bool getAsPercentage, bool asString)
{
	int coreSelect = settingsSig / 100;

	switch (coreSelect)
	{
	case 0:
		// Meta-Data Settings
		return putMetaData(interpreter, settingsSig, subSig, object, isGet, asString);
		break;
	case 1:
		if (settings.count(TSetType::pan))
		{
			return getPan()->putProperty(
				interpreter, shared_from_this(),
				settingsSig, subSig, object,
				isGet, getAsPercentage, asString);
		}
		else { return std::make_shared<CObject>(nullptr); }
		break;
	case 2:
		if (settings.count(TSetType::zoom))
		{
			return getZoom()->putProperty(
				interpreter, shared_from_this(), 
				settingsSig, subSig, object, 
				isGet, getAsPercentage, asString);
		}
		else { return std::make_shared<CObject>(nullptr); }
		break;
	case 3:
		if (settings.count(TSetType::rotate))
		{
			return getRotate()->putProperty(
				interpreter, shared_from_this(),
				settingsSig, subSig, object,
				isGet, getAsPercentage, asString);
		}
		else { return std::make_shared<CObject>(nullptr); }
		break;
	case 4:
		if (settings.count(TSetType::continuous))
		{
			return getContinuousControl()->putProperty(
				interpreter, shared_from_this(), 
				settingsSig, subSig, object, 
				isGet, getAsPercentage, asString);
		}
		else { return std::make_shared<CObject>(nullptr); }
		break;
	case 5:
		if (settings.count(TSetType::drag))
		{
			return getDragControl()->putProperty(
				interpreter, shared_from_this(),
				settingsSig, subSig, object,
				isGet, getAsPercentage, asString);
		}
		else { return std::make_shared<CObject>(nullptr); }
		break;
	case 6:
		if (settings.count(TSetType::onepoint))
		{
			return getOnePointControl()->putProperty(
				interpreter, shared_from_this(),
				settingsSig, subSig, object,
				isGet, getAsPercentage, asString);
		}
		else { return std::make_shared<CObject>(nullptr); }
		break;
	case 7:
		if (settings.count(TSetType::twopoint))
		{
			return getTwoPointControl()->putProperty(
				interpreter, shared_from_this(),
				settingsSig, subSig, object,
				isGet, getAsPercentage, asString);
		}
		else { return std::make_shared<CObject>(nullptr); }
		break;
	case 8:
		if (settings.count(TSetType::threepoint))
		{
			return getThreePointControl()->putProperty(
				interpreter, shared_from_this(),
				settingsSig, subSig, object,
				isGet, getAsPercentage, asString);
		}
		else { return std::make_shared<CObject>(nullptr); }
		break;
	case 9:
		if (settings.count(TSetType::sampler))
		{
			return getSampler()->putProperty(
				interpreter, shared_from_this(),
				settingsSig, subSig, object,
				isGet, getAsPercentage, asString);
		}
		else { return std::make_shared<CObject>(nullptr); }
		break;
	case 20:
		if (settings.count(TSetType::blend))
		{
			return getBlend()->putProperty(
				interpreter, shared_from_this(),
				settingsSig, subSig, object,
				isGet, getAsPercentage, asString);
		}
		else { return std::make_shared<CObject>(nullptr); }
		break;
	case 21:
		if (settings.count(TSetType::basic))
		{
			return getBasic()->putProperty(
				interpreter, shared_from_this(),
				settingsSig, subSig, object,
				isGet, getAsPercentage, asString);
		}
		else { return std::make_shared<CObject>(nullptr); }
		break;
	case 22:
		if (settings.count(TSetType::image)) 
		{
			return getImage()->putProperty(
				interpreter, shared_from_this(), 
				settingsSig, subSig, object, 
				isGet, getAsPercentage, asString);
		}
		else { return std::make_shared<CObject>(nullptr); }
		break;
	case 23:
		if (settings.count(TSetType::character))
		{
			return getCharacter()->putProperty(
				interpreter, shared_from_this(),
				settingsSig, subSig, object,
				isGet, getAsPercentage, asString);
		}
		else { return std::make_shared<CObject>(nullptr); }
		break;
	case 24: break;
	case 25: break;
	case 26:
		if (settings.count(TSetType::scatter))
		{
			return getScatter()->putProperty(
				interpreter, shared_from_this(), 
				settingsSig, subSig, object, 
				isGet, getAsPercentage, asString);
		}
		else { return std::make_shared<CObject>(nullptr); }
		break;
	case 27:
		if (settings.count(TSetType::color))
		{
			return getColor()->putProperty(
				interpreter, shared_from_this(), 
				settingsSig, subSig, object, 
				isGet, getAsPercentage, asString);
		}
		else { return std::make_shared<CObject>(nullptr); }
		break;
	case 28:
		if (settings.count(TSetType::alpha))
		{
			return getAlpha()->putProperty(
				interpreter, shared_from_this(), 
				settingsSig, subSig, object, 
				isGet, getAsPercentage, asString);
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
	case 50:
		if (settings.count(TSetType::vector))
		{
			return getVector()->putProperty(
				interpreter, shared_from_this(),
				settingsSig, subSig, object,
				isGet, getAsPercentage, asString);
		}
		else { return std::make_shared<CObject>(nullptr); }
		break;
	case 51:
		if (settings.count(TSetType::polygon))
		{
			return getPolygon()->putProperty(
				interpreter, shared_from_this(),
				settingsSig, subSig, object,
				isGet, getAsPercentage, asString);
		}
		else { return std::make_shared<CObject>(nullptr); }
		break;
	case 52:
		if (settings.count(TSetType::polyline))
		{
			return getPolyLine()->putProperty(
				interpreter, shared_from_this(),
				settingsSig, subSig, object,
				isGet, getAsPercentage, asString);
		}
		else { return std::make_shared<CObject>(nullptr); }
		break;
	case 53:
		// getMesh
	case 54:
		if (settings.count(TSetType::vortex))
		{
			return getVortex()->putProperty(
				interpreter, shared_from_this(),
				settingsSig, subSig, object,
				isGet, getAsPercentage, asString);
		}
		else { return std::make_shared<CObject>(nullptr); }
		break;
	case 55:
		if (settings.count(TSetType::fan))
		{
			return getFan()->putProperty(
				interpreter, shared_from_this(),
				settingsSig, subSig, object,
				isGet, getAsPercentage, asString);
		}
		else { return std::make_shared<CObject>(nullptr); }
		break;
	case 56:
		if (settings.count(TSetType::rake))
		{
			return getRake()->putProperty(
				interpreter, shared_from_this(),
				settingsSig, subSig, object,
				isGet, getAsPercentage, asString);
		}
		else { return std::make_shared<CObject>(nullptr); }
		break;
	case 57: break;
	case 58: break;
	case 59: break;
	case 60:
		if (settings.count(TSetType::field))
		{
			return getField()->putProperty(
				interpreter, shared_from_this(),
				settingsSig, subSig, object,
				isGet, getAsPercentage, asString);
		}
		else { return std::make_shared<CObject>(nullptr); }
		break;
	case 61: break;
	case 62: break;
	case 63: break;
	case 70: break;
	case 76: break;
	case 80:
	case 81:
	case 82:
	case 83:
	case 84:
	case 85:
	case 86:
	case 87:
	case 88:
	case 89:
	case 90:
	case 91:
	case 95:
		// All FX related settings are dispatched from Effects
		if (settings.count(TSetType::effects))
		{
			return getEffects()->putProperty(
				interpreter, shared_from_this(),
				settingsSig, subSig, object,
				isGet, getAsPercentage, asString);
		}
		else { return std::make_shared<CObject>(nullptr); }
		break;
	case 98: break;
	case 99: break;
	}
	return std::make_shared<CObject>(nullptr);
}

std::shared_ptr<CObject> Tool::putMetaData(
	std::shared_ptr<CInterpreter> interpreter, int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asString)
{
	int value = 0;
	std::string string = "";
	TSetType type = TSetType::null;
	MethodType method = MethodType::in_noInput;
	if (!isGet && (object.get()->obj.index() == 0 || 
		object.get()->objType.type == CLiteralTypes::_CNil)) 
	{ 
		return std::make_shared<CObject>(nullptr); 
	}

	switch (settingsSig)
	{
	case 0:
		if (!isGet) 
		{ 
			// Suggestion : Add an ID validator here to check duplicate ID numbers
			id = (int)std::get<double>(object.get()->obj); 
		}
		else
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(id, settingsSig, subSig, "id")); }
			return std::make_shared<CObject>((double)id);
		}
		break;
	case 1:
		if (!isGet)
		{
		}
		else
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(inID, settingsSig, subSig, "inID")); }
			return std::make_shared<CObject>((double)inID);
		}
		break;
	case 2:
		if (!isGet)
		{
		}
		else
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(outID, settingsSig, subSig, "outID")); }
			return std::make_shared<CObject>((double)outID);
		}
		break;
	case 3:
		if (!isGet)
		{
			name = std::get<std::string>(object.get()->obj);
		}
		else
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(name, settingsSig, subSig, "name")); }
			return std::make_shared<CObject>(name);
		}
		break;
	case 8:
		if (!isGet)
		{
			useTipAsIcon = std::get<bool>(object.get()->obj);
		}
		else
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(useTipAsIcon, settingsSig, subSig, "useTipAsIcon")); }
			return std::make_shared<CObject>(useTipAsIcon);
		}
		break;
	case 15:
		if (!isGet)
		{
			tags = stringVec_fromCommaSeparated(std::get<std::string>(object.get()->obj), false);
		}
		else
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(tags, settingsSig, subSig, "tags")); }
			return std::make_shared<CObject>(tags);
		}
		break;
	case 16:
		if (!isGet)
		{
			category = std::get<std::string>(object.get()->obj);
		}
		else
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(category, settingsSig, subSig, "category")); }
			return std::make_shared<CObject>(category);
		}
		break;
	case 17:
		if (!isGet)
		{
			toolType = std::get<std::string>(object.get()->obj);
		}
		else
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(toolType, settingsSig, subSig, "toolType")); }
			return std::make_shared<CObject>(toolType);
		}
		break;
	case 18:
		if (!isGet)
		{
			date = std::get<std::string>(object.get()->obj);
		}
		else
		{
			// Note : Adding "" around the date for safety when pulling as a string value for writing .ctf files
			if (asString) { return std::make_shared<CObject>(makeSettingString(date, settingsSig, subSig, "date")); }
			return std::make_shared<CObject>(date);
		}
		break;
	case 19:
		if (!isGet)
		{
			author = std::get<std::string>(object.get()->obj);
		}
		else
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(author, settingsSig, subSig, "author")); }
			return std::make_shared<CObject>(author);
		}
		break;
	// Key Bind Type
	case 20:
		if (!isGet)
		{
			bindType = (int)std::get<double>(object.get()->obj);
		}
		else
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(bindType, settingsSig, subSig, "bindType")); }
			return std::make_shared<CObject>((double)bindType);
		}
		break;
	// Key Bindings
	case 21:
		if (!isGet)
		{
			keybind = intToKeybind((int)std::get<double>(object.get()->obj));
		}
		else
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(keybind, settingsSig, subSig, "keybind")); }
			return std::make_shared<CObject>(keybindToString(keybind));
		}
		break;
	case 24:
		if (!isGet)
		{
			cursorUp = owner.get()->getCursor(convertCursorName(stringToLower(std::get<std::string>(object.get()->obj))));
			owner.get()->notifyCursorChange(false, true);
		}
		else
		{
			string = cursorUp.get()->getName();
			if (asString) { return std::make_shared<CObject>(makeSettingString(string, settingsSig, subSig, "cursorUp")); }
			return std::make_shared<CObject>(stringToUpper(string));
		}
		break;
	case 25:
		if (!isGet)
		{
			cursorDown = owner.get()->getCursor(convertCursorName(stringToLower(std::get<std::string>(object.get()->obj))));
			owner.get()->notifyCursorChange(true, false);
		}
		else
		{
			string = cursorDown.get()->getName();
			if (asString) { return std::make_shared<CObject>(makeSettingString(string, settingsSig, subSig, "cursorDown")); }
			return std::make_shared<CObject>(stringToUpper(string));
		}
		break;
	case 30:
		if (!isGet)
		{
			
		}
		else
		{
			string = input.get()->getName();
			if (asString) { return std::make_shared<CObject>(makeSettingString(string, settingsSig, subSig, "input")); }
			return std::make_shared<CObject>(stringToUpper(string));
		}
		break;
	case 31:
		if (!isGet)
		{

		}
		else
		{
			string = output.get()->getName();
			if (asString) { return std::make_shared<CObject>(makeSettingString(string, settingsSig, subSig, "output")); }
			return std::make_shared<CObject>(stringToUpper(string));
		}
		break;
	case 32:
		if (!isGet)
		{

		}
		else
		{
			type = input.get()->controlScheme;
			method = input.get()->type;
			string = "NULL";
			switch (method)
			{
			case MethodType::in_pan: string = "PAN"; break;
			case MethodType::in_zoom: string = "ZOOM"; break;
			case MethodType::in_rotate: string = "ROTATE"; break;
			case MethodType::in_sampler: string = "SAMPLER"; break;
			default:
				switch (type)
				{
				case TSetType::continuous: string = "CONTINUOUS"; break;
				case TSetType::drag: string = "DRAG"; break;
				case TSetType::onepoint: string = "ONE POINT"; break;
				case TSetType::twopoint: string = "TWO POINT"; break;
				case TSetType::threepoint: string = "THREE POINT"; break;
				}
				break;
			}

			if (asString) { return std::make_shared<CObject>(makeSettingString(string, settingsSig, subSig, "controlScheme")); }
			return std::make_shared<CObject>(string);
		}
		
		break;
	case 41:
		if (!isGet)
		{
			activeSettingsPage = (int)std::round(std::get<double>(object.get()->obj));
		}
		else
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(activeSettingsPage, settingsSig, subSig, "activeSettingsPage")); }
			return std::make_shared<CObject>((double)activeSettingsPage);
		}
		break;
	case 51:
		if (!isGet)
		{
			lockSettings = std::get<bool>(object.get()->obj);
		}
		else
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(lockSettings, settingsSig, subSig, "lockSettings")); }
			return std::make_shared<CObject>(lockSettings);
		}
		break;
	case 52:
		if (!isGet)
		{
			autoSaveSettings = std::get<bool>(object.get()->obj);
		}
		else
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(autoSaveSettings, settingsSig, subSig, "autoSaveSettings")); }
			return std::make_shared<CObject>(autoSaveSettings);
		}
		break;
	}
	return std::make_shared<CObject>(nullptr);
}

// Serialize all tool settings for saving
void Tool::serializeToolData()
{

}

// Return serialized tool data to requester
std::vector<char> Tool::getSerialData()
{
	return serialData;
}