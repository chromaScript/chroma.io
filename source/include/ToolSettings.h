#ifndef TOOLSETTINGS_H
#define TOOLSETTINGS_H

class CFunction;
class CObject;
class CInterpreter;
class Tool;

#include <glm.hpp>
#include <string>
#include <vector>
#include <memory>
#include <map>

#include "structs.h"

class TSetGraph
{
public:
	glm::vec2 start = glm::vec2(0, 0);
	glm::vec2 end = glm::vec2(1, 1);
	std::vector<glm::vec2> controlPoints;
};

enum class TSetProp
{
	null,
	tip,
	shape,
	radius,
	point,
	simple,
	complex,
	allLayers,
	currentLayer,
	currentBelow,
	triange,
	ellipse,
	rectangle,
	sides,
	custom,
	pattern,
	random,
	gaussian,
	uniform,
	useDefault,
	noConnect,
	rings,
	lines,
	ringsLines,
	shuffleOrder,
	draw,
	polyLine,
	polyDraw,
	automatic,
	circular,
	linear,
	axis,
	points,
	scale,
	time,
	distance,
	count,
	triangles,
	fanPoints,
	triangleSubdivision,
	fanSubdivision,
	curves,
	polygon,
	none,
	polyCircle,
	curveCircle,
	customShape,
	red,
	green,
	blue,
	alpha,
	value,
	foregroundColor,
	backgroundColor,
	customColor,
	all,
	allChannels,
	rgb,
	rgba,
	useGiven,
	pointsShuffle,
	arrow,
	arc,
	cone
};

inline std::map<std::string, TSetProp> TSetPropNamesMap = {
	{"null", TSetProp::null },
	{"tip", TSetProp::tip },
	{"shape", TSetProp::shape },
	{"radius", TSetProp::radius },
	{"point", TSetProp::point },
	{"simple", TSetProp::simple },
	{"complex", TSetProp::complex },
	{"all_layers", TSetProp::allLayers },
	{"current_layer", TSetProp::currentLayer },
	{"current_below", TSetProp::currentBelow },
	{"triange", TSetProp::triange },
	{"ellipse", TSetProp::ellipse },
	{"rectangle", TSetProp::rectangle },
	{"sides", TSetProp::sides },
	{"custom", TSetProp::custom },
	{"pattern", TSetProp::pattern },
	{"random", TSetProp::random },
	{"gaussian", TSetProp::gaussian },
	{"uniform", TSetProp::uniform },
	{"use_default", TSetProp::useDefault },
	{"no_connect", TSetProp::noConnect },
	{"rings", TSetProp::rings },
	{"lines", TSetProp::lines },
	{"rings_lines", TSetProp::ringsLines },
	{"shuffle_order", TSetProp::shuffleOrder },
	{"draw", TSetProp::draw },
	{"polyline", TSetProp::polyLine },
	{"polydraw", TSetProp::polyDraw },
	{"automatic", TSetProp::automatic },
	{"circular", TSetProp::circular },
	{"linear", TSetProp::linear },
	{"axis", TSetProp::axis },
	{"points", TSetProp::points },
	{"pattern", TSetProp::pattern },
	{"scale", TSetProp::scale },
	{"time", TSetProp::time },
	{"distance", TSetProp::distance },
	{"count", TSetProp::count },
	{"triangles", TSetProp::triangles },
	{"fan_points", TSetProp::fanPoints },
	{"triangle_subdivision", TSetProp::triangleSubdivision },
	{"fan_subdivision", TSetProp::fanSubdivision },
	{"curves", TSetProp::curves },
	{"polygon", TSetProp::polygon },
	{"none", TSetProp::none },
	{"poly_circle", TSetProp::polyCircle },
	{"curve_circle", TSetProp::curveCircle },
	{"custom_shape", TSetProp::customShape },
	{"red", TSetProp::red },
	{"green", TSetProp::green },
	{"blue", TSetProp::blue },
	{"alpha", TSetProp::alpha },
	{"value", TSetProp::value },
	{"foreground_color", TSetProp::foregroundColor },
	{"background_color", TSetProp::backgroundColor },
	{"custom_color", TSetProp::customColor },
	{"all", TSetProp::all },
	{"all_channels", TSetProp::allChannels },
	{"rgb", TSetProp::rgb },
	{"rgba", TSetProp::rgba },
	{"use_given", TSetProp::useGiven },
	{"points_shuffle", TSetProp::pointsShuffle },
	{"arrow", TSetProp::arrow },
	{"arc", TSetProp::arc },
	{"cone", TSetProp::cone },
};

