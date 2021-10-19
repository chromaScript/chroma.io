#ifndef TSET_RAKE_H
#define TSET_RAKE_H

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
// Rake Settings
////////////////////////////////////////////////////////////////

#define TSET_RAKE_SIZE_MIN 0.01f
#define TSET_RAKE_SIZE_MAX 500.0f
#define TSET_RAKE_ROTATION_MIN -180.0f
#define TSET_RAKE_ROTATION_MAX 180.0f
#define TSET_RAKE_POINTS_MIN 1
#define TSET_RAKE_POINTS_MAX 16
#define TSET_RAKE_OFFSETPOS_MIN -500.0f
#define TSET_RAKE_OFFSETPOS_MAX 500.0f

class TSet_Rake : public ToolSettings // 5600
{
public:
	float size; // 5600
	bool sizeRelative; // 5601
	bool pushToCanvasEdge; // 5602
	TSetController sizeControl;
	TSetNoise sizeNoise;
	bool usePressure_size; // 5603
	float sizeTotal_min; // 5604
	float sizeTotal_max; // 5605
	float sizeTotal_bias; // 5606
	float sizeNoise_min; // 5607
	float sizeNoise_max; // 5608
	float sizeNoise_bias; // 5609
	float finalSize = 0.0f;

	bool useDirection; // 5610
	float splineOffsetX_min; // 5611
	float splineOffsetX_max; // 5612
	float splineOffsetY_min; // 5613
	float splineOffsetY_max; // 5614
	float rotation; // 5615
	TSetController rotationControl;
	TSetNoise rotationNoise;
	bool usePressure_rotation; // 5616
	float rotationTotal_min; // 5617
	float rotationTotal_max; // 5618
	float rotationNoise_min; // 5619
	float rotationNoise_max; // 5620

	// Whether points are placed from 0 to 1 or 1 to 0, or from -1 to 0/0 to 1 mirrored
	// when determining points position along the rake path
	bool mirrorPoints; // 5630
	bool flipPoints; // 5631
	int pointsCount; // 5632
	// The distribution of points along the line. Default is uniform, although other
	// non-linear distributions are available. Mirroring scales the distribution by 1/2 and flips it
	TSetProp distribution; // 5635
		// Below this would be variables to control various input factors for distribution

	// Random point distribution
	bool pointOffsetX_usePressure; // 5640
	float pointOffsetX_min; // 5641
	float pointOffsetX_max; // 5642
	bool pointOffsetY_usePressure; // 5643
	float pointOffsetY_min; // 5644
	float pointOffsetY_max; // 5645

	// Set the rake shape that gets placed along the input spline. This shape is not able to be mirrored
	// Linear / Arrow / Arc / Cone (Scatter)
	TSetProp rakeShape; // 5650
	bool shape_arrow_usePressure; // 5651
	float shape_arrow_angleMin; // 5652
	float shape_arrow_angleMax; // 5653
	bool shape_bend_usePressure; // 5654
	float shape_bend_angleMin; // 5655
	float shape_bend_angleMax; // 5656
	bool shape_cone_angleUsePressure; // 5657
	float shape_cone_angleMin; // 5658
	float shape_cone_angleMax; // 5659
	bool shape_cone_radiusUsePressure; // 5660
	float shape_cone_radiusMin; // 5661
	float shape_cone_radiusMax; // 5662

	// No Connect / use Given / Along Axis / Along Points / Along Points Shuffle
	TSetProp connectionOrdering; // 5670

	virtual TSetType getType() { return type; }

	virtual TSetControl_Node* getControlNode(int settingSig, int subSig);
	virtual TSetController* getController(int settingSig, int subSig);
	virtual TSetGraph* getGraph(int settingSig, int subSig);
	virtual TSetNoise* getNoise(int settingSig, int subSig);

	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	TSet_Rake();
};

#endif