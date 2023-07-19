#include "../include/analytics/AutoShape.h"

#include "../include/entities/visuals/VertexData.h"
#include "../include/entities/visuals/FragmentAnchor.h"

#include "../include/math/collision.h"
#include "../include/math/math.h"

#ifndef APPLICATION_H
#include "../include/Application.h"
#endif
#ifndef USERINTERFACE_H
#include "../include/entities/UserInterface.h"
#endif

#include <deque>

AutoShape::AutoShape()
{
	preview.insert(std::pair<std::string, unsigned int>("directionLines", 0));
	preview.insert(std::pair<std::string, unsigned int>("line_bounds", 0));
	preview.insert(std::pair<std::string, unsigned int>("polyLine_bounds", 0));
	preview.insert(std::pair<std::string, unsigned int>("line_minLenLine", 0));
	preview.insert(std::pair<std::string, unsigned int>("line_cornerPoints", 0));
	//
	preview.insert(std::pair<std::string, unsigned int>("circle_axisLines", 0));
	preview.insert(std::pair<std::string, unsigned int>("circle_distanceLines", 0));
	preview.insert(std::pair<std::string, unsigned int>("circle_bounds", 0));
	preview.insert(std::pair<std::string, unsigned int>("circle_pointsA", 0));
	//
	preview.insert(std::pair<std::string, unsigned int>("smooth_lines", 0));
	preview.insert(std::pair<std::string, unsigned int>("smooth_points", 0));
	//
	preview.insert(std::pair<std::string, unsigned int>("shape_rawLines", 0));
	preview.insert(std::pair<std::string, unsigned int>("shape_rawPoints", 0));
	preview.insert(std::pair<std::string, unsigned int>("shape_regularLines", 0));
	preview.insert(std::pair<std::string, unsigned int>("shape_regularPoints", 0));
	//
	preview.insert(std::pair<std::string, unsigned int>("debug_points", 0));
	preview.insert(std::pair<std::string, unsigned int>("debug_lines", 0));
	preview.insert(std::pair<std::string, unsigned int>("debug_bounds", 0));
}

void AutoShape::initialize(std::shared_ptr<Application> owner)
{
	this->owner = owner;
}

AutoShapeType AutoShape::visualizeData(VertexData* splineData)
{
	size_t size = splineData->anchors.size();
	if (size < 2) { return AutoShapeType::none; }
	if (preview.at("directionLines") == 0) {
		unsigned int linesLayer = owner->ui->visualizer->requestNewLayer(PreviewLayerType::inputLine);
		if (owner->ui->visualizer->addLayer(linesLayer, size, BlendMode::multiply) == linesLayer) {
			preview.at("directionLines") = linesLayer;
			owner->ui->visualizer->setPreview(PreviewLayerType::inputLine, true);
		}
	}
	if (preview.at("directionLines") != 0) {
		for (int i = 0; i < size; i++) {
			float t = (atan2f(splineData->anchors[i].dir.x, splineData->anchors[i].dir.y) + MATH_PI) / (2.0f * MATH_PI);
			t = abs(t - 0.5f) * 2.0f;
			CColor color = CColor(0.0f + t, 1.0f - t, 1.0f);
			owner->ui->visualizer->updateLayerObject(preview.at("directionLines"), i,
				PreviewObj(splineData->anchors[i].ID,
					splineData->anchors[i].pos, 
					splineData->anchors[i].pos + 
						(splineData->anchors[i].dir * (splineData->averageSegLen * 1500.0f)),
					color, ShapeType::line, 2.0f));
		}
	}
	return AutoShapeType::none;
}

void AutoShape::visualizeShape(VertexData* rawShape, VertexData* regularShape, bool renderRaw, bool renderRegular)
{
	if (renderRaw) {
		size_t vertCount = rawShape->anchors.size();
		if (preview.at("shape_rawLines") == 0) {
			unsigned int linesLayer = owner->ui->visualizer->requestNewLayer(PreviewLayerType::inputLine);
			if (owner->ui->visualizer->addLayer(linesLayer, vertCount, BlendMode::multiply) == linesLayer) {
				preview.at("shape_rawLines") = linesLayer;
				owner->ui->visualizer->setPreview(PreviewLayerType::inputLine, true);
			}
		}
		if (preview.at("shape_rawLines") != 0) {
			for (int i = 1; i < vertCount; i++) {
				owner->ui->visualizer->updateLayerObject(preview.at("shape_rawLines"), i,
					PreviewObj(rawShape->anchors[i].ID,
						rawShape->anchors[i].pos,
						rawShape->anchors[i - 1].pos,
						green, ShapeType::line, 2.0f));
			}
		}

		if (preview.at("shape_rawPoints") == 0) {
			unsigned int pointsLayer = owner->ui->visualizer->requestNewLayer(PreviewLayerType::inputPoint);
			if (owner->ui->visualizer->addLayer(pointsLayer, 0, BlendMode::multiply) == pointsLayer) {
				preview.at("shape_rawPoints") = pointsLayer;
				owner->ui->visualizer->setPreview(PreviewLayerType::inputPoint, true);
			}
		}
		if (preview.at("shape_rawPoints") != 0) {
			for (size_t k = 0; k < vertCount; k++) {
				owner->ui->visualizer->putLayerObject(preview.at("shape_rawPoints"), k,
					PreviewObj(k, rawShape->anchors[k].pos, rawShape->anchors[k].dir,
						lime, ShapeType::square, 8.0f));
			}
		}
	}
	
	if (renderRegular) {
		size_t vertCount = regularShape->anchors.size();
		if (preview.at("shape_regularLines") == 0) {
			unsigned int linesLayer = owner->ui->visualizer->requestNewLayer(PreviewLayerType::inputLine);
			if (owner->ui->visualizer->addLayer(linesLayer, vertCount, BlendMode::multiply) == linesLayer) {
				preview.at("shape_regularLines") = linesLayer;
				owner->ui->visualizer->setPreview(PreviewLayerType::inputLine, true);
			}
		}
		if (preview.at("shape_regularLines") != 0) {
			for (int i = 1; i < vertCount; i++) {
				owner->ui->visualizer->updateLayerObject(preview.at("shape_regularLines"), i,
					PreviewObj(regularShape->anchors[i].ID,
						regularShape->anchors[i].pos,
						regularShape->anchors[i - 1].pos,
						orange, ShapeType::line, 2.0f));
			}
		}

		if (preview.at("shape_regularPoints") == 0) {
			unsigned int pointsLayer = owner->ui->visualizer->requestNewLayer(PreviewLayerType::inputPoint);
			if (owner->ui->visualizer->addLayer(pointsLayer, 0, BlendMode::multiply) == pointsLayer) {
				preview.at("shape_regularPoints") = pointsLayer;
				owner->ui->visualizer->setPreview(PreviewLayerType::inputPoint, true);
			}
		}
		if (preview.at("shape_regularPoints") != 0) {
			for (size_t k = 0; k < vertCount; k++) {
				owner->ui->visualizer->putLayerObject(preview.at("shape_regularPoints"), k,
					PreviewObj(k, regularShape->anchors[k].pos, regularShape->anchors[k].dir,
						red, ShapeType::square, 8.0f));
			}
		}
	}
	
}

void AutoShape::clearVisuals()
{
	for (auto& item : preview) {
		if (item.second != 0) { owner->ui->visualizer->removeLayer(true, item.second); item.second = 0; }
	}
}

void AutoShape::resetData()
{
	resetAvgAttribs();
	
	shapesValid = false;
	line.reset();
	polyLine.reset();
	circle.reset();
	ellipse.reset();
	triangle.reset();
	equilateralTriangle.reset();
	rectangle.reset();
	square.reset();
	pentagon.reset();
	hexagon.reset();
	septagon.reset();
	octagon.reset();
	nonagon.reset();
	decagon.reset();
}

void AutoShape::resetAvgAttribs()
{
	avgDataValid = false;
	totalLength = avgLength = leastLength = greatestLength = medianLength = 0.0f;
	totalLength_uv = avgLength_uv = leastLength_uv = greatestLength_uv = medianLength_uv = 0.0f;
	totalAngle = avgAngle = leastAngle = greatestAngle = medianAngle = 0.0f;

	angles.clear(); angles.shrink_to_fit();
	lengths.clear(); lengths.shrink_to_fit();
	lengths_uv.clear(); lengths_uv.shrink_to_fit();
}

