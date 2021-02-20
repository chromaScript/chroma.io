#include <iostream>
#include <Ole2.h>
#include <RTSCOM.h>
#include <rtscom_i.c>
#include <wtypes.h>
#include <msinkaut.h>

#ifndef APPLICATION_H
#include "include/Application.h"
#endif

#include "include/IOClasses.h"

#include "include/Camera.h"
#include "include/Toolbox.h"
#include "include/entities/UserInterface.h"
#include "include/methods/InputMethod.h"
#include "include/WinStylusHandler.h"

#include "include/entities/WidgetStyle.h"

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

extern bool doDebugMouseInput;

// Constructors
Application::Application()
{

}
Application::Application(int width, int height)
{
	createNewWindow(width, height);
	// Note that in an application that is not using windowed-full-screen mode (no title bar), the
	// actual window width and height will vary from the size of the window as "created"
	// Usually the height is height - title bar.
	setWindowProperties(width, height);
}
// Shader Functions
void Application::initializeShaders()
{
	compositeShader = std::make_shared<Shader>(
		"source/shaders/compositeShader.vert", "source/shaders/compositeShader.frag", "COMPOSITE_SHADER");
	std::cout << "APPLICATION::COMPOSITE_SHADER::BOUND" << std::endl;
	//
	compositeFrameShader = std::make_shared<Shader>(
		"source/shaders/compositeFrameShader.vert", "source/shaders/compositeFrameShader.frag", "COMPOSITE_FRAME_SHADER");
	std::cout << "APPLICATION::COMPOSITE_FRAME_SHADER::BOUND" << std::endl;
	//
	frameShader = std::make_shared<Shader>(
		"source/shaders/frameShader.vert", "source/shaders/frameShader.frag", "FRAME_SHADER");
	std::cout << "APPLICATION::FRAME_SHADER::BOUND" << std::endl;
	//
	canvasShader = std::make_shared<Shader>(
		"source/shaders/canvasShader.vert", "source/shaders/canvasShader.frag", "CANVAS_SHADER");
	std::cout << "APPLICATION::CANVAS_SHADER::BOUND" << std::endl;
	//
	layerShader = std::make_shared<Shader>(
		"source/shaders/layerShader.vert", "source/shaders/layerShader.frag", "LAYER_SHADER");
	std::cout << "APPLICATION::LAYER_SHADER::BOUND" << std::endl;
	//
	shardShader = std::make_shared<Shader>(
		"source/shaders/shardShader.vert", "source/shaders/shardShader.frag", "SHARD_SHADER");
	std::cout << "APPLICATION::STROKE_SHADER::BOUND" << std::endl;
	//
	widgetShader = std::make_shared<Shader>(
		"source/shaders/widgetShader.vert", "source/shaders/widgetShader.frag", "WIDGET_SHADER");
	std::cout << "APPLICATION::WIDGET_SHADER::BOUND" << std::endl;
	//
	textShader = std::make_shared<Shader>(
		"source/shaders/textShader.vert", "source/shaders/textShader.frag", "TEXT_SHADER");
	std::cout << "APPLICATION::TEXT_SHADER::BOUND" << std::endl;
	//
	debugLineShader = std::make_shared<Shader>(
		"source/shaders/debugLineShader.vert", "source/shaders/debugLineShader.frag", "DEBUG_LINE_SHADER");
	std::cout << "APPLICATION::DEBUG_LINE_SHADER::BOUND" << std::endl;
	//
	gradientBoxShader = std::make_shared<Shader>(
		"source/shaders/gradientBoxShader.vert", "source/shaders/gradientBoxShader.frag", "GRADIENT_BOX_SHADER");
	std::cout << "APPLICATION::GRADIENT_BOX_SHADER::BOUND" << std::endl;
	//
}

// Monitor Functions
void Application::setMonitorDPI()
{
	GLFWmonitor* primary = glfwGetPrimaryMonitor();
	glfwGetMonitorContentScale(primary, &displayScaleX, &displayScaleY);
	glfwGetMonitorPhysicalSize(primary, &displaySizeX, &displaySizeY);
	const GLFWvidmode* mode = glfwGetVideoMode(primary);
	displayPixelsX = mode->width;
	displayPixelsY = mode->height;
	float dpiX = ((float)displayPixelsX / (float)displaySizeX) * displayScaleX;
	float dpiY = ((float)displayPixelsY / (float)displaySizeY) * displayScaleY;
	if (abs(dpiX - dpiY) >= .01)
	{
		std::cout << "MONITOR::WARNING::NON_SQUARE_DPI" << std::endl;
	}
	displayDPI = (dpiX + dpiY) / 2.0f;
	std::cout << "MONITOR::DPIX=" << dpiX << "::DPIY=" << dpiY << "::DPIAVERAGE" << displayDPI << std::endl;
	
}
void Application::centerWindowToMonitor()
{
	int x = (displayPixelsX - winWidth) / 2;
	int y = (displayPixelsY - winHeight) / 2;
	glfwSetWindowPos(appWindow, x, y);
}
float Application::getMonitorDPI() { return displayDPI; }

