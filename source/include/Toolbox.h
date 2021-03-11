#define DT_CLOSE_WINDOW -1
#define DT_NO_TOOL 0
#define DT_PAN 1
#define DT_ROTATE 2
#define DT_ZOOM_SCRUB 3
#define DT_ZOOM_IN 4
#define DT_ZOOM_OUT 5
#define DT_BRUSH 10
#define DT_PENCIL 11
#define DT_MIXER 12
#define DT_SMUDGE 13
#define DT_ERASER 20
#define DT_COLORPICKER 31
#define DT_MOVE 35
#define DT_SELECT_LASSO 40
#define DT_SELECT_POLYGON 41
#define DT_SELECT_RECTANGLE 42
#define DT_SELECT_CIRCLE 46
#define DT_SELECT_MAGIC 50
#define DT_SHAPE 60
#define DT_SHAPE_CUSTOM 61
#define DT_STROKE_RECTANGLE 62
#define DT_STROKE_SHAPEFIELDPOINTS 63
#define DT_STROKE_VORTEX 64
#define DT_STROKE_RAKE 65
#define DT_STROKE_FAN 66
#define DT_STROKE_SHAPELINE 67
#define DT_STROKE_LASSO 68
#define DT_STROKE_SHAPEDRAW 69
#define DT_STROKE_SCANLINE 70
#define DT_STROKE_POLYLINE 71
#define DT_TEXT_HORIZONTAL 80
#define DT_TEXT_VERTICAL 81
#define DT_STAMP_SAMPLE 90
#define DT_STAMP_PATTERN 91
#define DT_BLUR 100
#define DT_DODGE 101
#define DT_BURN 102
#define DT_FILL_UNBOUND 120
#define DT_FILL_BUCKET 121
#define DT_FILL_SELECTION 122
#define DT_GRADIENT_UNBOUND 140
#define DT_GRADIENT_BUCKET 141
#define DT_GRADIENT_SELECTION 142
#define DT_RULER_GUIDE 180
#define DT_RULER_PERSPECTIVE 181
#define DT_RULER_ANGLE 182
#define DT_RULER_CIRCLE 183

#ifndef TOOLBOX_H
#define TOOLBOX_H
#include "structs.h"

#include "CustomCursor.h"
#include "cscript/CEnums.h"
#include "ToolSettings.h"
#include "Tool.h"

class Application;
class CFunction;
class CInterpreter;
class CObject;

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <filesystem>

class Toolbox : public std::enable_shared_from_this<Toolbox>
{
private:
// Callback Arrays
	std::map<std::string, std::shared_ptr<CFunction>> colorpickStartCallbacks;
	std::map<std::string, std::shared_ptr<CFunction>> colorpickMoveCallbacks;
	std::map<std::string, std::shared_ptr<CFunction>> colorpickEndCallbacks;
	//
	std::map<std::string, std::shared_ptr<CFunction>> toolSwitchCallbacks;

// Cursor pointers (Useful to have these for easier custom tool creation)
	std::shared_ptr<CustomCursor> pointer;
	std::shared_ptr<CustomCursor> hand;
	std::shared_ptr<CustomCursor> grab;
	std::shared_ptr<CustomCursor> rotate;
	std::shared_ptr<CustomCursor> zoomScrub;
	std::shared_ptr<CustomCursor> zoomIn;
	std::shared_ptr<CustomCursor> zoomOut;
	std::shared_ptr<CustomCursor> brush;
	std::shared_ptr<CustomCursor> dropper;
	std::shared_ptr<CustomCursor> circleSmall;
	// Tool Variables
	std::vector<std::shared_ptr<Tool>> tools;
	std::shared_ptr<Tool> activeTool;
protected:
public:
	Toolbox();
	// Cursor Functions (Use to create library of cursors for making tools with
	std::shared_ptr<CustomCursor> getCursor(int name);
	// Check Functions
	bool checkValidIOCombination(std::shared_ptr<CInterpreter> interpreter, std::string inputName, std::string outputName);
	bool checkValidIOCombination(std::shared_ptr<CInterpreter> interpreter, int inputMacro, int outputMacro);
	bool checkValidControlScheme(std::shared_ptr<CInterpreter> interpreter, std::string inputName, std::string controlScheme);
	bool checkValidControlScheme(std::shared_ptr<CInterpreter> interpreter, int inputMacro, int controlMacro);
	int convertIOStringToMacro(int kind, std::string name);
	TSetType convertControlMacro(int macro);
	// Input Functions
	int sendClick(Application* sender, MouseEvent dat);
	int sendMove(Application* sender, MouseEvent dat);
	void sendPreview(Application* sender);
	void sendFinialize(Application* sender);
	// Callback Functions
	bool bindCallback(std::shared_ptr<CInterpreter> interpreter,
		CCallbackType callType, std::string callerID,
		std::shared_ptr<CFunction> callFunc);
	void toolSwitchCallback();
	void startCallback(std::shared_ptr<CInterpreter> interpreter, double xpos, double ypos);
	void moveCallback(std::shared_ptr<CInterpreter> interpreter, double xpos, double ypos);
	void endCallback(std::shared_ptr<CInterpreter> interpreter, double xpos, double ypos);
	// Tool Functions
	void initializeTools(bool isNew);
	bool createCustomTool(
		std::string cursorUp, std::string cursorDown,
		int toolID, std::string toolName,
		std::string inputMethod, std::string controlScheme, std::string outputMethod,
		int modBit, int glfwKey);
	void createDefaultTools();
	void setActiveTool_byID(int id);
	void setActiveTool_byIndex(int i);
	void notifyColorChange(CColor input, bool isBG);
	int checkToolHotkeys(int sig);
	std::shared_ptr<Tool> getActiveTool();
};

#endif