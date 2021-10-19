#ifndef FRAGMENTANCHOR_H
#define FRAGMENTANCHOR_H

#include "../../input/input.h"
#include "../../math/transform.h"

#include <glm.hpp>

struct FragmentAnchor
{
	// Core
	int ID = 0;
	float time = 0.0f;
	int wait = 0; // Manual set only
	int randSeedA = 0;
	int randSeedB = 0;

	// Stylus Data
	Input input = Input();

	size_t connectionTarget = -1;
	// Transform
	glm::vec3 pos = glm::vec3(0, 0, 0);
	glm::vec3 dir = glm::vec3(0, 0, 0);
	float dirInterpFactor = 0.5f;

	// Curve handles
	HandleType headType = HandleType::_auto;
	bool headControl = false;
	glm::vec3 headHandle = glm::vec3(0, 0, 0);
	HandleType tailType = HandleType::_auto;
	bool tailControl = false;
	glm::vec3 tailHandle = glm::vec3(0, 0, 0);
	HandleRel handleRelationship = HandleRel::independent;

	FragmentAnchor();
	FragmentAnchor(const FragmentAnchor& copy);
	FragmentAnchor(int ID, glm::vec3 pos, glm::vec3 dir,
		HandleType headType, bool headControl, glm::vec3 headHandle,
		HandleType tailType, bool tailControl, glm::vec3 tailHandle,
		HandleRel handleRelationship,
		Input input);
	FragmentAnchor(int ID, glm::vec3 pos, glm::vec3 dir, 
		float dirInterpFactor,
		HandleType headType, bool headControl, glm::vec3 headHandle,
		HandleType tailType, bool tailControl, glm::vec3 tailHandle,
		HandleRel handleRelationship,
		Input input);
	void copyStylusData(FragmentAnchor* target);
	void swapStylusData(FragmentAnchor* target);
	void clearStylusData();
};

#endif