#ifndef APPLICATION_H
#define APPLICATION_H

#include <RTSCOM.h>
#include <glm.hpp>

typedef struct GLFWwindow GLFWwindow;

class UI;
class Camera;
class Toolbox;
class WidgetStyle;
#include "methods/IOClasses.h"
#include "methods/InputMethod.h"
#include "methods/OutputMethod.h"
#include "../include/WinStylusHandler.h"
#include "cscript/ChromaScript.h"
#include "cscript/CObject.h"
#include "clayout/ChromaLayout.h"
#include "cstyle/ChromaStyle.h"
#include "cscript/CCallable.h"
#include "ctoolfile/ChromaToolFile.h"
#include "entities/layers/Canvas.h"
#include "entities/layers/Layer.h"
#include "entities/visuals/Stroke.h"
#include "CustomCursor.h"

#include <vector>
#include <memory>
#include <map>

struct TimerCallback
{
	double startTime = 0;
	double duration = 0;
	int repeat = 0;
	bool forever = false;
	std::shared_ptr<CFunction> callFunction = nullptr;
	TimerCallback(double startTime, double duration, int repeat, std::shared_ptr<CFunction> callFunction)
	{
		this->startTime = startTime;
		this->duration = duration;
		this->repeat = repeat;
		if (repeat == -1) { this->forever = true; }
		this->callFunction = callFunction;
	}
	~TimerCallback()
	{
		this->callFunction.reset();
	}
};

class Application : public std::enable_shared_from_this<Application>
{
private:
	// Monitor Variables
	float displayScaleX = 0.0f;
	float displayScaleY = 0.0f;
	int displaySizeX = 0;
	int displaySizeY = 0;
	int displayPixelsX = 0;
	int displayPixelsY = 0;
	float displayDPI = 0.0f;
	std::shared_ptr<Application> shared;
	float winRatio = 0.0f; // ratio in Height / Width
	bool isLandscape = true;
	int winWidth = 0;
	int winHeight = 0;
	glm::ivec2 winDimensions = glm::ivec2(0);
	glm::vec4 bgColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	
	// Input Handler Variables
	double dblThreshold = 0.10;
	double dblForgiveness = 0.15;
	double lastClickTime = -1;
	InputMouseButton lastMouseButton = InputMouseButton::null;
	bool isMouseDrag = false;
	bool isDragStarted = false;
	bool dragReleaseGate = true;
	InputMouseButton dragMouseButtton = InputMouseButton::null;
	bool hasMouseHold = false;
	Input heldInput;
	Input heldDragPosition;
	glm::vec2 lastUIMousePos = glm::vec2(0);

	// Shader Variables
	std::shared_ptr<Shader> compositeShader = nullptr;
	std::shared_ptr<Shader> compositeFrameShader = nullptr;
	std::shared_ptr<Shader> frameShader = nullptr;
	std::shared_ptr<Shader> canvasShader = nullptr;
	std::shared_ptr<Shader> layerShader = nullptr;
	std::shared_ptr<Shader> shardShader = nullptr;
	std::shared_ptr<Shader> widgetShader = nullptr;
	std::shared_ptr<Shader> textShader = nullptr;
	std::shared_ptr<Shader> debugLineShader = nullptr;
	std::shared_ptr<Shader> previewShader_pointLines = nullptr;
	std::shared_ptr<Shader> previewShader_curves = nullptr;
	std::shared_ptr<Shader> gradientBoxShader = nullptr;
	std::shared_ptr<Shader> graphWidgetShader = nullptr;
	std::shared_ptr<Shader> noiseWidgetShader = nullptr;

	// Stylus Variables
	WinStylusHandler* stylusHandler_win = new WinStylusHandler();
	IRealTimeStylus* stylus_win = nullptr;
	double storedDownscaleFactor = 0;

	// Camera variables
	std::shared_ptr<Camera> camera = nullptr;
	bool dynamicPan = false;

