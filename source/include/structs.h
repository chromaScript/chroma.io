#ifndef TOOLSTRUCTS_H
#define TOOLSTRUCTS_H

#include <glm.hpp>

#include "Color.h"

#include <deque>
#include <vector>
#include <memory>
#include <filesystem>

#include <iostream>
#include <fstream>
#include <sstream>
#include <Ole2.h>

// Note: These structs should only contain generic data types, 
// including class headers here creates circular dependencies and usually won't compile

// Useful macros
#define _SIZE_T static_cast<size_t>

inline std::string ConvertWCSToMBS(const wchar_t* pstr, long wslen)
{
	int len = ::WideCharToMultiByte(CP_ACP, 0, pstr, wslen, NULL, 0, NULL, NULL);

	std::string dblstr(len, '\0');
	len = ::WideCharToMultiByte(CP_ACP, 0 /* no flags */,
		pstr, wslen /* not necessary NULL-terminated */,
		&dblstr[0], len,
		NULL, NULL /* no default char */);

	return dblstr;
}

inline std::string ConvertBSTRToMBS(BSTR bstr)
{
	int wslen = ::SysStringLen(bstr);
	return ConvertWCSToMBS((wchar_t*)bstr, wslen);
}


inline bool readFileToString(std::string &str, std::filesystem::path path)
{
	std::ifstream file;
	// check that the ifstream object can throw exceptions
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// Open the file
		file.open(path);
		std::stringstream fileStream;
		// Read file's buffer contents into the stream
		fileStream << file.rdbuf();
		// Convert the stream into a string
		str = fileStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		(void)e;
		std::cout << "ERROR::UTILITY::READFILETOSTRING::FAILED_TO_STREAM_FILE::" << path << std::endl;
		return false;
	}
	return true;
}
inline std::string floatVecToString(std::vector<float> vector)
{
	std::string out = "{ ";
	size_t i = vector.size();
	for (size_t k = 0; k < i; k++)
	{
		out += " " + std::to_string(vector[k]) + ",";
	}
	out.pop_back();
	out += " }";
	return out;
}
inline std::string stringVecToString(std::vector<std::string> vector)
{
	std::string out = "{ ";
	size_t i = vector.size();
	for (size_t k = 0; k < i; k++)
	{
		out += vector[k] + ",";
	}
	out.pop_back();
	out += " }";
	return out;
}
inline std::string vec3VecToString(std::vector<glm::vec3> vector)
{
	std::string out = "{ ";
	size_t i = vector.size();
	for (size_t k = 0; k < i; k++)
	{
		out += std::to_string(k + 1) + "=[" + std::to_string(vector[k].x) + ", " + std::to_string(vector[k].y) + ", " + std::to_string(vector[k].z) + "] ";
	}
	out += " }";
	return out;
}
inline std::string vec2VecToString(std::vector<glm::vec2> vector)
{
	std::string out = "{ ";
	size_t i = vector.size();
	for (size_t k = 0; k < i; k++)
	{
		out += std::to_string(k + 1) + "=[" + std::to_string(vector[k].x) + ", " + std::to_string(vector[k].y) + "] ";
	}
	out += " }";
	return out;
}
inline std::string sliceRegularString(std::string &str, std::string delimStart, std::string delimEnd)
{
	size_t start;
	size_t delimStart_size = delimStart.size();
	//size_t end = 0;
	size_t slice;
	std::string out;
	while ((start = str.find(delimStart, 0)) != std::string::npos)
	{
		str = str.substr(start + delimStart_size, std::string::npos);
		slice = str.find_first_of(delimEnd);
		out = str.substr(0, slice);
		str = str.substr(slice + 1, std::string::npos);
	}
	return out;
}
inline std::string splitRegularString(std::string &str, std::string delim)
{
	size_t splitAt;
	std::string out;
	splitAt = str.find(delim, 0);
	out = str.substr(0, splitAt);
	str = str.substr(splitAt + 1, std::string::npos);
	return out;
}

