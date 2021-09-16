#ifndef TSET_FX_FILL_H
#define TSET_FX_FILL_H

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
#include "../../structs.h"

class TSet_FX_Fill : public ToolSettings // 8100
{
public:
	TSetProp colorUsage;
	CColor color; // 8100 // 0, 1, 2, 3
	float hueMin; // 8101
	float hueMax; // 8102
	float satMin; // 8103
	float satMax; // 8104
	float valMin; // 8105
	float valMax; // 8106
	float mixAmountMin; // 8107
	float mixAmountMax; // 8108
	float mixAmountBias; // 8109

	bool useChannelMask; // 8120
	TSetProp channelMask; // 8121
	bool channelMask_randomize; // 8122
	TSetProp channelMask_randomize_mode; // 8123

	bool maskX; // 8130
	bool maskX_invert; // 8131
	bool maskX_invertRandom; // 8132
	float maskX_variance; // 8133
	bool maskX_random; // 8134
	bool maskY; // 8135
	bool maskY_invert; // 8136
	bool maskY_invertRandom; // 8137
	float maskY_variance; // 8138
	bool maskY_random; // 8139
	float maskX_noiseChunkSize = 20.0f; // 8140
	float maskY_noiseChunkSize = 20.0f; // 8141
	bool maskXY_noiseSizeRelative = true; // 8142

	bool maskUsingAngle; // 8150
	float maskAngleMin; // 8152
	float maskAngleMax; // 8153
	bool maskUsingDirection; // 8151

	bool maskUsingCenter; // 8160
	bool combineCenterMask; // 8161
	bool maskCenter_invert; // 8162
	bool maskCenter_invertRandom; // 8163
	float maskCenter_offsetXMin; // 8164
	float maskCenter_offsetXMax; // 8165
	float maskCenter_offsetXBias; // 8166
	float maskCenter_offsetYMin; // 8167
	float maskCenter_offsetYMax; // 8168
	float maskCenter_offsetYBias; // 8169

	CColor finalColor = black;
	TSetProp finalChannelMask = TSetProp::none;
	float maskAngle = 0.0f;
	float maskCenter_offsetX = 0.0f;
	float maskCenter_offsetY = 0.0f;
	bool finalMaskX = true;
	bool finalMaskY = true;
	bool finalMaskXInvert = false;
	bool finalMaskYInvert = false;
	bool finalMaskCenterInvert = false;
	float finalChunkSizeX = 1.0f;
	float finalChunkSizeY = 1.0f;

	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	TSet_FX_Fill();
	void initializeData(CColor FGColor, CColor BGColor, int tipSize);
	int getChannelMask();
};

#endif