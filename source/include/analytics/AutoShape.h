#ifndef AUTOSHAPE_H
#define AUTOSHAPE_H

#include "AutoShapeType.h"
#include "../entities/visuals/VertexData.h"
#include "../math/transform.h"

class Application;

#include <glm.hpp>
#include <memory>
#include <string>
#include <map>

struct Auto_Shape {
	bool isSolved = false;
	float confidence = 0.0f;
	EntityTransform transform = EntityTransform();
	virtual void reset() = 0;
	virtual void center() = 0;
	virtual void setDir() = 0;
};

struct Auto_Point {
	Auto_Point() {};
	std::pair<size_t, size_t> mappedIndices = std::pair<size_t, size_t>(0, 0);
	FragmentAnchor vertex = FragmentAnchor();
	bool isActive = false;
};

// Unify All Shapes Under One Type w/ Enum Class for Shape Type/Name

struct Auto_Line : public Auto_Shape {
	Auto_Line() {};
	std::vector<Auto_Point> points{ Auto_Point(), Auto_Point() };
	virtual void reset() {
		this->isSolved = false;
		this->confidence = 0.0f;
	};
	virtual void center() {
		glm::vec3 totalPos = glm::vec3(0.0f);
		for (int i = 0; i < points.size(); i++) {
			totalPos += points.at(i).vertex.pos;
		}
		this->transform.origin = totalPos / (float)points.size();
	}
	virtual void setDir() {
		for (int i = 0; i < points.size(); i++) {
			if (i == points.size() - 1) {
				points.at(i).vertex.dir = makeDir(points.at(i).vertex.pos, points.at(0).vertex.pos);
			}
			else {
				points.at(i).vertex.dir = makeDir(points.at(i).vertex.pos, points.at(i + 1).vertex.pos);
			}
		}
	}
};

struct Auto_PolyLine : public Auto_Shape {
	Auto_PolyLine() {};
	std::vector<Auto_Point> points;
	virtual void reset() {
		this->isSolved = false;
		this->confidence = 0.0f;
		this->points.clear();
	};
	virtual void center() {
		glm::vec3 totalPos = glm::vec3(0.0f);
		for (int i = 0; i < points.size(); i++) {
			totalPos += points.at(i).vertex.pos;
		}
		this->transform.origin = totalPos / (float)points.size();
	}
	virtual void setDir() {
		for (int i = 0; i < points.size(); i++) {
			if (i == points.size() - 1) {
				points.at(i).vertex.dir = makeDir(points.at(i).vertex.pos, points.at(0).vertex.pos);
			}
			else {
				points.at(i).vertex.dir = makeDir(points.at(i).vertex.pos, points.at(i + 1).vertex.pos);
			}
		}
	}
};

struct Auto_Circle : public Auto_Shape {
	Auto_Circle() {};
	std::vector<Auto_Point> points{ Auto_Point(), Auto_Point(), Auto_Point(), Auto_Point() };
	virtual void reset() {
		this->isSolved = false;
		this->confidence = 0.0f;
	};
	virtual void center() {
		glm::vec3 totalPos = glm::vec3(0.0f);
		for (int i = 0; i < points.size(); i++) {
			totalPos += points.at(i).vertex.pos;
		}
		this->transform.origin = totalPos / (float)points.size();
	}
	virtual void setDir() {
		for (int i = 0; i < points.size(); i++) {
			if (i == points.size() - 1) {
				points.at(i).vertex.dir = makeDir(points.at(i).vertex.pos, points.at(0).vertex.pos);
			}
			else {
				points.at(i).vertex.dir = makeDir(points.at(i).vertex.pos, points.at(i + 1).vertex.pos);
			}
		}
	}
};

struct Auto_Ellipse : public Auto_Shape {
	Auto_Ellipse() {};
	std::vector<Auto_Point> points{ Auto_Point(), Auto_Point(), Auto_Point(), Auto_Point() };
	virtual void reset() {
		this->isSolved = false;
		this->confidence = 0.0f;
	};
	virtual void center() {
		glm::vec3 totalPos = glm::vec3(0.0f);
		for (int i = 0; i < points.size(); i++) {
			totalPos += points.at(i).vertex.pos;
		}
		this->transform.origin = totalPos / (float)points.size();
	}
	virtual void setDir() {
		for (int i = 0; i < points.size(); i++) {
			if (i == points.size() - 1) {
				points.at(i).vertex.dir = makeDir(points.at(i).vertex.pos, points.at(0).vertex.pos);
			}
			else {
				points.at(i).vertex.dir = makeDir(points.at(i).vertex.pos, points.at(i + 1).vertex.pos);
			}
		}
	}
};