// ObjectTransform is used to condense multiple direction vectors into a single container
struct ObjectTransform
{
	glm::vec3 up = glm::vec3(0, 0, 0);
	glm::vec3 right = glm::vec3(0, 0, 0);
	glm::vec3 fwd = glm::vec3(0, 0, 0);
	ObjectTransform() {}
	ObjectTransform(glm::vec3 up, glm::vec3 right, glm::vec3 fwd) {
		this->up = up;
		this->right = right;
		this->fwd = fwd;
	}
};

// Bounds is used to hold two 2D coordinates for storing a rectangle as corners
//
// int x1 / int y1 / int x2 / int y2 where X2,Y2 is usually also width,height
//
// Always store information for data using integers, something less than a pixel
// can not be rendered, and use of floating point values to store critical dimension
// information for texture data can potentially lead to errors from rounding.
struct Bounds
{
	int x1 = 0;
	int y1 = 0;
	int x2 = 0;
	int y2 = 0;
	Bounds() {}
	Bounds(int x1, int y1, int x2, int y2)
	{
		this->x1 = x1;
		this->y1 = y1;
		this->x2 = x2;
		this->y2 = y2;
	}
	void reset()
	{
		x1 = y1 = x2 = y2 = 0;
	}
};
struct RectBounds
{
	glm::vec3 p1 = glm::vec3(0, 0, 0);
	glm::vec3 p2 = glm::vec3(0, 0, 0);
	glm::vec3 p3 = glm::vec3(0, 0, 0);
	glm::vec3 p4 = glm::vec3(0, 0, 0);
	RectBounds() {}
	RectBounds(glm::vec3 uniform)
	{
		this->p4 = this->p3 = this->p2 = this->p1 = uniform;
	}
	RectBounds(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4)
	{
		this->p1 = p1;
		this->p2 = p2;
		this->p3 = p3;
		this->p4 = p4;
	}
	RectBounds(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4)
	{
		this->p1 = glm::vec3(p1, 0.0f);
		this->p2 = glm::vec3(p2, 0.0f);
		this->p3 = glm::vec3(p3, 0.0f);
		this->p4 = glm::vec3(p4, 0.0f);
	}
	RectBounds(float p1x, float p1y, float p2x, float p2y, float p3x, float p3y, float p4x, float p4y)
	{
		this->p1 = glm::vec3(p1x, p1y, 0.0f);
		this->p2 = glm::vec3(p2x, p2y, 0.0f);
		this->p3 = glm::vec3(p3x, p3y, 0.0f);
		this->p4 = glm::vec3(p4x, p4y, 0.0f);
	}
	void reset()
	{
		p4 *= p3 *= p2 *= p1 *= 0;
	}
};


/*
// Camera Structs
*/
// Camera types
#define CAM_ORTHO 0
#define CAM_PERSPECTIVE 1

// ShaderTransform is used to condense matrix data from the camera
struct ShaderTransform
{
	glm::mat4 p = glm::mat4(1.0f); // projection matrix
	glm::mat4 v = glm::mat4(1.0f); // view matrix
	glm::mat4 m = glm::mat4(1.0f); // model matrix
	float zoom = 0; // camera zoom
	ShaderTransform() {}
	ShaderTransform(glm::mat4 projection, glm::mat4 view, glm::mat4 model)
	{
		this->p = projection;
		this->v = view;
		this->m = model;
	}
	ShaderTransform(glm::mat4 projection, glm::mat4 view, glm::mat4 model, float zoom)
	{
		this->p = projection;
		this->v = view;
		this->m = model;
		this->zoom = zoom;
	}
};

