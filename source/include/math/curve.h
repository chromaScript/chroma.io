#ifndef CURVE_H
#define CURVE_H

#include "curve.h"
#include "transform.h"
#include "../entities/visuals/FragmentAnchor.h"

#include <glm.hpp>
#include <vector>

float binomialCoefficient(int n, int k);
glm::vec2 explicitBezier(glm::vec2 control_points[4], float t);
glm::vec4 bboxBezier(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3);
bool computeTransformation(FragmentAnchor* start, FragmentAnchor* end, glm::vec3& translation, glm::mat2& rotation);
bool getCubicBezierOBB(RectBounds& bounds, FragmentAnchor* start, FragmentAnchor* end, bool addPadding, float padding);
bool getCubicBezierOBB(glm::vec3* p1, glm::vec3* p2, glm::vec3* p3, glm::vec3* p4, 
	FragmentAnchor* start, FragmentAnchor* end, bool addPadding, float padding);


void getCubicBezierPoints(FragmentAnchor* start, FragmentAnchor* end, 
	int resolution, float offset, std::vector<glm::vec3>& points, float& length, 
	float spacing, float startScale, float endScale);
void getCubicBezierPoints(glm::vec2 control_points[4],
	int resolution, float offset, std::vector<glm::vec3>& points, float& length, 
	float spacing, float startScale, float endScale);

#endif