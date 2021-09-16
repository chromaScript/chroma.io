#ifndef TSET_POLYLINE_H
#define TSET_POLYLINE_H

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

////////////////////////////////////////////////////////////////
// PolyLine Settings
////////////////////////////////////////////////////////////////

class TSet_PolyLine : public ToolSettings // 5200
{
public:
	bool useMiterJoints;
	float maxMiterLength;
	TSetProp bevelType;
	float bevelWidth;
	float segmentCount;
	bool constantSegmentCount;
	TSetProp startCapType;
	float startCapScale;
	TSetProp endCapType;
	float endCapScale;
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	TSet_PolyLine();
};

#endif