	// Callback Arrays
	std::map<std::string, std::shared_ptr<CFunction>> storedEventListenerCallbacks;
	std::map<std::string, std::shared_ptr<CFunction>> keyListenerAllBlockingCallbacks;
	std::map<std::string, std::shared_ptr<CFunction>> keyListenerAllCallbacks;
	std::map<std::string, std::shared_ptr<CFunction>> keyListenerBlockingCallbacks;
	std::map<std::string, std::shared_ptr<CFunction>> keyListenerCallbacks;
	std::map<std::string, std::shared_ptr<CFunction>> canvasRightClickPressCallbacks;

	// TimerCallbackMap
	std::map<std::string, TimerCallback> timerCallbackMap;
	std::vector<std::string> cancelTimerIDs;

	// I/O Variables
		// Management variables
	InputKey keyWatch = InputKey::unknown;
	bool isDoingInput = false;
	InputModKey modKeys = InputModKey::none;
	int mousePosBufferMax = 3;
	float mouseVelocity = 0.0f;
	float maxMouseVelocity = 2500.0f;
	std::vector<Input> mousePosBuffer = { Input(), Input(), Input(), Input() };

protected:
public:
	// Clipboard
	std::string textClipboard = "";

	// History Tables
	std::vector<std::pair<std::weak_ptr<Layer>, std::weak_ptr<Stroke>>> undoStrokes;

	// UI variables
	std::shared_ptr<UI> ui = nullptr;

	// Tool/Toolbox variables
	std::shared_ptr<Toolbox> toolbox = nullptr;

	// Window Variables
	GLFWwindow* appWindow = nullptr;

	// Window Variables (public)
	bool shouldClose = false;

	// Script Console
	std::shared_ptr<ChromaScript> scriptConsole = nullptr;
	std::shared_ptr<ChromaStyle> styleConsole = nullptr;
	std::shared_ptr<ChromaLayout> layoutConsole = nullptr;
	// File Consoles
	std::shared_ptr<ChromaToolFile> toolFileConsole = nullptr;


	// Constructors
	Application();
	Application(int width, int height);

	// Undo Functions
	int undoRequestCount = 0;
	void clearUndoRequests();
	// Undo Lists
	void requestRemove_targetStroke(std::weak_ptr<Layer> layer, std::weak_ptr<Stroke> target);

	// Monitor Functions
	void setMonitorDPI();
	float getMonitorDPI();
	void centerWindowToMonitor();

	// Window Functions
	void createNewWindow(int width, int height);
	void moveApplication(int xpos, int ypos);
	void resizeApplication(int width, int height);
	void closeApplication();
	void minimizeApplication();
	void maximizeApplication();
	void setWindowProperties(); // By framebufferSize
	void setWindowProperties(int width, int height); // By initialization / indirect framebufferSize
	GLFWwindow* getWindow();
	glm::ivec2 getWindowPosition();
	glm::ivec2* getWindowDimensions();
	int getWindowWidth();
	int getWindowHeight();
	float getWindowRatio();
	bool getWindowOrientation();
	glm::vec4 getBoundsVec4();
	void setWindowColor(glm::vec4 color);
	void setShared(std::shared_ptr<Application> ptr);
	std::shared_ptr<Application> getShared();
	Application* getSelf();

	// Shader Functions
	void initializeShaders();
	std::shared_ptr<Shader> getCompositeShader() { return compositeShader; }
	std::shared_ptr<Shader> getCompositeFrameShader() { return compositeFrameShader; }
	std::shared_ptr<Shader> getFrameShader() { return frameShader; }
	std::shared_ptr<Shader> getCanvasShader() { return canvasShader; }
	std::shared_ptr<Shader> getLayerShader() { return layerShader; }
	std::shared_ptr<Shader> getShardShader() { return shardShader; }
	std::shared_ptr<Shader> getWidgetShader() { return widgetShader; }
	std::shared_ptr<Shader> getTextShader() { return textShader; }
	std::shared_ptr<Shader> getDebugLineShader() { return debugLineShader; }
	std::shared_ptr<Shader> getPreviewShader_pointLines() { return previewShader_pointLines; }
	std::shared_ptr<Shader> getPreviewShader_curves() { return previewShader_curves; }
	std::shared_ptr<Shader> getGradientBoxShader() { return gradientBoxShader; }
	std::shared_ptr<Shader> getGraphWidgetShader() { return graphWidgetShader; }
	std::shared_ptr<Shader> getNoiseWidgetShader() { return noiseWidgetShader; }

