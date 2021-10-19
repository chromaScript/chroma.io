#ifndef VERTEXDATA_H
#define VERTEXDATA_H

#include "FragmentAnchor.h"
#include "../../input/keys.h"

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
	//BlendModes blendType;
	void reset();
	void resetAnchors();
	void originToVertices();
	void centerVertsToOrigin();
};

#endif // !VERTEXDATA_H
