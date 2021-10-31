/*
chroma.io:

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Documentation Guidelines:
//
////////////////////////////////////////////////////////////////////////////////////////////////

-Keep track of things that need to be fixed by commenting "Warning:", "Fix:", "Suggestions:"

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Features Documentation/Notes:
//
////////////////////////////////////////////////////////////////////////////////////////////////

-The In_Draw method features axis-constraints for X,Y and +/-45. The constraints are applied to the input.

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Important Information:
//
////////////////////////////////////////////////////////////////////////////////////////////////

*/

// Enable Memory Alloc Tracking
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
//

#include "include/WinStylusHandler.h"

//#include <GLFW/glfw3.h>
#include "include/gladHelper.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "include/math/math.h"
#include "include/math/Color.h"
#include "include/loadImageData.h"

#include "include/Application.h"
#include "include/cscript/ChromaScript.h"
#include "include/tool/Tool.h"
#include "include/tool/Toolbox.h"
#include "include/entities/layers/Layer.h"
#include "include/Canvas.h"
#include "include/entities/widgets/Widget.h"
#include "include/entities/UserInterface.h"
#include "include/Camera.h"
#include "include/methods/IOClasses.h"
#include "include/ctoolfile/ChromaToolFile.h"

#include "include/CustomCursor.h"
#include "include/Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

#include <cmath>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <map>

// GLOBAL DEBUG VARIABLES
bool doStrokeDebugFrames = false;
bool doDebugMouseInput = true;

// GLOBAL VARIABLES
#define APP_NAME "chroma.io 0.0.6de"
std::shared_ptr<Application> chromaIO;
int WINDOW_WIDTH;
int WINDOW_HEIGHT;
float DELTA_TIME = 0.0f;
float LAST_FRAME = 0.0f;
bool skipNextMoveCB = false;
int skipCount = 0;
FILE *stream;

// CALLBACK FUNCTIONS
// OpenGL Debug Callback message printer
void APIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id,
	GLenum severity, GLsizei length,
	const GLchar *msg, const void *data);
// Window Resize Callback
static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// Mouse Callbacks
void custom_click_callback(GLFWwindow* window, int button, int action, int mods);
static void custom_cursor_callback(GLFWwindow* window, double xpos, double ypos);
static void custom_cursor_callback(GLFWwindow* window, double xpos, double ypos);
// Key Event Callback
static void custom_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void bindCursorCallbacks();
void unbindCursorCallbacks();

int main()
{
// APPLICATION SETUP
	// glfw: Initialize and Configure OpenGL Parameters
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE); // ENABLE DEBUG CONTEXT
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	// Initialize the Application Window
	chromaIO = std::make_shared<Application>(1080, 1080);
	chromaIO.get()->setShared(chromaIO);
	chromaIO.get()->setWindowColor(glm::vec4(0.19f, 0.19f, 0.19f, 1.0f));
	chromaIO.get()->setMonitorDPI();

	// glad: Load All Open GL Function Pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}
	// Bind error callback function
	glDebugMessageCallback(GLDebugMessageCallback, NULL);

	// Set Initial Blend Parameters
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Redirect error stream to log file
	freopen_s(&stream, "log/log.txt", "w+", stderr);

	// Use set window width / height by getFramebufferSize
	chromaIO.get()->setWindowProperties(); 

	// Create uninitialized UI object
	chromaIO.get()->createUI();

	// Create uninitialized Toolbox object
	chromaIO.get()->initializeToolbox();

	// Initialize chromaScript, chromaStyle, chromaLayout console
	chromaIO.get()->scriptConsole = std::make_shared<ChromaScript>(chromaIO);
	chromaIO.get()->scriptConsole.get()->initializeConsole();
	chromaIO.get()->styleConsole = std::make_shared<ChromaStyle>(chromaIO, chromaIO.get()->scriptConsole);
	chromaIO.get()->styleConsole.get()->initializeConsole();
	chromaIO.get()->layoutConsole = std::make_shared<ChromaLayout>(chromaIO, chromaIO.get()->scriptConsole);
	chromaIO.get()->layoutConsole.get()->initializeConsole();
	chromaIO.get()->toolFileConsole = std::make_shared<ChromaToolFile>(chromaIO);
	chromaIO.get()->toolFileConsole.get()->initializeConsole();

