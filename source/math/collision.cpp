#include "../include/math/collision.h"

#include "../include/math/math.h"
#include <glm.hpp>
#include <gtc/quaternion.hpp>

bool boxCollision_point(glm::vec3* point, glm::vec3* p1, glm::vec3* p2)
{
	bool result = false;
	if (int(point->x) >= p1->x && int(point->x) <= p2->x)
	{
		if (int(point->y) >= p1->y && int(point->y) <= p2->y)
		{
			result = true;
		}
	}
	return result;
}

bool boxCollision_pointf(glm::vec3* point, glm::vec3* p1, glm::vec3* p2)
{
	bool result = false;
	if (point->x >= p1->x && point->x <= p2->x)
	{
		if (point->y >= p1->y && point->y <= p2->y)
		{
			result = true;
		}
	}
	return result;
}

bool boxCollision_rotatedPoint(glm::vec3* point, glm::vec3* p1, glm::vec3* p3, glm::vec3* boxDir)
{
	bool result = false;

	glm::vec3 boxOrigin = (*p1 + *p3) / 2.0f;
	float boxAngle = atan2f(boxDir->y, boxDir->x);
	glm::quat rotator = glm::quat(glm::vec3(0.0f, 0.0f, boxAngle));

	glm::vec3 unrotP1 = (*p1 - boxOrigin) * rotator;

	glm::vec3 unrotP3 = (*p3 - boxOrigin) * rotator;

	glm::vec3 unrotPoint = (*point - boxOrigin) * rotator;

	return boxCollision_pointf(&unrotPoint, &unrotP1, &unrotP3);
}