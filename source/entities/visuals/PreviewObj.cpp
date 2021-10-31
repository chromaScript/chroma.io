#include "../../include/entities/visuals/PreviewObj.h"

PreviewObj::PreviewObj()
{

}
PreviewObj::PreviewObj(int id, glm::vec3 p1, glm::vec3 p2, CColor color, ShapeType shape, float size)
{
	this->ID = id;
	this->p1 = p1;
	this->p2 = p2;
	this->color = color;
	this->shape = shape;
	this->size = size;
}
// Curve Object Constructor :: Where p1/p2 is start position/handle, p3/p4 is end position/handle
PreviewObj::PreviewObj(int id, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, RectBounds bounds, CColor color, ShapeType shape, float size)
{
	this->ID = id;
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;
	this->p4 = p4;
	this->bounds = bounds;
	this->color = color;
	this->shape = shape;
	this->size = size;
}