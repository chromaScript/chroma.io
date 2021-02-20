#include "include/loadImageData.h"

#include <stb_image/stb_image.h>

#include <iostream>
#include <fstream>

unsigned char* loadImageData(const char* imagePath, int* width, int* height, int* nrChannels, bool doFlip) 
{
	unsigned char *data;
	stbi_set_flip_vertically_on_load(doFlip);
	// load the image data and assign the pointer to it
	data = stbi_load(imagePath, width, height, nrChannels, 0);
	// verify that the data byte array is valid
	if (!data)
	{
		std::cout << "Failed to load texture at input path : " << imagePath << std::endl;
	}
	else
	{
		std::cout << "Image data found at input path : " << imagePath << std::endl;
	}
	return data;
}
