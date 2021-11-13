#ifndef AUTOSHAPE_H
#define AUTOSHAPE_H

#include "AutoShapeType.h"
#include "../entities/visuals/VertexData.h"

class Application;

#include <glm.hpp>
#include <memory>
#include <map>

class AutoShape
{
private:
	std::shared_ptr<Application> owner;

	unsigned int directionLinesLayer = 0;

	unsigned int shapeBoundsLayer = 0;
	unsigned int shapeLinesLayer = 0;
	unsigned int shapePointsLayer = 0;

	unsigned int lineDetectBoundsLayer = 0;

protected:
public:
	AutoShape();
	void initialize(std::shared_ptr<Application> owner);
	//
	AutoShapeType visualizeData(VertexData* splineData);
	void visualizeShape(VertexData* splineData);
	void clearVisuals();
	//
	AutoShapeType analyzeData(VertexData& outData,
		glm::ivec2 canvasDimensions, float zoomAmount, VertexData* splineData);
	float detectLine(VertexData& outData,
		glm::ivec2 canvasDimensions, float zoomAmount, VertexData* splineData);
	//
	float averageVertexSpacing(VertexData* splineData);
};

#endif