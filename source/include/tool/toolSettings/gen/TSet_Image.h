#ifndef TSET_IMAGE_H
#define TSET_IMAGE_H

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

#define TSET_IMAGE_TIPSIZE_MIN 1
#define TSET_IMAGE_TIPSIZE_MAX 500
#define TSET_IMAGE_SPACING_MIN 0.01f
#define TSET_IMAGE_SPACING_MAX 500.0f

////////////////////////////////////////////////////////////////
// Image Settings
////////////////////////////////////////////////////////////////

class TSet_Image : public ToolSettings // 2200
{
public:
	int tipSize; // 2201
	bool relativeSpacing; // 2202
	float spacing; // 2203 // When in relative mode, use (spacing / 100) to get the % value
	float sampleRate; // 2205 // Performance based slider to modulate delta time between input samples

	float colorTotalInfluence; // 2209
	float fgInfluence_total; // 2210
	float fgInfluence_maskRed; // 2211
	float fgInfluence_maskBlue; // 2212
	float fgInfluence_maskGreen; // 2213
	float fgInfluence_maskValue; // 2214
	float fgInfluence_maskAlpha; // 2215
	float bgInfluence_total; // 2216
	float bgInfluence_maskRed; // 2217 
	float bgInfluence_maskBlue; // 2218
	float bgInfluence_maskGreen; // 2219
	float bgInfluence_maskValue; // 2220
	float bgInfluence_maskAlpha; // 2221

	// Settings for the Tip Image - The first version of the tool settings
	// will offer a "stack" of tips which are blended together to produce
	// the final image data for the brush tip. Each tip will have blending modes,
	// color masking/influence, opacity, offsets, rotation, scale, anti-aliasing etc.
	// Image / Radius (Circle) / Polygon
	// Shape (SVG)
	TSetProp tipType; // 2230

	// Settings applied to the final tip image after merging the tip layers
	float flipX; // 2250
	float flipY; // 2251
	float scaleX; // 2252
	float scaleY; // 2253
	float rotation; // 2254
	bool offsetRelative; // 2255
	float offsetX; // 2256
	float offsetY; // 2257
	float blurFinal_amount; // 2258
	float blurFinal_quality; // 2259
	float antiAliasing; // 2260

	// Hidden Value - Pre-calculated final spacing version.
	float trueOffsetX = 0.0f;
	float trueOffsetY = 0.0f;
	float trueSpacing = 0.0f;

	virtual TSetType getType() { return type; }

	virtual TSetControl_Node* getControlNode(int settingSig, int subSig);
	virtual TSetController* getController(int settingSig, int subSig);
	virtual TSetGraph* getGraph(int settingSig, int subSig);
	virtual TSetNoise* getNoise(int settingSig, int subSig);

	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	TSet_Image();
	void updateTrueSpacing();
	void updateTrueOffset();
};

#endif