/*
// Property Structs
*/
// Blend modes
enum class BlendModes
{
	normal,
	erase,
	//
	multiply,
	darken,
	colorburn,
	linearburn,
	darkercolor,
	//
	lighten,
	lightercolor,
	screen,
	colordodge,
	lineardodge,
	glowdodge,
	//
	overlay,
	softlight,
	hardlight,
	vividlight,
	linearlight,
	pinlight,
	hardmix,
	//
	difference,
	exclusion,
	subtract,
	divide,
	intersect,
	//
	hue,
	color,
	saturation,
	luminosity,
	//
	dissolve,
	shuffle,
	scramble,
	noise,
	redshift,
	blueshift,
	greenshift,
	randomize,
};
// String name array for blend modes, must match the above Enum
static const char* BlendModes_cstr[] = 
{
	"normal",
	"erase",
	//
	"multiply",
	"darken",
	"colorburn",
	"linearburn",
	"darkercolor",
	//
	"lighten",
	"lightercolor",
	"screen",
	"colordodge",
	"lineardodge",
	"glowdodge",
	//
	"overlay",
	"softlight",
	"hardlight",
	"vividlight",
	"linearlight",
	"pinlight",
	"hardmix",
	//
	"difference",
	"exclusion",
	"subtract",
	"divide",
	"intersect",
	//
	"hue",
	"color",
	"saturation",
	"luminosity",
	//
	"dissolve",
	"shuffle",
	"scramble",
	"noise",
	"redshift",
	"blueshift",
	"greenshift",
	"randomize"
};
/*
// Layer Structs
*/
// LayerProperties is used to hold various default type layer properties like ID, Z-Index, Group ID, Parent/Clip ID, Alpha/Pixel/Position Lock, etc.
struct LayerProperties
{
	bool alphaLock;
	bool dataLock;
	bool positionLock;
	bool mergeLock;
	LayerProperties()
	{
		alphaLock = false;
		dataLock = false;
		positionLock = false;
		mergeLock = false;
	}
};
// GroupProperties is used to hold group information
struct GroupProperties
{

};

/*
// Fragment Structs
*/
// FragmentAnchor handle types, Linear is no curve, Auto is automatic non-controlled handle positions, Manual is bezier-style control handle
#define FHANDLE_LINEAR 0
#define FHANDLE_AUTO 1
#define FHANDLE_MANUAL 2
// FragmentAnchor handle relationships (tentative implementation). Independent is no relationship, Right is locked 90 degree, Pivot is pivot fixed to anchor
#define FHANDLE_REL_INDEPENDENT 0
#define FHANDLE_REL_RIGHT 1
#define FHANDLE_REL_PIVOT 2
// EntityTransform is used to hold the transform data for a visual entity
struct EntityTransform
{
	glm::vec3 pos = glm::vec3(0, 0, 0);
	glm::vec3 origin = glm::vec3(0, 0, 0);
	Bounds boundBox = Bounds(0, 0, 0, 0); // x1, y1, x2, y2 // Corner->Corner rectangle bounds of fragment
	bool drawCornersOnly = false;
	RectBounds bounds = RectBounds(glm::vec3(0, 0, 0));
	float roll = 0.0f;
	EntityTransform() {}
	EntityTransform(glm::vec3 pos, Bounds rect, float roll)
	{
		this->pos = pos;
		this->boundBox = rect;
		this->roll = roll;
	}
	void reset()
	{
		pos = glm::vec3(0, 0, 0);
		origin = glm::vec3(0, 0, 0);
		boundBox.reset();
		bounds.reset();
		roll = 0.0f;
	}
};
/*
// StrokeShard is the building block of Stroke rendering
// Components: ID / pos / dir / scale / color / rotation / pressure / tilt / velocity
// Each shard must store it's own data about the input information (pressure, etc.) because the render
// loop for Stroke will query this saved data instead of reinterpreting the anchor data every frame.
// Also, allow for each shard to store color data for use with ColorDynamics
// Shards are also responsible for their own position, direction, rotation, and scale information,
// so that proper stroke tapering/rotation can be achieved per shard.
// Shards store two pieces of rotation information (direction & rotation), to allow for brush settings
// to make use of Tip Angle follows Direction, and individual random or stylus-defined rotation
// Keep StrokeShard small in size, avoid adding unnecessary data to it.
*/
struct StrokeShard
{
	int ID = 0;
	glm::vec3 pos = glm::vec3(0, 0, 0);
	glm::vec3 dir = glm::vec3(0, 0, 0);
	glm::vec4 scale = glm::vec4(0, 0, 0, 1);
	CColor color = CColor(0, 0, 0);
	float opacity = 0;
	float pressure = 0;
	float rotation = 0;
	float tiltx = 0;
	float tilty = 0;
	float velocity = 0;
	StrokeShard() {}
	StrokeShard(int ID, glm::vec3 pos, glm::vec3 dir, glm::vec4 scale,
		CColor color, float opacity, float pressure, float rotation, float tiltx, float tilty, float velocity)
	{
		this->ID = ID;
		this->pos = pos;
		this->dir = dir;
		this->scale = scale;
		this->color = color;
		this->opacity = opacity;
		this->pressure = pressure;
		this->rotation = rotation;
		this->tiltx = tiltx;
		this->tilty = tilty;
		this->velocity = velocity;
	}
};
// Easy copy-paste for StrokeShard
/*
(ID, pos, dir, scale,
	color, opacity, pressure, 
	rotation, tiltx, tilty, velocity)
*/