struct Auto_Triangle : public Auto_Shape {
	Auto_Triangle() {};
	std::vector<Auto_Point> points{ Auto_Point(), Auto_Point(), Auto_Point(), };
	virtual void reset() {
		this->isSolved = false;
		this->confidence = 0.0f;
	};
	virtual void center() {
		glm::vec3 totalPos = glm::vec3(0.0f);
		for (int i = 0; i < points.size(); i++) {
			totalPos += points.at(i).vertex.pos;
		}
		this->transform.origin = totalPos / (float)points.size();
	}
	virtual void setDir() {
		for (int i = 0; i < points.size(); i++) {
			if (i == points.size() - 1) {
				points.at(i).vertex.dir = makeDir(points.at(i).vertex.pos, points.at(0).vertex.pos);
			}
			else {
				points.at(i).vertex.dir = makeDir(points.at(i).vertex.pos, points.at(i + 1).vertex.pos);
			}
		}
	}
};

struct Auto_EquillateralTriangle : public Auto_Shape {
	Auto_EquillateralTriangle() {};
	std::vector<Auto_Point> points{ Auto_Point(), Auto_Point(), Auto_Point() };
	virtual void reset() {
		this->isSolved = false;
		this->confidence = 0.0f;
	};
	virtual void center() {
		glm::vec3 totalPos = glm::vec3(0.0f);
		for (int i = 0; i < points.size(); i++) {
			totalPos += points.at(i).vertex.pos;
		}
		this->transform.origin = totalPos / (float)points.size();
	}
	virtual void setDir() {
		for (int i = 0; i < points.size(); i++) {
			if (i == points.size() - 1) {
				points.at(i).vertex.dir = makeDir(points.at(i).vertex.pos, points.at(0).vertex.pos);
			}
			else {
				points.at(i).vertex.dir = makeDir(points.at(i).vertex.pos, points.at(i + 1).vertex.pos);
			}
		}
	}
};

struct Auto_Rectangle : public Auto_Shape {
	Auto_Rectangle() {}; 
	std::vector<Auto_Point> points{ Auto_Point(), Auto_Point(), Auto_Point(), Auto_Point() };
	virtual void reset() {
		this->isSolved = false;
		this->confidence = 0.0f;
	};
	virtual void center() {
		glm::vec3 totalPos = glm::vec3(0.0f);
		for (int i = 0; i < points.size(); i++) {
			totalPos += points.at(i).vertex.pos;
		}
		this->transform.origin = totalPos / (float)points.size();
	}
	virtual void setDir() {
		for (int i = 0; i < points.size(); i++) {
			if (i == points.size() - 1) {
				points.at(i).vertex.dir = makeDir(points.at(i).vertex.pos, points.at(0).vertex.pos);
			}
			else {
				points.at(i).vertex.dir = makeDir(points.at(i).vertex.pos, points.at(i + 1).vertex.pos);
			}
		}
	}
};

struct Auto_Square : public Auto_Shape {
	Auto_Square() {};
	std::vector<Auto_Point> points{ Auto_Point(), Auto_Point(), Auto_Point(), Auto_Point() };
	virtual void reset() {
		this->isSolved = false;
		this->confidence = 0.0f;
	};
	virtual void center() {
		glm::vec3 totalPos = glm::vec3(0.0f);
		for (int i = 0; i < points.size(); i++) {
			totalPos += points.at(i).vertex.pos;
		}
		this->transform.origin = totalPos / (float)points.size();
	}
	virtual void setDir() {
		for (int i = 0; i < points.size(); i++) {
			if (i == points.size() - 1) {
				points.at(i).vertex.dir = makeDir(points.at(i).vertex.pos, points.at(0).vertex.pos);
			}
			else {
				points.at(i).vertex.dir = makeDir(points.at(i).vertex.pos, points.at(i + 1).vertex.pos);
			}
		}
	}
};

