#ifndef STROKESHARD_H
#define STROKESHARD_H

#include <glm.hpp>
#include "../../math/Color.h"
#include "../../input/input.h"

struct StrokeShard
{
	int ID = 0;
	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 dir = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec4 scale = glm::vec4(1.0f);
	glm::vec2 vec0 = glm::vec2(0.0f);
	glm::vec2 vec1 = glm::vec2(0.0f);
	glm::vec2 vec2 = glm::vec2(0.0f);
	glm::vec2 vec3 = glm::vec2(0.0f);
	CColor color = CColor(0.0f, 0.0f, 0.0f);
	float opacity = 0.0f;
	float flow = 0.0f;
	Input input = Input();
	StrokeShard();
	StrokeShard(int* ID, glm::vec3* pos, glm::vec3* dir, glm::vec4* scale, 
		glm::vec2* vec0, glm::vec2* vec1, glm::vec2* vec2, glm::vec2* vec3,
		CColor* color, float* opacity, float* flow, Input* input);
};

#endif // ! STROKESHARD_H