// Window Functions
void Application::createNewWindow(int width, int height)
{
	appWindow = glfwCreateWindow(width, height, "chroma.io", NULL, NULL);
	if (appWindow == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(appWindow);
}
void Application::closeApplication()
{
	shouldClose = true;
	glfwSetWindowShouldClose(appWindow, true);
	std::cout << "Exit Application" << std::endl;
}
void Application::minimizeApplication()
{
	glfwIconifyWindow(appWindow);
	std::cout << "Minimize Application" << std::endl;
}
void Application::maximizeApplication()
{
	glfwRestoreWindow(appWindow);
	std::cout << "Maximize Application" << std::endl;
}
void Application::setWindowProperties()
{
	glfwGetFramebufferSize(appWindow, &winWidth, &winHeight);
	winRatio = (float)winWidth / (float)winHeight;
	if (winRatio >= 1.0f)
	{
		isLandscape = true;
	}
	else
	{
		isLandscape = false;
	}
	WINDOW_WIDTH = winWidth;
	WINDOW_HEIGHT = winHeight;
}
void Application::setWindowProperties(int width, int height)
{
	winRatio = (float)width / (float)height;
	if (winRatio >= 1.0f)
	{
		isLandscape = true;
	}
	else
	{
		isLandscape = false;
	}
	winWidth = width;
	winHeight = height;
}
void Application::setWindowColor(glm::vec4 color)
{
	bgColor = color;
}
GLFWwindow* Application::getWindow() { return appWindow; }
int Application::getWindowWidth() { return winWidth; }
int Application::getWindowHeight() { return winHeight; }
float Application::getWindowRatio() { return winRatio; }
bool Application::getWindowOrientation() { return isLandscape; }
glm::vec4 Application::getBoundsVec4() { return glm::vec4(0.0f, 0.0f, (float)winWidth, (float)winHeight); }
Application* Application::getSelf() { return this; }
void Application::setShared(std::shared_ptr<Application> ptr) { shared = ptr; }
std::shared_ptr<Application> Application::getShared() { return shared; }

// Stylus Functions
bool Application::createWinStylus()
{
	// Create the stylus object
	HRESULT result = CoCreateInstance(CLSID_RealTimeStylus, NULL, CLSCTX_ALL, IID_PPV_ARGS(&stylus_win));
	if (FAILED(result))
	{
		std::cout << "APPLICATION::CREATEWINSTYLUS::COCREATEINSTANCE::FAILED" << std::endl;
		return false;
	}
	// Attach the stylus object to the application window
	result = stylus_win->put_HWND((HANDLE_PTR)GetActiveWindow());
	if (FAILED(result))
	{
		std::cout << "APPLICATION::CREATEWINSTYLUS::PUT_HWND::FAILED" << std::endl;
		releaseWinStylus();
		return false;
	}
	// Create free-threaded marshaller for this object and aggregate it. (Note: ?)
	result = CoCreateFreeThreadedMarshaler(stylusHandler_win, &stylusHandler_win->FTMarshaler);
	if (FAILED(result))
	{
		std::cout << "APPLICATION::CREATEWINSTYLUS::COCREATEFTMARSHALER::FAILED" << std::endl;
		releaseWinStylus();
		return false;
	}
	// Add the WinStylusHandler object to the list of synchronous plugins in the RealTimeStylus object
	result = stylus_win->AddStylusAsyncPlugin(0, stylusHandler_win);
	if (FAILED(result))
	{
		std::cout << "APPLICATION::CREATEWINSTYLUS::COCREATEINSTANCE::FAILED" << std::endl;
		releaseWinStylus();
		return false;
	}
	// Set desired data packets
	GUID packetProperties[] = {
		GUID_PACKETPROPERTY_GUID_X,						// X Location
		GUID_PACKETPROPERTY_GUID_Y,						// Y Location
		GUID_PACKETPROPERTY_GUID_NORMAL_PRESSURE,		// Pen Pressure
		GUID_PACKETPROPERTY_GUID_X_TILT_ORIENTATION,	// X Tilt
		GUID_PACKETPROPERTY_GUID_Y_TILT_ORIENTATION,	// Y Tilt
		GUID_PACKETPROPERTY_GUID_AZIMUTH_ORIENTATION	// Rotation
	};
	result = stylus_win->SetDesiredPacketDescription(6, packetProperties);
	if (FAILED(result))
	{
		std::cout << "APPLICATION::CREATEWINSTYLUS::SETDESIREDPACKETDESCRIPTION::FAILED" << std::endl;
		releaseWinStylus();
		return false;
	}
	// Set Data Packet Size
	ULONG pSize = sizeof(packetProperties) / sizeof(GUID);
	result = stylusHandler_win->setPropertiesCount(pSize);
	if (FAILED(result))
	{
		std::cout << "APPLICATION::CREATEWINSTYLUS::SETNUMBERPROPERTIES::FALSE::FAILED" << std::endl;
		releaseWinStylus();
		return false;
	}
	// Disable mouse input
	result = stylus_win->SetAllTabletsMode(FALSE);
	if (FAILED(result))
	{
		std::cout << "APPLICATION::CREATEWINSTYLUS::SETALLTABLETSMODE::FALSE::FAILED" << std::endl;
		releaseWinStylus();
		return false;
	}
	// Enable the stylus
	result = stylus_win->put_Enabled(true);
	if (FAILED(result))
	{
		std::cout << "APPLICATION::CREATEWINSTYLUS::PUT_ENABLED::FAILED" << std::endl;
		releaseWinStylus();
		return false;
	}
	std::cout << "APPLICATION::CREATEWINSTYLUS::FINISHED" << std::endl;
	return true;
}
void Application::releaseWinStylus()
{
	if (stylus_win)
	{
		stylusHandler_win->Release(); // Member function of this Handler class, not unique_ptr.release(); !
		stylusHandler_win = NULL;
	}

	if (stylusHandler_win)
	{
		stylusHandler_win->Release(); // Member function of this Handler class, not unique_ptr.release(); !
		stylusHandler_win = NULL;
	}
}
WinStylusHandler* Application::getWinStylusHandler()
{
	if (stylusHandler_win == nullptr)
	{
		return nullptr;
	}
	return stylusHandler_win;
}
IRealTimeStylus* Application::getWinStylus()
{
	if (stylus_win == nullptr)
	{
		return nullptr;
	}
	return stylus_win;
}

// Camera Functions
void Application::createOrthoCamera() 
{ 
	camera = std::make_shared<Camera>(
		0.05f, 0.05f, 0.05f, 
		winRatio, 
		ui.get()->getCanvas()->getDimensions());
}
std::shared_ptr<Camera> Application::getCamera() { return camera; }

// UI Functions
void Application::createUI()
{
	ui = std::make_shared<UI>(shared_from_this());
}
std::shared_ptr<UI> Application::getUI() { return ui; }

// Canvas Functions
glm::ivec2 Application::getCanvasDimensions() 
{ 
	return ui.get()->getCanvas().get()->getDimensions(); 
}
void Application::renderCanvas_toFile()
{
	float* result = ui.get()->getCanvas().get()->renderCanvas();
	glm::ivec2 dimensions = getCanvasDimensions();

	unsigned int headers[13];
	FILE* outFile;
	errno_t err;
	int extrabytes;
	int paddedsize;
	int x; int y; int n;

	extrabytes = 4 - ((dimensions.x * 3) % 4);
	if (extrabytes == 4) { extrabytes = 0; }

	paddedsize = ((dimensions.x * 3) + extrabytes) * dimensions.y;

	headers[0] = paddedsize + 54;
	headers[1] = 0;
	headers[2] = 54;
	headers[3] = 40;
	headers[4] = dimensions.x;
	headers[5] = dimensions.y;
	
	headers[7] = 0;
	headers[8] = paddedsize;
	headers[9] = 0;
	headers[10] = 0;
	headers[11] = 0;
	headers[12] = 0;

	std::filesystem::path root = std::filesystem::current_path();
	root /= std::filesystem::path("renders");
	std::string outPath = "renders/image0.bmp";
	if (std::filesystem::exists(root) && std::filesystem::is_directory(root))
	{
		int i = 0;
		for (auto const& entry : std::filesystem::recursive_directory_iterator(root))
		{
			if (std::filesystem::is_regular_file(entry) && entry.path().extension() == ".bmp")
			{
				std::string filePath = entry.path().stem().string();
				size_t suffixLoc = 0;
				size_t pos = 0;
				size_t end = 0;
				std::string numString = "";
				if (filePath.find("image") != std::string::npos)
				{
					pos = filePath.find("image") + 5;
					while (isdigit(filePath[pos]))
					{
						numString.push_back(filePath[pos]);
						pos++;
					}
					int fileNum = std::stoi(numString);
					if (fileNum >= i)
					{
						i = fileNum + 1;
					}
				}
			}
		}
		outPath = "renders/image" + std::to_string(i) + ".bmp";
	}

	err = fopen_s(&outFile, outPath.c_str(), "wb");

	fprintf(outFile, "BM");

	for (n = 0; n <= 5; n++)
	{
		fprintf(outFile, "%c", headers[n] & 0x000000FF);
		fprintf(outFile, "%c", (headers[n] & 0x0000FF00) >> 8);
		fprintf(outFile, "%c", (headers[n] & 0x00FF0000) >> 16);
		fprintf(outFile, "%c", (headers[n] & (unsigned int)0xFF000000) >> 24);
	}

	fprintf(outFile, "%c", 1);
	fprintf(outFile, "%c", 0);
	fprintf(outFile, "%c", 24);
	fprintf(outFile, "%c", 0);

	for (n = 7; n <= 12; n++)
	{
		fprintf(outFile, "%c", headers[n] & 0x000000FF);
		fprintf(outFile, "%c", (headers[n] & 0x0000FF00) >> 8);
		fprintf(outFile, "%c", (headers[n] & 0x00FF0000) >> 16);
		fprintf(outFile, "%c", (headers[n] & (unsigned int)0xFF000000) >> 24);
	}

	for (y = dimensions.y - 1; y >= 0; y--)     // BMP image format is written from bottom to top...
	{
		for (x = 0; x <= dimensions.x - 1; x++)
		{

			unsigned char r = (unsigned char)(result[((x + (y * dimensions.x)) * 4) + 0] * 255);
			unsigned char g = (unsigned char)(result[((x + (y * dimensions.x)) * 4) + 1] * 255);
			unsigned char b = (unsigned char)(result[((x + (y * dimensions.x)) * 4) + 2] * 255);

			// Also, it's written in (b,g,r) format...

			fprintf(outFile, "%c", b);
			fprintf(outFile, "%c", g);
			fprintf(outFile, "%c", r);
		}
		if (extrabytes)      // See above - BMP lines must be of lengths divisible by 4.
		{
			for (n = 1; n <= extrabytes; n++)
			{
				fprintf(outFile, "%c", 0);
			}
		}
	}

	fclose(outFile);

	ui.get()->getCanvas().get()->deleteRenderData();
}

// Toolbox Functions
void Application::initializeToolbox()
{ 
	toolbox = std::make_shared<Toolbox>();
	
}
std::shared_ptr<Toolbox> Application::getToolbox() { return toolbox; }

// Render Functions
void Application::clearScreen()
{
	glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.y);
	//glClear(GL_COLOR_BUFFER_BIT); // Dont need to do this? Only need one call with both bit options
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Cursor Functions
void Application::setCursor(int selector)
{
	ui.get()->updateCursorImage(toolbox.get()->getCursor(selector));
}

// Callback Functions
bool Application::bindCallback(std::shared_ptr<CInterpreter> interpreter,
	CCallbackType callType, std::string callerID,
	std::shared_ptr<CFunction> callFunc)
{
	bool validSignature = true;
	switch (callType)
	{
	case CCallbackType::canvasRightClickPress:
		// scriptSignature - void function(num xpos, num ypos)
		std::vector<std::shared_ptr<CToken>> paramTypes = callFunc.get()->funcDeclaration.get()->paramsTypes;
		if (paramTypes.size() != 2) { validSignature = false; }
		if (callFunc.get()->funcDeclaration.get()->returnType.get()->type != CTokenType::_VOID) { validSignature = false; }
		if (paramTypes[0].get()->type != CTokenType::NUM) { validSignature = false; }
		if (paramTypes[1].get()->type != CTokenType::NUM) { validSignature = false; }
		break;
	}
	if (!validSignature)
	{
		interpreter.get()->console.get()->error("[toolbox:0101] Callback function supplied for '" + CCallbackNames[size_t(callType)] +
			"' does not match the required signature. Check the documentation for callback signatures.");
		return false;
	}
	// Then if there were no errors, bind the callback
	switch (callType)
	{
	case CCallbackType::canvasRightClickPress:
		canvasRightClickPressCallbacks.insert(std::pair<std::string, std::shared_ptr<CFunction>>(callerID, callFunc));
		break;
	}
	return true;
}

// Timer Callbacks
void Application::updateTimerCallbacks(double time)
{
	if (timerCallbackMap.size() == 0) { return; }
	if (time >= 15)
	{
		int i = 0;
	}
	double nowTime = glfwGetTime();
	std::vector<std::string> clearIDs;
	for (auto& timer : timerCallbackMap)
	{
		if (nowTime >= timer.second.startTime + timer.second.duration)
		{
			std::string timerID = timer.first;
			// Do call
			std::vector<std::shared_ptr<CObject>> args = { std::make_shared<CObject>(nowTime) };
			timer.second.callFunction.get()->call(scriptConsole.get()->getInterpreter(), &args);
			if (timerCallbackMap.count(timerID) == 1)
			{
				if (timer.second.forever == true)
				{
					timer.second.startTime = nowTime;
				}
				else if (timer.second.repeat > 0)
				{
					timer.second.startTime = nowTime;
					timer.second.repeat--;
				}
				else
				{
					clearIDs.push_back(timer.first);
				}
			}
		}
	}
	clearIDs.insert(clearIDs.end(), cancelTimerIDs.begin(), cancelTimerIDs.end());
	for (std::string id : clearIDs)
	{
		if (timerCallbackMap.count(id)) { timerCallbackMap.erase(id); }
	}
	cancelTimerIDs.clear();
}
void Application::bindTimerCallback(
	std::string id, double duration, int repeat, 
	std::shared_ptr<CFunction> callFunc)
{
	if (timerCallbackMap.count(id) == 1)
	{
		timerCallbackMap.at(id).duration += duration;
		timerCallbackMap.at(id).repeat = repeat;
	}
	else
	{
		timerCallbackMap.insert(std::pair<std::string, TimerCallback>(id, TimerCallback(glfwGetTime(), duration, repeat, callFunc)));
	}
}
void Application::cancelTimerCallback(std::string id)
{
	// Allow the updateTimerCallbacks to clear the timer rather than immediately delete
	// This gives better safety for when a callback clears itself
	if (timerCallbackMap.count(id) == 1)
	{
		cancelTimerIDs.push_back(id);
	}
}

// Keyboard Callback & Set Callbacks
void Application::keyEventHandler(int sig, int action)
{
	// Debug Setting: PRINT
	bool debugPrint = false;
	// Key Watcher
	if (keyWatchSig > 0)
	{
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			std::cout << "KEYEVENTHANDLER::ABORT" << std::endl;
			return;
		}
		else if (action == GLFW_RELEASE && (sig / 10) == keyWatchSig)
		{
			std::cout << "KEYEVENTHANDLER::KEYWATCHSIG::RELEASED::=" << keyWatchSig << std::endl;
			keyWatchSig = 0;
			return;
		}
	}
	// Handle Key Events By Object List Query and Signature Match
	// Key signature (int sig) is they ((keyboard key * 10) + modbit), which produces a unique signature for any normal hotkey
	// By using int, we guarantee accuracy of the key-sig.
	// Assigning a "Hotkey" to something doesn't assign a literal leter, but the appropriate openGL key-sig, which can be read back out to string for UI display
	// Because each context level has similar or identical function calls (Tools merely setActive, Buttons would clickButton, etc.) we can be fairly confident
	// that any potential output function can be easily selected and handled.

	// First check if a text box or entry field is in focus
	
	// Neck check if currently drawing with a tool

	// Do immediate filtering by Esc/Backspace or any other static keybinds
	// Do basic filtering by range/exact >= , <= or == (Cull Ctrl/Shift/Alt/CapsLock/Tab/PgUp/PgUp/PgDn/End/Prtsc/Enter/Backspace/F10/F11/F12/Fn/Del)
	if (sig >= 2560 && sig <= 2567) // ESCAPE
	{
		// If active context or brush stroke, cancel action without closing application
		if (debugPrint == true) { std::cout << "KEYHANDLER::" << "ESCAPE" << std::endl; }
		shouldClose = true;
		return;
	}
	if (sig >= 2610 && sig <= 2617) // DELETE
	{
		if (debugPrint == true) { std::cout << "KEYHANDLER::" << "DELETE" << std::endl; }
		return;
	}
	if (sig >= 320 && sig <= 327) // SPACE
	{
		if (debugPrint == true) { std::cout << "KEYHANDLER::" << "SPACE" << std::endl; }
		return;
	}
	if (sig >= 2620 && sig <= 2657) // RIGHT / LEFT / DOWN / UP ARROWS
	{
		if (debugPrint == true) { std::cout << "KEYHANDLER::" << "RIGHT / LEFT / DOWN / UP ARROWS" << std::endl; }
		return;
	}
	if (sig >= 2990 && sig <= 3017) // F10 / F11 / F12
	{
		if (debugPrint == true) { std::cout << "KEYHANDLER::" << "F10 / F11 / F12" << std::endl; }
		return;
	}
	if (sig >= 2570 && sig <= 2607) // ENTER / TAB / BACKSPACE / INSERT
	{
		if (debugPrint == true) { std::cout << "KEYHANDLER::" << "ENTER / TAB / BACKSPACE / INSERT" << std::endl; }
		return;
	}
	if (sig >= 2660 && sig <= 2847) // PAGE_UP / PAGE_DOWN / HOME / END / CAPS_LOCK / SCROLL_LOCK / NUM_LOCK / PRINT_SCREEN / PAUSE
	{
		if (debugPrint == true) { std::cout << "KEYHANDLER::" << "PAGE_UP / PAGE_DOWN / HOME / END / CAPS_LOCK / SCROLL_LOCK / NUM_LOCK / PRINT_SCREEN / PAUSE" << std::endl; }
		return;
	}
	if (sig >= 3400 && sig <= 3487) // RIGHT & LEFT SHIFT / CTRL / ALT / SUPER / MENU
	{
		if (debugPrint == true) { std::cout << "KEYHANDLER::" << "RIGHT & LEFT SHIFT / CTRL / ALT / SUPER / MENU" << std::endl; }
		return;
	}
	if (sig >= 3487 || sig < 320) // OUT OF BOUNDS SIGNATURE
	{
		if (debugPrint == true) { std::cout << "KEYHANDLER::" << "OUT OF BOUNDS SIGNATURE" << std::endl; }
		return;
	}
	// Want to do actions on press, rather than release
	if (action == GLFW_PRESS)
	{
		// Only interested in watching non-modifier keys, so divide by 10 to remove modifier bit
		// The modifier bit is always set regardless of application state, so tools will still know the modifier state
		keyWatchSig = sig / 10;
		std::cout << "KEYEVENTHANDLER::KEYWATCHSIG::SET::=" << keyWatchSig << std::endl;
	}

	
	// 1. Query Context (If a popup, submenu, dropdown, or alternate window is in focus)
	// 2. Query Tools (While more numerous in binds than Menus/Actions/Macros, they are more often used)
	int tIndex = toolbox->checkToolHotkeys(sig);
	if (tIndex != -1 && !isDoingInput)
	{
		toolbox->setActiveTool_byIndex(tIndex);
		ui->updateCursorImage(toolbox->getActiveTool()->getCursorUp());
	}
	// 3. Query UI Non-Tool Buttons (This may just be included in actions/macros for simplicity, as they have similar outputs)
	// 4. Query Actions/Macros (Could be more or less binds than Menus, but they are more important)
	// 5. Query Menus (Least important keybinds, and potentially the fewest in number for users with a lot of tools/macros/actions)
	if (sig == 832)
	{
		renderCanvas_toFile();
	}
}