AutoShapeType AutoShape::analyzeData(VertexData& outData,
	glm::ivec2 canvasDimensions, float zoomAmount, VertexData* splineData)
{
	// Prevent Bad Calls
	if (splineData->anchors.size() < 2) { return AutoShapeType::none; }

	AutoShapeType outShape = AutoShapeType::none;
	resetData();
	matrix = *owner->getCamera()->getShaderTransform();
	// 0 Initialize Debug Preview Visuals
	if (preview.at("debug_points") == 0) {
		unsigned int pointsLayer = owner->ui->visualizer->requestNewLayer(PreviewLayerType::inputPoint);
		if (owner->ui->visualizer->addLayer(pointsLayer, 0, BlendMode::multiply) == pointsLayer) {
			preview.at("debug_points") = pointsLayer;
			owner->ui->visualizer->setPreview(PreviewLayerType::inputPoint, true);
		}
	}
	if (preview.at("debug_lines") == 0) {
		unsigned int linesLayer = owner->ui->visualizer->requestNewLayer(PreviewLayerType::inputLine);
		if (owner->ui->visualizer->addLayer(linesLayer, 0, BlendMode::multiply) == linesLayer) {
			preview.at("debug_lines") = linesLayer;
			owner->ui->visualizer->setPreview(PreviewLayerType::inputLine, true);
		}
	}
	if (preview.at("debug_bounds") == 0) {
		unsigned int boundsLayer = owner->ui->visualizer->requestNewLayer(PreviewLayerType::inputBounds);
		if (owner->ui->visualizer->addLayer(boundsLayer, 0, BlendMode::multiply) == boundsLayer) {
			preview.at("debug_bounds") = boundsLayer;
			owner->ui->visualizer->setPreview(PreviewLayerType::inputBounds, true);
		}
	}

	size_t vertCount = splineData->anchors.size() - 1;

	RectBounds bounds = RectBounds();
	RectBounds bounds_uv = RectBounds();
	//RectBounds bounds = createVertexBounds(splineData);
	//RectBounds bounds_uv = createVertexBounds_uv(splineData, &matrix);
	createVertexBounds_compound(bounds, bounds_uv, splineData, &matrix);
	printStr(vec3VecToString({ bounds_uv.p1, bounds_uv.p2, bounds_uv.p3, bounds_uv.p4 }));
	glm::vec2 size = sizeFromBounds(bounds, 0.0f, false, false);
	glm::vec2 size_uv = sizeFromBounds(bounds_uv, 0.0f, false, true);
	glm::ivec2 windowDimensions = *owner->getWindowDimensions();
	windowDimensions /= 100.0f;
	float screenSizeRatio = abs((size_uv.x * windowDimensions.x) * (size_uv.y * windowDimensions.y));
	float screenFactor = (clampf(screenSizeRatio, 0.02f, 0.5f) - 0.02f) / (0.5f - 0.02f);
	glm::vec3 dir = DEFAULT_DIR;

	owner->ui->visualizer->putBoundsObject(preview.at("debug_bounds"), 0, bounds.p1, bounds.p3, size, dir, magenta);

	std::cout << "SIZE_UV-X:: " << size_uv.x << "  SIZE_UV-Y:: " << size_uv.y << 
		"  SCREEN_SPACE_SIZE:: " << screenSizeRatio << std::endl;


	// 1A Analyze for line segments and store this data
	VertexData segments = VertexData();
	float lineConfidence = constructSegments(segments, lerpf(0.008f, 0.022f, screenFactor), 1.05f, 1.0f, 5.0f, 5.0f, false, canvasDimensions, zoomAmount, splineData);
	if (lineConfidence >= 0.7f) { 
		std::cout << "AUTOSHAPE :: LINE DETECTED" << std::endl; 
		return AutoShapeType::line; 
	}
	if (segments.anchors.size() <= 1) { return outShape; }

	// 2A Smooth out the segments to further remove segments beneath the angle change threshold,
	// and double check for line detection, only for case of smoothing creating only 2 anchors.
	VertexData smoothed = VertexData();
	float smoothSegmentsConfidence = smoothSegments(smoothed, 0.08f, 22.0f, &segments);
	if (smoothed.anchors.size() <= 2) { 
		std::cout << "AUTOSHAPE :: LINE DETECTED" << std::endl;
		return AutoShapeType::line; 
	}
	//if (smoothed.anchors.size() == 0) { return outShape; }

	visualizeShape(&segments, &smoothed, true, true);

	// 3A Detect circle from splineData. Circle will always be found, and is the fallback shape
	// if no closely matching polygons are found.
	//float circleConfidence = detectCircle(outData, true, canvasDimensions, zoomAmount, splineData);
	//if (circleConfidence >= 0.5f) { outShape = AutoShapeType::circle; }

	//visualizeShape(&smoothed, &segments, true, true);

	// 4A Generate Shape Analytics
	std::vector<std::tuple<float, glm::vec3, float>> screenPoints = generateScreenPoints(&smoothed);
	float avgResult = generateAverageAttribs(&smoothed, true);
	if (avgResult < 0.5f) { return outShape; }

	// 4B Check For Small/Thin Rectangle from Smoothed
	VertexData closeSmoothed = VertexData();
	float closeSmoothedConfidence = closeShape(closeSmoothed, lerpf(0.007f, 0.014f, screenFactor), 1.2f, 1.2f, 7.2f, 3.5f, true, canvasDimensions, zoomAmount, &smoothed, screenPoints);
	smoothIterate(closeSmoothed, &closeSmoothed, 65.0f);
	std::vector<std::tuple<float, glm::vec3, float>> rectangleTestPoints = generateScreenPoints(&closeSmoothed);
	avgResult = generateAverageAttribs(&closeSmoothed, true);
	vertCount = closeSmoothed.anchors.size();

	if (vertCount >= 4 && vertCount <= 6) {
		if ((avgAngle >= 82.0f && avgAngle <= 98.0f) && 
			(medianAngle >= 82.0f && medianAngle <= 98.0f) && 
			(greatestAngle >= 65.0f && greatestAngle <= 135.0f && leastAngle >= 40.0f)) {
			std::cout << "SQUARE FOUND EARLY DETECTED - " << std::endl;
			std::cout << "POLYGON:: - SQUARE/RECTANGLE" << std::endl;
		}
	}

	

	// 5A Collapse points that occupy the same area
	VertexData overlapSegments = VertexData();
	float overlapConfidence = removeOverlap(overlapSegments, 
		lerpf(0.006f, 0.018f, screenFactor),
		lerpf(0.008f, 0.024f, screenFactor),
		0.032f, 2.8f, 5.2f, true, canvasDimensions, zoomAmount, &smoothed, screenPoints);
	screenPoints = generateScreenPoints(&overlapSegments);
	avgResult = generateAverageAttribs(&overlapSegments, true);
	overlapConfidence = removeOverlap(overlapSegments, 
		lerpf(0.002f, 0.008f, screenFactor),
		lerpf(0.006f, 0.022f, screenFactor), 0.022f, 5.5f, 7.5f, true, canvasDimensions, zoomAmount, &smoothed, screenPoints);
	screenPoints = generateScreenPoints(&overlapSegments);

	// 6A Close off the shape such that the beginning and end occupy the same position
	VertexData closeSegments = VertexData();
	float closeShapeConfidence = closeShape(closeSegments, 0.012f, 1.2f, 1.2f, 7.2f, 3.5f, true, canvasDimensions, zoomAmount, &overlapSegments, screenPoints);
	closeSegments.setDir();
	screenPoints = generateScreenPoints(&closeSegments);
	avgResult = generateAverageAttribs(&closeSegments, false);

	//visualizeShape(&closeSmoothed, &closeSegments, true, true);

	vertCount = closeSegments.anchors.size() - 1;

	std::cout << "VERT COUNT :: " << vertCount
		<< "\n :AVG_ANGLE :: " << avgAngle << "\n :MEDIAN_ANGLE :: " << medianAngle << "\n :TOTAL_ANGLE :: " << totalAngle 
		<< "\n :LEAST_ANGLE :: " << leastAngle << "\n :GREATEST_ANGLE :: " << greatestAngle
		<< "\n :AVG_LEN :: " << avgLength << " / " << avgLength_uv << "\n :MEDIAN_LEN :: " << medianLength << " / " << medianLength_uv
		<< "\n :LEAST_LEN :: " << leastLength << " / " << leastLength_uv << "\n :GREATEST_LEN :: " << greatestLength << " / " << greatestLength_uv << std::endl;

	bool cleanShapeResult = true;
	if (totalAngle >= 350.0f && totalAngle <= 370.0f) {
		std::cout << "REGULAR POLYGON DETECTED - " << std::endl;
		float greatAngleFactor = 1.65f;
		if (vertCount >= 3 && vertCount <= 6) {
			if (avgAngle >= 108.0f && avgAngle <= 138.0f && 
				greatestAngle <= 120.0f * greatAngleFactor) {
				std::cout << "POLYGON:: - TRIANGLE" << std::endl;
				cleanShapeResult = false;
			}
		}
		if (vertCount >= 4 && vertCount <= 6) {
			if (avgAngle >= 82.0f && avgAngle <= 98.0f && 
				greatestAngle <= 90.0f * greatAngleFactor) {
				std::cout << "POLYGON:: - SQUARE/RECTANGLE" << std::endl;
				cleanShapeResult = false;
			}
		}
		if (vertCount >= 5 && vertCount <= 7) {
			if (avgAngle >= 64.0f && avgAngle <= 81.0f && 
				greatestAngle <= 72.0f * greatAngleFactor) {
				std::cout << "POLYGON:: - PENGTAGON" << std::endl;
				cleanShapeResult = false;
			}
		}
		if (vertCount >= 6 && vertCount <= 9) {
			if (avgAngle >= 53.0f && avgAngle <= 72.0f && 
				greatestAngle <= 60.0f * greatAngleFactor) {
				std::cout << "POLYGON:: - HEXAGON" << std::endl;
				cleanShapeResult = false;
			}
		}
		if (vertCount >= 7 && vertCount <= 10) {
			if (avgAngle >= 46.0f && avgAngle <= 56.0f && 
				greatestAngle <= 51.0f * greatAngleFactor) {
				std::cout << "POLYGON:: - SEPTAGON" << std::endl;
				cleanShapeResult = false;
			}
		}
		if (vertCount >= 8 && vertCount <= 12) {
			if (avgAngle >= 41.0f && avgAngle <= 50.0f && 
				greatestAngle <= 45.0f * greatAngleFactor) {
				std::cout << "POLYGON:: - OCTAGON" << std::endl;
				cleanShapeResult = false;
			}
		}
		if (vertCount >= 9 && vertCount <= 14) {
			if (avgAngle >= 37.0f && avgAngle <= 42.0f && 
				greatestAngle <= 40.0f * greatAngleFactor) {
				std::cout << "POLYGON:: - NONAGON" << std::endl;
				cleanShapeResult = false;
			}
		}
		if (vertCount >= 10 && vertCount <= 16) {
			if (avgAngle >= 32.0f && avgAngle <= 40.0f && 
				greatestAngle <= 36.0f * greatAngleFactor) {
				std::cout << "POLYGON:: - DECAGON" << std::endl;
				cleanShapeResult = false;
			}
		}
		VertexData cleanShapeSegments = VertexData();
		float closeShapeConfidence = 0.0f;
		if (cleanShapeResult) {
			closeShapeConfidence = cleanShape(cleanShapeSegments, 0.012f, 1.2f, 1.2f, 7.2f, 3.5f, true, canvasDimensions, zoomAmount, &closeSegments, screenPoints);
			cleanShapeSegments.setDir();
			//visualizeShape(&closeSegments, &cleanShapeSegments, true, true);
		}
		else {
			cleanShapeSegments = closeSegments;
			closeShapeConfidence = 1.0f;
			//visualizeShape(&segments, &closeSegments, true, true);
		}
	}
	std::cout << "\n" << std::endl;
	//visualizeShape(&segments, &closeSegments, false, true);

	
	
	
	return outShape;
}

////////////////////////////////////////////////////////////////
//
// Segment Functions
//
////////////////////////////////////////////////////////////////

