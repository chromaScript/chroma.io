#include "../../include/entities/visuals/VertexData.h"

#include <glm.hpp>

void VertexData::reset()
{
	transform.reset();
	startTime = 0.0f;
	endTime = 0.0f;
	resetAnchors();
}
void VertexData::resetAnchors() 
{ 
	anchors.clear();
	anchors.swap(emptyAnchors); 
}
void VertexData::originToVertices()
{
	if (anchors.size() == 0) { return; }
	else if (anchors.size() == 1) { transform.origin = anchors.front().pos; }
	else
	{
		glm::vec3 avg = glm::vec3(0.0f);
		for (const auto& anchor : anchors)
		{
			avg += anchor.pos;
		}
		avg /= anchors.size();
		transform.origin = avg;
		transform.origin.z = 0.0f;
	}
}
void VertexData::centerVertsToOrigin()
{
	for (auto& anchor : anchors)
	{
		anchor.pos -= transform.origin;
		anchor.pos.z = 0.0f;
	}
	transform.origin = glm::vec3(0.0f);
}