// Update the Mouse Buffer
MousePosition Application::getMousePosition(bool isUIFetch)
{
	double xpos = 0, ypos = 0;
	float t = (float)glfwGetTime();
	if (isUIFetch && stylusHandler_win->getStylusIsHover())
	{
		xpos = mousePosBuffer.back().x;
		ypos = mousePosBuffer.back().y;
	}
	else
	{
		glfwGetCursorPos(appWindow, &xpos, &ypos);
	}
	MousePosition outPos = MousePosition(xpos, ypos, t);
	return outPos;
}
void Application::updateMouseBuffer(MouseEvent mouseEvent)
{
	if (mousePosBuffer.size() < mousePosBufferMax)
	{
		mousePosBuffer.push_back(mouseEvent);
	}
	else
	{
		mousePosBuffer.erase(mousePosBuffer.begin());
		mousePosBuffer.push_back(mouseEvent);
	}
	float timeSum = 0.0f;
	for (auto const& e : mousePosBuffer) { timeSum += (float)abs(mousePosBuffer.front().time - e.time); }
	mouseVelocity = glm::length(
		glm::vec2((float)mouseEvent.x, (float)mouseEvent.y) -
		glm::vec2((float)mousePosBuffer.front().x, (float)mousePosBuffer.front().y)) / timeSum;
	mouseVelocity /= maxMouseVelocity;
}
void Application::updateBufferMetaData(MouseEvent mouseEvent)
{
	MouseEvent* backEvent = &mousePosBuffer.back();
	backEvent->pressure = mouseEvent.pressure;
	backEvent->tiltx = mouseEvent.tiltx;
	backEvent->tilty = mouseEvent.tilty;
	backEvent->rotation = mouseEvent.rotation;
}
std::vector<MouseEvent> Application::getMouseBuffer() { return mousePosBuffer; }
MouseEvent* Application::getMouseBuffer_back()
{
	return &mousePosBuffer.back();
}
MouseEvent* Application::getMouseBuffer_doubleBack()
{
	return &mousePosBuffer[mousePosBuffer.size() - 2];
}

