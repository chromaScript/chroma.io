#include "../include/entities/VisualEntity.h"
#include "../include/Color.h"

#include <glad/glad.h>

#include "../include/loadImageData.h"

// Constructor / Destructor
VisualEntity::~VisualEntity()
{

}

// UEID functions
void VisualEntity::generateUEID()
{
	static long nextUEID = 0;
	this->UEID = nextUEID;
	nextUEID++;
}
long VisualEntity::getUEID() { return UEID; }

// Property Functions
std::string VisualEntity::getName() { return this->name; }
glm::vec4 VisualEntity::getHighlightColor() { return this->highlight; }

// Transform Functions
void VisualEntity::setTransform(glm::vec3 pos) { transform.pos = pos; }
void VisualEntity::setTransform(Bounds rect) { transform.boundBox = rect; }
void VisualEntity::setTransform(float angle) { transform.roll = angle; }
void VisualEntity::setTransform(glm::vec3 pos, Bounds rect, float angle)
{
	transform.pos = pos;
	transform.boundBox = rect;
	transform.roll = angle;
}
void VisualEntity::setTransform(int x1, int y1, int x2, int y2)
{
	transform.boundBox.x1 = x1;
	transform.boundBox.x1 = y1;
	transform.boundBox.x2 = x2;
	transform.boundBox.y2 = y2;
}
EntityTransform VisualEntity::getTransform() { return transform; }

// Dimension functions
glm::ivec2 VisualEntity::getDimensions() { return glm::ivec2(transform.boundBox.x2, transform.boundBox.y2); }
float VisualEntity::getRatio() { return float(transform.boundBox.x2) / float(transform.boundBox.y2); }
int VisualEntity::getiArea() { return transform.boundBox.x2 * transform.boundBox.y2; }

// Data Functions
// This does data for the brush tip image
void VisualEntity::initializeData(int area)
{
	this->data = new unsigned char[(size_t)area * 4];
}
// Fill Data, used to completely overwrite a layer with input color, (temporarily) assumes Alpha value is always 1
// This is used to fill the brush tip image data
void VisualEntity::fillData(int area, CColor_uc fill)
{
	for (int i = 0; i < area; i++)
	{
		int j = i * 4;
		data[j + 0] = fill.r;
		data[j + 1] = fill.g;
		data[j + 2] = fill.b;
		data[j + 3] = 255;
	}
}
// Set the Data to an image from file
void VisualEntity::setData_toImage(const char* texPath, int *width, int *height)
{
	data = loadImageData(texPath, width, height, &channelCount, true);
}
void VisualEntity::clearData(int area)
{
	for (int i = 0; i < area; i++)
	{
		int j = i * 4;
		data[j + 0] = 0;
		data[j + 1] = 0;
		data[j + 2] = 0;
		data[j + 3] = 0;
	}
}

// Update the vertex data when changing the entity dimensions
void VisualEntity::setVertData()
{
	float ox = float(transform.boundBox.x1);
	float oy = float(transform.boundBox.y1);
	float vx = float(transform.boundBox.x2);
	float vy = float(-transform.boundBox.y2);
	float tx;
	float ty;
	if (getRatio() >= 1.0f)
	{
		tx = 1.0f;
		ty = tx / getRatio();
	}
	else
	{
		ty = 1.0f;
		tx = ty * getRatio();
	}
	// Now create a new float array with these bounds
	float newData[20] = {
		// The top and bottom are swapped so that 0,0 is in the upper left corner
		// positions			// texture coords - 
		vx, oy, 0.0f,			tx, oy, // top right
		vx, vy,	0.0f,			tx, ty, // bottom right
		ox, vy, 0.0f,			ox, ty,  // bottom left
		ox, oy, 0.0f,			ox, oy // top left
	};
	// Overwrite the old data with the new data
	for (int i = 0; i < 20; i++)
	{
		vertData[i] = newData[i];
	}
}

// Blend Functions
void VisualEntity::setBlendMode(BlendModes mode) { blendMode = mode; }
BlendModes VisualEntity::getBlendMode() { return blendMode; }

// Render Functions
void VisualEntity::setShader(std::shared_ptr<Shader> program) { shader = program; }
std::shared_ptr<Shader> VisualEntity::getShader() { return shader; }
void VisualEntity::generateBuffers(unsigned int* va, unsigned int* vb, unsigned int* eb, unsigned int* tx0)
{
	glGenVertexArrays(1, va);
	glGenBuffers(1, vb);
	glGenBuffers(1, eb);
	glGenTextures(1, tx0);
}
void VisualEntity::bindBuffers()
{
	// Bind vertex array and buffers with data
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertData_size, vertData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertArrangement_size, vertArrangement, GL_STATIC_DRAW);
	// Set vertex position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, attribStride, (void*)0);
	glEnableVertexAttribArray(0);
	// Set texture coordinate attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, attribStride, (void*)tAttribOffset);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}
void VisualEntity::bindTexture(int width, int height)
{
	// Make a new texture ID, then bind that as the texture to work on
	glGenTextures(1, &TEX0);
	glBindTexture(GL_TEXTURE_2D, TEX0);
	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// Bind the texture2D buffer with the data, and generate mipmaps
	if (channelCount == 3)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	 // Note: Need to also use composite data later.
	glGenerateMipmap(GL_TEXTURE_2D);
}