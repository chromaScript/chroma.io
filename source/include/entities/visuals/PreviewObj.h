#ifndef PREVIEWOBJ_H
#define PREVIEWOBJ_H

#include "../../math/Color.h"
#include "../../math/transform.h"
#include <glm.hpp>

enum class ShapeType : int {
	circle = 0,
	square = 1,
	triangle = 2,
	pentagon = 3,
	hexagon = 4,
	octagon = 5,
	line = 6,
	curve = 7,
};

struct PreviewObj
{
public:
	int ID = 0;
	glm::vec3 p1 = glm::vec3(0.0f);
	glm::vec3 p2 = glm::vec3(0.0f);
	glm::vec3 p3 = glm::vec3(0.0f);
	glm::vec3 p4 = glm::vec3(0.0f);
	RectBounds bounds = RectBounds();
	CColor color = blue;
	ShapeType shape = ShapeType::square;
	float size = 4.0f;
	PreviewObj();
	PreviewObj(int id, glm::vec3 p1, glm::vec3 p2, CColor color, ShapeType shape, float size);
	PreviewObj(int id, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, RectBounds bounds, CColor color, ShapeType shape, float size);
};

#endif