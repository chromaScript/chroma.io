#include "include/gladHelper.h"

#include <iostream>
#include <Ole2.h>
#include <RTSCOM.h>
#include <rtscom_i.c>
#include <wtypes.h>
#include <msinkaut.h>
#include <filesystem>

#ifndef APPLICATION_H
#include "include/Application.h"
#endif

#include "include/methods/IOClasses.h"

#include "include/Camera.h"
#include "include/tool/Toolbox.h"
#include "include/entities/UserInterface.h"
#include "include/methods/InputMethod.h"
#include "include/WinStylusHandler.h"
#include "include/input/keys.h"
#include "include/entities/layers/Canvas.h"
#include "include/analytics/AutoShape.h"

#include "include/entities/widgets/WidgetStyle.h"
#include "include/entities/widgets/Line.h"
#include "include/entities/layers/Layer.h"
#include "include/entities/visuals/Stroke.h"

#include "include/CustomCursor.h"

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

extern bool doDebugMouseInput;

// Constructors
Application::Application()
{
	mousePosBufferMax = 12;
}
Application::Application(int width, int height)
{
	mousePosBufferMax = 12;
	autoShape = std::make_shared<AutoShape>();
	// Read the config.txt file into a string
	std::filesystem::path root = std::filesystem::current_path() /= std::filesystem::path("config");
	if (std::filesystem::exists(root) && std::filesystem::is_directory(root))
	{
		std::filesystem::path configPath = root /= std::filesystem::path("config.txt");
		std::string configString = "";
		readFileToString(configString, configPath);
		// Identify Window Dimensions to load with
		size_t widthPos = configString.find("WINDOW_WIDTH:") + std::string("WINDOW_WIDTH:").size();
		size_t widthTerminal = configString.find_first_of(";", widthPos);
		std::string widthStr = configString.substr(widthPos, widthTerminal - widthPos);
		stringRemoveSpace(widthStr);
		size_t heightPos = configString.find("WINDOW_HEIGHT:") + std::string("WINDOW_HEIGHT:").size();
		size_t heightTerminal = configString.find_first_of(";", heightPos);
		std::string heightStr = configString.substr(heightPos, heightTerminal - heightPos);
		stringRemoveSpace(heightStr);

		int widthConfig = std::stoi(widthStr); int heightConfig = std::stoi(heightStr);

		size_t tabDownFactorPos = configString.find("WINTABLET_DOWNSCALEFACTOR:") + std::string("WINTABLET_DOWNSCALEFACTOR:").size();
		size_t tabDownFactorTerminal = configString.find_first_of(";", tabDownFactorPos);
		std::string tabDownFactorStr = configString.substr(tabDownFactorPos, tabDownFactorTerminal - tabDownFactorPos);
		stringRemoveSpace(tabDownFactorStr);
		storedDownscaleFactor = std::stod(tabDownFactorStr);

		createNewWindow(widthConfig, heightConfig);
		setWindowProperties(widthConfig, heightConfig);
	}
	else
	{
		createNewWindow(width, height);
		setWindowProperties(width, height);
	}
}
// Shader Functions
void Application::initializeShaders()
{
	compositeShader = std::make_shared<Shader>(
		"shaders/compositeShader.vert", "shaders/compositeShader.frag", "COMPOSITE_SHADER");
	std::cout << "APPLICATION::COMPOSITE_SHADER::BOUND" << std::endl;
	//
	compositeFrameShader = std::make_shared<Shader>(
		"shaders/compositeFrameShader.vert", "shaders/compositeFrameShader.frag", "COMPOSITE_FRAME_SHADER");
	std::cout << "APPLICATION::COMPOSITE_FRAME_SHADER::BOUND" << std::endl;
	//
	frameShader = std::make_shared<Shader>(
		"shaders/frameShader.vert", "shaders/frameShader.frag", "FRAME_SHADER");
	std::cout << "APPLICATION::FRAME_SHADER::BOUND" << std::endl;
	//
	canvasShader = std::make_shared<Shader>(
		"shaders/canvasShader.vert", "shaders/canvasShader.frag", "CANVAS_SHADER");
	std::cout << "APPLICATION::CANVAS_SHADER::BOUND" << std::endl;
	//
	layerShader = std::make_shared<Shader>(
		"shaders/layerShader.vert", "shaders/layerShader.frag", "LAYER_SHADER");
	std::cout << "APPLICATION::LAYER_SHADER::BOUND" << std::endl;
	//
	shardShader = std::make_shared<Shader>(
		"shaders/shardShader.vert", "shaders/shardShader.frag", "SHARD_SHADER");
	std::cout << "APPLICATION::STROKE_SHADER::BOUND" << std::endl;
	//
	widgetShader = std::make_shared<Shader>(
		"shaders/widgetShader.vert", "shaders/widgetShader.frag", "WIDGET_SHADER");
	std::cout << "APPLICATION::WIDGET_SHADER::BOUND" << std::endl;
	//
	textShader = std::make_shared<Shader>(
		"shaders/textShader.vert", "shaders/textShader.frag", "TEXT_SHADER");
	std::cout << "APPLICATION::TEXT_SHADER::BOUND" << std::endl;
	//
	debugLineShader = std::make_shared<Shader>(
		"shaders/debugLineShader.vert", "shaders/debugLineShader.frag", "DEBUG_LINE_SHADER");
	std::cout << "APPLICATION::DEBUG_LINE_SHADER::BOUND" << std::endl;
	//
	previewShader_pointLines = std::make_shared<Shader>(
		"shaders/previewShader_pointLines.geom", "shaders/previewShader_pointLines.vert", "shaders/previewShader_pointLines.frag", "PREVIEW_SHADER_POINTSLINES");
	std::cout << "APPLICATION::PREVIEW_SHADER_POINTSLINES::BOUND" << std::endl;
	//
	previewShader_curves = std::make_shared<Shader>(
		"shaders/previewShader_curves.geom", "shaders/previewShader_curves.vert", "shaders/previewShader_curves.frag", "PREVIEW_SHADER_CURVES");
	std::cout << "APPLICATION::PREVIEW_SHADER_CURVES::BOUND" << std::endl;
	//
	gradientBoxShader = std::make_shared<Shader>(
		"shaders/gradientBoxShader.vert", "shaders/gradientBoxShader.frag", "GRADIENT_BOX_SHADER");
	std::cout << "APPLICATION::GRADIENT_BOX_SHADER::BOUND" << std::endl;
	//
	graphWidgetShader = std::make_shared<Shader>(
		"shaders/graphWidgetShader.vert", "shaders/graphWidgetShader.frag", "GRAPH_WIDGET_SHADER");
	std::cout << "APPLICATION::GRAPH_WIDGET_SHADER::BOUND" << std::endl;
	//
	noiseWidgetShader = std::make_shared<Shader>(
		"shaders/noiseWidgetShader.vert", "shaders/noiseWidgetShader.frag", "NOISE_WIDGET_SHADER");
	std::cout << "APPLICATION::NOISE_WIDGET_SHADER::BOUND" << std::endl;
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
void Application::moveApplication(int xpos, int ypos)
{
	glfwSetWindowPos(appWindow, xpos, ypos);
}
void Application::resizeApplication(int width, int height)
{
	glfwSetWindowSize(appWindow, width, height);

	ui.get()->buildAllWidgetTrees();
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
	winDimensions = glm::ivec2(winWidth, winHeight);
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
	winWidth = WINDOW_WIDTH = width;
	winHeight = WINDOW_HEIGHT = height;
	winDimensions = glm::ivec2(winWidth, winHeight);
}
void Application::setWindowColor(glm::vec4 color)
{
	bgColor = color;
}
glm::ivec2 Application::getWindowPosition()
{
	int xpos, ypos;
	glfwGetWindowPos(appWindow, &xpos, &ypos);
	return glm::ivec2(xpos, ypos);
}
GLFWwindow* Application::getWindow() { return appWindow; }
int Application::getWindowWidth() { return winWidth; }
int Application::getWindowHeight() { return winHeight; }
glm::ivec2* Application::getWindowDimensions() { return &winDimensions; }
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
	// Apply saved config.txt values to the stylus
	if (storedDownscaleFactor == 0)
	{
		stylusHandler_win->resetCalibration();
	}
	else
	{
		stylusHandler_win->stopCalibration();
		stylusHandler_win->downscaleFactor = storedDownscaleFactor;
	}
	
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
double Application::getStoredDownscaleFactor() { return storedDownscaleFactor; }
void Application::saveDownscaleFactor(double factor) 
{ 
	storedDownscaleFactor = factor; 

	std::filesystem::path root = std::filesystem::current_path() /= std::filesystem::path("config");
	if (std::filesystem::exists(root) && std::filesystem::is_directory(root))
	{
		std::filesystem::path configPath = root /= std::filesystem::path("config.txt");
		std::string configString = "";
		readFileToString(configString, configPath);

		size_t tabDownFactorPos = configString.find("WINTABLET_DOWNSCALEFACTOR:") + std::string("WINTABLET_DOWNSCALEFACTOR:").size();
		size_t tabDownFactorTerminal = configString.find_first_of(";", tabDownFactorPos);
		std::string newFactorStr = " " + std::to_string(storedDownscaleFactor);
		configString.replace(tabDownFactorPos, tabDownFactorTerminal - tabDownFactorPos, newFactorStr);

		std::string outPath = configPath.string();
		FILE* outFile = NULL;
		errno_t err;
		err = fopen_s(&outFile, outPath.c_str(), "wb");
		if (err != 0 && outFile == NULL) { fclose(outFile); return; }

		fprintf(outFile, configString.c_str());

		fclose(outFile);
	}
}

bool Application::stylusInRange()
{
	return stylusHandler_win->getStylusIsHover();
}

// Undo Functions


void Application::clearUndoRequests()
{
	if (undoRequestCount == 0) { return; }
	for (std::pair<std::weak_ptr<Layer>, std::weak_ptr<Stroke>> undoItem : undoStrokes)
	{
		if (!undoItem.first.expired())
		{
			undoItem.first.lock()->removeStroke(undoItem.second);
			undoRequestCount--;
		}
	}
	undoStrokes.clear();
}

void Application::requestRemove_targetStroke(std::weak_ptr<Layer> layer, std::weak_ptr<Stroke> target)
{
	if (!target.expired())
	{
		undoRequestCount++;
		undoStrokes.push_back(std::pair<std::weak_ptr<Layer>, std::weak_ptr<Stroke>>(layer, target));
	}
}


// Camera Functions
void Application::createOrthoCamera() 
{ 
	glm::ivec2 dimensions = glm::ivec2(0, 0);
	if (ui.get()->getDocumentsCount() == 0)
	{
		camera = std::make_shared<Camera>(
			0.05f, 0.05f, 0.05f,
			winRatio,
			glm::ivec2(512, 512));
	}
	else
	{
		camera = std::make_shared<Camera>(
			0.05f, 0.05f, 0.05f,
			winRatio,
			ui.get()->getCanvas()->getDimensions());
	}
	
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
	if (ui.get()->activeCanvas == nullptr) { return glm::ivec2(0, 0); }
	return ui.get()->getCanvas().get()->getDimensions(); 
}
void Application::renderCanvas_toFile(std::shared_ptr<Canvas> target)
{
	if (ui.get()->activeCanvas == nullptr) { return; }
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
	if (err != 0 && outFile == NULL) { fclose(outFile); return; }

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Cursor Functions
void Application::setCursor(CursorType selector)
{
	ui.get()->updateCursorImage(toolbox.get()->getCursor(selector));
}

// Callback Functions
bool Application::bindCallback(std::shared_ptr<CInterpreter> interpreter,
	CCallbackType callType, std::string callerID,
	std::shared_ptr<CFunction> callFunc)
{
	bool validSignature = true;
	std::vector<std::shared_ptr<CToken>> paramTypes;
	switch (callType)
	{
	case CCallbackType::storedEventListener:
		// scriptSignature - void function()  
		paramTypes = callFunc.get()->funcDeclaration.get()->paramsTypes;
		if (paramTypes.size() != 0) { validSignature = false; }
		if (callFunc.get()->funcDeclaration.get()->returnType.get()->type != CTokenType::_VOID) { validSignature = false; }
		break;
	case CCallbackType::keyListenerAll:
	case CCallbackType::keyListenerAll_blocking:
	case CCallbackType::keyListener:
	case CCallbackType::keyListener_blocking:
		// scriptSignature - bool function(num modBit, num glfwKey)  
		paramTypes = callFunc.get()->funcDeclaration.get()->paramsTypes;
		if (paramTypes.size() != 2) { validSignature = false; }
		if (callFunc.get()->funcDeclaration.get()->returnType.get()->type != CTokenType::BOOL) { validSignature = false; }
		if (paramTypes[0].get()->type != CTokenType::NUM) { validSignature = false; }
		if (paramTypes[1].get()->type != CTokenType::NUM) { validSignature = false; }
		break;
	case CCallbackType::canvasRightClickPress:
		// scriptSignature - void function(num xpos, num ypos)
		paramTypes = callFunc.get()->funcDeclaration.get()->paramsTypes;
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
	case CCallbackType::storedEventListener:
		storedEventListenerCallbacks.insert(std::pair<std::string, std::shared_ptr<CFunction>>(callerID, callFunc));
		break;
	case CCallbackType::keyListenerAll:
		keyListenerAllCallbacks.insert(std::pair<std::string, std::shared_ptr<CFunction>>(callerID, callFunc));
		break;
	case CCallbackType::keyListenerAll_blocking:
		keyListenerAllBlockingCallbacks.insert(std::pair<std::string, std::shared_ptr<CFunction>>(callerID, callFunc));
		break;
	case CCallbackType::keyListener:
		keyListenerCallbacks.insert(std::pair<std::string, std::shared_ptr<CFunction>>(callerID, callFunc));
		break;
	case CCallbackType::keyListener_blocking:
		keyListenerBlockingCallbacks.insert(std::pair<std::string, std::shared_ptr<CFunction>>(callerID, callFunc));
		break;
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

bool Application::triggerStoredEventCallback(std::string callbackID, bool eraseEvent)
{
	if (storedEventListenerCallbacks.count(callbackID) == 0) { return false; }
	else
	{
		std::vector<std::shared_ptr<CObject>> args;
		storedEventListenerCallbacks.at(callbackID).get()->call(scriptConsole.get()->getInterpreter(), &args);
		if (eraseEvent) { storedEventListenerCallbacks.erase(callbackID); }
		return true;
	}
}

bool Application::getKeyState(InputKey namedKey) 
{
	return glfwGetKey(appWindow, static_cast<int>(namedKey) / 10); 
}
bool Application::getKeyState(InputModKey modKey)
{
	if (getModKeys() == InputModKey::none) { return false; }
	return (modKey == getModKeys());
}

// Keyboard Callback & Set Callbacks
void Application::keyEventHandler(Keybind keybind, InputAction action)
{
	// Debug Setting: PRINT
	bool debugPrint = false;
	// First filter the input through keyListenerAll callbacks
	if (keyListenerAllCallbacks.size() != 0 || keyListenerAllBlockingCallbacks.size() != 0)
	{
		// Build The Arguments
		std::vector<std::shared_ptr<CObject>> args;
		
		args.push_back(std::make_shared<CObject>(double(static_cast<int>(keybind.glfwKey))));
		args.push_back(std::make_shared<CObject>(double(static_cast<int>(keybind.modKey))));
		if (keyListenerAllCallbacks.size() != 0)
		{
			// Call the functions
			for (auto const& item : keyListenerAllCallbacks)
			{
				item.second.get()->call(scriptConsole.get()->getInterpreter(), &args);
			}
			keyListenerAllCallbacks.clear();
		}
		if (keyListenerAllBlockingCallbacks.size() != 0)
		{
			// Call the functions
			for (auto const& item : keyListenerAllBlockingCallbacks)
			{
				item.second.get()->call(scriptConsole.get()->getInterpreter(), &args);
			}
			keyListenerAllBlockingCallbacks.clear();
			// The key is absorbed so the function returns here.
			return;
		}
	}
	// Next filter the input through keyListener callbacks
	if (!isKeybind_modKey(keybind) && (keyListenerCallbacks.size() != 0 || keyListenerBlockingCallbacks.size() != 0))
	{
		// Build The Arguments
		std::vector<std::shared_ptr<CObject>> args;
		args.push_back(std::make_shared<CObject>(double(static_cast<int>(keybind.glfwKey))));
		args.push_back(std::make_shared<CObject>(double(static_cast<int>(keybind.modKey))));
		if (keyListenerCallbacks.size() != 0)
		{
			// Call the functions
			for (auto const& item : keyListenerCallbacks)
			{
				item.second.get()->call(scriptConsole.get()->getInterpreter(), &args);
			}
			keyListenerCallbacks.clear();
		}
		if (keyListenerBlockingCallbacks.size() != 0)
		{
			// Call the functions
			for (auto const& item : keyListenerBlockingCallbacks)
			{
				item.second.get()->call(scriptConsole.get()->getInterpreter(), &args);
			}
			keyListenerBlockingCallbacks.clear();
			// The key is absorbed so the function returns here.
			return;
		}
	}
	// Second check if a text box or entry field is in focus
	if (ui.get()->activeInputType != LTokenType::NIL && !ui.get()->activeInputWidget.expired())
	{
		if (ui.get()->activeInputType == LTokenType::LINE)
		{
			textInputHandler(keybind, action);
			return;
		}
	}

	// Key Watcher - Prevents Repeat Calls to a currently held key input
	if (keyWatch != InputKey::unknown)
	{
		if (action == InputAction::press || action == InputAction::repeat)
		{
			if (debugPrint) { std::cout << "KEYEVENTHANDLER::ABORT" << std::endl; }
			return;
		}
		else if (action == InputAction::release && keybind.glfwKey == keyWatch)
		{
			if (debugPrint) { std::cout << "KEYEVENTHANDLER::KEYWATCHSIG::RELEASED::=" << static_cast<int>(keyWatch) << std::endl; }
			keyWatch = InputKey::unknown;
			//return;
		}
	}
	// Handle Key Events By Object List Query and Signature Match
	// Key signature (int sig) is they ((keyboard key * 10) + modbit), which produces a unique signature for any normal hotkey
	// By using int, we guarantee accuracy of the key-sig.
	// Assigning a "Hotkey" to something doesn't assign a literal leter, but the appropriate openGL key-sig, which can be read back out to string for UI display
	// Because each context level has similar or identical function calls (Tools merely setActive, Buttons would clickButton, etc.) we can be fairly confident
	// that any potential output function can be easily selected and handled.

	// Pass an update notification to actively hovered widgets with onmouseover events. They only update on mouse-movement, but should also update on key
	// changes in case they have conditionals depending on key state.
	if (ui.get()->overWidgets.size() != 0 && action != InputAction::repeat)
	{
		ui.get()->widgetOverTest(&mousePosBuffer.back(), 0);
	}

	// Do immediate filtering by Esc/Backspace or any other static keybinds
	// Do basic filtering by range/exact >= , <= or == (Cull Ctrl/Shift/Alt/CapsLock/Tab/PgUp/PgUp/PgDn/End/Prtsc/Enter/Backspace/F10/F11/F12/Fn/Del)
	bool reservedKey = false;
	if (isKeybind_escape(keybind)) // ESCAPE
	{
		// Should only ever trigger ESCAPE on key press
		if (!isDoingInput && action == InputAction::release) { 
			if (ui.get()->sendPopupEscape()) { return; }
		}
		if (debugPrint == true) { std::cout << "KEYHANDLER::" << "ESCAPE" << std::endl; }
		reservedKey = true;
	}
	if (isKeybind_delete(keybind)) // DELETE
	{
		if (debugPrint == true) { std::cout << "KEYHANDLER::" << "DELETE" << std::endl; }
		reservedKey = true;
	}
	if (isKeybind_arrows(keybind)) // RIGHT / LEFT / DOWN / UP ARROWS
	{
		if (debugPrint == true) { std::cout << "KEYHANDLER::" << "RIGHT / LEFT / DOWN / UP ARROWS" << std::endl; }
		reservedKey = true;
	}
	if (isKeybind_f10f11f12(keybind)) // F10 / F11 / F12
	{
		if (debugPrint == true) { std::cout << "KEYHANDLER::" << "F10 / F11 / F12" << std::endl; }
		reservedKey = true;
	}
	if (isKeybind_enterTabBackInsert(keybind)) // ENTER / TAB / BACKSPACE / INSERT
	{
		if (debugPrint == true) { std::cout << "KEYHANDLER::" << "ENTER / TAB / BACKSPACE / INSERT" << std::endl; }
		reservedKey = true;
	}
	if (isKeybind_pagination(keybind)) // PAGE_UP / PAGE_DOWN / HOME / END / CAPS_LOCK / SCROLL_LOCK / NUM_LOCK / PRINT_SCREEN / PAUSE
	{
		if (debugPrint == true) { std::cout << "KEYHANDLER::" << "PAGE_UP / PAGE_DOWN / HOME / END / CAPS_LOCK / SCROLL_LOCK / NUM_LOCK / PRINT_SCREEN / PAUSE" << std::endl; }
		reservedKey = true;
	}
	if (isKeybind_modKey(keybind)) // RIGHT & LEFT SHIFT / CTRL / ALT / SUPER / MENU
	{
		if (debugPrint == true) { std::cout << "KEYHANDLER::" << "RIGHT & LEFT SHIFT / CTRL / ALT / SUPER / MENU" << std::endl; }
		reservedKey = true;
	}
	if (isKeybind_outOfBounds(keybind)) // OUT OF BOUNDS SIGNATURE
	{
		if (debugPrint == true) { std::cout << "KEYHANDLER::" << "OUT OF BOUNDS SIGNATURE" << std::endl; }
		reservedKey = true;
	}
	// Want to do actions on press, rather than release
	if (action == InputAction::press)
	{
		// Only interested in watching non-modifier keys, so divide by 10 to remove modifier bit
		// The modifier bit is always set regardless of application state, so tools will still know the modifier state
		keyWatch = keybind.glfwKey;
		if (debugPrint == true) { std::cout << "KEYEVENTHANDLER::KEYWATCHSIG::SET::=" << static_cast<int>(keyWatch) << std::endl; }
	}
	if (reservedKey) {
		Input dat = *getMouseBuffer_back();
		InputHandlerFlag result = toolbox->sendKey(this, dat, keybind, action, getModKeys());
		// React based on result
		switch (result) {
		case InputHandlerFlag::terminateInput:
			ui->updateCursorImage(toolbox->getActiveTool().get()->getCursorUp());
			// Do finalize, then postprocess. Tools without either step simply have blank functions.
			toolbox->sendFinialize(this);
			toolbox->endCallback(scriptConsole.get()->getInterpreter(), dat.x, dat.y);
			isDoingInput = false;
			break;
		case InputHandlerFlag::finalizeCurve:
			toolbox->sendPreview(this, result);
			toolbox->sendFinialize(this);
			toolbox->endCallback(scriptConsole.get()->getInterpreter(), dat.x, dat.y);
			isDoingInput = false;
			break;
		case InputHandlerFlag::releaseCurve:
			toolbox->sendPreview(this, result);
			break;
		}
		if (result != InputHandlerFlag::noSignal) { return; }
	}

	
	// 1. Query Context (If a popup, submenu, dropdown, or alternate window is in focus)
	// Check if the active popup should block the input from falling through here
	if (ui.get()->checkPopupBlocking()) { return; }
	// 2. Block Release and Repeat actions
	if (action != InputAction::press) { return; }
	// 3. Query Active Tool (While more numerous in binds than Menus/Actions/Macros, they are more often used)
	bool toolResult = false;
	if (toolbox.get()->checkActiveTool()) {
		toolResult = toolbox.get()->getActiveTool()->sendInput(keybind, getModKeys());
	}
	if (toolResult) { return; }
	// 4. Query Active Current Context for Settings Binds -> Users May create any number of keybind Environments with
	// full sets of keybinds.
	// 4A - Query for Tool binds
	int tIndex = toolbox->checkToolHotkeys(keybind);
	if (tIndex != -1 && !isDoingInput)
	{
		toolbox->setActiveTool_byIndex(tIndex);
		ui->updateCursorImage(toolbox->getActiveTool()->getCursorUp());
	}
	// 4B - Query active context for menu items and other binds
	// 5. Fall Back to the global context
	// 5A - Query for Tool binds
	// 5B - Query for global menu hotkeys (Which are below, hardcoded for now)
	if (compareKeybind(keybind, InputKey::s, InputModKey::ctrl, false))
	{
		renderCanvas_toFile(ui.get()->getCanvas());
	}
	if (compareKeybind(keybind, InputKey::d, InputModKey::none, false))
	{
		ui.get()->resetFGBGColor(black, white);
	}
	if (compareKeybind(keybind, InputKey::x, InputModKey::none, false))
	{
		ui.get()->swapFGBGColor();
	}
}

void Application::textInputHandler(Keybind keybind, InputAction action)
{
	if (action == InputAction::release) { return; }
	Widget* active = &*ui.get()->activeInputWidget.lock();
	if (!active->checkVisibility()) 
	{
		ui.get()->putActiveInputWidget(ui.get()->activeInputWidget, true, true, UI_WEVENT_CANCEL);
		return; 
	}
	if (action == InputAction::press || action == InputAction::release)
	{
		Line* lineWidget = dynamic_cast<Line*>(active);
		std::string addChar = keybindToCharacter(keybind);
		if (addChar != "")
		{
			lineWidget->acceptCharInput(addChar);
		}
		else
		{
			int result = lineWidget->acceptInputCmd(keybind_toInputCmd(keybind), textClipboard);
			// Result returned an escape
			if (result == 1)
			{
				ui.get()->putActiveInputWidget(ui.get()->activeInputWidget, true, true, UI_WEVENT_CANCEL);
			}
			// Result returned an escape commit
			else if (result == 2)
			{
				ui.get()->putActiveInputWidget(ui.get()->activeInputWidget, true, true, UI_WEVENT_ENTRY);
			}
		}
	}
}

bool Application::isValidKeybind_tool(Keybind keybind)
{
	if (keybind.modKey == InputModKey::none && keybind.glfwKey == InputKey::unknown) { return true; }
	if (isKeybind_arrows(keybind) ||
		isKeybind_escape(keybind) ||
		isKeybind_delete(keybind) ||
		isKeybind_pagination(keybind) ||
		isKeybind_enterTabBackInsert(keybind) ||
		isKeybind_outOfBounds(keybind))
	{
		return false;
	}
	return true;
}
bool Application::isValidKeybind_alphaOnly(Keybind keybind)
{
	if (keybind.modKey != InputModKey::none) { return false; }
	if (isKeybind_alphaOnly(keybind))
	{
		return true;
	}
	return false;
}
bool Application::isValidKeybind_symbolOrChar(Keybind keybind)
{
	if (keybind.modKey != InputModKey::none) { return false; }
	if (isKeybind_alphaOnly(keybind) ||
		isKeybind_numericOnly(keybind) ||
		isKeybind_punctuation(keybind))
	{
		return true;
	}
	return false;
}
bool Application::isValidKeybind_modOnly(Keybind keybind)
{
	return isKeybindModOnly(keybind, false);
}

// Update the Mouse Buffer
Input Application::getMousePosition(bool isUIFetch)
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
	Input outPos = Input();
	outPos.x = xpos;
	outPos.y = ypos;
	outPos.time = t;
	return outPos;
}
float Application::getMouseVelocity(float averagingStrength)
{
	if (averagingStrength <= 0.001f) { return mouseVelocity; }
	else {
		float bufferSize = (float)mousePosBuffer.size();
		float sumVelocity = 0.0f;
		float i = 0;
		for (auto const& e : mousePosBuffer)
		{
			sumVelocity += e.velocity * pow(((bufferSize - i) / bufferSize), 1.75f);
			i++;
		}
		return mix(mouseVelocity, sumVelocity / (float)mousePosBuffer.size(), averagingStrength);
	}
}
glm::vec3 Application::getMouseDirection(float averagingStrength)
{
	if (averagingStrength <= 0.001f || mousePosBuffer.size() < 2) {
		return DEFAULT_DIR;
	}
	else {
		size_t buf = mousePosBuffer.size();
		float buf_f = (float)buf;

		glm::vec3 lastDir = makeDir(
			glm::vec3(mousePosBuffer.at(buf - 2).x, mousePosBuffer.at(buf - 2).y, 0.0f),
			glm::vec3(mousePosBuffer.at(buf - 1).x, mousePosBuffer.at(buf - 1).y, 0.0f));
		
		glm::vec3 sumDir = lastDir;

		for (size_t i = 1; i <= buf; i++) {
			glm::vec3 nextDir = makeDir(
				glm::vec3(mousePosBuffer.at(buf - (i + 1)).x, mousePosBuffer.at(buf - (i + 1)).y, 0.0f),
				glm::vec3(mousePosBuffer.at(buf - i).x, mousePosBuffer.at(buf - i).y, 0.0f));
			sumDir += nextDir * pow((float(buf_f - i) / float(buf_f + 1)), 1.45f);
			i++;
		}
		
		return mix(lastDir, sumDir / buf_f, averagingStrength);
	}
}
void Application::updateMouseBuffer(Input inputEvent)
{
	if (mousePosBuffer.size() < mousePosBufferMax)
	{
		mousePosBuffer.push_back(inputEvent);
	}
	else
	{
		mousePosBuffer.erase(mousePosBuffer.begin());
		mousePosBuffer.push_back(inputEvent);
	}
	float timeSum = 0.0f;
	float lenSum = 0.0f;
	int i = 0;
	for (auto const& e : mousePosBuffer) {
		timeSum += (float)abs(mousePosBuffer.front().time - e.time);
		if (i > 0 && i < mousePosBufferMax)
		{
			lenSum += glm::length(
				glm::vec2((float)e.x, (float)e.y) -
				glm::vec2((float)mousePosBuffer[(size_t)i - 1].x, (float)mousePosBuffer[(size_t)i - 1].y));
		}
		i++;
	}
	//std::cout << lenSum << std::endl;
	mouseVelocity = (lenSum * (1.0f + (1.0f / mousePosBufferMax))) / timeSum;
}
void Application::updateBufferMetaData(Input inputEvent)
{
	Input* backEvent = &mousePosBuffer.back();
	backEvent->pressure = inputEvent.pressure;
	backEvent->tiltX = inputEvent.tiltX;
	backEvent->tiltY = inputEvent.tiltY;
	backEvent->rotation = inputEvent.rotation;
}
std::vector<Input> Application::getMouseBuffer() { return mousePosBuffer; }
Input* Application::getMouseBuffer_back()
{
	return &mousePosBuffer.back();
}
Input* Application::getMouseBuffer_doubleBack()
{
	return &mousePosBuffer[mousePosBuffer.size() - 2];
}

// Mouse Event Handlers
void Application::clearMouseHold(double time)
{
	// Clear existing hold
	Input savedEvent = heldInput;
	// Update drag state if waiting to start
	if (!isMouseDrag && dragReleaseGate && dragMouseButtton != InputMouseButton::null && time - lastClickTime > dblThreshold)
	{
		if (doDebugMouseInput) { std::cout << "APP::MOUSEDRAG::PENDING" << std::endl; }
		clickEventHandler(heldInput.button, heldInput.action, heldInput, true);
		if (isDragStarted) { isMouseDrag = true; dragMouseButtton = savedEvent.button; }
		else { dragReleaseGate = false; }
	}
	else if (hasMouseHold && time - lastClickTime > dblThreshold)
	{
		clickEventHandler(heldInput.button, heldInput.action, heldInput, true);
	}
	// Clear clickedWidget once the hold is cleared
	if (!hasMouseHold && ui.get()->clickReleaseGate && !ui.get()->clickedWidget.expired() && 
		glfwGetMouseButton(appWindow, static_cast<int>(savedEvent.button)) == 0)
	{
		savedEvent.action = InputAction::release;
		clickEventHandler(savedEvent.button, InputAction::release, savedEvent, false);
		ui.get()->clickReleaseGate = false;
	}
	
}
void Application::clickEventHandler(InputMouseButton button, InputAction action, Input inputEvent, bool clearHold)
{
	//std::cout << "APP::CLICK_CALLBACK::TIME=" << inputEvent.time << " X= " << inputEvent.x << " Y= " << inputEvent.y << std::endl;

	ui.get()->didFocusThisClick = false;
	// ClearHold entry point for mainLoop to clear click events
	if (clearHold)
	{
		if (doDebugMouseInput) { std::cout << "APP::CLEARHOLD" << std::endl; }
		lastClickTime = -1;
		lastMouseButton = InputMouseButton::null;
		hasMouseHold = false;
		bool result = ui.get()->widgetHitTest(inputEvent, false, true);
		return;
	}

	// Check if can be valid UI click
	bool uiGate = !isDoingInput; // Replace later with more complex function??

	if (uiGate && ui.get()->widgetSweepTest(inputEvent))
	{
		// Set mouse drag
		if (!hasMouseHold && inputEvent.action != InputAction::release)
		{
			dragMouseButtton = inputEvent.button;
		}
		// Wait/Check for double click handling
		// Check if a sweepTest (does not execute event) returns true
		
		if (!hasMouseHold && inputEvent.action != InputAction::release && ui.get()->widgetSweepTest(inputEvent))
		{
			// Do immediate trigger of drag events rather than wait for the hold to release
			if (!isDragStarted) { ui.get()->widgetHitTest(inputEvent, true, false); }
			if (doDebugMouseInput) { std::cout << "APP::SETMOUSEHOLD" << std::endl; }
			lastClickTime = inputEvent.time;
			lastMouseButton = inputEvent.button;
			hasMouseHold = true;
			heldInput = inputEvent;
			return;
		}
		// MouseHold exists, if the clicks match, modify the incoming event, and send it
		else if (hasMouseHold && inputEvent.action != InputAction::release && inputEvent.button == lastMouseButton)
		{
			if (true) { std::cout << "APP::DOUBLECLICK" << std::endl; }
			inputEvent.button = static_cast<InputMouseButton>(static_cast<int>(inputEvent.button) + 10);
			// Run hitTest, which will fire a callback if valid hit and widget has a callback
			bool result = ui.get()->widgetHitTest(inputEvent, false, true);
			lastClickTime = -1;
			lastMouseButton = InputMouseButton::null;
			hasMouseHold = false;
			if (isDragStarted) 
			{ 
				isMouseDrag = true; 
				heldDragPosition = inputEvent;
				dragMouseButtton = static_cast<InputMouseButton>(static_cast<int>(inputEvent.button) - 10);
			}
			//isMouseDrag = false;
			if (result) { return; }
		}
		// Clear hold if rapid press of two different buttons
		else if (hasMouseHold && inputEvent.action != InputAction::release && inputEvent.button != lastMouseButton)
		{
			clickEventHandler(heldInput.button, heldInput.action, heldInput, true);
		}
		// Clear drag with additional check for adding hold time for double click
		else if (inputEvent.action == InputAction::release)
		{
			// Send release commands
			if (!ui.get()->clickedWidget.expired())
			{
				// Widget will only fire the release callback if the pointer is still
				// over the widget at time of release
				Input recheck = inputEvent;
				recheck.x = getMousePosition(false).x;
				recheck.y = getMousePosition(false).y;
				unsigned int maxZ = 0;
				ui.get()->clickedWidget.lock().get()->selfHit(&recheck, false, false, maxZ, false);
				ui.get()->clickedWidget.reset();
			}
			// Send dragRelease commands if was mouse drag
			if (isMouseDrag && dragMouseButtton == inputEvent.button)
			{
				if (!ui.get()->dragWidget.expired())
				{
					if (doDebugMouseInput) { std::cout << "APP::MOUSEDRAG::TRIGGERED::RELEASE" << std::endl; }
					ui.get()->dragWidget.lock().get()->selfDragend(&inputEvent);
				}
				// Also check for leaving any entered widgets at this time. While a widget is being dragged,
				// onmouseleave events will not be triggered for that widget
				if (ui.get()->enteredWidgets.size() != 0)
				{
					if (ui.get()->widgetLeaveTest(&inputEvent, 0) && ui.get()->savedCursor != nullptr && !ui.get()->widgetSweepTest(inputEvent))
					{
						if (!ui.get()->checkPopupBlocking())
						{
							ui.get()->updateCursorImage(ui.get()->savedCursor);
							ui.get()->savedCursor = nullptr;
						}
					}
				}
				ui.get()->dragWidget.reset();
				isMouseDrag = false;
				isDragStarted = false;
				dragReleaseGate = true;
				dragMouseButtton = InputMouseButton::null;
			}
			// Release drag without trigger (if drag never started)
			if (dragMouseButtton == inputEvent.button)
			{
				// Reset drag variables
				ui.get()->dragWidget.reset();
				isMouseDrag = false;
				isDragStarted = false;
				dragReleaseGate = true;
				dragMouseButtton = InputMouseButton::null;
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
	if (inputEvent.action == InputAction::release)
	{
		if (!ui.get()->dragWidget.expired())
		{
			if (doDebugMouseInput) { std::cout << "APP::MOUSEDRAG::TRIGGERED::RELEASE" << std::endl; }
			ui.get()->dragWidget.lock().get()->selfDragend(&inputEvent);
		}
		ui.get()->dragWidget.reset();
		isMouseDrag = false;
		isDragStarted = false;
		dragReleaseGate = true;
		dragMouseButtton = InputMouseButton::null;
	}
	// If the click fell through to this point, clear the UI focus widget
	if (inputEvent.action != InputAction::release)
	{ 
		ui.get()->clearFocusWidget(); 
	}
	// If a popup is blocking new input, then prevent click input to tools
	if (!isDoingInput && ui.get()->checkPopupBlocking()) { return; }

	// Note: "Canvas" really means the entire field of view underneath the UI, not just the actual visible image
	//		Brush/Tool operations can begin from outside the canvas and then enter/leave it without breaking the action
	// If Canvas -> Send to tools
	// 1a. Allow the input functions to reject bad data when starting a new input
	// 1b. Also allow accounting for tools that should not update the cursor through their I/O functions
	//int i = toolbox->getActiveTool()->getInput()->click(inputEvent);
	if (ui.get()->activeCanvas == nullptr) { return; }
	InputHandlerFlag result = toolbox->sendClick(this, inputEvent);
	switch (result)
	{
	case InputHandlerFlag::reject:
		isDoingInput = false;
		break;
	case InputHandlerFlag::previewLine:
	case InputHandlerFlag::previewCurves:
	case InputHandlerFlag::editMode:
		toolbox->sendPreview(this, result);
		isDoingInput = true;
		break;
	case InputHandlerFlag::continueInput:
	case InputHandlerFlag::release_continueInput:
	case InputHandlerFlag::allowMove_updateCursor:
	case InputHandlerFlag::allowMove:
		if (result == InputHandlerFlag::allowMove_updateCursor) {
			ui->updateCursorImage(toolbox->getActiveTool()->getCursorDown());
		}
		toolbox->sendPreview(this, result);
		isDoingInput = true;
		return;
	case InputHandlerFlag::allowPress:
	case InputHandlerFlag::allowPress_updateCursor:
		if (result == InputHandlerFlag::allowPress_updateCursor) {
			ui->updateCursorImage(toolbox->getActiveTool()->getCursorDown());
		}
		autoShape.get()->clearVisuals();
		toolbox->sendPreview(this, result);
		toolbox->startCallback(scriptConsole.get()->getInterpreter(), inputEvent.x, inputEvent.y);
		isDoingInput = true;
		return;
	case InputHandlerFlag::releaseConnect:
	case InputHandlerFlag::releaseCurve:
	case InputHandlerFlag::release:
		if (result == InputHandlerFlag::releaseConnect || result == InputHandlerFlag::releaseCurve) {
			toolbox->sendPreview(this, result);
		}
		ui->updateCursorImage(toolbox->getActiveTool().get()->getCursorUp());
		// Do finalize, then postprocess. Tools without either step simply have blank functions.
		toolbox->sendFinialize(this);
		toolbox->endCallback(scriptConsole.get()->getInterpreter(), inputEvent.x, inputEvent.y);
		isDoingInput = false;
		return;
	case InputHandlerFlag::noSignal:
		break;
	}
	// Catch fall-through events
	if (!isDoingInput)
	{
		if (inputEvent.button == InputMouseButton::right)
		{
			glm::vec3 clickPos = pickScreenCoord(inputEvent.x, inputEvent.y);
			if (ui.get()->canvasHitTest(clickPos))
			{
				if (inputEvent.action == InputAction::press && canvasRightClickPressCallbacks.size() != 0)
				{
					std::shared_ptr<CObject> xArg = std::make_shared<CObject>(inputEvent.x);
					std::shared_ptr<CObject> yArg = std::make_shared<CObject>(inputEvent.y);
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
void Application::mousePosEventHandler(Input inputEvent)
{
	// Update the mouseBuffer & velocity (Always do this, needed for establishing brush direction)

	// Check if leaving a stored enteredWidget, only need to do this when the mouse moves
	if (ui.get()->enteredWidgets.size() != 0)
	{
		if (ui.get()->widgetLeaveTest(&inputEvent, 0) && ui.get()->savedCursor != nullptr && !ui.get()->widgetSweepTest(inputEvent))
		{
			if (!ui.get()->checkPopupBlocking())
			{
				ui.get()->updateCursorImage(ui.get()->savedCursor);
				ui.get()->savedCursor = nullptr;
			}
		}
	}

	// Suggestions: Add a hit-test of some kind here to check hovering over UI elements & change cursor/UI states
	// Warning: Never lock this function out completely, will always need to be checking hovers & positions
	if (isDoingInput && !isDragStarted)
	{
		// 1. Update the InputData by forwarding the inputEvent
		InputHandlerFlag result = toolbox->sendMove(this, inputEvent);
		// 2. Do preview visualization & fragment data building via Tool.outputMethod.preview
		if (result != InputHandlerFlag::wait)
		{
			switch (result)
			{
			case InputHandlerFlag::wait: return; break;
			case InputHandlerFlag::previewLine:
			case InputHandlerFlag::previewCurves:
			case InputHandlerFlag::editMode:
				toolbox->sendPreview(this, result);
				break;
			default:
				toolbox->sendPreview(this, result);
				toolbox->moveCallback(scriptConsole.get()->getInterpreter(), inputEvent.x, inputEvent.y);
			}
		}
	}
	else
	{
		// Check if can be valid UI movement
		bool clickGate = hasMouseHold; // Replace later with more complex function??
		glm::vec2 newUIMousePos = glm::vec2((float)inputEvent.x, (float)inputEvent.y);
		float length = glm::length(newUIMousePos - lastUIMousePos);
		if (!clickGate && length > (ROOT2 / 2.0f))
		{
			lastUIMousePos = newUIMousePos;
			if (!isMouseDrag && ui.get()->widgetSweepTest(inputEvent))
			{
				if (ui.get()->savedCursor == nullptr)
				{
					ui.get()->savedCursor = ui.get()->currentCursor;
					ui.get()->updateCursorImage(toolbox.get()->getCursor(CursorType::pointer));
				}
				// Handle mouse-over and mouse-enter/leave events here
				bool result = ui.get()->widgetHoverTest(&inputEvent);
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
				if (inputEvent.x == heldDragPosition.x && inputEvent.y == heldDragPosition.y) { return; }
				else { heldDragPosition = inputEvent; }
				if (!ui.get()->dragWidget.expired())
				{
					ui.get()->dragWidget.lock().get()->selfDrag(&inputEvent);
				}
				// Dispatch ondragover/enter/leave events if bound
				bool result = ui.get()->widgetSweepTest(inputEvent);
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

void Application::mouseScrollEventHandler(Input inputEvent)
{
	// Widget Collision & Intercept

	// Canvas Intercept (Pan camera up/down)
}

// Color Functions
CColor Application::sampleScreen(int x, int y)
{
	float pixel[3];
	glReadPixels(x, winHeight - y, 1, 1, GL_RGB, GL_FLOAT, &pixel);
	return CColor(pixel[0], pixel[1], pixel[2]);
}
CColor Application::sampleScreen(int x, int y, int radius)
{
	if (radius <= 0) { return sampleScreen(x, y); }
	GLsizei rad = radius;
	GLubyte* pixels = new GLubyte[(size_t)rad * (size_t)rad * (size_t)4];
	GLint xRead = clampi(x - (rad / 2), 0, INT_MAX);
	GLint yRead = clampi(winHeight - (y + (rad / 2)), 0, INT_MAX);
	glReadPixels(xRead, yRead, rad, rad, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	int redSum = 0.0f; int greenSum = 0.0f; int blueSum = 0.0f; int pixelCount = 0.0f;
	for (int y = 0; y < rad; y++)
	{
		for (int x = 0; x < rad; x++)
		{
			if (pow((x - rad), 2) + pow((y - rad), 2) < pow(rad, 2)) {
				redSum += pixels[((x + (y * rad)) * 4) + 0];
				greenSum += pixels[((x + (y * rad)) * 4) + 1];
				blueSum += pixels[((x + (y * rad)) * 4) + 2];
				pixelCount++;
			}
		}
	}
	delete[]pixels;
	redSum /= pixelCount; greenSum /= pixelCount; blueSum /= pixelCount;
	return CColor(redSum / 255.0f, greenSum / 255.0f, blueSum / 255.0f);
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
	ShaderTransform* matrix = getCamera()->getShaderTransform();
	// Convert the screenCoord to NDC
	glm::vec4 screenCoord = convertScreenCoord(x, y);
	// Create the inverse matrix transform and apply it to screenCoord
	glm::mat4 inverseMatrix = matrix->v * matrix->p;
	inverseMatrix = glm::inverse(inverseMatrix);
	screenCoord = screenCoord * inverseMatrix;
	glm::vec3 outPos = glm::vec3(screenCoord.x, screenCoord.y, 0.0f);
	// Translate & Rotate the coordinate using the camera
	outPos = getCamera()->translateWorldPos(outPos);
	outPos.z = 0.0f;
	return outPos;
}
glm::vec3 Application::pickScreenCoord(glm::vec3 point)
{
	return pickScreenCoord(point.x, point.y);
}
glm::vec3 Application::pickScreenCoord(glm::vec3 point, bool skipConversion)
{
	if (!skipConversion) { return pickScreenCoord(point.x, point.y); }
	else {
		// Get the view / projection matrix
		ShaderTransform* matrix = getCamera()->getShaderTransform();
		// Convert the screenCoord to NDC
		glm::vec4 screenCoord = glm::vec4(point.x, point.y, 1.0f, 1.0f);
		// Create the inverse matrix transform and apply it to screenCoord
		glm::mat4 inverseMatrix = matrix->v * matrix->p;
		inverseMatrix = glm::inverse(inverseMatrix);
		screenCoord = screenCoord * inverseMatrix;
		glm::vec3 outPos = glm::vec3(screenCoord.x, screenCoord.y, 0.0f);
		// Translate & Rotate the coordinate using the camera
		outPos = getCamera()->translateWorldPos(outPos);
		outPos.z = 0.0f;
		return outPos;
	}
}


double Application::getTime()
{
	return glfwGetTime();
}
// Data dump function for input data. Will loop through the entire struct and print important info to log file
void Application::inputDataDump(InputData dat, std::string name)
{
	/*
	std::cerr << "// --------- BEGIN INPUT DATA DUMP --------- //\n" << "TOOLBOX::ACTIVETOOL::NAME=" << name << "\n";
	std::cerr << "EVENT.INDEX=" << "START"
		<< "::ACTION=" << dat.start.action
		<< "::COORDINATES=(" << dat.start.x
		<< ", " << dat.start.y << ")"
		<< "::TIME=" << dat.start.time
		<< "::FLAG_PRIMARY=" << (int)dat.start.flagPrimary
		<< "::FLAG_SECONDARY=" << (int)dat.start.flagSecondary
		<< "::OP=" << dat.start.modKey
		<< "\n";
	int i = 0;
	for (const Input& e : dat.inputEvents)
	{
		std::cerr << "EVENT.INDEX=" << i
			<< "::ACTION=" << e.action
			<< "::COORDINATES=(" << e.x
			<< ", " << e.y << ")"
			<< "::TIME=" << e.time
			<< "::FLAG_PRIMARY=" << (int)e.flagPrimary
			<< "::FLAG_SECONDARY=" << (int)e.flagSecondary
			<< "::OP=" << e.modKey
			<< "\n";
		i++;
	}
	std::cerr << "EVENT.INDEX=" << "END"
		<< "::ACTION=" << dat.end.action
		<< "::COORDINATES=(" << dat.end.x
		<< ", " << dat.end.y << ")"
		<< "::TIME=" << dat.end.time
		<< "::FLAG_PRIMARY=" << (int)dat.end.flagPrimary
		<< "::FLAG_SECONDARY=" << (int)dat.end.flagSecondary
		<< "::OP=" << dat.end.modKey
		<< "\n";
	std::cerr << "// --------- END INPUT DATA DUMP --------- //\n";
	*/
}
// Data dump for Fragment Data. Will loop through the entire struct and print important info to log file
void Application::fragmentDataDump(std::shared_ptr<VertexData> dat, std::string name, int UEID)
{
	/*
	std::cerr << "// --------- BEGIN FRAGMENT DATA DUMP --------- //\n" << "FRAGMENT::TYPE::NAME=" << name << "\n";
	std::cerr << "STARTTIME::=" << dat.get()->startTime << "::ENDTIME=" << dat.get()->endTime << "::UEID=" << UEID
		<< "::TRANSFORM::POSITION=(" << dat.get()->transform.pos.x << ", " << dat.get()->transform.pos.y << ", " << dat.get()->transform.pos.z << ")"
		<< "::TRANSFORM::DIMENSIONS=(" << dat.get()->transform.boundBox.x2 << " x " << dat.get()->transform.boundBox.y2 << ")"
		<< "\n";
		*/
	int i = 0;
	for (const FragmentAnchor& a : dat.get()->anchors)
	{
		/*
		std::cerr << "ANCHOR.INDEX=" << i
			<< "::ID=" << a.ID
			<< "::FLAG_PRIMARY=" << (int)a.flagPrimary
			<< "::FLAG_SECONDARY=" << (int)a.flagSecondary
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
			<< "::TILT.X=" << a.tiltX
			<< "::TILT.Y=" << a.tiltY
			<< "::PEN.ROTATION=" << a.rotation
			<< "::VELOCITY=" << a.velocity
			<< "::INTENSITY=" << a.intensity
			<< "\n";
			*/
		i++;
	}
	//std::cerr << "// --------- END FRAGMENT DATA DUMP --------- //\n";
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
