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
#include <deque>

#include "../math/string.h"
#include "../input/input.h"
#include "../entities/widgets/Graph_ToolControl.h"
#include "../methods/MethodType.h"

class TSetGraph;
class TSetControl_Node;
class TSetController;
class TSetNoise;

enum class TSetProp
{
	// null
	none,
	null,
	// image
	tip,
	shape,
	radius,
	point,
	simple,
	complex,
	// sampling
	allLayers,
	currentLayer,
	currentBelow,
	// shape
	triangle,
	ellipse,
	rectangle,
	sides,
	custom,
	// field distribution
	pattern,
	random,
	uniform,
	// connection type
	useGiven,
	useDefault,
	noConnect,
	perScatter,
	interval,
	rings,
	lines,
	ringsLines,
	shuffleOrder,
	pointsShuffle,
	// misc
	line,
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
	polyCircle,
	curveCircle,
	customShape,
	editHandles,
	// color channels
	red,
	green,
	blue,
	alpha,
	value,
	all,
	allChannels,
	rgb,
	rgba,
	// color modes
	hsl,
	hsv,
	// saved colors
	foreground,
	background,
	// rake shapes
	arrow,
	arc,
	cone,
	// noise intervals
	perShard,
	perAnchor,
	perSpline,
	perEntity,
	// noise
	vorotwist,
	voroblur,
	gaussian,
	perlin,
	voroni,
	simplex,
	fractal,
	grunge,
	canvas,
	cellularA,
	cellularB,
	halftoneDots,
	maze,
	round,
	// numbers
	zero,
	one,
	two,
	three,
	four,
};

inline std::map<std::string, TSetProp> TSetPropNamesMap = {
	// null
	{"none", TSetProp::none },
	{"null", TSetProp::null },
	// image
	{"tip", TSetProp::tip },
	{"shape", TSetProp::shape },
	{"radius", TSetProp::radius },
	{"point", TSetProp::point },
	{"simple", TSetProp::simple },
	{"complex", TSetProp::complex },
	// sampling
	{"alllayers", TSetProp::allLayers },
	{"currentlayer", TSetProp::currentLayer },
	{"currentbelow", TSetProp::currentBelow },
	// shape
	{"triangle", TSetProp::triangle },
	{"ellipse", TSetProp::ellipse },
	{"rectangle", TSetProp::rectangle },
	{"sides", TSetProp::sides },
	{"custom", TSetProp::custom },
	// field distribution
	{"pattern", TSetProp::pattern },
	{"random", TSetProp::random }, // also noise
	{"uniform", TSetProp::uniform },
	// connection type
	{"usegiven", TSetProp::useGiven },
	{"usedefault", TSetProp::useDefault },
	{"noconnect", TSetProp::noConnect },
	{"perscatter", TSetProp::perScatter },
	{"interval", TSetProp::interval },
	{"rings", TSetProp::rings },
	{"lines", TSetProp::lines },
	{"ringslines", TSetProp::ringsLines },
	{"shuffleorder", TSetProp::shuffleOrder },
	{"pointsshuffle", TSetProp::pointsShuffle },
	// misc
	{"line", TSetProp::line },
	{"draw", TSetProp::draw },
	{"polyline", TSetProp::polyLine },
	{"polydraw", TSetProp::polyDraw },
	{"automatic", TSetProp::automatic },
	{"circular", TSetProp::circular },
	{"linear", TSetProp::linear },
	{"axis", TSetProp::axis },
	{"points", TSetProp::points },
	{"scale", TSetProp::scale },
	{"time", TSetProp::time },
	{"distance", TSetProp::distance },
	{"count", TSetProp::count },
	{"triangles", TSetProp::triangles },
	{"fanpoints", TSetProp::fanPoints },
	{"trianglesubdivision", TSetProp::triangleSubdivision },
	{"fansubdivision", TSetProp::fanSubdivision },
	{"curves", TSetProp::curves },
	{"polygon", TSetProp::polygon },
	{"polycircle", TSetProp::polyCircle },
	{"curvecircle", TSetProp::curveCircle },
	{"customshape", TSetProp::customShape },
	{"editHandles", TSetProp::editHandles },
	// color channels
	{"red", TSetProp::red },
	{"green", TSetProp::green },
	{"blue", TSetProp::blue },
	{"alpha", TSetProp::alpha },
	{"value", TSetProp::value },
	{"all", TSetProp::all },
	{"allchannels", TSetProp::allChannels },
	{"rgb", TSetProp::rgb },
	{"rgba", TSetProp::rgba },
	// color modes
	{"hsl", TSetProp::hsl },
	{"hsv", TSetProp::hsv },
	// saved colors
	{"foreground", TSetProp::foreground },
	{"background", TSetProp::background },
	// rake shapes
	{"arrow", TSetProp::arrow },
	{"arc", TSetProp::arc },
	{"cone", TSetProp::cone },
	// noise interval
	{"pershard", TSetProp::perShard },
	{"peranchor", TSetProp::perAnchor },
	{"perspline", TSetProp::perSpline },
	{"perentity", TSetProp::perEntity },
	// noise
	{"gaussian", TSetProp::gaussian },
	{"perlin", TSetProp::perlin },
	{"voroni", TSetProp::voroni },
	{"voroblur", TSetProp::voroblur },
	{"vorotwist", TSetProp::vorotwist },
	{"simplex", TSetProp::simplex },
	{"fractal", TSetProp::fractal },
	{"grunge", TSetProp::grunge },
	{"fractal", TSetProp::fractal },
	{"canvas", TSetProp::canvas },
	{"cellulara", TSetProp::cellularA },
	{"cellularb", TSetProp::cellularB },
	{"halftonedots", TSetProp::halftoneDots },
	{"maze", TSetProp::maze },
	{"round", TSetProp::round },
	// numbers
	{ "0", TSetProp::zero },
	{ "1", TSetProp::one },
	{ "2", TSetProp::two },
	{ "3", TSetProp::three },
	{ "4", TSetProp::four }
};