inline TSetProp stringToTSetProp(std::string query)
{
	std::string search = query;
	stringRemoveSpace(search);
	stringToLower(search);
	if (TSetPropNamesMap.count(search) == 1)
	{
		return TSetPropNamesMap.at(search);
	}
	return TSetProp::null;
}

inline std::string TSetPropToString(TSetProp query)
{
	for (auto& it : TSetPropNamesMap) {
		if (it.second == query) {
			std::string found = it.first;
			return stringToUpper(found);
		}
	}
	return "NULL";
}

// Settings Types : Used for I/O interest masks and Tool -> 'settings'
// Determines what settings are available per I/O method combination
enum class TSetType
{
	metadata,
	pan,
	zoom,
	rotate,
	usedefault,
	continuous,
	drag,
	onepoint,
	twopoint,
	threepoint,
	fourpoint, 
	blend,
	basic,
	image,
	polygon,
	field,
	mesh,
	combo,
	character,
	scatter,
	color,
	alpha,
	distance,
	mixing,
	multiTip,
	texture,
	grain,
	wetEdges,
	repeat,
	reference,
	vector,
	gradient,
	fill,
	pattern,
	shader,
	filter,
	rake,
	fan,
	vortex,
	sampler,
	smoothing,
	antiAliasing,
	polyline,
	effects,
	fx_gradient,
	fx_fill,
	fx_posterize,
	fx_brightnessContrast,
	fx_HSV,
	fx_invert,
	fx_colorRamp,
	fx_exposure,
	fx_colorBalance,
	fx_threshold,
	fx_greyscale,
	fx_blur,
	fx_clampHSV,
	fx_clampRGB,
	fx_modulo,
	fx_power,
	null
};

inline std::map<std::string, TSetType> TSetTypeNamesMap = {
	{"metadata", TSetType::metadata },
	{"pan", TSetType::pan },
	{"zoom", TSetType::zoom },
	{"rotate", TSetType::rotate },
	{"usedefault", TSetType::usedefault },
	{"continuous", TSetType::continuous },
	{"drag", TSetType::drag },
	{"onepoint", TSetType::onepoint },
	{"twopoint", TSetType::twopoint },
	{"threepoint", TSetType::threepoint },
	{"fourpoint", TSetType::fourpoint },
	{"blend", TSetType::blend },
	{"basic", TSetType::basic },
	{"image", TSetType::image },
	{"polygon", TSetType::polygon },
	{"field", TSetType::field },
	{"mesh", TSetType::mesh },
	{"combo", TSetType::combo },
	{"character", TSetType::character },
	{"scatter", TSetType::scatter },
	{"color", TSetType::color },
	{"alpha", TSetType::alpha },
	{"distance", TSetType::distance },
	{"mixing", TSetType::mixing },
	{"multiTip", TSetType::multiTip },
	{"texture", TSetType::texture },
	{"grain", TSetType::grain },
	{"wetEdges", TSetType::wetEdges },
	{"repeat", TSetType::repeat },
	{"reference", TSetType::reference },
	{"vector", TSetType::vector },
	{"gradient", TSetType::gradient },
	{"fill", TSetType::fill },
	{"pattern", TSetType::pattern },
	{"shader", TSetType::shader },
	{"filter", TSetType::filter },
	{"rake", TSetType::rake },
	{"fan", TSetType::fan },
	{"vortex", TSetType::vortex },
	{"sampler", TSetType::sampler },
	{"smoothing", TSetType::smoothing },
	{"antiAliasing", TSetType::antiAliasing },
	{"polyline", TSetType::polyline },
	{"effects", TSetType::effects },
	{"null", TSetType::null }
};