/*
// FragmentAnchor is used to hold individual vector locations of anchors/waypoints/etc.
// Components: flag / ID / pos / headType / headControl / headHandle(pos) / tailType / tailHandle(pos) / handleRelationship
//			pressure / tiltx / tilty / rotation / speed / intensity
// flag : Use flags from MouseStruct (FLAG_)(NULL, WAYPOINT, PAUSE, CONSTRAIN, ETC)
// head/tailType : Use (FHANDLE)(LINEAR, AUTO, MANUAL)
// head/tailControl: Setting to allow/block user control of bezier-handle positions
// handleRelationship : Use (FHANDLE_REL)(INDEPENDENT, RIGHT, PIVOT)
// Intensity is a value used to amplify stroke attributes when using build-up
*/
struct FragmentAnchor
{
	int flag = 0;
	int ID = 0;
	int wait = 0; // Manual set only
	glm::vec3 pos = glm::vec3(0, 0, 0);
	glm::vec3 dir = glm::vec3(0, 0, 0);
	float dirInterpFactor = 0.5f;
	int headType = 0;
	bool headControl = false;
	glm::vec3 headHandle = glm::vec3(0, 0, 0);
	int tailType = 0;
	bool tailControl = false;
	glm::vec3 tailHandle = glm::vec3(0, 0, 0);
	int handleRelationship = 0;
	float pressure = 0.0f;
	float tiltx = 0.0f;
	float tilty = 0.0f;
	float rotation = 0.0f;
	float time = 0.0f;
	float velocity = 0.0f;
	float intensity = 0.0f;
	int randSeedA = 0;
	int randSeedB = 0;
	FragmentAnchor() {}
	FragmentAnchor(const FragmentAnchor& copy)
	{
		this->flag = copy.flag;
		this->ID = copy.ID;
		this->pos = copy.pos;
		this->dir = copy.dir;
		this->dirInterpFactor = copy.dirInterpFactor;
		this->headType = copy.headType;
		this->headControl = copy.headControl;
		this->headHandle = copy.headHandle;
		this->tailType = copy.tailType;
		this->tailControl = copy.tailControl;
		this->tailHandle = copy.tailHandle;
		this->handleRelationship = copy.handleRelationship;
		this->pressure = copy.pressure;
		this->tiltx = copy.tiltx;
		this->tilty = copy.tilty;
		this->rotation = copy.rotation;
		this->time = copy.time;
		this->velocity = copy.velocity;
		this->intensity = copy.intensity;
	}
	FragmentAnchor(int flag, int ID, glm::vec3 pos, glm::vec3 dir, 
		int headType, bool headControl, glm::vec3 headHandle,
		int tailType, bool tailControl, glm::vec3 tailHandle,
		int handleRelationship,
		float pressure, float tiltx, float tilty, float rotation, float velocity, float intensity)
	{
		this->flag = flag;
		this->ID = ID;
		this->pos = pos;
		this->dir = dir;
		this->headType = headType;
		this->headControl = headControl;
		this->headHandle = headHandle;
		this->tailType = tailType;
		this->tailControl = tailControl;
		this->tailHandle = tailHandle;
		this->handleRelationship = handleRelationship;
		this->pressure = pressure;
		this->tiltx = tiltx;
		this->tilty = tilty;
		this->rotation = rotation;
		this->velocity = velocity;
		this->intensity = intensity;
	}
	FragmentAnchor(int flag, int ID, glm::vec3 pos, glm::vec3 dir, float dirInterpFactor,
		int headType, bool headControl, glm::vec3 headHandle,
		int tailType, bool tailControl, glm::vec3 tailHandle,
		int handleRelationship,
		float pressure, float tiltx, float tilty, float rotation, float time, float velocity, float intensity)
	{
		this->flag = flag;
		this->ID = ID;
		this->pos = pos;
		this->dir = dir;
		this->dirInterpFactor = dirInterpFactor;
		this->headType = headType;
		this->headControl = headControl;
		this->headHandle = headHandle;
		this->tailType = tailType;
		this->tailControl = tailControl;
		this->tailHandle = tailHandle;
		this->handleRelationship = handleRelationship;
		this->pressure = pressure;
		this->tiltx = tiltx;
		this->tilty = tilty;
		this->rotation = rotation;
		this->time = time;
		this->velocity = velocity;
		this->intensity = intensity;
	}
	void copyStylusData(FragmentAnchor* target)
	{
		this->pressure = target->pressure;
		this->tiltx = target->tiltx;
		this->tilty = target->tilty;
		this->rotation = target->rotation;
		this->velocity = target->velocity;
	}
	void swapStylusData(FragmentAnchor* target)
	{
		float p = pressure;
		float tx = tiltx;
		float ty = tilty;
		float r = rotation;
		float v = velocity;
		copyStylusData(target);
		target->pressure = p;
		target->tiltx = tx;
		target->tilty = ty;
		target->rotation = r;
		target->velocity = v;
	}
	void clearStylusData()
	{
		this->pressure = 0.0f;
		this->tiltx = 0.0f;
		this->tilty = 0.0f;
		this->rotation = 0.0f;
		this->velocity = 0.0f;
	}
};
/* Easy constructor copy-paste for FragmentAnchor
(flag, ID, pos, dir,
	headType, headControl, headHandle,
	tailType, tailControl, tailHandle,
	handleRelationship,
	pressure, tiltx, tilty, rotation, 
	velocity, intensity)
*/

