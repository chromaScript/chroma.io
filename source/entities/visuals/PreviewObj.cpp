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