// Mouse Event Handlers
void Application::clearMouseHold(double time)
{
	// Clear existing hold
	MouseEvent savedEvent = heldMouseEvent;
	// Update drag state if waiting to start
	if (!isMouseDrag && dragReleaseGate && dragMouseButtton != -1 && time - lastClickTime > dblThreshold)
	{
		if (doDebugMouseInput) { std::cout << "APP::MOUSEDRAG::PENDING" << std::endl; }
		clickEventHandler(heldMouseEvent.button, heldMouseEvent.action, heldMouseEvent, true);
		if (isDragStarted) { isMouseDrag = true; dragMouseButtton = savedEvent.button; }
		else { dragReleaseGate = false; }
	}
	else if (hasMouseHold && time - lastClickTime > dblThreshold)
	{
		clickEventHandler(heldMouseEvent.button, heldMouseEvent.action, heldMouseEvent, true);
	}
	// Clear clickedWidget once the hold is cleared
	if (!hasMouseHold && ui.get()->clickReleaseGate && !ui.get()->clickedWidget.expired() && 
		glfwGetMouseButton(appWindow, savedEvent.button) == 0)
	{
		savedEvent.action = 0;
		clickEventHandler(savedEvent.button, 0, savedEvent, false);
		ui.get()->clickReleaseGate = false;
	}
	
}
void Application::clickEventHandler(int button, int action, MouseEvent mouseEvent, bool clearHold)
{
	//std::cout << "APP::CLICK_CALLBACK::TIME=" << mouseEvent.time << " X= " << mouseEvent.x << " Y= " << mouseEvent.y << std::endl;

	ui.get()->didFocusThisClick = false;
	// ClearHold entry point for mainLoop to clear click events
	if (clearHold)
	{
		if (doDebugMouseInput) { std::cout << "APP::CLEARHOLD" << std::endl; }
		lastClickTime = lastMouseButton = -1;
		hasMouseHold = false;
		bool result = ui.get()->widgetHitTest(mouseEvent, false);
		return;
	}

	// Check if can be valid UI click
	bool uiGate = !isDoingInput; // Replace later with more complex function??

	if (uiGate && ui.get()->widgetSweepTest(mouseEvent))
	{
		// Set mouse drag
		if (!hasMouseHold && mouseEvent.action != 0)
		{
			dragMouseButtton = mouseEvent.button;
		}
		// Wait/Check for double click handling
		// Check if a sweepTest (does not execute event) returns true
		
		if (!hasMouseHold && mouseEvent.action != 0 && ui.get()->widgetSweepTest(mouseEvent))
		{
			// Do immediate trigger of drag events rather than wait for the hold to release
			if (ui.get()->widgetHitTest(mouseEvent, true))
			{
				return;
			}
			if (doDebugMouseInput) { std::cout << "APP::SETMOUSEHOLD" << std::endl; }
			lastClickTime = mouseEvent.time;
			lastMouseButton = mouseEvent.button;
			hasMouseHold = true;
			heldMouseEvent = mouseEvent;
			return;
		}
		// MouseHold exists, if the clicks match, modify the incoming event, and send it
		else if (hasMouseHold && mouseEvent.action != 0 && mouseEvent.button == lastMouseButton)
		{
			if (true) { std::cout << "APP::DOUBLECLICK" << std::endl; }
			mouseEvent.button += 10;
			// Run hitTest, which will fire a callback if valid hit and widget has a callback
			bool result = ui.get()->widgetHitTest(mouseEvent, false);
			lastClickTime = lastMouseButton = -1;
			hasMouseHold = false;
			if (isDragStarted) 
			{ 
				isMouseDrag = true; 
				heldDragPosition = mouseEvent;
				dragMouseButtton = mouseEvent.button - 10; 
			}
			//isMouseDrag = false;
			if (result) { return; }
		}
		// Clear hold if rapid press of two different buttons
		else if (hasMouseHold && mouseEvent.action != 0 && mouseEvent.button != lastMouseButton)
		{
			clickEventHandler(heldMouseEvent.button, heldMouseEvent.action, heldMouseEvent, true);
		}
		// Clear drag with additional check for adding hold time for double click
		else if (mouseEvent.action == 0)
		{
			// Send release commands
			if (!ui.get()->clickedWidget.expired())
			{
				// Widget will only fire the release callback if the pointer is still
				// over the widget at time of release
				MouseEvent recheck = mouseEvent;
				recheck.x = getMousePosition(false).x;
				recheck.y = getMousePosition(false).y;
				ui.get()->clickedWidget.lock().get()->selfHit(recheck, false);
				ui.get()->clickedWidget.reset();
			}
			// Send dragRelease commands if was mouse drag
			if (isMouseDrag && dragMouseButtton == mouseEvent.button)
			{
				
				if (!ui.get()->dragWidget.expired())
				{
					if (doDebugMouseInput) { std::cout << "APP::MOUSEDRAG::TRIGGERED::RELEASE" << std::endl; }
					ui.get()->dragWidget.lock().get()->selfDragend(mouseEvent);
				}
				ui.get()->dragWidget.reset();
				isMouseDrag = false;
				isDragStarted = false;
				dragReleaseGate = true;
				dragMouseButtton = -1;
			}
			// Release drag without trigger (if drag never started)
			if (dragMouseButtton == mouseEvent.button)
			{
				// Reset drag variables
				ui.get()->dragWidget.reset();
				isMouseDrag = false;
				isDragStarted = false;
				dragReleaseGate = true;
				dragMouseButtton = -1;
			}
			// Avoid firing subsequent events if hasHold & the button is a release.
			// Give additional timing window for the double click when the button releases
			// This allows for double clicks to have wider windows, and also
			// give drag/hold/singleClick to have faster response times
			if (hasMouseHold)
			{
				lastClickTime += dblForgiveness; return;
			}
		}
		return;
	}
	// Capture any mouse releases when the mouse is no longer hovering a UI widget
	if (mouseEvent.action == 0)
	{
		if (!ui.get()->dragWidget.expired())
		{
			if (doDebugMouseInput) { std::cout << "APP::MOUSEDRAG::TRIGGERED::RELEASE" << std::endl; }
			ui.get()->dragWidget.lock().get()->selfDragend(mouseEvent);
		}
		ui.get()->dragWidget.reset();
		isMouseDrag = false;
		isDragStarted = false;
		dragReleaseGate = true;
		dragMouseButtton = -1;
	}
	// If the click fell through to this point, clear the UI focus widget
	if (mouseEvent.action != 0) 
	{ 
		ui.get()->clearFocusWidget(); 
	}
	

	// Note: "Canvas" really means the entire field of view underneath the UI, not just the actual visible image
	//		Brush/Tool operations can begin from outside the canvas and then enter/leave it without breaking the action
	// If Canvas -> Send to tools
	// 1a. Allow the input functions to reject bad data when starting a new input
	// 1b. Also allow accounting for tools that should not update the cursor through their I/O functions
	//int i = toolbox->getActiveTool()->getInput()->click(mouseEvent);
	int i = toolbox->sendClick(this, mouseEvent);
	switch (i)
	{
	case INPUT_REJECT:
		isDoingInput = false;
		break;
	case INPUT_CONTINUE:
		ui->updateCursorImage(toolbox->getActiveTool()->getCursorDown());
		toolbox->sendPreview(this);
		isDoingInput = true;
		return;
	case INPUT_ALLOW_PRESS:
		toolbox->sendPreview(this);
		toolbox->startCallback(scriptConsole.get()->getInterpreter(), mouseEvent.x, mouseEvent.y);
		isDoingInput = true;
		return;
	case INPUT_ALLOW_PRESS_UPDATECURSOR:
		ui->updateCursorImage(toolbox->getActiveTool()->getCursorDown());
		toolbox->sendPreview(this);
		toolbox->startCallback(scriptConsole.get()->getInterpreter(), mouseEvent.x, mouseEvent.y);
		isDoingInput = true;
		return;
	case INPUT_ALLOW_RELEASE:
		inputDataDump(toolbox->getActiveTool().get()->getInput()->getData(), toolbox->getActiveTool().get()->name); // Debug: Datadump to log file
		ui->updateCursorImage(toolbox->getActiveTool().get()->getCursorUp());
		// Do finalize, then postprocess. Tools without either step simply have blank functions.
		toolbox->sendFinialize(this);
		toolbox->endCallback(scriptConsole.get()->getInterpreter(), mouseEvent.x, mouseEvent.y);
		isDoingInput = false;
		return;
	case INPUT_NOSIGNAL:
		break;
	}
	// Catch fall-through events
	if (!isDoingInput)
	{
		if (mouseEvent.button == UI_MOUSE_RIGHT)
		{
			glm::vec3 clickPos = pickScreenCoord(mouseEvent.x, mouseEvent.y);
			if (ui.get()->canvasHitTest(clickPos))
			{
				if (mouseEvent.action == INPUT_PRESS && canvasRightClickPressCallbacks.size() != 0)
				{
					std::shared_ptr<CObject> xArg = std::make_shared<CObject>(mouseEvent.x);
					std::shared_ptr<CObject> yArg = std::make_shared<CObject>(mouseEvent.y);
					std::vector<std::shared_ptr<CObject>> args = { xArg, yArg };
					for (auto const& item : canvasRightClickPressCallbacks)
					{
						item.second.get()->call(scriptConsole.get()->getInterpreter(), &args);
					}
					return;
				}
			}
		}
	}
}
void Application::mousePosEventHandler(MouseEvent mouseEvent)
{
	//std::cout << "APP::MOUSE_MOVE_CALLBACK::TIME=" << mouseEvent.time << " X= " << mouseEvent.x << " Y= " << mouseEvent.y << std::endl;
	// Update the mouseBuffer & velocity (Always do this, needed for establishing brush direction)

	// Check if leaving a stored enteredWidget, only need to do this when the mouse moves
	if (ui.get()->enteredWidgets.size() != 0)
	{
		if (ui.get()->widgetLeaveTest(mouseEvent) && ui.get()->savedCursor != nullptr && !ui.get()->widgetSweepTest(mouseEvent))
		{
			ui.get()->updateCursorImage(ui.get()->savedCursor);
			ui.get()->savedCursor = nullptr;
		}
	}

	// Suggestions: Add a hit-test of some kind here to check hovering over UI elements & change cursor/UI states
	// Warning: Never lock this function out completely, will always need to be checking hovers & positions
	if (isDoingInput && !isDragStarted)
	{
		// 1. Update the InputData by forwarding the mouseEvent
		int result = toolbox->sendMove(this, mouseEvent);
		// 2. Do preview visualization & fragment data building via Tool.outputMethod.preview
		if (result != INPUT_WAIT)
		{
			toolbox->sendPreview(this);
			toolbox->moveCallback(scriptConsole.get()->getInterpreter(), mouseEvent.x, mouseEvent.y);
		}
	}
	else
	{
		// Check if can be valid UI movement
		bool clickGate = hasMouseHold; // Replace later with more complex function??
		glm::vec2 newUIMousePos = glm::vec2((float)mouseEvent.x, (float)mouseEvent.y);
		float length = glm::length(newUIMousePos - lastUIMousePos);
		if (!clickGate && length > (ROOT2 / 2))
		{
			lastUIMousePos = newUIMousePos;
			if (!isMouseDrag && ui.get()->widgetSweepTest(mouseEvent))
			{
				if (ui.get()->savedCursor == nullptr)
				{
					ui.get()->savedCursor = ui.get()->currentCursor;
					ui.get()->updateCursorImage(toolbox.get()->getCursor(CURSOR_POINTER));
				}
				// Handle mouse-over and mouse-enter/leave events here
				bool result = ui.get()->widgetHoverTest(mouseEvent);
				if (result)
				{
					//std::cout << "APP::MOUSEOVER,MOUSEENTER,MOUSELEAVE" << std::endl;
				}
			}
			else if (isMouseDrag)
			{
				// Dispatch ondrag events if callback is bound
				// Make sure that callbacks only trigger when the mouse has actually updated. This
				// prevents the stylus from spamming inputs to the UI when held down.
				if (mouseEvent.x == heldDragPosition.x && mouseEvent.y == heldDragPosition.y) { return; }
				else { heldDragPosition = mouseEvent; }
				if (!ui.get()->dragWidget.expired())
				{
					ui.get()->dragWidget.lock().get()->selfDrag(mouseEvent);
				}
				// Dispatch ondragover/enter/leave events if bound
				bool result = ui.get()->widgetSweepTest(mouseEvent);
				if (result)
				{
					// Documentation: When a drag event has started, onmouseenter/over/leave
					// should not be fired. Widgets should only now react to drag/drop events
					//std::cout << "APP::ONDRAG,DRAGENTER,DRAGOVER,DRAGLEAVE" << std::endl;
				}
			}
			else
			{
				if (ui.get()->savedCursor != nullptr)
				{
					ui.get()->updateCursorImage(ui.get()->savedCursor);
					ui.get()->savedCursor = nullptr;
				}
			}
		}
	}
}


