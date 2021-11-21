#ifndef VERTEXDATA_H
#define VERTEXDATA_H

#include "FragmentAnchor.h"
#include "../../input/keys.h"
#include "../../math/blend.h"
#include "../../analytics/AutoShapeType.h"

// VertexData is used to hold the data processed from an InputData object. This is what gets stored as history and canvas objects. The original
// mouse input is not salvageable. VertexData should only contain information useful for all fragment types. Output asks the active tool for
// how to render the fragments.
struct VertexData
{
private:
	std::deque<FragmentAnchor> emptyAnchors = {};
public:
	EntityTransform transform;
	InputModKey activeModKey = InputModKey::none;
	bool constantSize = false;
	bool linearStream = true;
	bool connectEnds = false;
	bool centerAboutOrigin = false;
	std::deque<FragmentAnchor> anchors;
	float startTime = 0.0f;
	float endTime = 0.0f;
	int subdivCount = 0; // Manual Set Only
	int depth = 0; // Manual Set Only
	// Stored Analytics Data
	float averageSegLen = 0.0f;
	AutoShapeType storedShape = AutoShapeType::none;
	int polySides = 0;
	BlendMode blend = BlendMode::normal;
	void reset();
	void resetAnchors();
	void originToVertices();
	void centerVertsToOrigin();
};

void copyVertexData(VertexData& target, VertexData* copy);
void copyVertices(VertexData& target, VertexData* copy);

#endif // !VERTEXDATA_H
