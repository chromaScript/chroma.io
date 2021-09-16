/*
// Cursor Structs
*/
// Cursor justification options
#define CURSOR_ULCORNER 0
#define CURSOR_CENTER 1
#define CURSOR_LLCORNER 2
#define CURSOR_UCENTER 3

// Cursor names
#define CURSOR_POINTER 0
#define CURSOR_HAND 1 
#define CURSOR_ROTATE 2
#define CURSOR_GRAB 3
#define CURSOR_ZOOMSCRUB 10
#define CURSOR_ZOOMIN 11
#define CURSOR_ZOOMOUT 12
#define CURSOR_DROPPER 13
#define CURSOR_BRUSH 20
#define CURSOR_CIRCLESMALL 26

#ifndef CUSTOMCURSOR_H
#define CUSTOMCURSOR_H

#include <GLFW/glfw3.h>

#include "structs.h"
#include <string>

inline int convertCursorName(std::string name)
{
	std::string testValue = name;
	testValue.erase(std::remove_if(testValue.begin(), testValue.end(), isspace), testValue.end());
	if (testValue == "pointer") { return CURSOR_POINTER; }
	if (testValue == "hand") { return CURSOR_HAND; }
	if (testValue == "rotate") { return CURSOR_ROTATE; }
	if (testValue == "grab") { return CURSOR_GRAB; }
	if (testValue == "zoomscrub" || testValue =="zoom") { return CURSOR_ZOOMSCRUB; }
	if (testValue == "zoomin") { return CURSOR_ZOOMIN; }
	if (testValue == "zoomout") { return CURSOR_ZOOMOUT; }
	if (testValue == "brush") { return CURSOR_BRUSH; }
	if (testValue == "dropper" || testValue == "eyedropper" || testValue == "colorpicker") { return CURSOR_DROPPER; }
	if (testValue == "circleSmall" || testValue =="circlesmall") { return CURSOR_CIRCLESMALL; }
	return 0;
}

class CustomCursor
{
private:
	int id = 0;
	std::string name = "";
	unsigned char* imgData;
	int width;
	int height;
	int offsetX;
	int offsetY;
	int nrChannels = 4; // The cursor texture is always RGB_A
public:
	// public member variables
	GLFWimage image;
	GLFWcursor* cursor;
	// constructor / destructor
	CustomCursor();
	CustomCursor(const char* texPath, int id, int offType, std::string name); // Overload for initialization with offset override;
	~CustomCursor();
	std::string getName() { return name; }
};

#endif