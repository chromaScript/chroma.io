#ifndef TSET_POLYGON_H
#define TSET_POLYGON_H

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
// Polygon Settings
////////////////////////////////////////////////////////////////

class TSet_Polygon : public ToolSettings // 5100
{
public:
	TSetProp shapeType; // Triangle / Rectangle / Ellipse / Sides / Custom
	int sides;
	bool useSize = false; // Hidden Setting for when used with continuous draw
	float size;
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	TSet_Polygon();
};

#endif