float AutoShape::constructSegments(VertexData& outData, float minSegLen_uv, float backFactor, float forwardFactor,
	float backAngleThreshold, float forwardAngleThreshold, bool doRender,
	glm::ivec2 canvasDimensions, float zoomAmount, VertexData* splineData)
{
	if (splineData->anchors.size() <= 2) { return 1.0f; }

	glm::vec3 p1 = splineData->anchors.front().pos;
	glm::vec3 p2 = splineData->anchors.back().pos;

	float zoomFactor = zoomAmount / ((canvasDimensions.x + canvasDimensions.y) / 2.0f);

	float padding = zoomFactor * 80.0f;
	float boxHeight = zoomFactor * 250.0f;
	glm::vec2 size = glm::vec2(glm::length(p2 - p1) + padding, boxHeight);

	glm::vec3 lineDir = makeDir(p1, p2);
	glm::vec3 lineNormal = lineDir * glm::quat(glm::vec3(0.0f, 0.0f, MATH_PI / 2.0f));

	if (doRender) {
		if (preview.at("line_bounds") == 0) {
			unsigned int boundsLayer = owner->ui->visualizer->requestNewLayer(PreviewLayerType::inputBounds);
			if (owner->ui->visualizer->addLayer(boundsLayer, 8, BlendMode::multiply) == boundsLayer) {
				preview.at("line_bounds") = boundsLayer;
				owner->ui->visualizer->setPreview(PreviewLayerType::inputBounds, true);
			}
		}
		if (preview.at("line_minLenLine") == 0) {
			unsigned int linesLayer = owner->ui->visualizer->requestNewLayer(PreviewLayerType::inputLine);
			if (owner->ui->visualizer->addLayer(linesLayer, 0, BlendMode::multiply) == linesLayer) {
				preview.at("line_minLenLine") = linesLayer;
				owner->ui->visualizer->setPreview(PreviewLayerType::inputLine, true);
			}
		}
	}

	bool boxBroken = false;
	bool lineBroken = false;
	bool averageBroken = false;
	bool minSegLen_found = false;

	glm::vec3 totalSegmentDir = glm::vec3(0.0f);
	glm::vec3 avgSegmentDir = glm::vec3(0.0f);
	float avgSegmentAngle = 0.0f;
	int runningSegCount = 0;
	float runningSegLen_real = 0.0f;
	float runningSegLen_uv = 0.0f;

	float lookBehind_segLen_uv = minSegLen_uv * backFactor;
	float lookForward_segLen_uv = minSegLen_uv * forwardFactor;

	std::vector<float> segLengths_uv;
	segLengths_uv.push_back(0.0f);
	for (size_t i = 1; i < splineData->anchors.size(); i++) {
		segLengths_uv.push_back(glm::length(
			(matrix.p * matrix.v * matrix.m * glm::vec4(splineData->anchors.at(i).pos, 1.0f)) -
			(matrix.p * matrix.v * matrix.m * glm::vec4(splineData->anchors.at(i - 1).pos, 1.0f))));
	}
	size_t lastSegBackDepth = 1;

	std::vector<std::tuple<glm::vec3, glm::vec3, FragmentAnchor>> cornerPoints;

	// Push back first corner point (Start Anchor)
	cornerPoints.push_back(std::tuple<glm::vec3, glm::vec3, FragmentAnchor>(splineData->anchors.front().pos, splineData->anchors.front().dir, splineData->anchors.front()));

	float greatestBreakAngle = 0.0f;
	for (size_t i = 0; i < splineData->anchors.size(); i++) {

		glm::vec3 dir = splineData->anchors.at(i).dir;

		runningSegCount++;
		totalSegmentDir += dir;
		avgSegmentDir = glm::normalize(totalSegmentDir / (float)runningSegCount);
		avgSegmentAngle = glm::degrees(atan2f(avgSegmentDir.y, avgSegmentDir.x));

		if (!minSegLen_found && i > 0) {
			// Add the segment length - Measured in screen units.
			runningSegLen_uv += segLengths_uv.at(i);
			if (runningSegLen_uv >= minSegLen_uv) {
				minSegLen_found = true;
				if (doRender && preview.at("line_minLenLine") != 0) {
					owner->ui->visualizer->addLayerObject(preview.at("line_minLenLine"),
						PreviewObj(splineData->anchors[i].ID,
							splineData->anchors[lastSegBackDepth - 1].pos,
							splineData->anchors[i].pos,
							red, ShapeType::line, 2.0f));
				}
			}
		}
		if (minSegLen_found) {
			size_t backDepth = i;
			size_t segBackDepth = lastSegBackDepth;
			float lookBehind_totalLen = 0.0f;
			glm::vec3 lookBehind_totalDir = glm::vec3(0.0f);
			glm::vec3 lookBehind_avgDir = glm::vec3(0.0f);
			// Back search by length to sum direction vectors
			do {
				if (segBackDepth < segLengths_uv.size()) {
					lookBehind_totalLen += segLengths_uv.at(segBackDepth);
					lookBehind_totalDir += splineData->anchors[backDepth].dir;
					backDepth--;
					segBackDepth++;
				}
				else {
					break;
				}
			} while (backDepth != 0 && lookBehind_totalLen < lookBehind_segLen_uv);
			// Average the sum direction vector.
			lookBehind_avgDir = glm::normalize(lookBehind_totalDir / float(segBackDepth - 1));
			float lookBehindAngle = glm::degrees(atan2f(lookBehind_avgDir.y, lookBehind_avgDir.x));
			// Check whether the direction has shifted.
			// If the direction is broken, then evaluate a look forward to ensure the change is not anomalous.
			if (compareAngle(lookBehindAngle, avgSegmentAngle, backAngleThreshold)) {
				size_t forwardDepth = i;
				size_t segForwardDepth = 0;
				float lookForward_totalLen = 0.0f;
				glm::vec3 lookForward_totalDir = glm::vec3(0.0f);
				glm::vec3 lookForward_avgDir = glm::vec3(0.0f);
				bool hitEnd = false;
				do {
					if (forwardDepth + 1 == splineData->anchors.size()) { hitEnd = true; break; }
					lookForward_totalLen += segLengths_uv.at(i + segForwardDepth);
					lookForward_totalDir += splineData->anchors[forwardDepth].dir;
					forwardDepth++;
					segForwardDepth++;

				} while (forwardDepth < splineData->anchors.size() && lookForward_totalLen < lookForward_segLen_uv);

				lookForward_avgDir = glm::normalize(lookForward_totalDir / float(forwardDepth));
				float lookForwardAngle = glm::degrees(atan2f(lookForward_avgDir.y, lookForward_avgDir.x));

				// If the change is real, then reset the current running segment to start a new line segment.
				if (!hitEnd && compareAngle(lookForwardAngle, avgSegmentAngle, forwardAngleThreshold)) {

					float dif = angleDifference(lookForwardAngle, avgSegmentAngle);
					if (dif > greatestBreakAngle) { greatestBreakAngle = dif; }

					cornerPoints.push_back(
						std::tuple<glm::vec3, glm::vec3, FragmentAnchor>(
							splineData->anchors[i - 1].pos,
							splineData->anchors[i - 1].dir,
							splineData->anchors[i - 1]));
					averageBroken = true;
					minSegLen_found = false;
					runningSegLen_uv = 0.0f;
					lastSegBackDepth = i;
					lastSegBackDepth = i;
					runningSegCount = 0;
					totalSegmentDir = glm::vec3(0.0f);
				}
			}
		}
	}
	// Push back ending corner point (End Anchor)
	cornerPoints.push_back(
		std::tuple<glm::vec3, glm::vec3, FragmentAnchor>(
			splineData->anchors.back().pos,
			splineData->anchors.back().dir,
			splineData->anchors.back()));

	// Build the output segment data & Capture furthest point distance

	float greatestDistance = 0.0f;
	for (size_t i = 0; i < cornerPoints.size(); i++) {
		glm::vec3 pos = std::get<0>(cornerPoints[i]);
		glm::vec3 dir = std::get<1>(cornerPoints[i]);

		// Check whether the vertex position is outside the simple line bounding box for analytics use
		float lineDist = distancePointLine2D(pos, glm::vec4(p1.x, p1.y, p2.x, p2.y));
		if (lineDist > (boxHeight / 2.0f)) { boxBroken = true; }
		if (lineDist > greatestDistance) { greatestDistance = lineDist; }

		if (i + 1 < cornerPoints.size()) {
			dir = makeDir(std::get<0>(cornerPoints[i]), std::get<0>(cornerPoints[i + 1]));
		}
		else { dir = makeDir(std::get<0>(cornerPoints.back()), std::get<0>(cornerPoints.front())); }

		outData.anchors.push_back(FragmentAnchor(i, pos, dir, 1.0f, std::get<2>(cornerPoints[i]).input));
	}
	outData.startTime = splineData->startTime; outData.endTime = splineData->endTime;
	outData.averageSegLen = splineData->averageSegLen;

	// Calculate Line Confidence
	float confidence = 1.0f;

	if (outData.anchors.size() <= 3) { confidence += 0.4f; }
	if (outData.anchors.size() <= 6) { confidence += 0.25f; }
	else if (outData.anchors.size() <= 10) { confidence -= 0.15f; }
	else { confidence -= 0.4f; }

	float distFactor = clampf(greatestDistance / (boxHeight), 0.0f, 1.0f);
	confidence -= distFactor;

	float angleFactor = clampf(greatestBreakAngle / 160.0f, 0.0f, 1.0f);
	confidence -= angleFactor;

	if (outData.anchors.size() == 2) { confidence = clampf(confidence, 0.8f, 1.5f); }

	// Rendering Assignments
	if (doRender && preview.at("line_cornerPoints") == 0) {
		unsigned int pointsLayer = owner->ui->visualizer->requestNewLayer(PreviewLayerType::inputPoint);
		if (owner->ui->visualizer->addLayer(pointsLayer, 0, BlendMode::multiply) == pointsLayer) {
			preview.at("line_cornerPoints") = pointsLayer;
			owner->ui->visualizer->setPreview(PreviewLayerType::inputPoint, true);
		}
	}
	if (doRender && preview.at("line_cornerPoints") != 0) {
		for (size_t k = 0; k < cornerPoints.size(); k++) {
			owner->ui->visualizer->putLayerObject(preview.at("line_cornerPoints"), k,
				PreviewObj(k, std::get<0>(cornerPoints[k]), std::get<1>(cornerPoints[k]),
					orange, ShapeType::square, 4.0f));
		}
	}
	if (doRender && preview.at("line_bounds") != 0) {
		// Adjust now points p1 and p2 as p1 and p3 of a bounding box for a line from spline start to end
		p1 += -lineNormal * (size.y / 2.0f); p1 += -lineDir * (padding / 2.0f);
		p2 += lineNormal * (size.y / 2.0f); p2 += lineDir * (padding / 2.0f);
		CColor color = yellow;
		CColor errorCol = red;
		color.mixColor(&errorCol, confidence);
		owner->ui->visualizer->putBoundsObject(preview.at("line_bounds"), 0, p1, p2, size, lineDir, color);
	}

	std::cout << "LINE :: CONFIDENCE == " << confidence << std::endl;
	return confidence;
}

float AutoShape::smoothSegments(VertexData& outData, float minSegLen_uv, float breakThreshold, VertexData* splineData)
{
	float confidence = 0.0f;
	size_t vertCount = splineData->anchors.size();

	smoothIterate(outData, splineData, breakThreshold);

	// Loop through segments, to identify the angle between segments. If the shape turns away from its predominant
	// rotation (CW or CCW), then it is likely a PolyLine


	return confidence;
}

float AutoShape::closeShape(VertexData& outData, float minSegLen_uv, float backFactor, float forwardFactor,
	float backAngleThreshold, float forwardAngleThreshold, bool doRender,
	glm::ivec2 canvasDimensions, float zoomAmount, VertexData* splineData, std::vector<std::tuple<float, glm::vec3, float>>& screenPoints)
{
	float confidence = 0.0f;
	if (splineData->anchors.size() <= 2) { return 1.0f; }
	size_t splineCount = splineData->anchors.size();
	outData = *splineData;

	glm::vec3 startPos = std::get<1>(screenPoints.at(0));
	glm::vec3 startDir = makeDir(startPos, std::get<1>(screenPoints.at(1)));

	std::tuple<float, float, float> lengthAttribs = screenSpaceLengthAttributes(&screenPoints);
	float closureThreshold = mix(std::get<1>(lengthAttribs), std::get<2>(lengthAttribs), 0.75f);

	std::tuple<float, glm::vec3, float>* closestPoint = nullptr;

	// Run the segments and look for an intersection of a small segment at the start with the current segment
	glm::vec3 pr1 = startPos - (startDir * (closureThreshold / 1.2f));
	glm::vec3 pr2 = startPos + (startDir * (closureThreshold / 1.2f));
	for (size_t j = 2; j < screenPoints.size(); j++) {
		if (j + 1 != screenPoints.size() && lineSegmentIntersection(pr1, pr2, std::get<1>(screenPoints.at(j)), std::get<1>(screenPoints.at(j + 1)))) {
			glm::vec3 intersect = lineIntersect2D(pr1, pr2, std::get<1>(screenPoints.at(j)), std::get<1>(screenPoints.at(j + 1)));
			if (glm::length(startPos - intersect) < closureThreshold) {
				std::get<1>(screenPoints.at(j + 1)) = intersect;
				closestPoint = &screenPoints.at(j + 1);
				size_t closeIndex = std::get<2>(*closestPoint);
				intersect = owner->pickScreenCoord(std::get<1>(*closestPoint), true);
				outData.anchors.at(closeIndex).pos = outData.anchors.front().pos = intersect;
				outData.anchors.at(closeIndex - 1).dir = makeDir(outData.anchors.at(closeIndex - 1).pos, outData.anchors.at(closeIndex).pos);
				outData.anchors.at(closeIndex).dir = outData.anchors.front().dir;
				outData.anchors.erase(outData.anchors.begin() + closeIndex + 1, outData.anchors.end());
				confidence = 1.0f;
				break;
			}
		}
	}

	float activeScanDistance = minSegLen_uv;
	size_t index = 1;
	// If there was an intersection, this is skipped
	if (closestPoint == nullptr) {
		while (closestPoint == nullptr) {
			if (index >= screenPoints.size()) {
				activeScanDistance *= 1.2f;
				index = 1;
			}
			else if (index < screenPoints.size() && std::get<0>(screenPoints.at(index)) <= activeScanDistance) {
				closestPoint = &screenPoints.at(index);
				if (std::get<0>(*closestPoint) >= closureThreshold || std::get<2>(*closestPoint) <= 2) {
					closestPoint = nullptr;
				}
				else {
					confidence = 0.6f;
				}
				break;
			}
			else {
				index++;
			}
		}
		// Close the shape by adding another vertex
		if (closestPoint == nullptr) {
			outData.anchors.push_back(splineData->anchors.front());
			outData.anchors.at(outData.anchors.size() - 2).dir = makeDir(outData.anchors.at(outData.anchors.size() - 2).pos, outData.anchors.back().pos);
			//smoothIterate(outData, &outData, 0.0f, 18.0f);
			confidence = 1.0f;
		}
	}
	
	// Verify that the shape is closed. If not closed, add another vertex
	if (glm::length(outData.anchors.back().pos - outData.anchors.front().pos) > 6.8f) {
		outData.anchors.push_back(splineData->anchors.front());
	}

	// Loop the shape and correct the direction vectors
	for (size_t k = 0; k < outData.anchors.size(); k++) {
		if (k == outData.anchors.size() - 1) {
			outData.anchors.at(k).dir = makeDir(outData.anchors.at(k).pos, outData.anchors.at(1).pos);
		}
		else {
			outData.anchors.at(k).dir = makeDir(outData.anchors.at(k).pos, outData.anchors.at(k + 1).pos);
		}
	}
	if (confidence == 0.0f) {
		int k = 5;
	}
	return confidence;
}