	// Stylus Functions
	bool createWinStylus();
	void releaseWinStylus();
	WinStylusHandler* getWinStylusHandler();
	IRealTimeStylus* getWinStylus();
	double getStoredDownscaleFactor();
	void saveDownscaleFactor(double factor);

	// Camera Functions
	void createOrthoCamera();
	std::shared_ptr<Camera> getCamera();
	bool getDynamicPan() { return dynamicPan; }

	// Color Functions
	CColor sampleScreen(int x, int y);
	CColor sampleScreen(int x, int y, int radius);

	// UI Functions
	void createUI();
	std::shared_ptr<UI> getUI();

	// Canvas Functions
	void renderCanvas_toFile(std::shared_ptr<Canvas> target);
	glm::ivec2 getCanvasDimensions();

	// Callback Functions
	bool bindCallback(std::shared_ptr<CInterpreter> interpreter,
		CCallbackType callType, std::string callerID,
		std::shared_ptr<CFunction> callFunc);
	// Timer Callbacks
	void updateTimerCallbacks(double time);
	void bindTimerCallback(std::string id, double duration, int repeat, std::shared_ptr<CFunction> callFunc);
	void cancelTimerCallback(std::string id);

	// Cursor Functions
	void setCursor(CursorType selector);

	// I/O Management Functions
	void updateMouseBuffer(Input inputEvent);
	void updateBufferMetaData(Input inputEvent);
	void setModKeys(InputModKey mods) { modKeys = mods; }
	InputModKey getModKeys() { return modKeys; }
	bool getIsDoingInput() { return isDoingInput; }
	// I/O Keybind Functions
	bool isValidKeybind_tool(Keybind keybind);
	bool isValidKeybind_alphaOnly(Keybind keybind);
	bool isValidKeybind_symbolOrChar(Keybind keybind);
	bool isValidKeybind_modOnly(Keybind keybind);
	// I/O Key Functions
	bool getKeyState(InputKey namedKey);
	bool getKeyState(InputModKey modKey);
	void keyEventHandler(Keybind keybind, InputAction action);
	void textInputHandler(Keybind keybind, InputAction action);
	// I/O Mouse Functions
	void setDragStart() { isDragStarted = true; }
	bool getDragStart() { return isDragStarted; }
	void clearMouseHold(double time);
	void clickEventHandler(InputMouseButton button, InputAction action, Input inputEvent, bool clearHold);
	void mousePosEventHandler(Input inputEvent);
	void mouseScrollEventHandler(Input inputEvent);
	Input getMousePosition(bool isUIFetch);
	float getMouseVelocity(float averagingStrength);
	std::vector<Input> getMouseBuffer();
	Input* getMouseBuffer_back();
	Input* getMouseBuffer_doubleBack();

	// Tool/Toolbox Functions
	void initializeToolbox();
	std::shared_ptr<Toolbox> getToolbox();

	// Render Fucntions
	void clearScreen();

	// Callback Functions
	bool triggerStoredEventCallback(std::string callbackID, bool eraseEvent);

	// Miscellaneous Functions
	double getTime();
	glm::vec4 convertScreenCoord(double x, double y);
	glm::vec3 pickScreenCoord(double x, double y);
	void inputDataDump(InputData dat, std::string name);
	void fragmentDataDump(std::shared_ptr<VertexData> dat, std::string name, int UEID);
	void pluginDataDump(std::vector<std::shared_ptr<WPlugin>> pluginArray);
	void styleDataDump(std::vector<std::shared_ptr<WidgetStyle>> styleArray);
};

#endif