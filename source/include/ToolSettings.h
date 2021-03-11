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
	alongAxis,
	alongPoints,
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
};

// Settings Types : Used for I/O interest masks and Tool -> 'settings'
// Determines what settings are available per I/O method combination
enum class TSetType
{
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
	null
};

inline std::map<std::string, TSetType> TSetTypeNamesMap = {
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
	bool perTip = true; // false -> perStroke
	float frequency = 0;
	float smoothing = 0;
	float varianceMin = 0;
	float varianceMax = 1;
	std::shared_ptr<CFunction> function;
};

// Polymorphic ToolSettings Interest Mask Classes
// Stored inside of maps in the Tool class
class ToolSettings
{
public:
	virtual TSetType getType() = 0;
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, int settingsSig, std::shared_ptr<CObject> object, bool isGet) = 0;
	bool isEnabled = true;
	TSetType type;
};

class TSet_Pan : public ToolSettings // 100
{
public:
	bool constrainToCanvas; // 100
	bool doFlickPanning; // 101
	float friction; // 102
	int force; // 103
	float speedAmount; // 107
	int slowModKey; // 110
	float slowFactor; // 109
	bool clickTargetPanning; // 111
	int clickTarButton; // 112
	int clickTarModKey; // 113
	bool lockToX; // 114
	int lockXModKey; // 115
	bool lockToY; // 116
	int lockYModKey; // 117
	bool lockCustomAngle; // 118
	float customAngle; // 119
	int lockCustomModKey; // 120
	bool cameraPosRecording; // 121
	int cameraPosRecordKey; // 122
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet) { return nullptr; };
	TSet_Pan();
};

class TSet_Zoom : public ToolSettings // 200
{
public:
	int scrubThreshold; // 200
	float maxZoomFactor; // 201
	float minZoomFactor; // 202
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet) { return nullptr; };
	TSet_Zoom();
};

class TSet_Rotate : public ToolSettings // 300
{
public:
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet) { return nullptr; };
	TSet_Rotate();
};

class TSet_ContinuousControl : public ToolSettings // 400
{
public:
	TSetProp defaultMode; // draw / polyDraw // 400
	int alternateModeKey; // 401
	//
	float anchorSpacing; // 402
	int splineRandomX; // 403
	int splineRandomY; // 404
	//
	bool fillOnShiftClick;
	bool fillOnPolyDraw;
	float connectPropertiesWeighting;
	// Constraint angles must always be between -90 to +90. Because they are an 'axis',
	// the opposite angle will also be considered. Entering a negative angle, or angle beyond 180
	// should be handled by the UI widget and corrected before sending, elsewise it will
	// get clamped.
	// The user may set a different mod key for each constraint, enabling more customization and
	// finer control when two desired constraints would be difficult to distinguish.
	float constraintLengthThreshold; // 405
	// Constraint Angle A
	bool enableConstrainA; // 406
	float constrainA_angle; // 407
	int constrainA_modKey; // 408
	// Constraint Angle B
	bool enableConstrainB; // 409
	float constrainB_angle; // 410
	int constrainB_modKey; // 411
	// Constraint Angle C
	bool enableConstrainC; // 412
	float constrainC_angle; // 413
	int constrainC_modKey; // 414
	// Constraint Angle D
	bool enableConstrainD; // 415
	float constrainD_angle; // 416
	int constrainD_modKey; // 417
	// Control Variables (Unexposed)
	bool activated = false; // 418
	int activeKey = -1;
	glm::vec3 origin = glm::vec3(0);
	glm::vec4 line = glm::vec4(0);
	glm::vec3 dir = glm::vec3(0);
	glm::vec3 perpendicular = glm::vec3(0);
	float activeAngle = 0.0f;
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet);
	TSet_ContinuousControl();
	void clearConstraint();
};

class TSet_DragControl : public ToolSettings // 500
{
public:
	int snapModeKey; // 500
	float snapAngleIncrement; // 501
	int moveOriginModeKey; // Disabled when connectInputs is true // 502
	int sizeAboutOriginKey; // 503
	bool pressureMapping; // 504
	float pressureMapStrength; // 505
	float pressureMapDecayRate; // 506
	bool pressureMapRelative; // 507
	float pressureMapThreshold; // 508
	bool connectInputs; // 509
	int connectInputKey; // 510
	int terminateInputKey; // 511
	bool enableExtrusion = false; // Hidden Setting // 512
	// Extrusion Settings, only visible for tools with it enabled
	int extrudeModeKey; // 513
	float initialExtrusion; // 514
	TSetProp extrudeSnappingMode; // 515
	float extrudeSnapIncrement; // 516
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet) { return nullptr; };
	TSet_DragControl();
};