float AutoShape::removeOverlap(VertexData& outData, float minSegLen_uv, float backFactor, float forwardFactor,
	float backAngleThreshold, float forwardAngleThreshold, bool doRender,
	glm::ivec2 canvasDimensions, float zoomAmount, VertexData* splineData,
	std::vector<std::tuple<float, glm::vec3, float>>& screenPoints)
{
	float confidence = 0.0f;
	
	std::vector<std::tuple<float, glm::vec3, float>> scrPointWork = screenPoints;

	std::deque<std::pair<glm::vec3, int>> nodes;
	for (size_t k = 0; k < scrPointWork.size(); k++) {
		glm::vec3 pos = std::get<1>(scrPointWork[k]);
		if (nodes.size() == 0) {
			nodes.push_back(std::pair<glm::vec3, int>(pos, 1));
		}
		else {
			bool cullVertex = false;
			for (size_t j = 0; j < nodes.size(); j++) {
				float len = glm::length(std::get<1>(scrPointWork[k]) - nodes[j].first);
				if (len < avgLength_uv / backAngleThreshold) {
					if (len < avgLength_uv / forwardAngleThreshold) {
						nodes[j].second++;
						nodes[j].first = mix(nodes[j].first, ((nodes[j].first * float(nodes[j].second - 1)) + pos) / float(nodes[j].second), 0.32f);
					}
					cullVertex = true; break;
				}
				if (nodes.size() >= 2) {
					for (size_t z = 0; z < nodes.size() - 1; z++) {
						std::pair<glm::vec3, glm::vec3> line = std::pair<glm::vec3, glm::vec3>(nodes[z].first, nodes[z + 1].first);
						float dist = distancePointLineSegment(pos, line.first, line.second);
						if (dist < minSegLen_uv) {
							glm::vec3 proj = projectPointToLine2D(pos, line, false);
							dist = distancePointLineSegment(proj, line.first, line.second);
							if (dist < backFactor) {
								float distA = glm::length(proj - nodes[z].first);
								float distB = glm::length(proj - nodes[z + 1].first);
								if (distA < distB) {
									nodes[z].first = proj;
								}
								else { nodes[z + 1].first = proj; }
							}
							cullVertex = true; break;
						}
					}
				}
			}
			if (nodes.size() >= 2) {
				size_t nodeCount = nodes.size() - 1;
				glm::vec3 dirLast = makeDir(nodes[nodeCount - 1].first, nodes[nodeCount].first);
				glm::vec3 dirNext = makeDir(nodes[nodeCount].first, pos);
				if (compareParallel(glm::degrees(atan2f(dirLast.y, dirLast.x)), glm::degrees(atan2f(dirNext.y, dirNext.x)), 8.0f)) {
					cullVertex = true;
				}
			}
			if (!cullVertex) {
				if (scrPointWork.size() > 1) {
					glm::vec3 proxy = std::get<1>(scrPointWork[k - 1]);
					float lenA = glm::length(nodes[0].first - proxy);
					float lenB = glm::length(nodes.back().first - proxy);
					if (lenA < lenB) {
						std::deque<std::pair<glm::vec3, int>> nodeStore = nodes;
						nodes.clear();
						for (int p = nodeStore.size() - 1; p >= 0; p--) {
							nodes.push_back(nodeStore[p]);
						}
						nodes.push_back(std::pair<glm::vec3, int>(pos, 1));
					}
					else {
						nodes.push_back(std::pair<glm::vec3, int>(pos, 1));
					}
				}
				else {
					nodes.push_back(std::pair<glm::vec3, int>(pos, 1));
				}
			}
		}
	}
	outData.anchors.clear();
	for (size_t s = 0; s < nodes.size(); s++) {
		outData.anchors.push_back(FragmentAnchor(s, owner->pickScreenCoord(nodes[s].first, true), DEFAULT_DIR, 1.0f, Input()));
	}

	return confidence;
}

float AutoShape::cleanShape(VertexData& outData, float minSegLen_uv, float backFactor, float forwardFactor,
	float backAngleThreshold, float forwardAngleThreshold, bool doRender,
	glm::ivec2 canvasDimensions, float zoomAmount, VertexData* splineData,
	std::vector<std::tuple<float, glm::vec3, float>>& screenPoints)
{
	float confidence = 0.0f;



	return confidence;
}

////////////////////////////////////////////////////////////////
//
// Analysis Functions
//
////////////////////////////////////////////////////////////////

AutoShapeType AutoShape::analyzeShape(VertexData& outData, VertexData* splineData,
	std::vector<std::tuple<float, glm::vec3, float>>& screenPoints)
{
	AutoShapeType result = AutoShapeType::none;
	if (!compareVec3_byDistance(std::get<1>(screenPoints.front()), std::get<1>(screenPoints.back()), 2.4f) ||
		!compareVec3_byDistance(splineData->anchors.front().pos, splineData->anchors.back().pos, 2.4f)) {
		return AutoShapeType::polyLine;
	}
	size_t vertCount = splineData->anchors.size();

	//avgDataValid = true;

	if (lengths.size() >= 4 && vertCount >= 4 && vertCount <= 8) {
		square.confidence += 0.2f;
		if (greatestAngle >= 75.0f && greatestAngle <= 110.0f) {
			square.confidence += 0.3f;
		}
		if (avgAngle >= 80.0f && avgAngle <= 100.0f) {
			square.confidence += 0.3f;
		}
		if (medianAngle >= 70.0f && medianAngle <= 110.0f) {
			square.confidence += 0.3f;
		}
		if (square.confidence >= 0.5f) {
			float result = detectRectangle(splineData, &lengths_uv, &angles, &screenPoints);
		}
	}
	if (vertCount >= 4) {
		float result = detectPolygons(splineData, &lengths, &angles, &screenPoints);
	}
	if (vertCount >= 3 && vertCount <= 8) {
		float result = detectTriangles(splineData, &lengths, &angles, &screenPoints);
	}
	return result;
}

////////////////////////////////////////////////////////////////
//
// Detection Functions
//
////////////////////////////////////////////////////////////////

float AutoShape::detectLine(VertexData& outData, bool doRender,
	glm::ivec2 canvasDimensions, float zoomAmount, VertexData* splineData)
{
	float confidence = 0.0f;

	return confidence;
}

float AutoShape::detectTriangles(VertexData* splineData,
	std::vector<std::pair<float, size_t>>* lengthsSorted,
	std::vector<std::pair<float, size_t>>* anglesSorted,
	std::vector<std::tuple<float, glm::vec3, float>>* screenPoints)
{
	float confidence = 0.0f;

	VertexData shape = *splineData;
	size_t vertCount = shape.anchors.size();
	size_t sides = lengthsSorted->size() - 1;

	glm::vec3 centerPos = averagePosition_bySides(splineData, true);

	std::pair<size_t, size_t> indices1 = std::pair<size_t, size_t>(lengthsSorted->at(sides).second, lengthsSorted->at(sides).second + 1);
	if (indices1.first == vertCount - 1) { indices1.second = 0; }
	std::pair<glm::vec3, glm::vec3> side1 = std::pair<glm::vec3, glm::vec3>(
		splineData->anchors[indices1.first].pos,
		splineData->anchors[indices1.second].pos);
	float side1_angle = makeAngle(side1.first, side1.second);

	std::pair<size_t, size_t> indices2 = std::pair<size_t, size_t>(lengthsSorted->at(sides - 1).second, lengthsSorted->at(sides - 1).second + 1);
	if (indices2.first == vertCount - 1) { indices2.second = 0; }
	std::pair<glm::vec3, glm::vec3> side2 = std::pair<glm::vec3, glm::vec3>(
		splineData->anchors[indices2.first].pos,
		splineData->anchors[indices2.second].pos);
	float side2_angle = makeAngle(side2.first, side2.second);

	std::pair<size_t, size_t> indices3 = std::pair<size_t, size_t>(lengthsSorted->at(sides - 2).second, lengthsSorted->at(sides - 2).second + 1);
	if (indices3.first == vertCount - 1) { indices3.second = 0; }
	std::pair<glm::vec3, glm::vec3> side3 = std::pair<glm::vec3, glm::vec3>(
		splineData->anchors[indices3.first].pos,
		splineData->anchors[indices3.second].pos);
	float side3_angle = makeAngle(side3.first, side3.second);

	triangle.points[0].vertex.pos = lineIntersect2D(side1.first, side1.second, side2.first, side2.second);
	triangle.points[1].vertex.pos = lineIntersect2D(side1.first, side1.second, side3.first, side3.second);
	triangle.points[2].vertex.pos = lineIntersect2D(side2.first, side2.second, side3.first, side3.second);
	triangle.setDir();

	side1_angle = angleDifference(
		makeAngle(triangle.points[2].vertex.pos, triangle.points[0].vertex.pos), 
		makeAngle(triangle.points[0].vertex.pos, triangle.points[1].vertex.pos));
	side2_angle = angleDifference(
		makeAngle(triangle.points[0].vertex.pos, triangle.points[1].vertex.pos),
		makeAngle(triangle.points[1].vertex.pos, triangle.points[2].vertex.pos));
	side3_angle = angleDifference(
		makeAngle(triangle.points[1].vertex.pos, triangle.points[2].vertex.pos),
		makeAngle(triangle.points[2].vertex.pos, triangle.points[0].vertex.pos));

	glm::vec3 principleDir = glm::vec3(0.0f);
	glm::vec3 principleDir_p = glm::vec3(0.0f);
	std::pair<glm::vec3, glm::vec3> sideOpposite = side1;
	float lenB = 0.0f, lenC = 0.0f;
	bool search = false;
	float spread = 5.0f;
	while (search == false ) {
		if (side1_angle <= 90.0f + spread && side1_angle >= 90.0f - spread) {
			rightTriangle.points[0].vertex.pos = triangle.points[0].vertex.pos;
			lenB = glm::length(triangle.points[0].vertex.pos - triangle.points[1].vertex.pos);
			lenC = glm::length(triangle.points[0].vertex.pos - triangle.points[2].vertex.pos);
			if (lenB > lenC) { principleDir = makeDir(triangle.points[0].vertex.pos, triangle.points[1].vertex.pos); }
			else { principleDir = makeDir(triangle.points[0].vertex.pos, triangle.points[2].vertex.pos); }
			sideOpposite = std::pair<glm::vec3, glm::vec3>(triangle.points[1].vertex.pos, triangle.points[2].vertex.pos);
			search = true;
		}
		else if (side2_angle <= 90.0f + spread && side2_angle >= 90.0f - spread) {
			rightTriangle.points[0].vertex.pos = triangle.points[1].vertex.pos;
			lenB = glm::length(triangle.points[1].vertex.pos - triangle.points[0].vertex.pos);
			lenC = glm::length(triangle.points[1].vertex.pos - triangle.points[2].vertex.pos);
			if (lenB > lenC) { principleDir = makeDir(triangle.points[1].vertex.pos, triangle.points[0].vertex.pos); }
			else { principleDir = makeDir(triangle.points[1].vertex.pos, triangle.points[2].vertex.pos); }
			sideOpposite = std::pair<glm::vec3, glm::vec3>(triangle.points[0].vertex.pos, triangle.points[2].vertex.pos);
			search = true;
		}
		else if (side3_angle <= 90.0f + spread && side3_angle >= 90.0f - spread) {
			rightTriangle.points[0].vertex.pos = triangle.points[2].vertex.pos;
			lenB = glm::length(triangle.points[2].vertex.pos - triangle.points[1].vertex.pos);
			lenC = glm::length(triangle.points[2].vertex.pos - triangle.points[0].vertex.pos);
			if (lenB > lenC) { principleDir = makeDir(triangle.points[2].vertex.pos, triangle.points[1].vertex.pos); }
			else { principleDir = makeDir(triangle.points[2].vertex.pos, triangle.points[0].vertex.pos); }
			sideOpposite = std::pair<glm::vec3, glm::vec3>(triangle.points[1].vertex.pos, triangle.points[0].vertex.pos);
			search = true;
		}
		spread += 5.0f;
	}
	
	principleDir_p = principleDir * glm::quat(glm::vec3(0.0f, 0.0f, MATH_PI / 2.0f));
	rightTriangle.points[1].vertex.pos = lineIntersect2D(
		rightTriangle.points[0].vertex.pos, rightTriangle.points[0].vertex.pos + (principleDir * 100.0f),
		sideOpposite.first, sideOpposite.second);
	rightTriangle.points[2].vertex.pos = lineIntersect2D(
		rightTriangle.points[0].vertex.pos, rightTriangle.points[0].vertex.pos + (principleDir_p * 100.0f),
		sideOpposite.first, sideOpposite.second);
	rightTriangle.setDir();

	std::cout << "TRIANGLE:: " << side1_angle << " : " << side2_angle << " : " << side3_angle << std::endl;

	if (preview.at("debug_points") != 0) {
		owner->ui->visualizer->addLayerObject(preview.at("debug_points"),
			PreviewObj(0, centerPos, splineData->anchors[0].dir,
				magenta, ShapeType::square, 8.0f));
		owner->ui->visualizer->addLayerObject(preview.at("debug_points"),
			PreviewObj(0, rightTriangle.points[0].vertex.pos, rightTriangle.points[0].vertex.dir,
				yellow, ShapeType::square, 16.0f));
		owner->ui->visualizer->addLayerObject(preview.at("debug_points"),
			PreviewObj(0, rightTriangle.points[1].vertex.pos, rightTriangle.points[1].vertex.dir,
				olive, ShapeType::square, 16.0f));
		owner->ui->visualizer->addLayerObject(preview.at("debug_points"),
			PreviewObj(0, rightTriangle.points[2].vertex.pos, rightTriangle.points[2].vertex.dir,
				orange, ShapeType::square, 16.0f));
	}

	return confidence;
}