// INITIALIZE THE STYLUS
	// If Windows
	bool winStylusSuccess = chromaIO.get()->createWinStylus();
	if (!winStylusSuccess)
	{
		std::cout << "INITIALIZATION::FAILURE::STYLUS_NOT_CREATED" << std::endl;
	}
	else
	{
		std::cout << "INITIALIZATION::SETSTYLUSVARIABLES::BEGIN" << std::endl;
		chromaIO.get()->getWinStylusHandler()->owner = chromaIO.get()->getShared();
		chromaIO.get()->getWinStylusHandler()->updateTabletDPI();
	}

// INITIALIZE SHADERS
	// Instruct application to construct the shaders
	chromaIO.get()->initializeShaders();
	chromaIO.get()->ui->visualizer->setPreviewShaders(
		chromaIO.get()->getPreviewShader_pointLines(), 
		chromaIO.get()->getPreviewShader_curves());

// INITIALIZE UI, CAMERA, CANVAS, AND TOOLS
	// Initialize UI
	chromaIO.get()->getUI()->updateCursorImage(chromaIO.get()->toolbox.get()->getCursor(CursorType::pointer));
	chromaIO.get()->getUI()->initializeInterface();

	// Create Camera
	chromaIO.get()->createOrthoCamera();

	// Initialize the Toolbox
	chromaIO.get()->toolbox.get()->initializeTools(true, false);

	// Bind resize callback function
	glfwSetFramebufferSizeCallback(chromaIO.get()->getWindow(), framebuffer_size_callback);
	// Ensure that the window, camera, and tablet input all receive the correct window dimensions
	// Until this is done, the application believes the window is it's created size, but the window size
	// is actually it's created size minus the titlebar height
	framebuffer_size_callback(chromaIO.get()->getWindow(), chromaIO.get()->getWindowWidth(), chromaIO.get()->getWindowHeight());
	// Bind click callback function
	bindCursorCallbacks();
	// Bind key event callback function
	glfwSetKeyCallback(chromaIO.get()->getWindow(), custom_key_callback);

// BIND KEYBOARD EVENT CALLBACKS
	// Bind keys

// MISCELLANEOUS & TEST INITIALIZATION STEPS
	// For undecorated window, adjust window position
	chromaIO.get()->centerWindowToMonitor();
	chromaIO.get()->toolbox.get()->setActiveTool_byID(0);

// EXECUTION LOOP
	while (chromaIO.get()->shouldClose == false)
	{
	// Update Timing Loop
		float currentFrame = (float)glfwGetTime();
		DELTA_TIME = currentFrame - LAST_FRAME;
		LAST_FRAME = currentFrame;

	// Update Objects That Need Delta
		chromaIO.get()->getCamera()->updateSpeed(DELTA_TIME);
		if (chromaIO.get()->getDynamicPan() == true) 
		{
			chromaIO.get()->getCamera()->updateMomentum(chromaIO.get()->getIsDoingInput(), DELTA_TIME);
		}
		chromaIO.get()->clearMouseHold(currentFrame);
		

	// Do Upkeep
		// Undo Requests
		chromaIO.get()->clearUndoRequests();
		// UI Upkeep
		chromaIO.get()->ui.get()->clearResizeEvents();
		chromaIO.get()->ui.get()->checkFocusVisibility();
		chromaIO.get()->ui.get()->clearRebuildRequests();
		chromaIO.get()->ui.get()->clearDeletionRequests();

	// Input Loop
		
	// Render Loop
		// Clear the Buffer
		chromaIO.get()->clearScreen();

		ShaderTransform* xform = chromaIO.get()->getCamera()->getShaderTransform();
		// Draw the canvas background (Checker Pattern)
		if (chromaIO.get()->getUI()->activeCanvas != nullptr)
		{
			chromaIO.get()->getUI()->getCanvas()->draw(xform);
			// Draw the canvas objects
			chromaIO.get()->getUI()->getCanvas()->drawLayers(xform);
			// Draw Virtual Objects (Brush Tip, Preview Data, Vertices, Lines, etc.)
			
			chromaIO.get()->ui->visualizer->draw(xform, chromaIO.get()->getWindowDimensions());
		}

		// Draw the UI
		chromaIO.get()->getUI()->drawWidgets();
		// Update Timer Callbacks
		chromaIO.get()->updateTimerCallbacks(currentFrame);
		
		// Swap the Buffers and poll hardware IO events
		glfwSwapBuffers(chromaIO.get()->getWindow());
		glfwPollEvents();
	// Debug Loop
		// Check anything that should be looked at

	// Check ShouldClose (Handles application X button)
		if ((bool)glfwWindowShouldClose(chromaIO.get()->getWindow()) == true) {
#ifdef _CRTDBG_MAP_ALLOC
			_CrtDumpMemoryLeaks(); 
#endif
			chromaIO.get()->shouldClose = true; 
		}
	}
	// Save application settings and save backup for current documents (if any)

	// Close application with success
	glfwTerminate();
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// OpenGL Error Callback Message
//
////////////////////////////////////////////////////////////////////////////////////////////////