class TSet_OnePointControl : public ToolSettings // 600
{
public:
	float initialRotation; // 600
	int constrainRatioKey; // 601
	float constrainAngleIncrement; // 602
	int moveOriginModeKey; // 603
	int rotateModeKey; // 604
	int sizeAboutOriginKey; // 605
	bool pressureMapping; // 606
	float pressureMapStrength; // 607
	float pressureMapDecayRate; // 608
	bool pressureMapRelative; // 609
	float pressureMapThreshold; // 610
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet) { return nullptr; };
	TSet_OnePointControl();
};

class TSet_TwoPointControl : public ToolSettings // 700
{
public:
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet) { return nullptr; };
	TSet_TwoPointControl();
};

class TSet_ThreePointControl : public ToolSettings // 800
{
public:
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet) { return nullptr; };
	TSet_ThreePointControl();
};

class TSet_Blend : public ToolSettings // 2000
{
public:
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet) { return nullptr; };
	TSet_Blend();
};

class TSet_Basic : public ToolSettings // 2100
{
public:
	// Color Settings - When Color is locked, updates are no longer
	// immediately pushed to the savedColor. The savedColors are also
	// stored in the saved settings for the tool if a user wants to 
	// create a brush that always returns to the same color
	// Locking the colors also causes the tool to reset to it's saved colors
	// when it becomes active again.
	bool lockFGColor;
	bool lockBGColor;
	CColor savedFGColor;
	CColor currentFGColor;
	CColor savedBGColor;
	CColor currentBGColor;
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet) { return nullptr; };
	TSet_Basic();
};

class TSet_Image : public ToolSettings // 2200
{
public:
	TSetProp tipType; // 2200
	int tipSize; // 2201
	bool relativeSpacing; // 2202
	float spacing; // 2203 // When in relative mode, use (spacing / 100) to get the % value
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet);
	TSet_Image();
};

class TSet_Polygon : public ToolSettings // 2300
{
public:
	TSetProp shapeType; // Triangle / Rectangle / Ellipse / Sides / Custom
	int sides;
	bool useSize = false; // Hidden Setting for when used with continuous draw
	float size;
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet) { return nullptr; };
	TSet_Polygon();
};

class TSet_PolyLine : public ToolSettings // 2400
{
public:
	bool useMiterJoints;
	float maxMiterLength;
	TSetProp bevelType;
	float bevelWidth;
	float segmentCount;
	bool constantSegmentCount;
	TSetProp startCapType;
	float startCapScale;
	TSetProp endCapType;
	float endCapScale;
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet) { return nullptr; };
	TSet_PolyLine();
};

class TSet_Mesh : public ToolSettings // 2500
{
public:
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet) { return nullptr; };
	TSet_Mesh();
};

class TSet_Scatter : public ToolSettings // 2600
{
public:
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet) { return nullptr; };
	TSet_Scatter();
};

class TSet_Color : public ToolSettings // 2700
{
public:
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet) { return nullptr; };
	TSet_Color();
};

class TSet_Alpha : public ToolSettings // 2800
{
public:
	bool usePressureOpacity; // 2800
	float minOpacity; // 2801
	float maxOpacity; // 2802
	bool usePressureFlow; // 2803
	float minFlow; // 2804
	float maxFlow; // 2805
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet);
	TSet_Alpha();
};

class TSet_Distance : public ToolSettings // 2900
{
public:
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet) { return nullptr; };
	TSet_Distance();
};

class TSet_Mixing : public ToolSettings // 3000
{
public:
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet) { return nullptr; };
	TSet_Mixing();
};

class TSet_MultiTip : public ToolSettings // 3100
{
public:
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet) { return nullptr; };
	TSet_MultiTip();
};

class TSet_Texture : public ToolSettings // 3200
{
public:
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet) { return nullptr; };
	TSet_Texture();
};

class TSet_Grain : public ToolSettings // 3300
{
public:
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet) { return nullptr; };
	TSet_Grain();
};

class TSet_WetEdges : public ToolSettings // 3400
{
public:
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet) { return nullptr; };
	TSet_WetEdges();
};

class TSet_Repeat : public ToolSettings // 3500
{
public:
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet) { return nullptr; };
	TSet_Repeat();
};

class TSet_Reference : public ToolSettings // 3600
{
public:
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet) { return nullptr; };
	TSet_Reference();
};

class TSet_Vector : public ToolSettings // 3700
{
public:
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet) { return nullptr; };
	TSet_Vector();
};

class TSet_Vortex : public ToolSettings // 4800
{
public:
	bool constantRingCount;
	float rings;
	float radius;
	int ringDensity;
	TSetProp ringSpacingMode;
	float ringSpacing_simple;
	float ringRotationOffset;
	float ringRotationRandom;
	TSetProp scatteringMode;
	float angleVariance_simple;
	float radiusVariance_simple;
	float offsetVarianceX_simple;
	float offsetVarianceY_simple;
	TSetProp connectionOrdering;
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet) { return nullptr; };
	TSet_Vortex();
};

