#include "../../include/input/input.h"
#include "../../include/entities/visuals/StrokeShard.h"

StrokeShard::StrokeShard()
{

}
StrokeShard::StrokeShard(int* ID, glm::vec3* pos, glm::vec3* dir, glm::vec4* scale,
	glm::vec2* vec0, glm::vec2* vec1, glm::vec2* vec2, glm::vec2* vec3,
	CColor* color, float* opacity, float* flow, Input* input)
{
	this->ID = *ID;
	this->pos = *pos;
	this->dir = *dir;
	this->scale = *scale;
	this->vec0 = *vec0;
	this->vec1 = *vec1;
	this->vec2 = *vec2;
	this->vec3 = *vec3;
	this->color = *color;
	this->opacity = *opacity;
	this->flow = *flow;
	this->input = *input;
}