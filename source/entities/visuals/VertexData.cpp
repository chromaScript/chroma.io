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

void copyVertexData(VertexData& target, VertexData* copy)
{
	target.transform = copy->transform;
	target.activeModKey = copy->activeModKey;
	target.constantSize = copy->constantSize;
	target.linearStream = copy->linearStream;
	target.connectEnds = copy->connectEnds;
	target.centerAboutOrigin = copy->centerAboutOrigin;
	target.anchors = copy->anchors;
	target.startTime = copy->startTime;
	target.endTime = copy->endTime;
}

void copyVertices(VertexData& target, VertexData* copy)
{
	target.anchors = copy->anchors;
}