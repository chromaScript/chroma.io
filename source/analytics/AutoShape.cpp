#include "../include/analytics/AutoShape.h"

#include "../include/entities/visuals/VertexData.h"
#include "../include/entities/visuals/FragmentAnchor.h"

#include "../include/math/collision.h"

#ifndef APPLICATION_H
#include "../include/Application.h"
#endif
#ifndef USERINTERFACE_H
#include "../include/entities/UserInterface.h"
#endif

AutoShape::AutoShape()
{

}

void AutoShape::initialize(std::shared_ptr<Application> owner)
{
	this->owner = owner;
}

AutoShapeType AutoShape::visualizeData(VertexData* splineData)
{
	size_t size = splineData->anchors.size();
	if (size < 2) { return AutoShapeType::none; }
	if (directionLinesLayer == 0) {
		unsigned int linesLayer = owner->ui->visualizer->requestNewLayer(PreviewLayerType::inputLine);
		if (owner->ui->visualizer->addLayer(linesLayer, size, BlendMode::multiply) == linesLayer) {
			directionLinesLayer = linesLayer;
			owner->ui->visualizer->setPreview(PreviewLayerType::inputLine, true);
		}
	}
	splineData->averageSegLen = averageVertexSpacing(splineData);
	if (directionLinesLayer != 0) {
		for (int i = 0; i < size; i++) {
			owner->ui->visualizer->updateLayerObject(directionLinesLayer, i,
				PreviewObj(splineData->anchors[i].ID,
					splineData->anchors[i].pos, 
					splineData->anchors[i].pos + 
						(splineData->anchors[i].dir * (splineData->averageSegLen * 1.5f)),
					blue, ShapeType::line, 2.0f));
		}
	}
	return AutoShapeType::none;
}

void AutoShape::visualizeShape(VertexData* splineData)
{

}

void AutoShape::clearVisuals()
{
	if (directionLinesLayer != 0) { owner->ui->visualizer->removeLayer(true, directionLinesLayer); }
	if (shapeBoundsLayer != 0) { owner->ui->visualizer->removeLayer(true, shapeBoundsLayer); }
	if (shapeLinesLayer != 0) { owner->ui->visualizer->removeLayer(true, shapeLinesLayer); }
	if (shapePointsLayer != 0) { owner->ui->visualizer->removeLayer(true, shapePointsLayer); }
	if (lineDetectBoundsLayer != 0) { owner->ui->visualizer->removeLayer(true, lineDetectBoundsLayer); }
	directionLinesLayer = shapeBoundsLayer = shapeLinesLayer = shapePointsLayer = 
		lineDetectBoundsLayer =	0;
}

AutoShapeType AutoShape::analyzeData(VertexData& outData,
	glm::ivec2 canvasDimensions, float zoomAmount, VertexData* splineData)
{
	AutoShapeType outShape = AutoShapeType::none;
	float lineConfidence = detectLine(outData, canvasDimensions, zoomAmount, splineData);
	if (lineConfidence >= 0.5f) { outShape = AutoShapeType::line; }
	return outShape;
}

float AutoShape::averageVertexSpacing(VertexData* splineData)
{
	size_t size = splineData->anchors.size();
	if (size < 2) { return glm::length(splineData->anchors.back().pos - splineData->anchors.front().pos); }
	float lenSum = 0.0f;
	for (size_t i = 1; i < size; i++) {
		lenSum += glm::length(splineData->anchors.at(i).pos - splineData->anchors.at(i - 1).pos);
	}
	return lenSum / float(size - 1);
}

float AutoShape::detectLine(VertexData& outData,
	glm::ivec2 canvasDimensions, float zoomAmount, VertexData* splineData)
{
	float confidence = 0.0f;
	
	glm::vec3 p1 = splineData->anchors.front().pos;
	glm::vec3 p2 = splineData->anchors.back().pos;
	float zoomFactor = zoomAmount / ((canvasDimensions.x + canvasDimensions.y) / 2.0f);
	float padding = zoomFactor * 120.0f;
	float boxHeight = zoomFactor * 140.0f;
	glm::vec2 size = glm::vec2(glm::length(p2 - p1) + padding, boxHeight);
	glm::vec3 lineDir = makeDir(p1, p2);
	float lineAngle = glm::degrees(atan2f(lineDir.y, lineDir.x));
	glm::vec3 lineNormal = lineDir * glm::quat(glm::vec3(0.0f, 0.0f, MATH_PI / 2.0f));
	p1 += lineNormal * (size.y / 2.0f); p1 -= lineDir * (padding / 2.0f);
	p2 -= lineNormal * (size.y / 2.0f); p2 += lineDir * (padding / 2.0f);

	if (lineDetectBoundsLayer == 0) {
		unsigned int boundsLayer = owner->ui->visualizer->requestNewLayer(PreviewLayerType::inputBounds);
		if (owner->ui->visualizer->addLayer(boundsLayer, 16, BlendMode::multiply) == boundsLayer) {
			lineDetectBoundsLayer = boundsLayer;
			owner->ui->visualizer->setPreview(PreviewLayerType::inputBounds, true);
		}
	}
	glm::quat rotation = glm::quat(glm::vec3(0.0f, 0.0f, atan2f(lineDir.y, lineDir.x)));
	glm::vec3 rotP1 = p1 * rotation;
	glm::vec3 rotP2 = p2 * rotation;
	bool boxBroken = false;
	bool lineBroken = false;
	for (int i = 0; i < splineData->anchors.size(); i++) {
		glm::vec3 point = splineData->anchors.at(i).pos * rotation;
		if (!boxBroken && !boxCollision_point(&point, &rotP1, &rotP2)) {
			boxBroken = true;
		}
		glm::vec3 dir = splineData->anchors.at(i).dir;
		float pointAngle = glm::degrees(atan2f(dir.y, dir.x));
		if (abs(pointAngle - lineAngle) > 35.0f) {
			lineBroken = true;
		}
	}
	CColor color = green;
	if (boxBroken) { color = red; }
	if (lineBroken && boxBroken) { color = red; }
	if (lineDetectBoundsLayer != 0) {
		owner->ui->visualizer->putBoundsObject(lineDetectBoundsLayer, 0, p1, p2, size, lineDir, color);
	}
	return confidence;
}