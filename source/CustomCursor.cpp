#include "include/CustomCursor.h"

#include <GLFW/glfw3.h>

#include "include/loadImageData.h"
#include "include/structs.h"

#include <string>

CustomCursor::CustomCursor()
{

}

CustomCursor::CustomCursor(const char* texPath, int id, int offType, std::string name)
{
	this->name = name;
	int n;
	image.pixels = loadImageData(texPath, &image.width, &image.height, &n, false);
	nrChannels = n;
	width = image.width;
	height = image.height;
	imgData = image.pixels;
	switch (offType)
	{
	case CURSOR_UCENTER:
		offsetX = width / 2;
		offsetY = 0;
		break;
	case CURSOR_ULCORNER:
		offsetX = 0;
		offsetY = 0;
		break;
	case CURSOR_CENTER:
		offsetX = width / 2;
		offsetY = height / 2;
		break;
	case CURSOR_LLCORNER:
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