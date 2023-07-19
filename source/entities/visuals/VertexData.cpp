#include "../../include/entities/visuals/VertexData.h"
#include "../../include/math/transform.h"

#include <glm.hpp>

void VertexData::reset()
{
	transform.reset();
	startTime = 0.0f;
	endTime = 0.0f;
	subdivCount = 0;
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
void VertexData::setDir()
{
	for (int i = 0; i < anchors.size(); i++) {
		if (i == anchors.size() - 1) {
			anchors.at(i).dir = makeDir(anchors.at(i).pos, anchors.at(0).pos);
		}
		else {
			anchors.at(i).dir = makeDir(anchors.at(i).pos, anchors.at(i + 1).pos);
		}
	}
}

////////////////////////////////////////////////////////////////
//
// Utility Functions
//
////////////////////////////////////////////////////////////////

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

RectBounds createVertexBounds(VertexData* data)
{
	RectBounds out = RectBounds(data->transform.origin);
	for (size_t k = 0; k < data->anchors.size(); k++) {
		if (data->anchors.at(k).pos.x < out.p1.x) {
			out.p1.x = data->anchors.at(k).pos.x;
		}
		else if (data->anchors.at(k).pos.x > out.p3.x) {
			out.p3.x = data->anchors.at(k).pos.x;
		}
		if (data->anchors.at(k).pos.y > out.p1.y) {
			out.p1.y = data->anchors.at(k).pos.y;
		}
		else if (data->anchors.at(k).pos.y < out.p3.y) {
			out.p3.y = data->anchors.at(k).pos.y;
		}
	}
	out.p2.x = out.p3.x;
	out.p2.y = out.p1.y;
	out.p4.x = out.p1.x;
	out.p4.y = out.p3.y;
	return out;
}

RectBounds createVertexBounds_uv(VertexData* data, ShaderTransform* xform)
{
	RectBounds out = RectBounds(data->transform.origin);
	for (size_t k = 0; k < data->anchors.size(); k++) {
		if (data->anchors.at(k).pos.x < out.p1.x) {
			out.p1.x = data->anchors.at(k).pos.x;
		}
		else if (data->anchors.at(k).pos.x > out.p3.x) {
			out.p3.x = data->anchors.at(k).pos.x;
		}
		if (data->anchors.at(k).pos.y > out.p1.y) {
			out.p1.y = data->anchors.at(k).pos.y;
		}
		else if (data->anchors.at(k).pos.y < out.p3.y) {
			out.p3.y = data->anchors.at(k).pos.y;
		}
	}
	out.p2.x = out.p3.x;
	out.p2.y = out.p1.y;
	out.p4.x = out.p1.x;
	out.p4.y = out.p3.y;
	out.p1 = glm::vec3(xform->p * xform->v * xform->m * glm::vec4(out.p1, 1.0f));
	out.p2 = glm::vec3(xform->p * xform->v * xform->m * glm::vec4(out.p2, 1.0f));
	out.p3 = glm::vec3(xform->p * xform->v * xform->m * glm::vec4(out.p3, 1.0f));
	out.p4 = glm::vec3(xform->p * xform->v * xform->m * glm::vec4(out.p4, 1.0f));
	return out;
}

void createVertexBounds_compound(RectBounds& bounds, RectBounds& bounds_uv, VertexData* data, ShaderTransform* xform)
{
	RectBounds out = RectBounds(data->transform.origin);
	for (size_t k = 0; k < data->anchors.size(); k++) {
		if (data->anchors.at(k).pos.x < out.p1.x) {
			out.p1.x = data->anchors.at(k).pos.x;
		}
		else if (data->anchors.at(k).pos.x > out.p3.x) {
			out.p3.x = data->anchors.at(k).pos.x;
		}
		if (data->anchors.at(k).pos.y > out.p1.y) {
			out.p1.y = data->anchors.at(k).pos.y;
		}
		else if (data->anchors.at(k).pos.y < out.p3.y) {
			out.p3.y = data->anchors.at(k).pos.y;
		}
	}
	out.p2.x = out.p3.x;
	out.p2.y = out.p1.y;
	out.p4.x = out.p1.x;
	out.p4.y = out.p3.y;
	bounds = out;
	out.p1 = glm::vec3(xform->p * xform->v * xform->m * glm::vec4(out.p1, 1.0f));
	out.p2 = glm::vec3(xform->p * xform->v * xform->m * glm::vec4(out.p2, 1.0f));
	out.p3 = glm::vec3(xform->p * xform->v * xform->m * glm::vec4(out.p3, 1.0f));
	out.p4 = glm::vec3(xform->p * xform->v * xform->m * glm::vec4(out.p4, 1.0f));
	bounds_uv = out;
	//return out;
}