struct Auto_Pentagon : public Auto_Shape {
	Auto_Pentagon() {};
	std::vector<Auto_Point> points{ Auto_Point(), Auto_Point(), Auto_Point(), Auto_Point(), Auto_Point() };
	virtual void reset() {
		this->isSolved = false;
		this->confidence = 0.0f;
	};
	virtual void center() {
		glm::vec3 totalPos = glm::vec3(0.0f);
		for (int i = 0; i < points.size(); i++) {
			totalPos += points.at(i).vertex.pos;
		}
		this->transform.origin = totalPos / (float)points.size();
	}
	virtual void setDir() {
		for (int i = 0; i < points.size(); i++) {
			if (i == points.size() - 1) {
				points.at(i).vertex.dir = makeDir(points.at(i).vertex.pos, points.at(0).vertex.pos);
			}
			else {
				points.at(i).vertex.dir = makeDir(points.at(i).vertex.pos, points.at(i + 1).vertex.pos);
			}
		}
	}
};

struct Auto_Hexagon : public Auto_Shape {
	Auto_Hexagon() {};
	std::vector<Auto_Point> points{ 
		Auto_Point(), Auto_Point(), Auto_Point(), 
		Auto_Point(), Auto_Point(), Auto_Point() };
	virtual void reset() {
		this->isSolved = false;
		this->confidence = 0.0f;
	};
	virtual void center() {
		glm::vec3 totalPos = glm::vec3(0.0f);
		for (int i = 0; i < points.size(); i++) {
			totalPos += points.at(i).vertex.pos;
		}
		this->transform.origin = totalPos / (float)points.size();
	}
	virtual void setDir() {
		for (int i = 0; i < points.size(); i++) {
			if (i == points.size() - 1) {
				points.at(i).vertex.dir = makeDir(points.at(i).vertex.pos, points.at(0).vertex.pos);
			}
			else {
				points.at(i).vertex.dir = makeDir(points.at(i).vertex.pos, points.at(i + 1).vertex.pos);
			}
		}
	}
};

struct Auto_Septagon : public Auto_Shape {
	Auto_Septagon() {};
	std::vector<Auto_Point> points{ 
		Auto_Point(), Auto_Point(), Auto_Point(),
		Auto_Point(), Auto_Point(), Auto_Point(),
		Auto_Point() };
	virtual void reset() {
		this->isSolved = false;
		this->confidence = 0.0f;
	};
	virtual void center() {
		glm::vec3 totalPos = glm::vec3(0.0f);
		for (int i = 0; i < points.size(); i++) {
			totalPos += points.at(i).vertex.pos;
		}
		this->transform.origin = totalPos / (float)points.size();
	}
	virtual void setDir() {
		for (int i = 0; i < points.size(); i++) {
			if (i == points.size() - 1) {
				points.at(i).vertex.dir = makeDir(points.at(i).vertex.pos, points.at(0).vertex.pos);
			}
			else {
				points.at(i).vertex.dir = makeDir(points.at(i).vertex.pos, points.at(i + 1).vertex.pos);
			}
		}
	}
};


struct Auto_Octagon : public Auto_Shape {
	Auto_Octagon() {};
	std::vector<Auto_Point> points{
		Auto_Point(), Auto_Point(), Auto_Point(),
		Auto_Point(), Auto_Point(), Auto_Point(),
		Auto_Point(), Auto_Point() };
	virtual void reset() {
		this->isSolved = false;
		this->confidence = 0.0f;
	};
	virtual void center() {
		glm::vec3 totalPos = glm::vec3(0.0f);
		for (int i = 0; i < points.size(); i++) {
			totalPos += points.at(i).vertex.pos;
		}
		this->transform.origin = totalPos / (float)points.size();
	}
	virtual void setDir() {
		for (int i = 0; i < points.size(); i++) {
			if (i == points.size() - 1) {
				points.at(i).vertex.dir = makeDir(points.at(i).vertex.pos, points.at(0).vertex.pos);
			}
			else {
				points.at(i).vertex.dir = makeDir(points.at(i).vertex.pos, points.at(i + 1).vertex.pos);
			}
		}
	}
};