// OpenGL debug message callback printer (Very useful, native to openGL 4+)
void APIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id,
	GLenum severity, GLsizei length,
	const GLchar *msg, const void *data)
{
	const char* _source;
	const char* _type;
	const char* _severity;
	// Purge unnecessary notifications
	if (id == 131185)
	{
		return;
	}
	switch (source) {
	case GL_DEBUG_SOURCE_API:
		_source = "API";
		break;

	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		_source = "WINDOW SYSTEM";
		break;

	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		_source = "SHADER COMPILER";
		break;

	case GL_DEBUG_SOURCE_THIRD_PARTY:
		_source = "THIRD PARTY";
		break;

	case GL_DEBUG_SOURCE_APPLICATION:
		_source = "APPLICATION";
		break;

	case GL_DEBUG_SOURCE_OTHER:
		_source = "UNKNOWN";
		break;

	default:
		_source = "UNKNOWN";
		break;
	}

	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		_type = "ERROR";
		break;

	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		_type = "DEPRECATED BEHAVIOR";
		break;

	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		_type = "UDEFINED BEHAVIOR";
		break;

	case GL_DEBUG_TYPE_PORTABILITY:
		_type = "PORTABILITY";
		break;

	case GL_DEBUG_TYPE_PERFORMANCE:
		_type = "PERFORMANCE";
		break;

	case GL_DEBUG_TYPE_OTHER:
		_type = "OTHER";
		break;

	case GL_DEBUG_TYPE_MARKER:
		_type = "MARKER";
		break;

	default:
		_type = "UNKNOWN";
		break;
	}

	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:
  		_severity = "HIGH";
		break;

	case GL_DEBUG_SEVERITY_MEDIUM:
		_severity = "MEDIUM";
		break;

	case GL_DEBUG_SEVERITY_LOW:
		_severity = "LOW";
		break;

	case GL_DEBUG_SEVERITY_NOTIFICATION:
		_severity = "NOTIFICATION";
		break;

	default:
		_severity = "UNKNOWN";
		break;
	}

	printf("%d: %s of %s severity, raised from %s: %s\n",
		id, _type, _severity, _source, msg);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Callback Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

// OpenGL Window Resize Callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	int newvolume = width * height;
	int oldvolume = chromaIO.get()->getWindowWidth() * chromaIO.get()->getWindowHeight();
	// Area increases as a square, so to get a better rezie factor old/new, take the sqroot of each
	float resizeFactor = std::sqrtf((float)oldvolume) / std::sqrtf((float)newvolume);
	chromaIO.get()->setWindowProperties(width, height);
	chromaIO.get()->getCamera()->setOrthoXY(chromaIO.get()->getWindowRatio());
	chromaIO.get()->getCamera()->setOrthoZoom(chromaIO.get()->getCamera()->getOrthoZoom() / resizeFactor);
	// Update the orthographic projection (To avoid rendering distortion)
	chromaIO.get()->getCamera()->setProjection();
	// Re-center the camera
	chromaIO.get()->getCamera()->setPosition(resizeFactor); // Scale the current position offset by the resizeFactor
	if (chromaIO.get()->getUI()->activeCanvas == nullptr)
	{
		chromaIO.get()->getCamera()->centerToCanvas(glm::ivec2(512, 512));
	}
	else
	{
		chromaIO.get()->getCamera()->centerToCanvas(chromaIO.get()->getUI()->getCanvas()->getDimensions());
	}
	// Update the DPI factor for the tablet input
	chromaIO.get()->getWinStylusHandler()->updateTabletDPI();
	chromaIO.get()->ui.get()->buildAllWidgetTrees();
	std::cout << "WINDOW::RESIZED" << "::WIDTH=" << width << "::HEIGHT=" << height << std::endl;
}