float AutoShape::detectRectangle(VertexData* splineData,
	std::vector<std::pair<float, size_t>>* lengthsSorted,
	std::vector<std::pair<float, size_t>>* anglesSorted,
	std::vector<std::tuple<float, glm::vec3, float>>* screenPoints)
{
	float confidence = 0.0f;
	//std::cout << "DETECT::RECTANGLE" << std::endl;

	VertexData shape = *splineData;
	size_t vertCount = shape.anchors.size();
	size_t sides = lengthsSorted->size() - 1;

	glm::vec3 centerPos = averagePosition_byCorner(splineData, true);

	std::pair<size_t, size_t> indices1 = std::pair<size_t, size_t>(lengthsSorted->at(sides).second, lengthsSorted->at(sides).second + 1);
	if (indices1.first == vertCount - 1) { indices1.second = 0; }
	std::pair<glm::vec3, glm::vec3> side1 = std::pair<glm::vec3, glm::vec3>(
		splineData->anchors[indices1.first].pos,
		splineData->anchors[indices1.second].pos);
	float side1_angle = makeAngle(side1.first, side1.second);

	std::pair<size_t, size_t> indices2 = std::pair<size_t, size_t>(lengthsSorted->at(sides - 1).second, lengthsSorted->at(sides - 1).second + 1);
	if (indices2.first == vertCount - 1) { indices2.second = 0; }
	std::pair<glm::vec3, glm::vec3> side2 = std::pair<glm::vec3, glm::vec3>(
		splineData->anchors[indices2.first].pos,
		splineData->anchors[indices2.second].pos);
	float side2_angle = makeAngle(side2.first, side2.second);

	std::pair<size_t, size_t> indices3 = std::pair<size_t, size_t>(lengthsSorted->at(sides - 2).second, lengthsSorted->at(sides - 2).second + 1);
	if (indices3.first == vertCount - 1) { indices3.second = 0; }
	std::pair<glm::vec3, glm::vec3> side3 = std::pair<glm::vec3, glm::vec3>(
		splineData->anchors[indices3.first].pos,
		splineData->anchors[indices3.second].pos);
	float side3_angle = makeAngle(side3.first, side3.second);

	std::pair<size_t, size_t> indices4 = std::pair<size_t, size_t>(lengthsSorted->at(sides - 3).second, lengthsSorted->at(sides - 3).second + 1);
	if (indices4.first == vertCount - 1) { indices4.second = 0; }
	std::pair<glm::vec3, glm::vec3> side4 = std::pair<glm::vec3, glm::vec3>(
		splineData->anchors[indices4.first].pos,
		splineData->anchors[indices4.second].pos);
	float side4_angle = makeAngle(side4.first, side4.second);

	std::pair<glm::vec3, glm::vec3> sideA;
	std::pair<glm::vec3, glm::vec3> sideB;
	glm::quat rot = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
	if (compareParallel(side1_angle, side2_angle, 35.0f)) {
		sideA = side1;
		sideB = side2;
		rot = glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(180.0f - abs(angleDifference(side1_angle, side2_angle)))));
	}
	else if (compareParallel(side1_angle, side3_angle, 35.0f)) {
		sideA = side1;
		sideB = side3;
		rot = glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(180.0f - abs(angleDifference(side1_angle, side3_angle)))));
	}
	else if (compareParallel(side1_angle, side4_angle, 35.0f)) {
		sideA = side1;
		sideB = side4;
		rot = glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(180.0f - abs(angleDifference(side1_angle, side4_angle)))));
	}
	else if (compareParallel(side2_angle, side3_angle, 35.0f)) {
		sideA = side2;
		sideB = side3;
		rot = glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(180.0f - abs(angleDifference(side2_angle, side3_angle)))));
	}
	else if (compareParallel(side2_angle, side4_angle, 35.0f)) {
		sideA = side2;
		sideB = side4;
		rot = glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(180.0f - abs(angleDifference(side2_angle, side4_angle)))));
	}
	else if (compareParallel(side3_angle, side4_angle, 35.0f)) {
		sideA = side3;
		sideB = side4;
		rot = glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(180.0f - abs(angleDifference(side3_angle, side4_angle)))));
	}

	rectangle.points[0].vertex.pos = sideA.first;
	rectangle.points[1].vertex.pos = sideA.second;
	float sideLen = glm::length(sideA.first - sideA.second);
	glm::vec3 dir = makeDir(sideA.first, sideA.second);
	glm::vec3 dir_p = dir * glm::quat(glm::vec3(0.0f, 0.0f, MATH_PI / 2.0f));
	glm::vec3 sideB_center = (sideB.first + sideB.second) / 2.0f;
	rectangle.points[2].vertex.pos = lineIntersect2D(
		sideA.second, sideA.second + (dir_p * 100.0f),
		sideB_center, sideB_center + (dir * 100.0f));
	rectangle.points[3].vertex.pos = rectangle.points[2].vertex.pos + (-dir * sideLen);
	rectangle.center();
	rectangle.setDir();


	if (preview.at("debug_points") != 0) {
		/*
		owner->ui->visualizer->addLayerObject(preview.at("debug_points"),
			PreviewObj(0, centerPos, splineData->anchors[0].dir,
				magenta, ShapeType::square, 8.0f));
		owner->ui->visualizer->addLayerObject(preview.at("debug_points"),
			PreviewObj(0, rectangle.points[0].vertex.pos, rectangle.points[0].vertex.dir,
				purple, ShapeType::square, 16.0f));
		owner->ui->visualizer->addLayerObject(preview.at("debug_points"),
			PreviewObj(0, rectangle.points[1].vertex.pos, rectangle.points[1].vertex.dir,
				purple, ShapeType::square, 16.0f));
		owner->ui->visualizer->addLayerObject(preview.at("debug_points"),
			PreviewObj(0, rectangle.points[2].vertex.pos, rectangle.points[2].vertex.dir,
				purple, ShapeType::square, 16.0f));
		owner->ui->visualizer->addLayerObject(preview.at("debug_points"),
			PreviewObj(0, rectangle.points[3].vertex.pos, rectangle.points[3].vertex.dir,
				purple, ShapeType::square, 16.0f));//
				*/
	}

	return confidence;
}

float AutoShape::detectPolygons(VertexData* splineData,
	std::vector<std::pair<float, size_t>>* lengthsSorted,
	std::vector<std::pair<float, size_t>>* anglesSorted,
	std::vector<std::tuple<float, glm::vec3, float>>* screenPoints)
{
	float confidence = 0.0f;

	VertexData shape = *splineData;
	size_t vertCount = shape.anchors.size();
	size_t sides = lengthsSorted->size() - 1;

	glm::vec3 centerPos = averagePosition_byCorner(splineData, true);
	float radius = 0.0f;
	float toalCenterDist = 0.0f;
	for (size_t j = 0; j < sides; j++) {
		toalCenterDist += glm::length(centerPos - splineData->anchors[j].pos);
	}
	float avgCenterDist = toalCenterDist / (float)sides;

	std::vector<std::pair<float, size_t>> lengths = *lengthsSorted;
	for (size_t k = 0; k < lengths.size(); k++) {
		lengths[k].first = abs(avgLength - lengths[k].first);
	}
	sortFloatPair(lengths);
	size_t medianIndex = lengths.at(lengths.size() / 2).second;
	glm::vec3 pA = splineData->anchors[medianIndex].pos;
	glm::vec3 pB = splineData->anchors[medianIndex + 1].pos;
	glm::vec3 principleDir = makeDir(pA, pB);
	glm::vec3 principleDir_p = principleDir * glm::quat(glm::vec3(0.0f, 0.0f, MATH_PI / 2.0f));
	float principleAngle = atan2f(principleDir.y, principleDir.x);
	glm::vec3 principleCenter = averagePosition_bySides(splineData, true);
	glm::vec3 principleOrigin = lineIntersect2D(principleCenter, principleCenter + (principleDir_p * 100.0f), pA, pB);

	size_t polygonSides = 3;
	glm::vec3 startPos = principleOrigin + (principleDir * (avgLength / 2.0f));
	for (size_t s = 0; s < polygonSides; s++) {
		glm::quat rot = glm::quat(glm::vec3(0.0f, 0.0f, glm::radians((360.0f / (float)polygonSides) * float(s))));
		equilateralTriangle.points[s].vertex.pos = ((startPos - principleCenter) * rot) + principleCenter;
	}
	equilateralTriangle.setDir();

	polygonSides = 5;
	for (size_t s = 0; s < polygonSides; s++) {
		glm::quat rot = glm::quat(glm::vec3(0.0f, 0.0f, glm::radians((360.0f / (float)polygonSides) * float(s))));
		pentagon.points[s].vertex.pos = ((startPos - principleCenter) * rot) + principleCenter;
	}
	pentagon.setDir();

	polygonSides = 6;
	for (size_t s = 0; s < polygonSides; s++) {
		glm::quat rot = glm::quat(glm::vec3(0.0f, 0.0f, glm::radians((360.0f / (float)polygonSides) * float(s))));
		hexagon.points[s].vertex.pos = ((startPos - principleCenter) * rot) + principleCenter;
	}
	hexagon.setDir();

	polygonSides = 7;
	for (size_t s = 0; s < polygonSides; s++) {
		glm::quat rot = glm::quat(glm::vec3(0.0f, 0.0f, glm::radians((360.0f / (float)polygonSides) * float(s))));
		septagon.points[s].vertex.pos = ((startPos - principleCenter) * rot) + principleCenter;
	}
	septagon.setDir();

	polygonSides = 8;
	for (size_t s = 0; s < polygonSides; s++) {
		glm::quat rot = glm::quat(glm::vec3(0.0f, 0.0f, glm::radians((360.0f / (float)polygonSides) * float(s))));
		octagon.points[s].vertex.pos = ((startPos - principleCenter) * rot) + principleCenter;
	}
	octagon.setDir();

	polygonSides = 9;
	for (size_t s = 0; s < polygonSides; s++) {
		glm::quat rot = glm::quat(glm::vec3(0.0f, 0.0f, glm::radians((360.0f / (float)polygonSides) * float(s))));
		nonagon.points[s].vertex.pos = ((startPos - principleCenter) * rot) + principleCenter;
	}
	nonagon.setDir();

	polygonSides = 10;
	for (size_t s = 0; s < polygonSides; s++) {
		glm::quat rot = glm::quat(glm::vec3(0.0f, 0.0f, glm::radians((360.0f / (float)polygonSides) * float(s))));
		decagon.points[s].vertex.pos = ((startPos - principleCenter) * rot) + principleCenter;
	}
	decagon.setDir();

	if (preview.at("debug_points") != 0) {
		polygonSides = 3;
		for (size_t p = 0; p < polygonSides; p++) {
			owner->ui->visualizer->addLayerObject(preview.at("debug_points"),
				PreviewObj(0, equilateralTriangle.points[p].vertex.pos, equilateralTriangle.points[p].vertex.dir,
					blue, ShapeType::square, 16.0f));
		}

		polygonSides = 5;
		for (size_t p = 0; p < polygonSides; p++) {
			owner->ui->visualizer->addLayerObject(preview.at("debug_points"),
				PreviewObj(0, pentagon.points[p].vertex.pos, pentagon.points[p].vertex.dir,
					red, ShapeType::square, 8.0f));
		}
		polygonSides = 6;
		for (size_t p = 0; p < polygonSides; p++) {
			owner->ui->visualizer->addLayerObject(preview.at("debug_points"),
				PreviewObj(0, hexagon.points[p].vertex.pos, hexagon.points[p].vertex.dir,
					green, ShapeType::square, 8.0f));
		}
		polygonSides = 7;
		for (size_t p = 0; p < polygonSides; p++) {
			owner->ui->visualizer->addLayerObject(preview.at("debug_points"),
				PreviewObj(0, septagon.points[p].vertex.pos, septagon.points[p].vertex.dir,
					purple, ShapeType::square, 8.0f));
		}
		polygonSides = 8;
		for (size_t p = 0; p < polygonSides; p++) {
			owner->ui->visualizer->addLayerObject(preview.at("debug_points"),
				PreviewObj(0, octagon.points[p].vertex.pos, octagon.points[p].vertex.dir,
					blue, ShapeType::square, 8.0f));
		}
		polygonSides = 9;
		for (size_t p = 0; p < polygonSides; p++) {
			owner->ui->visualizer->addLayerObject(preview.at("debug_points"),
				PreviewObj(0, nonagon.points[p].vertex.pos, nonagon.points[p].vertex.dir,
					aqua, ShapeType::square, 8.0f));
		}
		polygonSides = 10;
		for (size_t p = 0; p < polygonSides; p++) {
			owner->ui->visualizer->addLayerObject(preview.at("debug_points"),
				PreviewObj(0, decagon.points[p].vertex.pos, decagon.points[p].vertex.dir,
					olive, ShapeType::square, 8.0f));
		}
	}

	return confidence;
}