struct Auto_Nonagon : public Auto_Shape {
	Auto_Nonagon() {};
	std::vector<Auto_Point> points{
		Auto_Point(), Auto_Point(), Auto_Point(),
		Auto_Point(), Auto_Point(), Auto_Point(),
		Auto_Point(), Auto_Point(), Auto_Point() };
	virtual void reset() {
		this->isSolved = false;
		this->confidence = 0.0f;
	};
	virtual void center() {
		glm::vec3 totalPos = glm::vec3(0.0f);
		for (int i = 0; i < points.size(); i++) {
			totalPos += points.at(i).vertex.pos;
		}
		this->transform.origin = totalPos / (float)points.size();
	}
	virtual void setDir() {
		for (int i = 0; i < points.size(); i++) {
			if (i == points.size() - 1) {
				points.at(i).vertex.dir = makeDir(points.at(i).vertex.pos, points.at(0).vertex.pos);
			}
			else {
				points.at(i).vertex.dir = makeDir(points.at(i).vertex.pos, points.at(i + 1).vertex.pos);
			}
		}
	}
};

struct Auto_Decagon : public Auto_Shape {
	Auto_Decagon() {};
	std::vector<Auto_Point> points{
		Auto_Point(), Auto_Point(), Auto_Point(),
		Auto_Point(), Auto_Point(), Auto_Point(),
		Auto_Point(), Auto_Point(), Auto_Point(), Auto_Point() };
	virtual void reset() {
		this->isSolved = false;
		this->confidence = 0.0f;
	};
	virtual void center() {
		glm::vec3 totalPos = glm::vec3(0.0f);
		for (int i = 0; i < points.size(); i++) {
			totalPos += points.at(i).vertex.pos;
		}
		this->transform.origin = totalPos / (float)points.size();
	}
	virtual void setDir() {
		for (int i = 0; i < points.size(); i++) {
			if (i == points.size() - 1) {
				points.at(i).vertex.dir = makeDir(points.at(i).vertex.pos, points.at(0).vertex.pos);
			}
			else {
				points.at(i).vertex.dir = makeDir(points.at(i).vertex.pos, points.at(i + 1).vertex.pos);
			}
		}
	}
};

class AutoShape
{
private:
	std::shared_ptr<Application> owner;
	ShaderTransform matrix = ShaderTransform();
	// Stored Shapes
	bool shapesValid = false;
	Auto_Line line = Auto_Line();
	Auto_PolyLine polyLine = Auto_PolyLine();
	Auto_Circle circle = Auto_Circle();
	Auto_Ellipse ellipse = Auto_Ellipse();
	Auto_Triangle triangle = Auto_Triangle();
	Auto_Triangle rightTriangle = Auto_Triangle();
	Auto_EquillateralTriangle equilateralTriangle = Auto_EquillateralTriangle();
	Auto_Rectangle rectangle = Auto_Rectangle();
	Auto_Square square = Auto_Square();
	Auto_Pentagon pentagon = Auto_Pentagon();
	Auto_Hexagon hexagon = Auto_Hexagon();
	Auto_Septagon septagon = Auto_Septagon();
	Auto_Octagon octagon = Auto_Octagon();
	Auto_Nonagon nonagon = Auto_Nonagon();
	Auto_Decagon decagon = Auto_Decagon();

	std::map<std::string, unsigned int> preview;

	// Stored Data
	bool avgDataValid = 0.0f;
	float totalLength = 0.0f;
	float avgLength = 0.0f;
	float leastLength = 0.0f;
	float greatestLength = 0.0f;
	float medianLength = 0.0f;
	float totalLength_uv = 0.0f;
	float avgLength_uv = 0.0f;
	float leastLength_uv = 0.0f;
	float greatestLength_uv = 0.0f;
	float medianLength_uv = 0.0f;

	float totalAngle = 0.0f;
	float avgAngle = 0.0f;
	float leastAngle = 0.0f;
	float greatestAngle = 0.0f;
	float medianAngle = 0.0f;

