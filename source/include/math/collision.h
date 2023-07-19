#ifndef COLLISION_H
#define COLLISION_H

#include <glm.hpp>

bool boxCollision_point(glm::vec3* point, glm::vec3* p1, glm::vec3* p2);
bool boxCollision_pointf(glm::vec3* point, glm::vec3* p1, glm::vec3* p2);
bool boxCollision_rotatedPoint(glm::vec3* point, glm::vec3* p1, glm::vec3* p3, glm::vec3* boxDir);

#endif