float AutoShape::detectCircle(VertexData& outData, bool doRender,
	glm::ivec2 canvasDimensions, float zoomAmount, VertexData* splineData)
{
	float confidence = 0.0f;
	if (splineData->anchors.size() <= 2) { return 0.0f; }

	ShaderTransform matrix = *owner->getCamera()->getShaderTransform();
	float zoomFactor = zoomAmount / ((canvasDimensions.x + canvasDimensions.y) / 2.0f);

	glm::vec3 totalDir = glm::vec3(0.0f);
	glm::vec3 avgDir = glm::vec3(0.0f);
	glm::vec3 startPos = glm::vec3(0.0f);

	glm::vec3 totalPos = glm::vec3(0.0f);
	glm::vec3 centerPos = glm::vec3(0.0f);

	glm::vec3 totalSegmentDir = glm::vec3(0.0f);
	glm::vec3 avgSegmentDir = glm::vec3(0.0f);
	float avgSegmentAngle = 0.0f;
	int runningSegCount = 0;
	float runningSegLen_real = 0.0f;
	float runningSegLen_uv = 0.0f;
	float startSegLen_uv = 0.0f;
	float minSegLen_uv = 0.032f;
	if (splineData->averageSegLen == 0.0f) { splineData->averageSegLen = averageVertexSpacing(splineData); }

	float lookBehind_segLen_uv = minSegLen_uv * 1.2f;
	float lookForward_segLen_uv = minSegLen_uv * 1.4f;

	std::vector<float> segLengths_uv;
	segLengths_uv.push_back(0.0f);
	for (size_t i = 1; i < splineData->anchors.size(); i++) {
		segLengths_uv.push_back(glm::length(
			(matrix.p * matrix.v * matrix.m * glm::vec4(splineData->anchors.at(i).pos, 1.0f)) -
			(matrix.p * matrix.v * matrix.m * glm::vec4(splineData->anchors.at(i - 1).pos, 1.0f))));
	}
	size_t lastSegBackDepth = 1;

	bool startPointFound = false;

	// Identify the start point and average direction
	size_t size = splineData->anchors.size();
	for (size_t i = 0; i < size; i++) {
		// Establish start point
		if (!startPointFound && startSegLen_uv < minSegLen_uv) {
			startSegLen_uv += segLengths_uv.at(i);
			if (startSegLen_uv >= minSegLen_uv) {
				startPointFound = true;
				int index = i;
				glm::vec3 sumPos = glm::vec3(0.0f);
				while (index >= 0) {
					sumPos += splineData->anchors[index].pos;
					index--;
				}
				startPos = sumPos / (float(i) + 1.0f);
			}
		}
		// Average Position
		totalPos += splineData->anchors[i].pos;
		// Average Dir
		totalSegmentDir += splineData->anchors[i].dir;
	}
	centerPos = totalPos / float(size);
	glm::vec3 startDir = makeDir(startPos, centerPos);
	glm::vec3 startDir_perp = startDir * glm::quat(glm::vec3(0.0f, 0.0f, MATH_PI / 2.0f));
	glm::vec3 startPos_perp = centerPos + (startDir_perp * 300.0f);
	float centerAngle = glm::degrees(atan2f(startDir.y, startDir.x));
	avgSegmentDir = glm::normalize(totalSegmentDir / float(size));

	float avgDistance = 0.0f;

	std::vector<std::tuple<float, glm::vec3, float>> distancePoints_lowerLeft;
	std::vector<std::tuple<float, glm::vec3, float>> distancePoints_lowerRight;
	std::vector<std::tuple<float, glm::vec3, float>> distancePoints_upperLeft;
	std::vector<std::tuple<float, glm::vec3, float>> distancePoints_upperRight;
	std::vector<std::tuple<float, glm::vec3, float>> distancePoints_all;

	bool lowLeftValid = false;
	bool lowRightValid = false;
	bool upperLeftValid = false;
	bool upperRightValid = false;
	// Develop a series of paired distance/point/angleToCenter 
	for (size_t i = 0; i < size; i++) {
		glm::vec3 point = splineData->anchors[i].pos;
		float distance = glm::length(centerPos - point);
		if (distance < 1.0f) { distance = 1.0f; }
		avgDistance += distance;
		glm::vec3 dir = makeDir(point, centerPos);
		float angle = glm::degrees(atan2f(dir.y, dir.x));
		float side = pointLineDeterminant(startPos, centerPos, point);
		float side_perp = pointLineDeterminant(startPos_perp, centerPos, point);
		if (side < 0.0f) {
			if (side_perp < 0.0f) {
				lowLeftValid = true;
				distancePoints_lowerLeft.push_back(std::tuple<float, glm::vec3, float>(distance, point, angle));
			}
			else {
				lowRightValid = true;
				distancePoints_lowerRight.push_back(std::tuple<float, glm::vec3, float>(distance, point, angle));
			}
		}
		else {
			if (side_perp < 0.0f) {
				upperLeftValid = true;
				distancePoints_upperLeft.push_back(std::tuple<float, glm::vec3, float>(distance, point, angle));
			}
			else {
				upperRightValid = true;
				distancePoints_upperRight.push_back(std::tuple<float, glm::vec3, float>(distance, point, angle));
			}
		}
		distancePoints_all.push_back(std::tuple<float, glm::vec3, float>(distance, point, angle));
	}
	avgDistance /= (float)size;

	// Sort the tuples. Returns as Least = index0, Greatest = indexLast
	sortDistanceTuple(distancePoints_lowerLeft); // 0
	sortDistanceTuple(distancePoints_lowerRight); // 1
	sortDistanceTuple(distancePoints_upperRight); // 2
	sortDistanceTuple(distancePoints_upperLeft); // 3

	std::tuple<float, glm::vec3, float> greatestLowerLeft = // 0
		(!lowLeftValid) ? distancePoints_all.front() : distancePoints_lowerLeft.back();
	std::tuple<float, glm::vec3, float> greatestLowerRight = // 1
		(!lowRightValid) ? distancePoints_all.front() : distancePoints_lowerRight.back();

	
	std::tuple<float, glm::vec3, float> greatestUpperRight = // 2
		(!upperRightValid) ? distancePoints_all.front() : distancePoints_upperRight.back();
	std::tuple<float, glm::vec3, float> greatestUpperLeft = // 3
		(!upperLeftValid) ? distancePoints_all.front() : distancePoints_upperLeft.back();

	int greatIndex = -1;
	float greatestDistance = 0.0f;
	glm::vec3 greatestPos = glm::vec3(0.0f);
	for (int i = 0; i < 4; i++) {
		if (i == 0 && lowLeftValid && std::get<0>(greatestLowerLeft) > greatestDistance) {
			greatIndex = 0; 
			greatestDistance = std::get<0>(greatestLowerLeft);
			greatestPos = std::get<1>(greatestLowerLeft);
		} 
		if (i == 1 && lowRightValid && std::get<0>(greatestLowerRight) > greatestDistance) {
			greatIndex = 1; 
			greatestDistance = std::get<0>(greatestLowerRight);
			greatestPos = std::get<1>(greatestLowerRight);
		}
		if (i == 2 && upperRightValid && std::get<0>(greatestUpperRight) > greatestDistance) {
			greatIndex = 2;
			greatestDistance = std::get<0>(greatestUpperRight);
			greatestPos = std::get<1>(greatestUpperRight);
		}
		if (i == 3 && upperLeftValid && std::get<0>(greatestUpperLeft) > greatestDistance) {
			greatIndex = 3; 
			greatestDistance = std::get<0>(greatestUpperLeft);
			greatestPos = std::get<1>(greatestUpperLeft);
		}
	}

	std::vector<std::tuple<float, glm::vec3, float>> distancePoints_opposite;
	bool oppositeInvalid = false;
	glm::vec3 oppositePoint = centerPos;

	switch (greatIndex) 
	{
	case 0:
		if (upperRightValid) { distancePoints_opposite = distancePoints_upperRight; }
		else { oppositeInvalid = true; }
		break;
	case 1:
		if (upperLeftValid) { distancePoints_opposite = distancePoints_upperLeft; }
		else { oppositeInvalid = true; }
		break;
	case 2:
		if (lowLeftValid) { distancePoints_opposite = distancePoints_lowerLeft; }
		else { oppositeInvalid = true; }
		break;
	case 3:
		if (lowRightValid) { distancePoints_opposite = distancePoints_lowerRight; }
		else { oppositeInvalid = true; }
		break;
	}

	if (oppositeInvalid) { confidence = 0.0f; return confidence; }

	for (int i = 0; i < distancePoints_opposite.size(); i++) {
		glm::vec3 point = std::get<1>(distancePoints_opposite[i]);
		float distance = glm::length(point - greatestPos);
		if (distance < 1.0f) { distance = 1.0f; }
		avgDistance += distance;
		glm::vec3 dir = makeDir(greatestPos, point);
		float angle = glm::degrees(atan2f(dir.y, dir.x));
		distancePoints_opposite[i] = std::tuple<float, glm::vec3, float>(distance, point, angle);
	}
	sortDistanceTuple(distancePoints_opposite);
	oppositePoint = std::get<1>(distancePoints_opposite.back());

	glm::vec3 ellipseCenter = (greatestPos + oppositePoint) / 2.0f;
	glm::vec3 majorAxis = makeDir(greatestPos, oppositePoint);
	float majorAxisAngle = atan2f(majorAxis.y, majorAxis.x);
	float majorAxisLength = glm::length(oppositePoint - greatestPos);
	glm::vec3 minorAxis = majorAxis * glm::quat(glm::vec3(0.0f, 0.0f, MATH_PI / 2.0f));
	glm::vec3 minorAxisPointA = ellipseCenter + (minorAxis * (majorAxisLength / 2.0f));
	glm::vec3 minorAxisPointB = ellipseCenter - (minorAxis * (majorAxisLength / 2.0f));
	glm::vec3 majorAxisPointA = ellipseCenter + (majorAxis * (majorAxisLength / 2.0f));
	glm::vec3 majorAxisPointB = ellipseCenter - (majorAxis * (majorAxisLength / 2.0f));

	glm::vec3 axisK = majorAxis * glm::quat(glm::vec3(0.0f, 0.0f, MATH_PI / 4.0f));
	glm::vec3 axisM = majorAxis * glm::quat(glm::vec3(0.0f, 0.0f, -MATH_PI / 4.0f));

	glm::vec3 pointK = ellipseCenter + (axisK * (majorAxisLength / 2.0f));
	glm::vec3 pointM = ellipseCenter + (axisM * (majorAxisLength / 2.0f));

	distancePoints_lowerLeft.clear();
	distancePoints_lowerRight.clear();
	distancePoints_upperLeft.clear();
	distancePoints_upperRight.clear();

	lowLeftValid = false;
	lowRightValid = false;
	upperLeftValid = false;
	upperRightValid = false;
	// Develop a series of paired distance/point/angleToCenter 
	for (size_t i = 0; i < size; i++) {
		glm::vec3 point = splineData->anchors[i].pos;
		float distance = glm::length(ellipseCenter - point);
		if (distance < 1.0f) { distance = 1.0f; }
		glm::vec3 dir = makeDir(point, ellipseCenter);
		float angle = glm::degrees(atan2f(dir.y, dir.x));
		float side = pointLineDeterminant(pointK, ellipseCenter, point);
		float side_perp = pointLineDeterminant(pointM, ellipseCenter, point);
		if (side < 0.0f) {
			if (side_perp < 0.0f) {
				lowLeftValid = true;
				distancePoints_lowerLeft.push_back(std::tuple<float, glm::vec3, float>(distance, point, angle));
			}
			else {
				lowRightValid = true;
				distancePoints_lowerRight.push_back(std::tuple<float, glm::vec3, float>(distance, point, angle));
			}
		}
		else {
			if (side_perp < 0.0f) {
				upperLeftValid = true;
				distancePoints_upperLeft.push_back(std::tuple<float, glm::vec3, float>(distance, point, angle));
			}
			else {
				upperRightValid = true;
				distancePoints_upperRight.push_back(std::tuple<float, glm::vec3, float>(distance, point, angle));
			}
		}
	}

	sortDistanceTuple(distancePoints_lowerLeft); // 0 -- Minor Axis Passthrough
	sortDistanceTuple(distancePoints_lowerRight); // 1
	sortDistanceTuple(distancePoints_upperRight); // 2 -- Minor Axis Passthrough
	sortDistanceTuple(distancePoints_upperLeft); // 3

	std::tuple<float, glm::vec3, float> leastLowerLeft = // 0
		(!lowLeftValid) ? distancePoints_all.front() : distancePoints_lowerLeft.front();
	std::tuple<float, glm::vec3, float> leastLowerRight = // 1
		(!lowRightValid) ? distancePoints_all.front() : distancePoints_lowerRight.front();

	std::tuple<float, glm::vec3, float> leastUpperRight = // 2
		(!upperRightValid) ? distancePoints_all.front() : distancePoints_upperRight.front();
	std::tuple<float, glm::vec3, float> leastUpperLeft = // 3
		(!upperLeftValid) ? distancePoints_all.front() : distancePoints_upperLeft.front();

	float longMinorLength = 0.0f;
	float shortMinorLength = 0.0f;
	float sign = 1.0f;
	glm::vec3 longMinorPoint = glm::vec3(0.0f);
	if (lowLeftValid && std::get<0>(leastLowerLeft) > longMinorLength) { 
		longMinorLength = std::get<0>(leastLowerLeft);
		longMinorPoint = std::get<1>(leastLowerLeft);
		sign = -1.0f;
		if (upperRightValid) { shortMinorLength = std::get<0>(leastUpperRight); }
	}
	if (upperRightValid && std::get<0>(leastUpperRight) > longMinorLength) { 
		longMinorPoint = std::get<1>(leastUpperRight);
		longMinorLength = std::get<0>(leastUpperRight);
		sign = 1.0f;
		if (lowLeftValid) { shortMinorLength = std::get<0>(leastLowerLeft); }
	}
	if (shortMinorLength == 0.0f) { shortMinorLength = longMinorLength; }
	else {
		shortMinorLength = shortMinorLength + ((longMinorLength - shortMinorLength) / 2.0f);
	}
	float minorAxisLength = shortMinorLength + longMinorLength;
	minorAxisPointA = ellipseCenter + (minorAxis * longMinorLength * sign);
	minorAxisPointB = ellipseCenter + (minorAxis * (minorAxisLength - longMinorLength) * -sign);
	ellipseCenter = (minorAxisPointA + minorAxisPointB) / 2.0f;

	majorAxisPointA = ellipseCenter + (majorAxis * (majorAxisLength / 2.0f));
	majorAxisPointB = ellipseCenter - (majorAxis * (majorAxisLength / 2.0f));

	// Render Preview Data
	
	if (preview.at("circle_bounds") == 0) {
		unsigned int boundsLayer = owner->ui->visualizer->requestNewLayer(PreviewLayerType::inputBounds);
		if (owner->ui->visualizer->addLayer(boundsLayer, 0, BlendMode::multiply) == boundsLayer) {
			preview.at("circle_bounds") = boundsLayer;
			owner->ui->visualizer->setPreview(PreviewLayerType::inputBounds, true);
		}
	}
	if (preview.at("circle_bounds") != 0) {
		glm::vec3 p1 = majorAxisPointA + (minorAxis * (minorAxisLength / 2.0f));
		glm::vec3 p2 = majorAxisPointB - (minorAxis * (minorAxisLength / 2.0f));
		owner->ui->visualizer->putBoundsObject(
			preview.at("circle_bounds"), 0, p1, p2, 
			glm::vec2(majorAxisLength, minorAxisLength), 
			makeDir(majorAxisPointA, majorAxisPointB), red);
	}

	if (preview.at("circle_distanceLines") == 0) {
		unsigned int linesLayer = owner->ui->visualizer->requestNewLayer(PreviewLayerType::inputLine);
		if (owner->ui->visualizer->addLayer(linesLayer, 0, BlendMode::multiply) == linesLayer) {
			preview.at("circle_distanceLines") = linesLayer;
			owner->ui->visualizer->setPreview(PreviewLayerType::inputLine, true);
		}
	}
	if (preview.at("circle_distanceLines") != 0) {
		for (int i = 0; i < distancePoints_lowerLeft.size(); i++) {
			glm::vec3 pos = std::get<1>(distancePoints_lowerLeft[i]);
			glm::vec3 dir = makeDir(std::get<1>(distancePoints_lowerLeft[i]), ellipseCenter);
			float t = (std::get<0>(distancePoints_lowerLeft[i]) - std::get<0>(distancePoints_lowerLeft.front())) / 
				(std::get<0>(distancePoints_lowerLeft.back()) - std::get<0>(distancePoints_lowerLeft.front()));
			CColor color = green;
			CColor mix = blue;
			color.mixColor(&mix, t);
			owner->ui->visualizer->addLayerObject(preview.at("circle_distanceLines"),
				PreviewObj(0,
					pos,
					pos + (dir * (std::get<0>(distancePoints_lowerLeft[i]) / 2.2f)),
					color, ShapeType::line, 2.0f));
		}
		for (int i = 0; i < distancePoints_lowerRight.size(); i++) {
			glm::vec3 pos = std::get<1>(distancePoints_lowerRight[i]);
			glm::vec3 dir = makeDir(std::get<1>(distancePoints_lowerRight[i]), ellipseCenter);
			float t = (std::get<0>(distancePoints_lowerRight[i]) - std::get<0>(distancePoints_lowerRight.front())) /
				(std::get<0>(distancePoints_lowerRight.back()) - std::get<0>(distancePoints_lowerRight.front()));
			CColor color = magenta;
			CColor mix = yellow;
			color.mixColor(&mix, t);
			owner->ui->visualizer->addLayerObject(preview.at("circle_distanceLines"),
				PreviewObj(0,
					pos,
					pos + (dir * (std::get<0>(distancePoints_lowerRight[i]) / 2.2f)),
					color, ShapeType::line, 2.0f));
		}
		for (int i = 0; i < distancePoints_upperRight.size(); i++) {
			glm::vec3 pos = std::get<1>(distancePoints_upperRight[i]);
			glm::vec3 dir = makeDir(std::get<1>(distancePoints_upperRight[i]), ellipseCenter);
			float t = (std::get<0>(distancePoints_upperRight[i]) - std::get<0>(distancePoints_upperRight.front())) /
				(std::get<0>(distancePoints_upperRight.back()) - std::get<0>(distancePoints_upperRight.front()));
			CColor color = orange;
			CColor mix = green;
			color.mixColor(&mix, t);
			owner->ui->visualizer->addLayerObject(preview.at("circle_distanceLines"),
				PreviewObj(0,
					pos,
					pos + (dir * (std::get<0>(distancePoints_upperRight[i]) / 2.2f)),
					color, ShapeType::line, 2.0f));
		}
		for (int i = 0; i < distancePoints_upperLeft.size(); i++) {
			glm::vec3 pos = std::get<1>(distancePoints_upperLeft[i]);
			glm::vec3 dir = makeDir(std::get<1>(distancePoints_upperLeft[i]), ellipseCenter);
			float t = (std::get<0>(distancePoints_upperLeft[i]) - std::get<0>(distancePoints_upperLeft.front())) /
				(std::get<0>(distancePoints_upperLeft.back()) - std::get<0>(distancePoints_upperLeft.front()));
			CColor color = red;
			CColor mix = blue;
			color.mixColor(&mix, t);
			owner->ui->visualizer->addLayerObject(preview.at("circle_distanceLines"),
				PreviewObj(0,
					pos,
					pos + (dir * (std::get<0>(distancePoints_upperLeft[i]) / 2.2f)),
					color, ShapeType::line, 2.0f));
		}
	}

	if (preview.at("circle_axisLines") == 0) {
		unsigned int linesLayer = owner->ui->visualizer->requestNewLayer(PreviewLayerType::inputLine);
		if (owner->ui->visualizer->addLayer(linesLayer, 0, BlendMode::multiply) == linesLayer) {
			preview.at("circle_axisLines") = linesLayer;
			owner->ui->visualizer->setPreview(PreviewLayerType::inputLine, true);
		}
	}
	if (preview.at("circle_axisLines") != 0) {
		// axisK / axisM

		owner->ui->visualizer->addLayerObject(preview.at("circle_axisLines"),
			PreviewObj(0,
				ellipseCenter - (axisK * (minorAxisLength / 2.0f)),
				ellipseCenter + (axisK * (minorAxisLength / 2.0f)),
				red, ShapeType::line, 2.0f));
		owner->ui->visualizer->addLayerObject(preview.at("circle_axisLines"),
			PreviewObj(0,
				ellipseCenter - (axisM * (minorAxisLength / 2.0f)),
				ellipseCenter + (axisM * (minorAxisLength / 2.0f)),
				magenta, ShapeType::line, 2.0f));

		// Distance Axis

		owner->ui->visualizer->addLayerObject(preview.at("circle_axisLines"),
			PreviewObj(0,
				greatestPos,
				centerPos,
				green, ShapeType::line, 2.0f));
		owner->ui->visualizer->addLayerObject(preview.at("circle_axisLines"),
			PreviewObj(0,
				majorAxisPointA,
				majorAxisPointB,
				orange, ShapeType::line, 2.0f));
		owner->ui->visualizer->addLayerObject(preview.at("circle_axisLines"),
			PreviewObj(0,
				minorAxisPointA,
				minorAxisPointB,
				blue, ShapeType::line, 2.0f));
	}
	
	return confidence;
}

