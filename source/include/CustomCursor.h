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
	if (name == "pointer") { return CURSOR_POINTER; }
	if (name == "hand") { return CURSOR_HAND; }
	if (name == "rotate") { return CURSOR_ROTATE; }
	if (name == "grab") { return CURSOR_GRAB; }
	if (name == "zoomscrub" || name =="zoom") { return CURSOR_ZOOMSCRUB; }
	if (name == "zoomin") { return CURSOR_ZOOMIN; }
	if (name == "zoomout") { return CURSOR_ZOOMOUT; }
	if (name == "brush") { return CURSOR_BRUSH; }
	if (name == "dropper" || name == "eyedropper" || name == "colorpicker") { return CURSOR_DROPPER; }
	if (name == "circleSmall" || name =="circlesmall") { return CURSOR_CIRCLESMALL; }
	return 0;
}

class CustomCursor
{
private:
	int id = 0;
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
	CustomCursor(const char* texPath, int id, int offType); // Overload for initialization with offset override;
	~CustomCursor();
};

#endif