	std::vector<std::pair<float, size_t>> angles;
	std::vector<std::pair<float, size_t>> lengths;
	std::vector<std::pair<float, size_t>> lengths_uv;

protected:
public:
	AutoShape();
	void initialize(std::shared_ptr<Application> owner);
	//
	AutoShapeType visualizeData(VertexData* splineData);
	void visualizeShape(VertexData* rawShape, VertexData* regularShape, bool renderRaw, bool renderRegular);
	//
	void clearVisuals();
	void resetData();
	void resetAvgAttribs();
	//
	AutoShapeType analyzeData(VertexData& outData,
		glm::ivec2 canvasDimensions, float zoomAmount, VertexData* splineData);
	//
	float constructSegments(VertexData& outData, float minSegLen_uv, float backFactor, float forwardFactor,
		float backAngleThreshold, float forwardAngleThreshold, bool doRender,
		glm::ivec2 canvasDimensions, float zoomAmount, VertexData* splineData);
	float smoothSegments(VertexData& outData, float minSegLen_uv, float breakThreshold,
		VertexData* splineData);
	float closeShape(VertexData& outData, float minSegLen_uv, float backFactor, float forwardFactor,
		float backAngleThreshold, float forwardAngleThreshold, bool doRender,
		glm::ivec2 canvasDimensions, float zoomAmount, VertexData* splineData,
		std::vector<std::tuple<float, glm::vec3, float>>& screenPoints);
	float removeOverlap(VertexData& outData, float minSegLen_uv, float backFactor, float forwardFactor,
		float backAngleThreshold, float forwardAngleThreshold, bool doRender,
		glm::ivec2 canvasDimensions, float zoomAmount, VertexData* splineData,
		std::vector<std::tuple<float, glm::vec3, float>>& screenPoints);
	float cleanShape(VertexData& outData, float minSegLen_uv, float backFactor, float forwardFactor,
		float backAngleThreshold, float forwardAngleThreshold, bool doRender,
		glm::ivec2 canvasDimensions, float zoomAmount, VertexData* splineData,
		std::vector<std::tuple<float, glm::vec3, float>>& screenPoints);
	AutoShapeType analyzeShape(VertexData& outData, VertexData* splineData,
		std::vector<std::tuple<float, glm::vec3, float>>& screenPoints);
	//
	float detectLine(VertexData& outData, bool doRender,
		glm::ivec2 canvasDimensions, float zoomAmount, VertexData* splineData);
	float detectTriangles(VertexData* splineData,
		std::vector<std::pair<float, size_t>>* lengthsSorted,
		std::vector<std::pair<float, size_t>>* anglesSorted,
		std::vector<std::tuple<float, glm::vec3, float>>* screenPoints);
	float detectRectangle(VertexData* splineData, 
		std::vector<std::pair<float, size_t>>* lengthsSorted,
		std::vector<std::pair<float, size_t>>* anglesSorted,
		std::vector<std::tuple<float, glm::vec3, float>>* screenPoints);
	float detectPolygons(VertexData* splineData,
		std::vector<std::pair<float, size_t>>* lengthsSorted,
		std::vector<std::pair<float, size_t>>* anglesSorted,
		std::vector<std::tuple<float, glm::vec3, float>>* screenPoints);
	float detectCircle(VertexData& outData, bool doRender,
		glm::ivec2 canvasDimensions, float zoomAmount, VertexData* splineData);
	//
	float averageVertexSpacing(VertexData* splineData);
	std::vector<std::tuple<float, glm::vec3, float>> generateScreenPoints(VertexData* splineData);
	std::tuple<float, float, float> screenSpaceLengthAttributes(std::vector<std::tuple<float, glm::vec3, float>>* screenPoints);
	std::tuple<float, glm::vec3, float> screenSpaceLengthAttributes(VertexData* splineData);
	float generateAverageAttribs(VertexData* splineData, bool countLast);
	glm::vec3 averagePosition_byCorner(VertexData* splineData, bool filter);
	glm::vec3 averagePosition_bySides(VertexData* splineData, bool filter);
	//
	void sortDistanceTuple(std::vector<std::tuple<float, glm::vec3, float>>& vector);
	void sortFloatPair(std::vector<std::pair<float, size_t>>& vector);
	float nextSegmentAngle(VertexData* splineData, size_t index);
	void smoothIterate(VertexData& outData, VertexData* splineData, float breakThreshold);
};

#endif