class TSet_Fan : public ToolSettings // 4900
{
public:
	TSetProp waitMode;
	float waitAmount;
	bool stopWait = false;
	int subdivisionCount;
	TSetProp scateringMode;
	float angleVariance_simple;
	float radiusVariance_simple;
	float offsetVarianceX_simple;
	float offsetVarianceY_simple;
	TSetProp connectionOrdering;
	bool airbrushMode;
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet) { return nullptr; };
	TSet_Fan();
};

class TSet_Rake : public ToolSettings // 5000
{
public:
	int points;
	bool pushToCanvasEdge;
	float size;
	TSetProp pointsSpacingMode;
	float pointsSpacing_simple;
	TSetProp pointsShapeMode;
	float maxAngle;
	float angleVariance_simple;
	float radiusVariance_simple;
	float offsetVarianceX_simple;
	float offsetVarianceY_simple;
	TSetProp connectionOrdering;
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet) { return nullptr; };
	TSet_Rake();
};

class TSet_Gradient : public ToolSettings // 6100
{
public:
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet) { return nullptr; };
	TSet_Gradient();
};

class TSet_Fill : public ToolSettings // 6200
{
public:
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet) { return nullptr; };
	TSet_Fill();
};

class TSet_Pattern : public ToolSettings // 6300
{
public:
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet) { return nullptr; };
	TSet_Pattern();
};

class TSet_Field : public ToolSettings // 7000
{
public:
	TSetProp distributionType;
	TSetProp randomizationType;
	bool constantSeed;
	int savedSeed;
	bool constantDensity;
	float density;
	float bias;
	bool randomizePressure;
	float pressureXInfluence;
	float pressureYInfluence;
	float pressureNoise;
	bool randomizeTilt;
	float tiltXInfluence;
	float tiltYInfluence;
	float tiltNoise;
	bool randomizeRotation;
	float rotationXInfluence;
	float rotationYInfluence;
	float rotationNoise;
	bool connectVertices;
	float connectionStrength;
	int maxConnectionLength;
	int minConnectionLength;
	bool connectConnectionEnds;
	TSetProp connectionOrdering;
	float secondaryScatteringX;
	float secondaryScatteringY;
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet) { return nullptr; };
	TSet_Field();
};

class TSet_Shader : public ToolSettings // 7600
{
public:
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet) { return nullptr; };
	TSet_Shader();
};

class TSet_Filter : public ToolSettings // 8200
{
public:
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet) { return nullptr; };
	TSet_Filter();
};

class TSet_Sampler : public ToolSettings // 9100
{
public:
	// Sampler Shape Type & Settings
	TSetProp sampleType = TSetProp::allLayers; // allLayers / currentLayer / currentBelow
	TSetProp shapeType = TSetProp::point; // Point / Radius / Shape / Tip
	int pointRandAmount;
	bool isCircle;
	unsigned char shapeRadius;
	unsigned char shapeSides;
	float shapeRotation;
	unsigned char* tipData;
	unsigned int tipSize;
	// Auto Color Jitter & Control
	bool useFGVariance;
	TSetController FGValueControl;
	TSetController FGHueControl;
	TSetController FGSaturationControl;
	TSetNoise FGValueNoise;
	TSetNoise FGHueNoise;
	TSetNoise FGSaturationNoise;
	bool useAutoBGUpdater;
	float bgValueOffset;
	float bgHueOffset;
	float bgSaturationOffset;
	TSetController BGValueControl;
	TSetController BGHueControl;
	TSetController BGSaturationControl;
	TSetNoise BGValueNoise;
	TSetNoise BGHueNoise;
	TSetNoise BGSaturationNoise;
	// Palette Constraints
	bool usePaletteConstrain;
	TSetProp paletteType; // Simple / Complex
	float valueMin;
	float valueMax;
	float hueMin;
	float hueMax;
	float saturationMin;
	float saturationMax;
	std::vector<std::vector<glm::vec2>> paletteShapes;
	float paletteValueMin;
	float paletteValueMax;
	// Block Callback Notifications?
	bool shouldNotify;
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet) { return nullptr; };
	virtual TSetType getType() { return type; }
	TSet_Sampler();
};

class TSet_Smoothing : public ToolSettings // 9800
{
public:
	// Note: Change this later? It tends to have the effect of turning a brush tip towards
	// the outer edge of a curve, rather than inwards. Might need to rotate by the negative
	// angle change as a different smoothing tool setting.
	bool smoothDirection;
	float directionThreshold;
	float directionFactor;
	bool smoothDirectionCurve;
	float curveDirectionThreshold;
	float curveDirectionFactor;
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet) { return nullptr; };
	TSet_Smoothing();
};

class TSet_AntiAliasing : public ToolSettings // 9900
{
public:
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner, 
		int settingsSig, std::shared_ptr<CObject> object, bool isGet) { return nullptr; };
	TSet_AntiAliasing();
};



#endif