////////////////////////////////////////////////////////////////
//
// Helper Functions
//
////////////////////////////////////////////////////////////////

float AutoShape::averageVertexSpacing(VertexData* splineData)
{
	size_t size = splineData->anchors.size();
	// Record average spacing as a measurement of screen distance
	if (size < 2) {
		return glm::length(
			glm::vec3(matrix.p * matrix.v * matrix.m * glm::vec4(splineData->anchors.back().pos, 1.0f)) -
			glm::vec3(matrix.p * matrix.v * matrix.m * glm::vec4(splineData->anchors.front().pos, 1.0f)));
	}
	float lenSum = 0.0f;
	for (size_t i = 1; i < size; i++) {
		lenSum += glm::length(
			glm::vec3(matrix.p * matrix.v * matrix.m * glm::vec4(splineData->anchors.at(i).pos, 1.0f)) -
			glm::vec3(matrix.p * matrix.v * matrix.m * glm::vec4(splineData->anchors.at(i - 1).pos, 1.0f)));
	}
	return lenSum / float(size - 1);
}

std::vector<std::tuple<float, glm::vec3, float>> AutoShape::generateScreenPoints(VertexData* splineData)
{
	// Gather each point as a distance to the startPos, screenSpacePos, and original array index
	std::vector<std::tuple<float, glm::vec3, float>> screenPoints;
	for (size_t i = 0; i < splineData->anchors.size(); i++) {
		screenPoints.push_back(
			std::tuple<float, glm::vec3, size_t>(0.0f, glm::vec3(matrix.p * matrix.v * matrix.m * glm::vec4(splineData->anchors.at(i).pos, 1.0f)), (float)i)
		);
		std::get<0>(screenPoints.back()) = glm::length(std::get<1>(screenPoints.front()) - std::get<1>(screenPoints.back()));
		std::get<1>(screenPoints.back()).z = 0.0f;
	}
	return screenPoints;
}