// VertexData is used to hold the data processed from an InputData object. This is what gets stored as history and canvas objects. The original
// mouse input is not salvageable. VertexData should only contain information useful for all fragment types. Output asks the active tool for
// how to render the fragments.
struct VertexData
{
private:
	std::deque<FragmentAnchor> emptyAnchors = {};
public:
	EntityTransform transform;
	bool constantSize = false;
	bool linearStream = true;
	bool connectEnds = false;
	bool centerAboutOrigin = false;
	std::deque<FragmentAnchor> anchors;
	float startTime = 0.0f;
	float endTime = 0.0f;
	//BlendModes blendType;
	void reset() 
	{
		transform.reset();
		startTime = 0.0f;
		endTime = 0.0f;
		resetAnchors();
	}
	void resetAnchors() { anchors.clear(); anchors.swap(emptyAnchors); }
};

// StrokeSettings is used to hold settings for stroke fragments
// Note: may reorganize this later into classes or break into smaller pieces. Many of these options are common to different fragment types, for
// instance colorDynamics or scatterOptions may be useful for shapes/gradients/patterns
struct StrokeSettings
{
	// Temporary mockup of names, variable types subject to change. May expand into sub-structs
	int tipShape = 0;
	int tipTransform = 0;
	int tipHardness = 0;
	bool spacingOptions;
	int blendMode = 0;
	int randomSeed1 = 0;
	int randomSeed2 = 0;
	int size = 0;
	int antiAlias = 0;
	int buildUp = 0;
	bool stylusOverride;
	bool transferOptions;
	bool scatterOptions;
	bool mixOptions;
	bool noiseOptions;
	bool shapeDynamics;
	bool colorDynamics;
	bool dualTip;
	bool smoothing;
	bool correction;
	bool specialFX;
};
// ShapeSettings is used to hold settings for shape fragments
// Gradient / Fill / Mask? / Image(Texture) / Ruler / Guide / etc. settings