// OpengGL Mouse & Click Callback
void custom_click_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (chromaIO.get()->getWinStylusHandler()->getStylusIsHover() == true)
		return;
	Input* backEvent = chromaIO.get()->getMouseBuffer_back();
	InputModKey modKey = static_cast<InputModKey>(mods);
	InputMouseButton buttonEnum = static_cast<InputMouseButton>(button);
	InputAction actionEnum = static_cast<InputAction>(action);
	Input click(
		modKey, buttonEnum, actionEnum,
		InputFlag::null, InputFlag::null,
		backEvent->x, backEvent->y, (float)glfwGetTime(), 1.0f, 0.0f, 0.0f, 0.0f, chromaIO.get()->getMouseVelocity(0.2f));
	//Input click(button, action, backEvent->x, backEvent->y, glfwGetTime(), modKey, InputFlag::null,
	//	1.0f, 0.0f, 0.0f, 0.0f);
	switch (buttonEnum)
	{
	case InputMouseButton::left:
	case InputMouseButton::right:
	case InputMouseButton::middle:
		chromaIO.get()->clickEventHandler(buttonEnum, actionEnum, click, false);
		break;
	}
}
static void custom_cursor_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (chromaIO.get()->getWinStylusHandler()->getStylusIsHover() == true)
		return;
	InputAction moveAction = (glfwGetMouseButton(chromaIO->getWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) ? InputAction::press : InputAction::move;
	Input move(chromaIO.get()->getModKeys(), InputMouseButton::hover, moveAction, InputFlag::null, InputFlag::null,
		xpos, ypos, (float)glfwGetTime(), 1.0f, 0.0f, 0.0f, 0.0f, chromaIO.get()->getMouseVelocity(0.2f));
	chromaIO.get()->updateMouseBuffer(move);
	chromaIO.get()->mousePosEventHandler(move);
}
static void custom_scroll_callback(GLFWwindow* window, double xpos, double ypos)
{
	Input scroll(chromaIO.get()->getModKeys(), InputMouseButton::scroll, InputAction::press, InputFlag::null, InputFlag::null,
		xpos, ypos, (float)glfwGetTime(), 1.0f, 0.0f, 0.0f, 0.0f, chromaIO.get()->getMouseVelocity(0.2f));
	chromaIO.get()->mouseScrollEventHandler(scroll);
}

// OpenGL Key Event Callback
static void custom_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Logic block for cosmetic cursor effects, because I/O has no frame-tick event to update cursor, manually
	// set the cursor here depending on toolID and mod type
	InputModKey modKey = static_cast<InputModKey>(mods);
	if (chromaIO.get()->getToolbox()->checkActiveTool())
	{
		int activeID = chromaIO.get()->getToolbox()->getActiveTool().get()->id;
		if (activeID == DT_ZOOM_SCRUB)
		{
			// Note: Later this must be expanded to handle all tools that have multi-cursors
			if (modKey == InputModKey::alt && chromaIO.get()->getIsDoingInput() == false)
			{
				chromaIO.get()->getUI()->updateCursorImage(chromaIO.get()->getToolbox()->getActiveTool()->getCursorDown());
			}
			else if (chromaIO.get()->getIsDoingInput() == false)
			{
				chromaIO.get()->getUI()->updateCursorImage(chromaIO.get()->getToolbox()->getActiveTool()->getCursorUp());
			}
		}
	}
	chromaIO.get()->setModKeys(modKey);
	chromaIO.get()->keyEventHandler(
		Keybind(key * 10, mods), 
		static_cast<InputAction>(action));
}

// OpenGL bind/unbind Mouse/Cursor Callback
void bindCursorCallbacks()
{
	// Bind click callback function
	glfwSetMouseButtonCallback(chromaIO.get()->appWindow, custom_click_callback);
	// Bind cursor callback function
	glfwSetCursorPosCallback(chromaIO.get()->appWindow, custom_cursor_callback);
	glfwSetScrollCallback(chromaIO.get()->appWindow, custom_scroll_callback);
}
void unbindCursorCallbacks()
{
	// Bind click callback function
	glfwSetMouseButtonCallback(chromaIO.get()->appWindow, NULL);
	// Bind cursor callback function
	glfwSetCursorPosCallback(chromaIO.get()->appWindow, NULL);
	glfwSetScrollCallback(chromaIO.get()->appWindow, NULL);
}