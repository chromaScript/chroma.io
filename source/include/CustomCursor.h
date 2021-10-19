#ifndef CUSTOMCURSOR_H
#define CUSTOMCURSOR_H

// Cursor justification options
enum class CursorJustification : int {
	upperLeftCorner = 0,
	center = 1,
	lowerLeftCorner = 2,
	upperCenter = 3
};

// Cursor names
enum class CursorType : int {
	pointer = 0,
	hand = 1,
	rotate = 2,
	grab = 3,
	zoomscrub = 10,
	zoomIn = 11,
	zoomOut = 12,
	dropper = 13,
	brush = 20,
	circleSmall = 26
};

#include <GLFW/glfw3.h>
#include <string>

inline CursorType convertCursorName(std::string name)
{
	std::string testValue = name;
	testValue.erase(std::remove_if(testValue.begin(), testValue.end(), isspace), testValue.end());
	if (testValue == "pointer") { return CursorType::pointer; }
	if (testValue == "hand") { return CursorType::hand; }
	if (testValue == "rotate") { return CursorType::rotate; }
	if (testValue == "grab") { return CursorType::grab; }
	if (testValue == "zoomscrub" || testValue =="zoom") { return CursorType::zoomscrub; }
	if (testValue == "zoomin") { return CursorType::zoomIn; }
	if (testValue == "zoomout") { return CursorType::zoomOut; }
	if (testValue == "brush") { return CursorType::brush; }
	if (testValue == "dropper" || testValue == "eyedropper" || testValue == "colorpicker") { return CursorType::dropper; }
	if (testValue == "circleSmall" || testValue =="circlesmall") { return CursorType::circleSmall; }
	return CursorType::pointer;
}

class CustomCursor
{
private:
	CursorType id = CursorType::pointer;
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
	CustomCursor(const char* texPath, CursorType id, CursorJustification offType, std::string name); // Overload for initialization with offset override;
	~CustomCursor();
	std::string getName() { return name; }
};

#endif