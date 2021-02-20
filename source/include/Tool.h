/*
// Tool Structs
*/
// Default Tool Group Names
#define TOOLS_DEFAULT 0
#define TOOLS_CAMERA 1

// #define block for Tool I/O methods
// Use this to more easily select function calls in callback functions for tool input/output
#define IN_NO_INPUT 0
#define IN_DRAG 1
#define IN_ROTATE 2
#define IN_ZOOM 3
#define IN_DRAW 4
#define IN_LINE 5
#define IN_RECTANGLE 6
#define IN_ELLIPSE 7
#define IN_NGON 8
#define IN_LASSO 9
#define IN_POLYGON 10
#define IN_PAN 11
#define IN_POINT 13
#define IN_SAMPLER 14
#define IN_SHAPEFIELD 15
#define IN_VORTEX 16
#define IN_RAKE 17
#define IN_FAN 18
#define IN_SHAPELINE 20
#define IN_SHAPEDRAW 21
#define IN_POLYLINE 22

#define OUT_NO_OUTPUT 0
#define OUT_CAMERAPAN 100
#define OUT_CAMERAROTATE 101
#define OUT_CAMERAZOOM 102
#define OUT_STROKE 103
#define OUT_SHAPE 104
#define OUT_GRADIENT 105
#define OUT_FILL 106
#define OUT_SAMPLER 107
#define OUT_SELECTION 110

#ifndef TOOL_H
#define TOOL_H

#include "structs.h"
#include "CustomCursor.h"
#include "Color.h"

#include "methods/InputMethod.h"
#include "methods/OutputMethod.h"

#include "ToolSettings.h"

class Toolbox;
class UI;
class CObject;
class CInterpreter;

#include <string>
#include <functional>
#include <map>
#include <memory>

class Tool : public std::enable_shared_from_this<Tool>
{
public:
	// Tool Identity
	int id;
	int inID;
	int outID;
	std::string name;

	// Tool Keybind
	int keySig;

	// Cursors
	std::shared_ptr<CustomCursor> cursorUp;
	std::shared_ptr<CustomCursor> cursorDown;

	// I/O Methods
	std::unique_ptr<InputMethod> input;
	std::unique_ptr<OutputMethod> output;

	// Containing Toolbox
	std::shared_ptr<Toolbox> owner;

	// Input data for this tool
	InputData data;

	// Tool Settings
	bool lockSettings = false;
	bool autoSaveSettings = false;
	std::map<TSetType, std::unique_ptr<ToolSettings>> settings;

	// Constructors
	Tool(
		std::shared_ptr<CustomCursor> cursorUp,
		std::shared_ptr<CustomCursor> cursorDown,
		int id, std::string n,
		int inID, int outID,
		TSetType controlScheme,
		int key, std::shared_ptr<Toolbox> owner);
	~Tool();
	// Initializer (Required due to shared_from_this())
	void initializeTool(TSetType controlScheme);

	// Activate
	std::shared_ptr<Tool> activate(std::pair<CColor, CColor> colors, std::shared_ptr<UI> ui);

	// Settings
	//void createMethods(int inID, int outID);
	//void buildSettings(std::vector<TSetType> inputMask, std::vector<TSetType> outputMask);
	void createMethods(TSetType controlScheme);
	void buildSettings();
	bool matchMask(TSetType query, std::vector<TSetType> interestMask);
	// Quick Settings Fetchers
	TSet_ContinuousControl* getContinuousControl();
	TSet_DragControl* getDragControl();
	TSet_OnePointControl* getOnePointControl();
	TSet_TwoPointControl* getTwoPointControl();
	TSet_ThreePointControl* getThreePointControl();
	//
	TSet_Sampler* getSampler();
	TSet_Pan* getPan();
	TSet_Rotate* getRotate();
	TSet_Zoom* getZoom();
	TSet_Basic* getBasic();
	TSet_Smoothing* getSmoothing();
	TSet_Image* getImage();
	TSet_Polygon* getPolygon();
	TSet_Alpha* getAlpha();
	TSet_Field* getField();
	TSet_Vortex* getVortex();
	TSet_Rake* getRake();
	TSet_Fan* getFan();
	TSet_PolyLine* getPolyLine();

	// Colors
	CColor getFGColor();
	CColor getBGColor();
	std::pair<bool, CColor> setFGColor(CColor color, bool isActivation);
	std::pair<bool, CColor> setBGColor(CColor color, bool isActivation);

	// Cursor Functions
	std::shared_ptr<CustomCursor> getCursorUp() { return cursorUp; }
	std::shared_ptr<CustomCursor> getCursorDown() { return cursorDown; }

	// Data Functions
	InputData* getData() { return &data; }
	void setData(InputData dat) { data = dat; }

	// Method Functions
	InputMethod* getInput() { return input.get(); }
	OutputMethod* getOutput() { return output.get(); }

	// Settings Functions
	bool checkInterestMask(std::shared_ptr<CInterpreter> interpreter, std::string name);
	std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, int settingsSig, std::shared_ptr<CObject> object, bool isGet);
};

#endif