inline TSetType stringToTSetType(std::string query)
{
	if (TSetTypeNamesMap.count(query) == 1)
	{
		return TSetTypeNamesMap.at(query);
	}
	return TSetType::null;
}

// Controllers and Graphs for pen input
class TSetController
{
public:
	bool isEnabled = false;

	bool usePressure = false;
	bool useTilt = false;
	bool useVelocity = false;
	bool useDirection = false;
	bool useFunction = false;

	// Pressure
	bool usePressureGraph = false;
	float pressureMin = 0;
	float pressureMax = 0;
	TSetGraph pressureGraph;

	// Tilt
	bool useTiltGraph = false;
	float tiltMin = 0;
	float tiltMax = 0;
	TSetGraph tiltGraph;

	// Velocity
	bool useVelocityGraph = false;
	float velocityMin = 0;
	float velocityMax = 0;
	TSetGraph velocityGraph;

	// Direction
	bool useDirectionGraph = false;
	bool lockStart = false;
	double lockTime = 0;
	float dirMin = 0;
	float dirMax = 1;
	TSetGraph directionGraph;

	// Function
	float functionMin = 0;
	float functionMax = 0;
	std::shared_ptr<CFunction> function;
};

// Randomizatiton settings
class TSetNoise
{
public:
	bool isEnabled = false;

	bool perTip = true; // false -> perStroke
	float frequency = 0;
	float smoothing = 0;
	float rangeMin = 0;
	float varianceMin = 0;
	float rangeMax = 1;
	float varianceMax = 1;
	std::shared_ptr<CFunction> function;
};

#define TSETTINGSWRITER_P1 std::string("ID=")
#define TSETTINGSWRITER_P2 std::string("::SID=")
#define TSETTINGSWRITER_P3 std::string("::")
#define TSETTINGSWRITER_P4 std::string(" = ")
#define TSETTINGSWRITER_P5 std::string(" ;\n")

std::string makeSettingString(int value, int settingsSig, int subSig, std::string settingName, bool isKey);
std::string makeSettingString(float value, int settingsSig, int subSig, std::string settingName);
std::string makeSettingString(bool value, int settingsSig, int subSig, std::string settingName);
std::string makeSettingString(std::string value, int settingsSig, int subSig, std::string settingName);
std::string makeSettingString(std::vector<std::string> value, int settingsSig, int subSig, std::string settingName);
std::string makeSettingString(std::vector<int> value, int settingsSig, int subSig, std::string settingName);
std::string makeSettingString(TSetProp value, int settingsSig, int subSig, std::string settingName);
std::string makeSettingString(TSetController value, int settingsSig, int subSig, std::string settingName);
std::string makeSettingString(TSetNoise value, int settingsSig, int subSig, std::string settingName);
std::string makeSettingString(TSetGraph value, int settingsSig, int subSig, std::string settingName);
std::string makeSettingString(std::shared_ptr<CFunction> value, int settingsSig, int subSig, std::string settingName);
std::string makeSettingString(std::shared_ptr<CObject> value, int settingsSig, int subSig, std::string settingName);

// Polymorphic ToolSettings Interest Mask Classes
// Stored inside of maps in the Tool class

class ToolSettings
{
public:
	virtual TSetType getType() = 0;
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, int subSig, std::shared_ptr<CObject> object, 
		bool isGet, bool asPercentage, bool asString) = 0;
	bool isEnabled = true;
	TSetType type;
};

#endif