// AlgorithmicSettings is used to hold settings for programatic effect objects (ie. blur, randomize, selective color)
// Note: (May not implement this ever, due to complexity)

/*
// Mouse Structs
*/
// Note: Depreciate the use of these flag values, use the new ones below the comment line
#define FLAG_NULL 0
#define FLAG_WAYPOINT 1
#define FLAG_PIVOT 2
#define FLAG_SCRUB 3
#define FLAG_PAUSE 4
#define FLAG_CONNECT 8
#define FLAG_CONSTRAIN_BEGIN 13
#define FLAG_CONSTRAIN_X 14
#define FLAG_CONSTRAIN_Y 15
#define FLAG_CONSTRAIN_45 16
#define FLAG_CONSTRAIN_NEG45 17
// New Flag values
#define FLAG_NULL 0
#define FLAG_BEGIN_STREAM 1
#define FLAG_NEW_INPUT 1
#define FLAG_POINT 2
#define FLAG_START 3
#define FLAG_LAST 4
#define FLAG_END 5
#define FLAG_ENDCON 6
#define FLAG_CONSTART 7
#define FLAG_CONORIGIN 8
#define FLAG_CONORIGIN_SWAPMETA 9
#define FLAG_CONORIGIN_SWAPCLEARMETA 11
#define FLAG_ORIGIN 16
#define FLAG_ORIGIN_POINT 17
#define FLAG_END_STREAM 18

// MousePosition is used to capture the mouse position when dealing with stylus input
struct MousePosition
{
	double x = 0.0;
	double y = 0.0;
	float time = 0.0f;
	MousePosition() {};
	MousePosition(double xpos, double ypos, float time)
	{
		this->x = xpos;
		this->y = ypos;
		this->time = time;
	}
};

// MouseEvent is used when capturing input events, which are then usually composed into a vector of events.
// Constructor(int action, double x, double y, double time, int modKey, int modKey, unsigned int pressure, float tiltx, float tilty, float rotation)
struct MouseEvent
{
	int button = 0;
	bool isDouble = false; // This gets set after construction by eventHandler
	int action = 0;
	double x = 0.0;
	double y = 0.0;
	double time = 0.0;
	int modKey = 0;
	int flag = 0; // Use FLAG_NULL / FLAG_WAYPOINT / FLAG_PIVOT / ETC.
	float pressure = 1.0f;
	float tiltx = 0.0f;
	float tilty = 0.0f;
	float rotation = 0.0f;
	MouseEvent() {};
	MouseEvent(int button, int action, double x, double y, double time, int modKey, int flag, float pressure, float tiltx, float tilty, float rotation)
	{
		this->button = button;
		this->action = action;
		this->x = x;
		this->y = y;
		this->time = time;
		this->modKey = modKey;
		this->flag = flag;
		this->pressure = pressure;
		this->tiltx = tiltx;
		this->tilty = tilty;
		this->rotation = rotation;
	}
	void reset()
	{
		button = action = modKey = flag = 0;
		isDouble = false;
		x = y = time = 0.0;
		pressure = tiltx = tilty = rotation = 0.0f;
	}
};

