#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm.hpp>

// ObjectTransform is used to condense multiple direction vectors into a single container
struct ObjectTransform
{
	glm::vec3 up = glm::vec3(0, 0, 0);
	glm::vec3 right = glm::vec3(0, 0, 0);
	glm::vec3 fwd = glm::vec3(0, 0, 0);
	ObjectTransform();
	ObjectTransform(glm::vec3 up, glm::vec3 right, glm::vec3 fwd);
};

// Bounds is used to hold two 2D coordinates for storing a rectangle as corners
//
// int x1 / int y1 / int x2 / int y2 where X2,Y2 is usually also width,height
//
// Always store information for data using integers, something less than a pixel
// can not be rendered, and use of floating point values to store critical dimension
// information for texture data can potentially lead to errors from rounding.
struct Bounds
{
	int x1 = 0;
	int y1 = 0;
	int x2 = 0;
	int y2 = 0;
	Bounds();
	Bounds(int x1, int y1, int x2, int y2);
	void reset();
};
struct RectBounds
{
	glm::vec3 p1 = glm::vec3(0, 0, 0);
	glm::vec3 p2 = glm::vec3(0, 0, 0);
	glm::vec3 p3 = glm::vec3(0, 0, 0);
	glm::vec3 p4 = glm::vec3(0, 0, 0);
	RectBounds();
	RectBounds(glm::vec3 uniform);
	RectBounds(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4);
	RectBounds(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4);
	RectBounds(float p1x, float p1y, float p2x, float p2y, float p3x, float p3y, float p4x, float p4y);
	void reset();
};


/*
// Camera Structs
*/
// Camera types
enum class CameraTypes
{
	perspective,
	ortho,
};


// ShaderTransform is used to condense matrix data from the camera
struct ShaderTransform
{
	glm::mat4 p = glm::mat4(1.0f); // projection matrix
	glm::mat4 v = glm::mat4(1.0f); // view matrix
	glm::mat4 m = glm::mat4(1.0f); // model matrix
	float zoom = 0; // camera zoom
	ShaderTransform();
	ShaderTransform(glm::mat4 projection, glm::mat4 view, glm::mat4 model);
	ShaderTransform(glm::mat4 projection, glm::mat4 view, glm::mat4 model, float zoom);
};

/*
// Fragment Structs
*/
enum class HandleType
{
	linear,
	_auto,
	manual,
};
enum class HandleRel
{
	independent,
	right,
	pivot
};

// EntityTransform is used to hold the transform data for a visual entity
struct EntityTransform
{
	glm::vec3 pos = glm::vec3(0, 0, 0);
	glm::vec3 origin = glm::vec3(0, 0, 0);
	Bounds boundBox = Bounds(0, 0, 0, 0); // x1, y1, x2, y2 // Corner->Corner rectangle bounds of fragment
	bool drawCornersOnly = false;
	RectBounds bounds = RectBounds(glm::vec3(0, 0, 0));
	float roll = 0.0f;
	EntityTransform();
	EntityTransform(glm::vec3 pos, Bounds rect, float roll);
	void reset();
};

#endif