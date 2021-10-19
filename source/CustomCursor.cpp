#include "include/CustomCursor.h"

#include <GLFW/glfw3.h>

#include "include/loadImageData.h"

#include <string>

CustomCursor::CustomCursor()
{

}

CustomCursor::CustomCursor(const char* texPath, CursorType id, CursorJustification offType, std::string name)
{
	this->name = name;
	this->id = id;
	int n;
	image.pixels = loadImageData(texPath, &image.width, &image.height, &n, false);
	nrChannels = n;
	width = image.width;
	height = image.height;
	imgData = image.pixels;
	switch (offType)
	{
	case CursorJustification::upperCenter:
		offsetX = width / 2;
		offsetY = 0;
		break;
	case CursorJustification::upperLeftCorner:
		offsetX = 0;
		offsetY = 0;
		break;
	case CursorJustification::center:
		offsetX = width / 2;
		offsetY = height / 2;
		break;
	case CursorJustification::lowerLeftCorner:
		offsetX = 0;
		offsetY = height;
		break;
	}
	cursor = glfwCreateCursor(&image, offsetX, offsetY);
}

CustomCursor::~CustomCursor() 
{
	glfwDestroyCursor(cursor);
}