inline std::map<TSetProp, std::string> TSetPropStringMap = {
	// null
	{ TSetProp::none, "None" },
	{ TSetProp::null, "Null" },
	// image
	{ TSetProp::tip, "Tip" },
	{ TSetProp::shape, "Shape" },
	{ TSetProp::radius, "Radius" },
	{ TSetProp::point, "Point" },
	{ TSetProp::simple, "Simple" },
	{ TSetProp::complex, "Complex" },
	// sampling
	{ TSetProp::allLayers, "All Layers" },
	{ TSetProp::currentLayer, "Current Layer" },
	{ TSetProp::currentBelow, "Current Below" },
	// shape
	{ TSetProp::triangle, "Triangle" },
	{ TSetProp::ellipse, "Ellipse" },
	{ TSetProp::rectangle, "Rectangle" },
	{ TSetProp::sides, "Sides" },
	{ TSetProp::custom, "Custom" },
	// field distribution
	{ TSetProp::pattern, "Pattern" },
	{ TSetProp::random, "Random" },
	{ TSetProp::uniform, "Uniform" },
	// connection type
	{ TSetProp::useGiven, "UseGiven" },
	{ TSetProp::useDefault, "Use Default" },
	{ TSetProp::noConnect, "No Connect" },
	{ TSetProp::rings, "Rings" },
	{ TSetProp::lines, "Lines" },
	{ TSetProp::ringsLines, "Rings Lines" },
	{ TSetProp::shuffleOrder, "Shuffle Order" },
	{ TSetProp::pointsShuffle, "Points Shuffle" },
	// misc
	{ TSetProp::line, "Line" },
	{ TSetProp::draw, "Draw" },
	{ TSetProp::polyLine, "Poly Line" },
	{ TSetProp::polyDraw, "Poly Draw" },
	{ TSetProp::automatic, "Automatic" },
	{ TSetProp::circular, "Circular" },
	{ TSetProp::linear, "Linear" },
	{ TSetProp::axis, "Axis" },
	{ TSetProp::points, "Points" },
	{ TSetProp::scale, "Scale" },
	{ TSetProp::time, "Time" },
	{ TSetProp::distance, "Distance" },
	{ TSetProp::count, "Count" },
	{ TSetProp::triangles, "Triangles" },
	{ TSetProp::fanPoints, "Fan Points" },
	{ TSetProp::triangleSubdivision, "Triangle Subdivision" },
	{ TSetProp::fanSubdivision, "Fan Subdivision" },
	{ TSetProp::curves, "Curves" },
	{ TSetProp::polygon, "Polygon" },
	{ TSetProp::polyCircle, "Poly Circle" },
	{ TSetProp::curveCircle, "Curve Circle" },
	{ TSetProp::customShape, "Custom Shape" },
	{ TSetProp::editHandles, "Edit Handles" },
	// color channels
	{ TSetProp::red, "Red" },
	{ TSetProp::green, "Green" },
	{ TSetProp::blue, "Blue" },
	{ TSetProp::alpha, "Alpha" },
	{ TSetProp::value, "Value" },
	{ TSetProp::all, "All" },
	{ TSetProp::allChannels, "All Channels" },
	{ TSetProp::rgb, "rgb" },
	{ TSetProp::rgba, "rgba" },
	// color modes
	{ TSetProp::hsl, "hsl" },
	{ TSetProp::hsv, "hsv" },
	// saved colors
	{ TSetProp::foreground, "Foreground" },
	{ TSetProp::background, "Background" },
	// rake shapes
	{ TSetProp::arrow, "Arrow" },
	{ TSetProp::arc, "Arc" },
	{ TSetProp::cone, "Cone" },
	// noise interval
	{ TSetProp::perShard, "Per Shard" },
	{ TSetProp::perAnchor, "Per Anchor" },
	{ TSetProp::perSpline, "Per Spline" },
	{ TSetProp::perEntity, "Per Entity" },
	// noise
	{ TSetProp::gaussian, "Gaussian" },
	{ TSetProp::perlin, "Perlin" },
	{ TSetProp::voroni, "Voroni" },
	{ TSetProp::voroblur, "Voroblur" },
	{ TSetProp::vorotwist, "Vorotwist" },
	{ TSetProp::simplex, "Simplex" },
	{ TSetProp::fractal, "Fractal" },
	{ TSetProp::grunge, "Grunge" },
	{ TSetProp::fractal, "Fractal" },
	{ TSetProp::canvas, "Canvas" },
	{ TSetProp::cellularA, "Cellular A" },
	{ TSetProp::cellularB, "Cellular B" },
	{ TSetProp::halftoneDots, "Halftone Dots" },
	{ TSetProp::maze, "Maze" },
	{ TSetProp::round, "Round" },
	// numbers
	{ TSetProp::zero, "0" },
	{ TSetProp::one, "1" },
	{ TSetProp::two, "2" },
	{ TSetProp::three, "3" },
	{ TSetProp::four, "4" }
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
	if (TSetPropStringMap.count(query) == 1)
	{
		std::string result = TSetPropStringMap.at(query);
		return stringToUpper(result);
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
	spline,
	multiSpline,
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
	{"null", TSetType::null },
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
	{"effects", TSetType::effects }
};

inline TSetType stringToTSetType(std::string query)
{
	if (TSetTypeNamesMap.count(query) == 1)
	{
		return TSetTypeNamesMap.at(query);
	}
	return TSetType::null;
}

class TSetGraph
{
public:
	std::string presetName = ""; // 0
	glm::vec2 start = glm::vec2(0, 0); // 1
	glm::vec2 end = glm::vec2(1, 1); // 2
	int handleCount = 0; // 3
	float dampeningValue = 1.0f; // 4
	float controlPoints[32] = { // 10 - 31
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
	};
	bool flipOutput = false; // 50

	// Internal Variables
	int subSigOffset = 0;

	TSetGraph() {};
	TSetGraph(int subSigOffset);
	std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	void updateControlPoints();
	void copyWidgetData(Graph_ToolControl* widget);
	float modulateInput(float* input);
	void resetGraph();
	void flipGraph();
	void shiftGraph(float amountX, float amountY);
	void scaleGraph(float amount);
};

#define TSETCONTROL_NODE_LOCK_FREE 0
#define TSETCONTROL_NODE_LOCK_OPPOSITE 1
#define TSETCONTROL_NODE_LOCK_SAME 2

class TSetControl_Node
{
public:
	// Numbers listed next to these are the SubSigID
	std::string nodeName = ""; // 0
	bool isEnabled = true; // 1

	int lockingMode = TSETCONTROL_NODE_LOCK_FREE; // 2
	float controlMin = 0.0f; //  3
	float controlMax = 1.0f; // 4

	bool centerRange = false;
	float rangeMin = 0.0f; // Internal Variable
	float rangeMax = 1.0f; // Internal Variable

	bool usePressure = true; // 10
	bool useDirection = false; // 11
	bool useTilt = false; // 12
	bool useVelocity = false; // 13
	bool useRotation = false; // 14
	bool useFunction = false; // 15

	bool initialDirFound = false; // Internal Variable
	int watchCount = -1;
	glm::vec3 watchDir = glm::vec3(1.0f); // Internal Variable
	glm::vec3 initialDir = glm::vec3(0.0f); // Internal Variable
	bool loopDirValue = true;

	TSetControl_Node() {};
	TSetControl_Node(float rangeMin, float rangeMax, bool centerRange, std::string nodeName);

	std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	bool reportEnabled();
	void calculateControl(float& outValue, 
		glm::vec3* pos, glm::vec3* dir, 
		Input* input, TSetController* controller, int* anchorCount);
};

// Controllers and Graphs for pen input
class TSetController
{
public:
	// Access the graphs by using subSig of XYY where X is the index of the graph type and YY is the graph setting address. Must be 5 digits

	// Pressure // Graph Return -1
	TSetGraph pressureGraph = TSetGraph(100); // 100 - 199

	// Direction // Graph Return -2
	float dirRotationOffset = 0.0f; // 2
	bool useInitialDir = false; // 3
	TSetGraph directionGraph = TSetGraph(200); // 200 - 299

	// Tilt // Graph Return -3
	TSetGraph tiltGraph = TSetGraph(300);  // 300 - 399

	// Velocity // Graph Return -4
	float velocityCap = 1.0f; // 4
	TSetGraph velocityGraph = TSetGraph(400);  // 400 - 499

	// Rotation // Graph Return -5
	float rotationOffset = 0.0f; // 5
	TSetGraph rotationGraph = TSetGraph(500);  // 500 - 599

	// Function
	std::string fieldValue = ""; // 600
	std::string functionString = ""; // 601
	std::shared_ptr<CFunction> function; // Hidden Setting

	std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
};

// Randomizatiton settings
class TSetNoise
{
public:
	std::string presetName = ""; // 0
	bool isEnabled = false; // 1

	
	bool clampNoiseToRange = true; // 3 // Whether noise can cause the final setting value to escape the controller's min/max values
	unsigned int seed = 0; // 4 // A seed value - May not implement.
	bool randomSeed = false; // 6
	BlendMode totalMixMode = BlendMode::multiply; // 7 - Mixing func used to mix the pre-mixed 1D & 2D values into the input value
	float noiseMin = 0.0f; // 8 // Total mixed noise output range. Always in 0 to 1 space
	float noiseMax = 1.0f; // 9

	// 1D Noise Values
	bool useNoise_1D = false; // 10
	TSetProp intervalMode = TSetProp::perShard; // 11
	bool intervalSmoothing = false; // 12
	float weight = 0.5f; // 13 // Sets the mid-point of the distribution
	float intensity = 0.0f; // 14 // Sets the intensity of re-mapping to a weighted distribution
	float smoothing = 0.0f; // 15 // Smoothing value to average previous noise values
	float mixAmount_1D = 1.0f; // 16 // Final mixing amount of the 1D noise value
	BlendMode noiseMixMode = BlendMode::multiply; // 17 // A blending mode to either mix between 1D and 2D, or mix between 1D, 2D, and the incoming value
	bool apply1DFirst = true; // 18

	// 1D Internal Values
	bool centerValRange = false;
	float finalValStatic = 0.0f; // Use this number when doing 1D perTip noise
	float finalValMin = 0.0f;
	float finalValMax = 1.0f;
	int bufferSize = 10;
	//std::vector<std::tuple<float, float, float>> buffer; // Where the arrangement is "noiseValue", "distanceTotal", "distanceDelta"
	std::vector<float> buffer;
	
	// 2D Noise Values
	bool useNoise_2D = false;  // 20
	TSetProp noiseMode = TSetProp::perlin;  // 21
	bool useOriginOffset = false;  // 22 // Whether to use the first shard position as the 0,0 point. If false, use the global canvas coordinates
	bool scaleToCanvas = false; // 23 // Whether to scale to canvas size
	bool scaleToToolSize = false; // 24 // Whether to scale to the active tool's current size. Combines with scaleToCanvas for final scale value
	glm::vec2 canvasCenter = glm::vec2(0.0f, 0.0f);
	float factorA = 1.0f; // 25 // A free variable that can be plugged into any of the noise values. Each noise type would have it's own implementation of the factor
	float factorB = 1.0f; // 26
	float factorC = 1.0f; // 27
	bool rotateNoise = false; // 28 // Whether to rotate the coordinates being fed into the noise
	float rotation = 0.0f; // 29
	float blackValue = 0.0f; // 30
	float whiteValue = 1.0f; // 31
	float brightness = 0.0f; // 32 // Amount by which to brighten/darken the noise
	float contrast = 0.0f; // 33 // Amount by which to contrast the noise
	float valueRangeMin = 0.0f; // 34 // Values to range clamp the resulting noise, with a third value to determine the smoothstep value
	float valueRangeMax = 1.0f; // 35
	float valueRangeSmooth = 1.0f; // 36
	bool invertValue = false; // 37 // Invert the value range at the end of calculation
	bool flipX = false; // 38
	bool flipY = false; // 39
	float mixAmount_2D = 1.0f; // 40 // Final mixing amount of the 2D noise value

	float scaleX = 1.0f; // 41 // Global scaling value for the noise
	float scaleY = 1.0f; // 42 // Global scaling value for the noise
	float skew = 0.0f; // 43
	float finalScale = 1.0f; // Internal variable

	// Function Settings
	std::string fieldValue = "void()"; // 50
	std::string functionString = ""; // 51
	bool useFunc = false; // 52
	bool runFuncFirst = false; // 53
	std::shared_ptr<CFunction> function;

	// Gen
	std::string noiseName = ""; // 60

	// 1D Continued
	int shardInterval = 1; // 70
	int anchorInterval = 1; // 71
	int splineInterval = 1; // 72
	int entityInterval = 1; // 73
	bool centerFinalNoise = false; // 74
	int lastShardCount = -1;
	int lastAnchorCount = -1;
	int lastSplineCount = -1;
	float lastInvervalValue = 1.0f;
	float nextIntervalValue = 1.0f;

	TSetNoise() {};
	TSetNoise(float finalRangeMin, float finalRangeMax, bool centerFinalRange, std::string noiseName);

	std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	void initialize(float* trueSpacing, int* entityCount, glm::ivec2* canvasDimensions);

	bool reportEnabled();

	void generateRandSeed();
	void calculateBufferSize(float trueSpacing);
	void fillBuffer();
	void calculateFinalScale(glm::ivec2* canvasDimensions);
	glm::vec2 getFinalScale();
	void calculateNoise(
		float& outValue, glm::vec3* pos, glm::vec3* origin, glm::vec3* dir, 
		Input* input, int* shardCount, int* anchorCount, int* splineCount, bool shuffleSeed);
	glm::vec2 adjustPos(glm::vec3* pos, glm::vec3* origin, glm::vec3* dir);
	float calculate1DNoise(glm::vec3* pos, glm::vec3* origin, glm::vec3* dir, 
		int* shardCount, int* anchorCount, int* splineCount, bool shuffleSeed);
	float generate1DNoise(float& outNoise, glm::vec3* pos, glm::vec3* origin, glm::vec3* dir, int* shuffle, bool shuffleSeed);
	float smooth1DNoise(float newNoise);
	float calculate2DNoise(glm::vec3* pos, glm::vec3* origin, glm::vec3* dir);
	float calculateFuncNoise(float& inValue, glm::vec3* pos, Input* input);
};

#define TSETTINGSWRITER_P1 std::string("ID=")
#define TSETTINGSWRITER_P2 std::string("::SID=")
#define TSETTINGSWRITER_P3 std::string("::")
#define TSETTINGSWRITER_P4 std::string(" = ")
#define TSETTINGSWRITER_P5 std::string(" ;\n")

std::string makeSettingString(Keybind value, int settingsSig, int subSig, std::string settingName);
std::string makeSettingString(MethodType value, int settingsSig, int subSig, std::string settingName);
std::string makeSettingString(int value, int settingsSig, int subSig, std::string settingName);
std::string makeSettingString(float value, int settingsSig, int subSig, std::string settingName);
std::string makeSettingString(bool value, int settingsSig, int subSig, std::string settingName);
std::string makeSettingString(std::string value, int settingsSig, int subSig, std::string settingName);
std::string makeSettingString(std::vector<std::string> value, int settingsSig, int subSig, std::string settingName);
std::string makeSettingString(std::vector<int> value, int settingsSig, int subSig, std::string settingName);
std::string makeSettingString(std::vector<float> value, int settingsSig, int subSig, std::string settingName);
std::string makeSettingString(std::vector<float> value, int settingsSig, int subSig, std::string settingName);
std::string makeSettingString(float* vector, int size, int settingsSig, int subSig, std::string settingName);
std::string makeSettingString(glm::vec2 value, int settingsSig, int subSig, std::string settingName);
std::string makeSettingString(TSetProp value, int settingsSig, int subSig, std::string settingName);
std::string makeSettingString(BlendMode value, int settingsSig, int subSig, std::string settingName);
std::string makeSettingString(TSetController value, int settingsSig, int subSig, std::string settingName);
std::string makeSettingString(TSetNoise value, int settingsSig, int subSig, std::string settingName);
std::string makeSettingString(TSetGraph value, int settingsSig, int subSig, std::string settingName);
std::string makeSettingString(std::shared_ptr<CFunction> value, int settingsSig, int subSig, std::string settingName);
std::string makeSettingString(std::shared_ptr<CObject> value, int settingsSig, int subSig, std::string settingName);
std::string makeSettingString(std::shared_ptr<Tool> target, TSetControl_Node* controlNode, int settingsSig, int subSig, std::string settingName);
std::string makeSettingString(std::shared_ptr<Tool> target, TSetController* controller, int settingsSig, int subSig, std::string settingName);
std::string makeSettingString(std::shared_ptr<Tool> target, TSetGraph* graph, int settingsSig, int subSig, std::string settingName);
std::string makeSettingString(std::shared_ptr<Tool> target, TSetNoise* noiseNode, int settingsSig, int subSig, std::string settingName);


// Polymorphic ToolSettings Interest Mask Classes
// Stored inside of maps in the Tool class

class ToolSettings
{
public:
	virtual TSetType getType() = 0;
	virtual TSetControl_Node* getControlNode(int settingSig, int subSig) = 0;
	virtual TSetController* getController(int settingSig, int subSig) = 0;
	virtual TSetGraph* getGraph(int settingSig, int subSig) = 0;
	virtual TSetNoise* getNoise(int settingSig, int subSig) = 0;
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, int subSig, std::shared_ptr<CObject> object, 
		bool isGet, bool asPercentage, bool asString) = 0;
	bool isEnabled = true;
	TSetType type = TSetType::null;
};

#endif
