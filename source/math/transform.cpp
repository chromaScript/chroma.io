#include "../include/math/transform.h"


ObjectTransform::ObjectTransform()
{

}
ObjectTransform::ObjectTransform(glm::vec3 up, glm::vec3 right, glm::vec3 fwd) {
	this->up = up;
	this->right = right;
	this->fwd = fwd;
}

// Bounds is used to hold two 2D coordinates for storing a rectangle as corners
//
// int x1 / int y1 / int x2 / int y2 where X2,Y2 is usually also width,height
//
// Always store information for data using integers, something less than a pixel
// can not be rendered, and use of floating point values to store critical dimension
// information for texture data can potentially lead to errors from rounding.
Bounds::Bounds()
{

}
Bounds::Bounds(int x1, int y1, int x2, int y2)
{
	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x2;
	this->y2 = y2;
}
void Bounds::reset()
{
	x1 = y1 = x2 = y2 = 0;
}

// Rectangle Bounds
RectBounds::RectBounds()
{

}
RectBounds::RectBounds(glm::vec3 uniform)
{
	this->p4 = this->p3 = this->p2 = this->p1 = uniform;
}
RectBounds::RectBounds(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4)
{
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;
	this->p4 = p4;
}
RectBounds::RectBounds(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4)
{
	this->p1 = glm::vec3(p1, 0.0f);
	this->p2 = glm::vec3(p2, 0.0f);
	this->p3 = glm::vec3(p3, 0.0f);
	this->p4 = glm::vec3(p4, 0.0f);
}
RectBounds::RectBounds(float p1x, float p1y, float p2x, float p2y, float p3x, float p3y, float p4x, float p4y)
{
	this->p1 = glm::vec3(p1x, p1y, 0.0f);
	this->p2 = glm::vec3(p2x, p2y, 0.0f);
	this->p3 = glm::vec3(p3x, p3y, 0.0f);
	this->p4 = glm::vec3(p4x, p4y, 0.0f);
}
void RectBounds::reset()
{
	p4 *= p3 *= p2 *= p1 *= 0;
}

/*
// Camera Structs
*/

// ShaderTransform is used to condense matrix data from the camera
ShaderTransform::ShaderTransform()
{

}
ShaderTransform::ShaderTransform(glm::mat4 projection, glm::mat4 view, glm::mat4 model)
{
	this->p = projection;
	this->v = view;
	this->m = model;
}
ShaderTransform::ShaderTransform(glm::mat4 projection, glm::mat4 view, glm::mat4 model, float zoom)
{
	this->p = projection;
	this->v = view;
	this->m = model;
	this->zoom = zoom;
}

/*
// Fragment Structs
*/

// EntityTransform is used to hold the transform data for a visual entity
EntityTransform::EntityTransform()
{

}
EntityTransform::EntityTransform(glm::vec3 pos, Bounds rect, float roll)
{
	this->pos = pos;
	this->boundBox = rect;
	this->roll = roll;
}
void EntityTransform::reset()
{
	pos = glm::vec3(0, 0, 0);
	origin = glm::vec3(0, 0, 0);
	boundBox.reset();
	bounds.reset();
	roll = 0.0f;
}