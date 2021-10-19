#ifndef TSET_FIELD_H
#define TSET_FIELD_H

class CFunction;
class CObject;
class CInterpreter;
class Tool;

#include <glm.hpp>
#include <string>
#include <vector>
#include <memory>
#include <map>

class ToolSettings;

#include "../../ToolSettings.h"

////////////////////////////////////////////////////////////////
// Field Settings
////////////////////////////////////////////////////////////////

class TSet_Field : public ToolSettings // 6000
{
public:
	// Distribution
	// Random // Pattern
	TSetProp distributionType; // 6000
	// Distribution N-Sides
	int distributionShape_sides; // 6001
	bool equilateralShape; // 6002
	// Seed Integer - Based On Time
	bool constantSeed; // 6003
	int savedSeed; // 6004

	// Random Variables
	// Uniform // Normal // Exponential // Double Exponent // Beta // Tukey-Lambda
	TSetProp random_type; // 6010
	// -1 to 1 value that pushes final positions towards or away from the center
	bool scatterFromCenter; // 6011
	float random_positionXBias; // 6012
	float random_positionYBias; // 6013
	// Density Settings
	bool random_relativeDensity; // 6014
	float random_densityMin; // 6015
	float random_densityMax; // 6016
	float random_densityBias; // 6017

	// Pattern Variables
	// Vertex // Shape
	TSetProp pattern_type; // 6020
	int patternShape_sides; // 6021
	float patternShape_radius; // 6022
	// Spacing
	bool patternRow_spacingConstant; // 6023
	int patternRow_spacingCount; // 6024
	float patternRow_spacingAmount; // 6025
	float patternRow_spacingNonLinearFactor; // 6026
	float patternRow_offsetPerRow; // 6027
	bool patternColumn_spacingConstant; // 6028
	int patternColumn_spacingCount; // 6029
	float patternColumn_spacingAmount; // 6030
	float patternColumn_spacingNonLinearFactor; // 6031
	float patternColumn_offsetPerColumn; // 6032
	// Randomization
	float pattern_randOffsetX; // 6033
	float pattern_randOffsetY; // 6034

	// Second Factor Position Randomization
	float secondaryScatteringX; // 6041
	float secondaryScatteringY; // 6042

	// Randomizing the Pen Per Vertex
	bool randomizePressure; // 6050
	float pressureRandMin; // 6051
	float pressureRandMax; // 6052
	float pressureXPosInfluence; // 6053
	float pressureYPosInfluence; // 6054
	bool randomizeTilt; // 6055
	float tiltRandMin; // 6056
	float tiltRandMax; // 6057
	float tiltXPosInfluence; // 6058
	float tiltYPosInfluence; // 6059
	bool randomizeRotation; // 6060
	float rotationRandMin; // 6061
	float rotationRandMax; // 6062
	float rotationXPosInfluence; // 6063
	float rotationYPosInfluence; // 6064
	bool randomizeVelocity; // 6065
	float velocityRandMin; // 6066
	float velocityRandMax; // 6067
	float velocityXPosInfluence; // 6068
	float velocityYPosInfluence; // 6069


	// Connection Variables
	// Use Given Order // Shuffle Order
	bool connectVertices; // 6080
	TSetProp connectionOrdering; // 6081
	bool maskByDistance; // 6082
	float connectionMaskDistance; // 6083
	bool maskByCount; // 6084
	float maxConnectionCount; // 6085
	float minConnectionCount; // 6086
	bool connectCountEnds; // 6087

	virtual TSetType getType() { return type; }

	virtual TSetControl_Node* getControlNode(int settingSig, int subSig);
	virtual TSetController* getController(int settingSig, int subSig);
	virtual TSetGraph* getGraph(int settingSig, int subSig);
	virtual TSetNoise* getNoise(int settingSig, int subSig);

	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	TSet_Field();
};

#endif