/*
// Input Structs
*/
// Documentation: Use these to define angle constraints by name
#define AXIS_EMPTY -540.0f
#define AXIS_NONE 0
#define AXIS_X 1
#define AXIS_Y 2
#define AXIS_45 3
#define AXIS_NEG45 4
// Documentation: Use these as flags for input flow gates
#define INPUT_REJECT -3
#define INPUT_RELEASE 0 // GLFW_RELEASE
#define INPUT_PRESS 1 // GLFW_PRESS
#define INPUT_CONTINUE 3
#define INPUT_ALLOW_PRESS 4
#define INPUT_ALLOW_PRESS_UPDATECURSOR 5
#define INPUT_ALLOW_RELEASE 8
#define INPUT_NOSIGNAL 11
#define INPUT_WAIT 12
// Documentation: Use these to define mod-key combinations, these match the GLFW macros
#define INPUT_MOD_NONE 0
#define INPUT_MOD_SHIFT 1
#define INPUT_MOD_CTRL 2
#define INPUT_MOD_ALT 4
#define INPUT_MOD_SHIFTCTRL 3
#define INPUT_MOD_SHIFTALT 5
#define INPUT_MOD_CTRLALT 6
#define INPUT_MOD_SHIFTCTRLALT 7
// #define for custom mouse buttons
#define UI_MOUSE_OVER -1 //GLFW_MOUSE_BUTTON_LEFT
#define UI_MOUSE_LEFT 0 //GLFW_MOUSE_BUTTON_LEFT
#define UI_MOUSE_RIGHT 1 //GLFW_MOUSE_BUTTON_RIGHT
#define UI_MOUSE_MIDDLE 2 //GLFW_MOUSE_BUTTON_MIDDLE
#define UI_MOUSE_LEFT_DBL 10 // LEFT + 10
#define UI_MOUSE_RIGHT_DBL 11 // RIGHT + 10 
#define UI_MOUSE_MIDDLE_DBL 12 // MIDDLE + 10

// InputData is used for packaging MouseEvents into a single object
struct InputData
{
private:
	std::vector<MouseEvent> emptyEvents = {};
public:
	int inputModKey = 0; // Use INPUT_MOD_NONE / INPUT_MOD_SHIFT / INPUT_MOD_ALT / INPUT_INTERSECT
	MouseEvent start;
	MouseEvent end;
	std::vector<MouseEvent> inputEvents;
	void reset()
	{
		if (!inputEvents.empty()) { inputEvents.clear(); }
		//inputEvents.clear();
		inputEvents.swap(emptyEvents);
		inputModKey = 0;
		start.reset();
		end.reset();
	}
};

/*
// UI Structs
*/

// WProperty is a struct for holding attribute information prior to parsing
struct WProperty
{
	std::string fieldName;
	std::string fieldValue;
	WProperty() {}
	WProperty(std::string name, std::string value)
	{
		this->fieldName = name;
		this->fieldValue = value;
	}
};

// WPlugin is a struct for holding data read from .plugin files
class WPlugin
{
public:
	std::string name;
	std::string version;
	std::string about;
	std::string script_namespace;
	std::filesystem::path plugin_path;
	std::vector<std::filesystem::path> layout_paths;
	std::vector<std::filesystem::path> style_paths;
	std::vector<std::filesystem::path> script_paths;
	std::vector<WProperty> pluginProperties;
	WPlugin () {}
	WPlugin(std::string name, std::string version, std::string about, std::string script_namespace,
		std::filesystem::path pluginP, 
		std::vector<std::filesystem::path> layout_paths, 
		std::vector<std::filesystem::path> style_paths,
		std::vector<std::filesystem::path> script_paths)
	{
		this->name = name;
		this->version = version;
		this->about = about;
		this->script_namespace = script_namespace;
		this->plugin_path = pluginP;
		this->layout_paths = layout_paths;
		this->style_paths = style_paths;
		this->script_paths = script_paths;
	}
};



#endif