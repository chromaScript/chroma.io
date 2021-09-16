#include "../../include/ToolSettings.h"
#include "../../include/keys.h"
#include "../../include/Color.h"

class CFunction;
#include "../../include/cscript/CInterpreter.h"
#include "../../include/cscript/CObject.h"
#include "../../include/Tool.h"

#include <glm.hpp>
#include <gtx/rotate_vector.hpp>
#include <string>
#include <vector>
#include <memory>
#include <random>

#include "../../include/structs.h"
#include "../../include/toolSettings/method/TSet_Field.h"

////////////////////////////////////////////////////////////////
// Field Settings
////////////////////////////////////////////////////////////////

TSet_Field::TSet_Field()
{
	this->type = TSetType::field;
	this->isEnabled = false;
	// Distribution
	// Random // Pattern
	this->distributionType = TSetProp::random; // 6000
	// Distribution N-Sides
	this->distributionShape_sides = 4; // 6001
	this->equilateralShape = false; // 6002
	// Seed Integer - Based On Time
	this->constantSeed = false; // 6003
	this->savedSeed = 0; // 6004

	// Random Variables
	// Uniform // Normal // Exponential // Double Exponent // Beta // Tukey-Lambda
	this->random_type = TSetProp::uniform; // 6010
	// -1 to 1 value that pushes final positions towards or away from the center
	this->scatterFromCenter = 0.0f; // 6011
	this->random_positionXBias = 0.0f; // 6012
	this->random_positionYBias = 0.0f; // 6013
	// Density Settings
	this->random_relativeDensity; // 6014
	this->random_densityMin = 0.0f; // 6015
	this->random_densityMax = 0.0f; // 6016
	this->random_densityBias = 0.0f; // 6017

	// Pattern Variables
	// Vertex // Shape
	this->pattern_type; // 6020
	this->patternShape_sides = 1; // 6021
	this->patternShape_radius = 5.0f; // 6022
	// Spacing
	this->patternRow_spacingConstant = true; // 6023
	this->patternRow_spacingCount = 6; // 6024
	this->patternRow_spacingAmount = 0.0f; // 6025
	this->patternRow_spacingNonLinearFactor = 0.0f; // 6026
	this->patternRow_offsetPerRow = 0.0f; // 6027
	this->patternColumn_spacingConstant = true; // 6028
	this->patternColumn_spacingCount = 6; // 6029
	this->patternColumn_spacingAmount = 0.0f; // 6030
	this->patternColumn_spacingNonLinearFactor = 0.0f; // 6031
	this->patternColumn_offsetPerColumn = 0.0f; // 6032
	// Randomization
	this->pattern_randOffsetX = 0.0f; // 6033
	this->pattern_randOffsetY = 0.0f; // 6034

	// Second Factor Position Randomization
	this->secondaryScatteringX = 0.0f; // 6041
	this->secondaryScatteringY = 0.0f; // 6042

	// Randomizing the Pen Per Vertex
	this->randomizePressure = true; // 6050
	this->pressureRandMin = 0.0f; // 6051
	this->pressureRandMax = 0.0f; // 6052
	this->pressureXPosInfluence = 0.0f; // 6053
	this->pressureYPosInfluence = 1.0f; // 6054
	this->randomizeTilt = true; // 6055
	this->tiltRandMin = 0.0f; // 6056
	this->tiltRandMax = 0.0f; // 6057
	this->tiltXPosInfluence = 0.0f; // 6058
	this->tiltYPosInfluence = 0.0f; // 6059
	this->randomizeRotation = true; // 6060
	this->rotationRandMin = 0.0f; // 6061
	this->rotationRandMax = 0.0f; // 6062
	this->rotationXPosInfluence = 0.0f; // 6063
	this->rotationYPosInfluence = 0.0f; // 6064
	this->randomizeVelocity = true; // 6065
	this->velocityRandMin = 0.0f; // 6066
	this->velocityRandMax = 0.0f; // 6067
	this->velocityXPosInfluence = 0.0f; // 6068
	this->velocityYPosInfluence = 0.0f; // 6069

	// Connection Variables
	// Use Given Order // Shuffle Order
	this->connectVertices = true; // 6080
	this->connectionOrdering = TSetProp::useDefault; // 6081
	this->maskByDistance = true; // 6082
	this->connectionMaskDistance = 10.0f; // 6083
	this->maskByCount = true; // 6084
	this->maxConnectionCount = 5; // 6085
	this->minConnectionCount = 8; // 6086
	this->connectCountEnds = true; // 6087
}

std::shared_ptr<CObject> TSet_Field::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}