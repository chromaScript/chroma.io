#include "../include/math/collision.h"

#include <glm.hpp>

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