// Return Structure : Least Distance, Median Distance, Average Distance
std::tuple<float, float, float> AutoShape::screenSpaceLengthAttributes(std::vector<std::tuple<float, glm::vec3, float>>* screenPoints)
{
	// Gather the screen length measurements of each segment, forward looking, last point has length 0.0f
	std::vector<std::tuple<float, glm::vec3, float>> lengthPoints = *screenPoints;
	float totalLength = 0.0f;
	for (size_t j = 0; j < lengthPoints.size(); j++) {
		float dist = (j + 1 == lengthPoints.size()) ? 0.0f : glm::length(std::get<0>(lengthPoints[j + 1]) - std::get<0>(lengthPoints[j]));
		if (dist > 0.0f) { totalLength += dist; }
		std::get<0>(lengthPoints[j]) = dist;
	}
	// Sort and then define the measurement of the closureThreshold
	sortDistanceTuple(lengthPoints);
	float leastDistance = std::get<0>(lengthPoints[1]);
	float medianDistance = (lengthPoints.size() <= 3) ? std::get<0>(lengthPoints[1]) : std::get<0>(lengthPoints[lengthPoints.size() / 2]);
	float avgDistance = totalLength / (lengthPoints.size() - 1);
	return std::tuple<float, float, float>(leastDistance, medianDistance, avgDistance);
}

void AutoShape::sortDistanceTuple(std::vector<std::tuple<float, glm::vec3, float>>& vector)
{
	if (vector.size() < 2) { return; }
	int selectLeast = -1;
	float leastValue = float(INT_MAX);
	std::vector<std::tuple<float, glm::vec3, float>> copy = vector;
	std::tuple<float, glm::vec3, float> leastObj = vector[0];
	for (int i = 0; i < vector.size(); i++) {
		leastValue = float(INT_MAX);
		for (int i = 0; i < copy.size(); i++) {
			if (std::get<0>(copy[i]) < leastValue) {
				leastValue = std::get<0>(copy[i]);
				selectLeast = i;
				leastObj = copy[i];
			}
		}
		vector[i] = leastObj;
		copy.erase(copy.begin() + selectLeast);
	}
}

void AutoShape::sortFloatPair(std::vector<std::pair<float, size_t>>& vector)
{
	if (vector.size() < 2) { return; }
	int selectLeast = -1;
	float leastValue = float(INT_MAX);
	std::vector<std::pair<float, size_t>> copy = vector;
	std::pair<float, size_t> leastObj = vector[0];
	for (int i = 0; i < vector.size(); i++) {
		leastValue = float(INT_MAX);
		for (int i = 0; i < copy.size(); i++) {
			if (std::get<0>(copy[i]) < leastValue) {
				leastValue = std::get<0>(copy[i]);
				selectLeast = i;
				leastObj = copy[i];
			}
		}
		vector[i] = leastObj;
		copy.erase(copy.begin() + selectLeast);
	}
}

float AutoShape::nextSegmentAngle(VertexData* splineData, size_t index)
{
	float nextAngle = 0.0f;
	if (index + 1 < splineData->anchors.size()) {
		nextAngle = glm::degrees(atan2f(splineData->anchors[index + 1].dir.y, splineData->anchors[index + 1].dir.x));
	}
	else {
		nextAngle = glm::degrees(atan2f(splineData->anchors.front().dir.y, splineData->anchors.front().dir.x));
	}
	return nextAngle;
}

void AutoShape::smoothIterate(VertexData& outData, VertexData* splineData, float breakThreshold)
{
	size_t vertCount = splineData->anchors.size();

	glm::vec3 totalSegmentDir = splineData->anchors.front().dir;
	glm::vec3 avgSegmentDir = splineData->anchors.front().dir;
	float avgSegmentAngle = glm::degrees(atan2f(avgSegmentDir.y, avgSegmentDir.x));
	int runningSegCount = 1;

	std::vector<size_t> clearIndices;

	for (size_t i = 0; i < vertCount; i++) {

		float peekAngle = nextSegmentAngle(splineData, i);

		if (compareAngle(peekAngle, avgSegmentAngle, breakThreshold)) {
			if (i + 1 == vertCount) { break; }
			totalSegmentDir = avgSegmentDir = splineData->anchors.at(i + 1).dir;
			avgSegmentAngle = glm::degrees(atan2f(avgSegmentDir.y, avgSegmentDir.x));
			runningSegCount = 1;
		}
		else {
			if (i + 1 < vertCount - 1) { clearIndices.push_back(i + 1); }
			glm::vec3 dir = splineData->anchors.at(i).dir;
			runningSegCount++;
			totalSegmentDir += dir;
			avgSegmentDir = glm::normalize(totalSegmentDir / (float)runningSegCount);
			avgSegmentAngle = glm::degrees(atan2f(avgSegmentDir.y, avgSegmentDir.x));
		}
	}

	outData = *splineData;
	for (int i = 0; i < clearIndices.size(); i++) {
		outData.anchors.erase(outData.anchors.begin() + (clearIndices[i] - i));
	}
	outData.anchors.shrink_to_fit();

	vertCount = outData.anchors.size();
	for (size_t i = 0; i < vertCount; i++) {
		glm::vec3 pos = outData.anchors.at(i).pos;
		glm::vec3 dir = outData.anchors.at(i).dir;
		if (i + 1 < vertCount) {
			dir = makeDir(pos, outData.anchors.at(i + 1).pos);
		}
		else { dir = makeDir(outData.anchors.back().pos, outData.anchors.front().pos); }
		outData.anchors.at(i).dir = dir;
	}
}

float AutoShape::generateAverageAttribs(VertexData* splineData, bool countLast)
{
	if (splineData->anchors.size() == 0) { return 0.0f; }
	resetAvgAttribs();
	size_t count = (countLast) ? 0 : 0;
	size_t vertCount = splineData->anchors.size() - 1;
	for (size_t i = 0; i < vertCount; i++) {
		glm::vec3 dirA, dirB;
		if (i == 0) {
			dirA = splineData->anchors.at(vertCount - 1).dir;
			dirB = splineData->anchors.at(0).dir;
		}
		else if (i == vertCount) {
			dirA = splineData->anchors.at(i - 1).dir;
			dirB = splineData->anchors.front().dir;
		}
		else {
			dirA = splineData->anchors.at(i - 1).dir;
			dirB = splineData->anchors.at(i).dir;
		}
		float angleA = glm::degrees(atan2f(dirA.y, dirA.x));
		float angleB = glm::degrees(atan2f(dirB.y, dirB.x));
		if (!compareParallel(angleA, angleB, 15.0f)) {
			float dif = angleDifference(angleA, angleB);
			totalAngle += dif;
			angles.push_back(std::pair<float, size_t>(dif, i));
			count++;
			//
			glm::vec3 posA = splineData->anchors.at(i).pos;
			glm::vec3 posB = splineData->anchors.at(i + 1).pos;
			float len = glm::length(posB - posA);
			totalLength += len;
			lengths.push_back(std::pair<float, size_t>(len, i));
			//
			posA = matrix.p * matrix.v * matrix.m * glm::vec4(posA, 1.0f);
			posB = matrix.p * matrix.v * matrix.m * glm::vec4(posB, 1.0f);
			posA.z = posB.z = 0.0f;
			len = glm::length(posB - posA);
			totalLength_uv += len;
			lengths_uv.push_back(std::pair<float, size_t>(len, i));
		}
	}
	if (angles.size() == 0 || lengths.size() == 1) {
		return 0.0f;
	}
	sortFloatPair(angles);
	avgAngle = totalAngle / float(count);

	sortFloatPair(lengths);
	sortFloatPair(lengths_uv);
	avgLength = totalLength / float(count);
	avgLength_uv = totalLength_uv / float(count);

	leastAngle = angles.front().first;
	greatestAngle = angles.back().first;
	medianAngle = angles.at(angles.size() / 2).first;

	leastLength_uv = lengths_uv.front().first;
	greatestLength_uv = lengths_uv.back().first;
	medianLength_uv = lengths_uv.at(lengths_uv.size() / 2).first;
	leastLength = lengths.front().first;
	greatestLength = lengths.back().first;
	medianLength = lengths.at(lengths.size() / 2).first;
	return 1.0f;
}

glm::vec3 AutoShape::averagePosition_byCorner(VertexData* splineData, bool filter)
{
	glm::vec3 totalPos = glm::vec3(0.0f);
	size_t sideCount = 0;
	for (size_t i = 0; i < splineData->anchors.size() - 1; i++) {
		if (filter && avgDataValid) {
			if (glm::length(splineData->anchors[i].pos - splineData->anchors[i + 1].pos) > avgLength / 3.0f) {
				totalPos += splineData->anchors[i].pos;
				sideCount++;
			}
		}
		else {
			totalPos += splineData->anchors[i].pos;
			sideCount++;
		}
	}
	return totalPos / float(sideCount);
}
glm::vec3 AutoShape::averagePosition_bySides(VertexData* splineData, bool filter)
{
	glm::vec3 totalPos = glm::vec3(0.0f);
	size_t sideCount = 0;
	for (size_t i = 0; i < splineData->anchors.size() - 1; i++) {
		float len = glm::length(splineData->anchors[i].pos - splineData->anchors[i + 1].pos);
		glm::vec3 pos = (splineData->anchors[i].pos + splineData->anchors[i + 1].pos) / 2.0f;
		if (filter) {
			if (len > avgLength / 3.0f) {
				totalPos += pos;
				sideCount++;
			}
		}
		else { totalPos += pos; sideCount++; }
	}
	return totalPos / float(sideCount);
}