// Color Functions
CColor Application::sampleScreen(int x, int y)
{
	float pixel[3];
	//std::cout << "GLFW::MOUSE= " << getMousePosition().x << ", " << getMousePosition().y << std::endl;
	//std::cout << "ARGS::MOUSE= " << x << ", " << y << std::endl;
	glReadPixels(x, winHeight - y, 1, 1, GL_RGB, GL_FLOAT, &pixel);
	return CColor(pixel[0], pixel[1], pixel[2]);
}

// Take a screen coordinate as X/Y and return a vec4 as | X & Y in -1 and 1 range | Z as pixel depth | W normal (1.0)
// Used for screen to world projection
glm::vec4 Application::convertScreenCoord(double x, double y)
{
	glm::vec4 outVec;
	float pxDepth;
	glReadPixels((int)x, (int)y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &pxDepth);
	outVec.x = (2.0f * ((float)x / (float)winWidth)) - 1.0f;
	outVec.y = 1.0f - (2.0f * ((float)y / (float)winHeight));
	outVec.z = pxDepth;
	outVec.w = 1.0f;
	return outVec;
}
// Take a screen coordinate and pick an X/Y/Z position in world space
glm::vec3 Application::pickScreenCoord(double x, double y)
{
	// Get the view / projection matrix
	ShaderTransform matrix = getCamera()->getShaderTransform();
	// Convert the screenCoord to NDC
	glm::vec4 screenCoord = convertScreenCoord(x, y);
	// Create the inverse matrix transform and apply it to screenCoord
	glm::mat4 inverseMatrix = matrix.v * matrix.p;
	inverseMatrix = glm::inverse(inverseMatrix);
	screenCoord = screenCoord * inverseMatrix;
	glm::vec3 outPos = glm::vec3(screenCoord.x, screenCoord.y, 0.0f);
	// Translate & Rotate the coordinate using the camera
	outPos = getCamera()->translateWorldPos(outPos);
	outPos.z = 0.0f;
	return outPos;
}
// Data dump function for input data. Will loop through the entire struct and print important info to log file
void Application::inputDataDump(InputData dat, std::string name)
{
	std::cerr << "// --------- BEGIN INPUT DATA DUMP --------- //\n" << "TOOLBOX::ACTIVETOOL::NAME=" << name << "\n";
	std::cerr << "EVENT.INDEX=" << "START"
		<< "::ACTION=" << dat.start.action
		<< "::COORDINATES=(" << dat.start.x
		<< ", " << dat.start.y << ")"
		<< "::TIME=" << dat.start.time
		<< "::FLAG=" << dat.start.flag
		<< "::OP=" << dat.start.modKey 
		<< "\n";
	int i = 0;
	for (const MouseEvent& e : dat.inputEvents)
	{
		std::cerr << "EVENT.INDEX=" << i
			<< "::ACTION=" << e.action
			<< "::COORDINATES=(" << e.x
			<< ", " << e.y << ")"
			<< "::TIME=" << e.time
			<< "::FLAG=" << e.flag
			<< "::OP=" << e.modKey
			<< "\n";
		i++;
	}
	std::cerr << "EVENT.INDEX=" << "END"
		<< "::ACTION=" << dat.end.action
		<< "::COORDINATES=(" << dat.end.x
		<< ", " << dat.end.y << ")"
		<< "::TIME=" << dat.end.time
		<< "::FLAG=" << dat.end.flag
		<< "::OP=" << dat.end.modKey
		<< "\n";
	std::cerr << "// --------- END INPUT DATA DUMP --------- //\n";
}
// Data dump for Fragment Data. Will loop through the entire struct and print important info to log file
void Application::fragmentDataDump(std::shared_ptr<VertexData> dat, std::string name, int UEID)
{
	std::cerr << "// --------- BEGIN FRAGMENT DATA DUMP --------- //\n" << "FRAGMENT::TYPE::NAME=" << name << "\n";
	std::cerr << "STARTTIME::=" << dat.get()->startTime << "::ENDTIME=" << dat.get()->endTime << "::UEID=" << UEID
		<< "::TRANSFORM::POSITION=(" << dat.get()->transform.pos.x << ", " << dat.get()->transform.pos.y << ", " << dat.get()->transform.pos.z << ")"
		<< "::TRANSFORM::DIMENSIONS=(" << dat.get()->transform.boundBox.x2 << " x " << dat.get()->transform.boundBox.y2 << ")"
		<< "\n";
	int i = 0;
	for (const FragmentAnchor& a : dat.get()->anchors)
	{
		std::cerr << "ANCHOR.INDEX=" << i
			<< "::ID=" << a.ID
			<< "::FLAG=" << a.flag
			<< "::POS=(" << a.pos.x << ", " << a.pos.y << ", " << a.pos.z << ")"
			<< "::DIR=(" << a.dir.x << ", " << a.dir.y << ", " << a.dir.z << ")"
			<< "::HEAD.TYPE=" << a.headType
			<< "::HEAD.CONTROL=" << a.headControl
			<< "::HEAD.HANDLE.POS=(" << a.headHandle.x << ", " << a.headHandle.y << ", " << a.headHandle.z << ")"
			<< "::TAIL.TYPE=" << a.headType
			<< "::TAIL.CONTROL=" << a.headControl
			<< "::TAIL.HANDLE.POS=(" << a.headHandle.x << ", " << a.headHandle.y << ", " << a.headHandle.z << ")"
			<< "::HANDLERELATIONSHIP=" << a.handleRelationship
			<< "::PRESSURE=" << a.pressure
			<< "::TILT.X=" << a.tiltx
			<< "::TILT.Y=" << a.tilty
			<< "::PEN.ROTATION=" << a.rotation
			<< "::VELOCITY=" << a.velocity
			<< "::INTENSITY=" << a.intensity
			<< "\n";
		i++;
	}
	std::cerr << "// --------- END FRAGMENT DATA DUMP --------- //\n";
}
// Data dump Plugin Information
void Application::pluginDataDump(std::vector<std::shared_ptr<WPlugin>> pluginArray)
{
	std::cerr << "// --------- BEGIN PLUGIN DATA DUMP --------- //\n";
	for (auto const& plugin : pluginArray)
	{
		std::cerr << "PLUGIN::NAME=" << plugin.get()->name << "\n"
			<< "PLUGIN::VERSION=" << plugin.get()->version << "\n"
			<< "PLUGIN::ABOUT=" << plugin.get()->about << "\n"
			<< "PLUGIN::NAMESPACE=" << plugin.get()->script_namespace << "\n"
			<< "PLUGIN::PLUGIN_PATH=" << plugin.get()->plugin_path << "\n";
		for (std::filesystem::path style : plugin.get()->style_paths)
		{
			std::cerr << "PLUGIN::STYLE_PATH=" << style << "\n";
		}
		for (std::filesystem::path layout : plugin.get()->layout_paths)
		{
			std::cerr << "PLUGIN::LAYOUT_PATH=" << layout << "\n";
		}
		for (std::filesystem::path cscript : plugin.get()->script_paths)
		{
			std::cerr << "PLUGIN::SCRIPT_PATH=" << cscript << "\n";
		}
		std::cerr << std::endl;
	}
	std::cerr << "// --------- END PLUGIN DATA DUMP --------- //\n";
}
// Data dump Plugin Style Information
void Application::styleDataDump(std::vector<std::shared_ptr<WidgetStyle>> styleArray)
{
	std::cerr << "// --------- BEGIN STYLE DATA DUMP --------- //\n";
	for (auto const& style : styleArray)
	{
		std::cerr << "STYLE::NAME=" << style.get()->name << "\n"
			<< "STYLE::MINX=" << style.get()->minX
				<< "::MAXX=" << style.get()->maxX
				<< "::MINY=" << style.get()->minY
				<< "::MAXY=" << style.get()->maxY << "\n"
			
			<< "STYLE::REL_MINX=" << style.get()->relMinX
				<< "::REL_MAXX=" << style.get()->relMaxX
				<< "::REL_MINY=" << style.get()->relMinY
				<< "::REL_MAXY=" << style.get()->relMaxY << "\n"

			<< "STYLE::POSITION=" << style.get()->position
				<< "::LEFT=" << style.get()->offset.left << "\n"
				<< "::RIGHT=" << style.get()->offset.right << "\n"
				<< "::TOP=" << style.get()->offset.top << "\n"
				<< "::BOTTOM=" << style.get()->offset.bottom << "\n"

			<< "STYLE::BORDER.TOP=" << style.get()->border.top 
				<< "::BORDER.BOTTOM=" << style.get()->border.bottom
				<< "::BORDER.LEFT=" << style.get()->border.left 
				<< "::BORDER.RIGHT=" << style.get()->border.right << "\n"

			<< "STYLE::MARGIN.TOP=" << style.get()->margin.top
				<< "::MARGIN.BOTTOM=" << style.get()->margin.bottom
				<< "::MARGIN.LEFT=" << style.get()->margin.left
				<< "::MARGIN.RIGHT=" << style.get()->margin.right << "\n"

			<< "STYLE::ANCHORING=" << style.get()->anchoring << "\n"
			<< "STYLE::FLOATING=" << style.get()->floating << "\n"

			<< "STYLE::BGCOLOR.RED=" << style.get()->backgroundColor.r 
				<< "::BGCOLOR.GREEN=" << style.get()->backgroundColor.g 
				<< "::BGCOLOR.BLUE=" << style.get()->backgroundColor.b
				<< "::BGCOLOR.ALPHA=" << style.get()->backgroundAlpha << "\n"

			<< "STYLE::BGTEXTUREPATH=" << style.get()->backgroundTexture << "\n"

			<< "STYLE::BOXSHADOW.X=" << style.get()->boxShadowSizeX
				<< "::BOXSHADOW.Y=" << style.get()->boxShadowSizeY << "\n"

			<< "STYLE::BOXSHADOW.RED=" << style.get()->boxShadowColor.r
				<< "::BOXSHADOW.GREEN=" << style.get()->boxShadowColor.g
				<< "::BOXSHADOW.BLUE=" << style.get()->boxShadowColor.b
				<< "::BOXSHADOW.ALPHA=" << style.get()->boxShadowAlpha << "\n"

			<< "STYLE::VISIBILITY=" << style.get()->visibility << "\n"
			<< "STYLE::CLIPPING=" << style.get()->overrideClipping
				<< "::OVERFLOWX=" << style.get()->overflowX
				<< "::OVERFLOWY=" << style.get()->overflowY << "\n"

			<< "STYLE::FONTPATH=" << style.get()->fontPath << "\n"
			<< "STYLE::FONTSIZE=" << style.get()->fontSize << "\n"

			<< std::endl;
	}
	std::cerr << "// --------- END STYLE DATA DUMP --------- //\n";
}
