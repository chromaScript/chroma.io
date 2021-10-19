#ifndef TOOL_H
#define TOOL_H

#include "../input/keys.h"
#include "../CustomCursor.h"
#include "../math/Color.h"

#include "../methods/InputMethod.h"
#include "../methods/OutputMethod.h"
#include "../methods/MethodType.h"

#include "ToolSettings.h"
#include "toolSettings/ToolSettings_Forward.h"

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
private:
	std::vector<char> serialData = {};
public:
	// Meta-Data Uses Property ID's 0-100 Currently
	// Tool Identity
	int id; // 0
	MethodType inID; // 1
	MethodType outID; // 2
	std::string name; // 3
	//Image icon;
	bool useTipAsIcon; // 8
	CColor iconHighlightColor; // 11
	CColor iconFillColor; // 12
	// Meta-Data
	std::vector<std::string> tags; // 15
	std::string category = ""; // 16
	std::string toolType = ""; // 17
	std::string date = ""; // 18
	std::string author = ""; // 19

	// Tool Keybind
	int bindType = 0; // 20
	Keybind keybind; // 21

	// Cursors
	std::shared_ptr<CustomCursor> cursorUp; // 24
	std::shared_ptr<CustomCursor> cursorDown; // 25

	// I/O Methods
	std::unique_ptr<InputMethod> input; // 30 // inputName - 33
	std::unique_ptr<OutputMethod> output; // 31 // outputName - 34
		// Control Scheme input->controlScheme // 32


	// Containing Toolbox
	std::shared_ptr<Toolbox> owner; // 40
	int activeSettingsPage = 0; // 41

	// Input data for this tool
	InputData data;

	// Tool Settings
	bool lockSettings = false; // 51
	bool autoSaveSettings = false; // 52
	std::map<TSetType, std::unique_ptr<ToolSettings>> settings;

	// Constructors
	Tool(
		std::shared_ptr<CustomCursor> cursorUp,
		std::shared_ptr<CustomCursor> cursorDown,
		int id, std::string n,
		MethodType inID, MethodType outID,
		TSetType controlScheme,
		Keybind keybind, std::shared_ptr<Toolbox> owner);
	Tool(
		std::shared_ptr<CustomCursor> cursorUp,
		std::shared_ptr<CustomCursor> cursorDown,
		int id, std::string n,
		MethodType inID, MethodType outID,
		TSetType controlScheme,
		Keybind keybind, std::shared_ptr<Toolbox> owner,
		std::vector<std::string> tags, std::string category, std::string type, std::string date, std::string author);
	~Tool();
	// Initializer (Required due to shared_from_this())
	void initializeTool(TSetType controlScheme);

	// Activate
	std::shared_ptr<Tool> activate(std::pair<CColor, CColor> colors, std::shared_ptr<UI> ui);

	// Send Key Input Query - Check active tool for any settings that might absorb key input
	bool sendInput(Keybind input, InputModKey activeModKey);

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
	TSet_Blend* getBlend();
	TSet_Basic* getBasic();
	TSet_Character* getCharacter();
	TSet_Smoothing* getSmoothing();
	TSet_Image* getImage();
	TSet_Polygon* getPolygon();
	TSet_PolyLine* getPolyLine();
	TSet_Alpha* getAlpha();
	TSet_Color* getColor();
	TSet_Scatter* getScatter();
	TSet_Effects* getEffects();
	TSet_Field* getField();
	TSet_Vector* getVector();
	TSet_Vortex* getVortex();
	TSet_Spline* getSpline();
	TSet_Rake* getRake();
	TSet_Fan* getFan();
	TSet_Gradient* getGradient();
	TSet_Fill* getFill();
	TSet_Shader* getShader();
	TSet_Filter* getFilter();
	TSet_AntiAliasing* getAntiAliasing();
	TSet_Pattern* getPattern();

	// Colors
	CColor getFGColor();
	CColor getBGColor();
	std::pair<bool, CColor> setFGColor(CColor color, bool isActivation);
	std::pair<bool, CColor> setBGColor(CColor color, bool isActivation);

	// Cursor Functions
	std::shared_ptr<CustomCursor> getCursorUp() { return cursorUp; }
	std::shared_ptr<CustomCursor> getCursorDown() { return cursorDown; }

	// Data Functions
	float getSize();
	float getTrueSpacing();
	void serializeToolData();
	std::vector<char> getSerialData();
	InputData* getData() { return &data; }
	void setData(InputData dat) { data = dat; }

	// Method Functions
	InputMethod* getInput() { return input.get(); }
	OutputMethod* getOutput() { return output.get(); }

	// Settings Functions
	bool checkInterestMask(std::shared_ptr<CInterpreter> interpreter, std::string name);
	bool checkInterestMask(TSetType interest);
	std::shared_ptr<CObject> putInterest(
		std::shared_ptr<CInterpreter> interpreter, TSetType interest, std::shared_ptr<CObject> object, bool isGet);
	ToolSettings* getSetting(int settingID, int subSigID);
	std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, int settingsSig, 
		int subSig, std::shared_ptr<CObject> object, bool isGet, 
		bool asPercentage, bool asString);
	std::shared_ptr<CObject> putMetaData(
		std::shared_ptr<CInterpreter> interpreter, int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGetbool, bool asString);
};

#endif