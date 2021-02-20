#include "include/ToolSettings.h"
#include "include/keys.h"

class CFunction;
#include "include/cscript/CInterpreter.h"
#include "include/cscript/CObject.h"
#include "include/Tool.h"

#include <glm.hpp>
#include <string>
#include <vector>
#include <memory>

#include "include/structs.h"

TSet_Pan::TSet_Pan()
{ 
	this->type = TSetType::pan;
	this->constrainToCanvas = false;
	this->doFlickPanning = false;
	this->friction = 0.0f;
	this->force = 1000;
	this->speedAmount = 0.0f;
	this->slowModKey = INPUT_KEY_LEFT_ALT;
	this->slowFactor = 0.85f;
	this->clickTargetPanning = true;
	this->clickTarButton = UI_MOUSE_RIGHT;
	this->clickTarModKey = -1;
	this->lockToX = false;
	this->lockXModKey = -1;
	this->lockToY = false;
	this->lockYModKey = -1;
	this->lockCustomAngle = false;
	this->customAngle = 22.5f;
	this->lockCustomModKey = -1;
	this->cameraPosRecording = false;
	this->cameraPosRecordKey = -1;
}

TSet_Zoom::TSet_Zoom() 
{ 
	this->type = TSetType::zoom;
	this->scrubThreshold = 16;
	this->maxZoomFactor = 1500.0f;
	this->minZoomFactor = 50.0f;
}

TSet_Rotate::TSet_Rotate() 
{ 
	this->type = TSetType::rotate; 
}

TSet_ContinuousControl::TSet_ContinuousControl()
{
	this->type = TSetType::continuous;
	// Input Mode
	this->defaultMode = TSetProp::draw; // draw / polyLine
	this->alternateModeKey = INPUT_MOD_ALT;
	//
	this->fillOnShiftClick = true;
	this->fillOnPolyDraw = false;
	this->connectPropertiesWeighting = 0.85f;
	//
	this->anchorSpacing = 5.0f;
	this->splineRandomX = 0;
	this->splineRandomY = 0;
	// Constraint Global
	this->constraintLengthThreshold = 1.2f;
	// Constrathis->Angle A
	this->enableConstrainA = true;
	this->constrainA_angle = 0.0f;
	this->constrainA_modKey = INPUT_MOD_SHIFT;
	// Constrathis->Angle B
	this->enableConstrainB = true;
	this->constrainB_angle = 90.0f;
	this->constrainB_modKey = INPUT_MOD_SHIFT;
	// Constrathis->Angle C
	this->enableConstrainC = true;
	this->constrainC_angle = 45.0f;
	this->constrainC_modKey = INPUT_MOD_SHIFT;
	// Constrathis->Angle D
	this->enableConstrainD = true;
	this->constrainD_angle = -45.0f;
	this->constrainD_modKey = INPUT_MOD_SHIFT;
}
std::shared_ptr<CObject> TSet_ContinuousControl::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, std::shared_ptr<CObject> object, bool isGet)
{
	if (settingsSig == 400) {}
	else if (settingsSig == 401) {}
	else if (settingsSig == 402) 
	{
		if (isGet) { return std::make_shared<CObject>(double(anchorSpacing)); }
		else { anchorSpacing = (float)std::get<double>(object.get()->obj); }
	}
	else if (settingsSig == 403) {}
	else if (settingsSig == 404) {}
	return std::make_shared<CObject>(nullptr);
}
void TSet_ContinuousControl::clearConstraint()
{
	activated = false;
	line = glm::vec4(0);
	origin = dir = perpendicular = glm::vec3(0);
	activeAngle = 0.0f;
	activeKey = -1;
}

TSet_DragControl::TSet_DragControl()
{
	this->type = TSetType::drag;
	this->snapModeKey = 340; // Left-Shift
	this->snapAngleIncrement = 15.0f;
	this->moveOriginModeKey = 32; // Space
	this->sizeAboutOriginKey = 341; // Left-Ctrl
	this->pressureMapping = true;
	this->pressureMapStrength = 0.9f;
	this->pressureMapDecayRate = 0.1f;
	this->pressureMapRelative = false;
	this->pressureMapThreshold = 120.0f;
	this->connectInputs = true;
	this->connectInputKey = 32; // Space
	this->terminateInputKey = 32; // Space
	// Extrusion Settings, only visible for tools with it enabled
	this->extrudeModeKey = 342; // Left-Alt
	this->initialExtrusion = 0.0f;
	this->extrudeSnappingMode = TSetProp::distance;
	this->extrudeSnapIncrement = 10.0f;
}

TSet_OnePointControl::TSet_OnePointControl()
{
	this->type = TSetType::onepoint;
	this->initialRotation = 0.0f;
	this->constrainAngleIncrement = 15.0f;
	this->constrainRatioKey = 340; // Left-Shift
	this->moveOriginModeKey = 32; // Space
	this->rotateModeKey = 342; // Left-Alt
	this->sizeAboutOriginKey = 341; // Left-Ctrl
	this->pressureMapping = true;
	this->pressureMapStrength = 0.9f;
	this->pressureMapDecayRate = 0.1f;
	this->pressureMapRelative = false;
	this->pressureMapThreshold = 120.0f;
}

TSet_TwoPointControl::TSet_TwoPointControl()
{
	this->type = TSetType::twopoint;
}

TSet_ThreePointControl::TSet_ThreePointControl()
{
	this->type = TSetType::threepoint;
}

TSet_Blend::TSet_Blend() 
{ 
	this->type = TSetType::blend; 
}

TSet_Basic::TSet_Basic()
{ 
	this->type = TSetType::basic; 
	this->lockFGColor = false;
	this->lockBGColor = false;
	this->savedFGColor = black;
	this->currentFGColor = black;
	this->savedBGColor = white;
	this->currentBGColor = white;
}

TSet_Image::TSet_Image() 
{		
	this->type = TSetType::image; 
	this->tipType = TSetProp::radius;
	this->tipSize = 40;
	this->relativeSpacing = true;
	this->spacing = 28.0f;
}
std::shared_ptr<CObject> TSet_Image::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, std::shared_ptr<CObject> object, bool isGet)
{
	if (settingsSig == 2200) {}
	else if (settingsSig == 2201) 
	{
		if (isGet) { return std::make_shared<CObject>(double(tipSize)); }
		else { tipSize = (int)std::get<double>(object.get()->obj); }
	}
	else if (settingsSig == 2202) {}
	else if (settingsSig == 2203) 
	{
		if (isGet) { return std::make_shared<CObject>(double(spacing)); }
		else { spacing = (float)std::get<double>(object.get()->obj); }
	}
	return std::make_shared<CObject>(nullptr);
}

TSet_Polygon::TSet_Polygon()
{
	this->type = TSetType::polygon;
	this->shapeType = TSetProp::rectangle;
	this->sides = 4;
	this->size = 160.0f;
}

TSet_PolyLine::TSet_PolyLine()
{
	this->useMiterJoints = true;
	this->maxMiterLength = 40.0f;
	this->bevelType = TSetProp::polygon;
	this->bevelWidth = 60.0f;
	this->segmentCount = 6;
	this->constantSegmentCount = false;
	this->startCapType = TSetProp::polyCircle;
	this->startCapScale = 1.0f;
	this->endCapType = TSetProp::polyCircle;
	this->endCapScale = 1.0f;
}

TSet_Mesh::TSet_Mesh() 
{ 
	this->type = TSetType::mesh;
}

TSet_Scatter::TSet_Scatter() 
{ 
	this->type = TSetType::scatter; 
}

TSet_Color::TSet_Color() 
{ 
	this->type = TSetType::color; 
}

TSet_Alpha::TSet_Alpha()
{ 
	this->type = TSetType::alpha;
	this->usePressureOpacity = true;
	this->minOpacity = 0.0f;
	this->maxOpacity = 1.0f;
	this->usePressureFlow = true;
	this->minFlow = 0.2f;
	this->maxFlow = 0.8f;
}
std::shared_ptr<CObject> TSet_Alpha::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, std::shared_ptr<CObject> object, bool isGet)
{
	if (settingsSig == 2800)
	{
		if (isGet) { return std::make_shared<CObject>(usePressureOpacity); }
		else { usePressureOpacity = std::get<bool>(object.get()->obj); }
	}
	else if (settingsSig == 2801)
	{
		if (isGet) { return std::make_shared<CObject>(double(minOpacity)); }
		else { minOpacity = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
	}
	else if (settingsSig == 2802)
	{
		if (isGet) { return std::make_shared<CObject>(double(maxOpacity)); }
		else { maxOpacity = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
	}
	else if (settingsSig == 2803)
	{
		if (isGet) { return std::make_shared<CObject>(usePressureFlow); }
		else { usePressureFlow = std::get<bool>(object.get()->obj); }
	}
	else if (settingsSig == 2804)
	{
		if (isGet) { return std::make_shared<CObject>(double(minFlow)); }
		else { minFlow = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
	}
	else if (settingsSig == 2805)
	{
		if (isGet) { return std::make_shared<CObject>(double(maxFlow)); }
		else { maxFlow = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
	}
	return std::make_shared<CObject>(nullptr);
}

TSet_Distance::TSet_Distance() 
{ 
	this->type = TSetType::distance; 
}

TSet_Mixing::TSet_Mixing() 
{ 
	this->type = TSetType::mixing; 
}

TSet_MultiTip::TSet_MultiTip() 
{ 
	this->type = TSetType::multiTip; 
}

TSet_Texture::TSet_Texture() 
{ 
	this->type = TSetType::texture; 
}

TSet_Grain::TSet_Grain() 
{ 
	this->type = TSetType::grain; 
}

TSet_WetEdges::TSet_WetEdges() 
{ 
	this->type = TSetType::wetEdges; 
}

TSet_Repeat::TSet_Repeat() 
{ 
	this->type = TSetType::repeat; 
}

TSet_Reference::TSet_Reference() 
{ 
	this->type = TSetType::reference; 
}

TSet_Vector::TSet_Vector() 
{
	this->type = TSetType::vector; 
}

TSet_Gradient::TSet_Gradient() 
{ 
	this->type = TSetType::gradient; 
}

TSet_Fill::TSet_Fill() 
{ 
	this->type = TSetType::fill; 
}

TSet_Pattern::TSet_Pattern() 
{ 
	this->type = TSetType::pattern; 
}

TSet_Field::TSet_Field()
{
	this->type = TSetType::field;
	this->distributionType = TSetProp::random;
	this->randomizationType = TSetProp::uniform;
	this->constantSeed = false;
	this->savedSeed = 0;
	this->constantDensity = false;
	this->density = 220.0f;
	this->bias = 0.0f;
	this->randomizePressure = true;
	this->pressureXInfluence = 0.0f;
	this->pressureYInfluence = 1.0f;
	this->pressureNoise = 0.4f;
	this->randomizeTilt = false;
	this->tiltXInfluence = 0.0f;
	this->tiltYInfluence = 0.0f;
	this->tiltNoise = 0.0f;
	this->randomizeRotation = true;
	this->rotationXInfluence = 0.0f;
	this->rotationYInfluence = 0.0f;
	this->rotationNoise = 0.4f;
	this->connectVertices = false;
	this->connectionStrength = 1.0f;
	this->maxConnectionLength = 8;
	this->minConnectionLength = 3;
	this->connectConnectionEnds = true;
	this->connectionOrdering = TSetProp::useDefault;
	this->secondaryScatteringX = 15.0f;
	this->secondaryScatteringY = 10.0f;
}

TSet_Vortex::TSet_Vortex()
{
	this->type = TSetType::vortex;
	this->constantRingCount = true;
	this->rings = 3.0f;
	this->radius = 120.0f;
	this->ringDensity = 24;
	this->ringSpacingMode = TSetProp::automatic;
	this->ringSpacing_simple = 0.0f;
	this->ringRotationOffset = 0.0f;
	this->ringRotationRandom = 0.0f;
	this->scatteringMode = TSetProp::circular;
	this->angleVariance_simple = 0.0f;
	this->radiusVariance_simple = 0.0f;
	this->offsetVarianceX_simple = 0.0f;
	this->offsetVarianceY_simple = 0.0f;
	this->connectionOrdering = TSetProp::useDefault;
}

TSet_Fan::TSet_Fan()
{
	this->waitMode = TSetProp::distance;
	this->waitAmount = 0.0f;
	this->subdivisionCount = 0;
	this->scateringMode = TSetProp::circular;
	this->angleVariance_simple = 0.0f;
	this->radiusVariance_simple = 0.0f;
	this->offsetVarianceX_simple = 0.0f;
	this->offsetVarianceY_simple = 0.0f;
	this->connectionOrdering = TSetProp::useDefault;
	this->airbrushMode = false;
}

TSet_Rake::TSet_Rake()
{
	this->type = TSetType::rake;
	this->pushToCanvasEdge = false;
	this->points = 3;
	this->size = 160.0f;
	this->pointsSpacingMode = TSetProp::automatic;
	this->pointsSpacing_simple = 0.0f;
	this->pointsShapeMode = TSetProp::linear;
	this->maxAngle = 0.0f;
	this->angleVariance_simple = 0.0f;
	this->radiusVariance_simple = 0.0f;
	this->offsetVarianceX_simple = 0.0f;
	this->offsetVarianceY_simple = 0.0f;
	this->connectionOrdering = TSetProp::useDefault;
}

TSet_Shader::TSet_Shader() 
{ 
	this->type = TSetType::shader; 
}

TSet_Filter::TSet_Filter() 
{ 
	this->type = TSetType::filter; 
}

TSet_Sampler::TSet_Sampler() 
{ 
	this->type = TSetType::sampler; 
	// Sampler Shape Type & Settings
	this->sampleType = TSetProp::allLayers; // allLayers / currentLayer / currentBelow
	this->shapeType = TSetProp::point; // Pothis->/ Radius / Shape / Tip
	this->pointRandAmount = 0;
	this->isCircle = true;
	this->shapeRadius = 1;
	this->shapeSides = 3;
	this->shapeRotation = 0;
	this->tipData = nullptr;
	this->tipSize = 1;
	// Auto Color Jitter & Control
	this->useFGVariance = false;
	this->FGValueControl;
	this->FGHueControl;
	this->FGSaturationControl;
	this->FGValueNoise;
	this->FGHueNoise;
	this->FGSaturationNoise;
	this->useAutoBGUpdater = false;
	this->bgValueOffset = 0;
	this->bgHueOffset = 0;
	this->bgSaturationOffset = 0;
	this->BGValueControl;
	this->BGHueControl;
	this->BGSaturationControl;
	this->BGValueNoise;
	this->BGHueNoise;
	this->BGSaturationNoise;
	// Palette Constraints
	this->usePaletteConstrain = false;
	this->paletteType = TSetProp::simple; // Simple / Complex
	this->valueMin = 0;
	this->valueMax = 0;
	this->hueMin = 0;
	this->hueMax = 0;
	this->saturationMin = 0;
	this->saturationMax = 0;
	this->paletteShapes;
	this->paletteValueMin = 0;
	this->paletteValueMax = 0;
	// Block Callback Notifications?
	this->shouldNotify = true;
}

TSet_Smoothing::TSet_Smoothing() 
{ 
	this->type = TSetType::smoothing; 
	this->smoothDirection = false;
	this->directionThreshold = 35.0f;
	this->directionFactor = 0.7f;
	this->smoothDirectionCurve = false;
	this->curveDirectionThreshold = 20.0f;
	this->curveDirectionFactor = 0.4f;
}

TSet_AntiAliasing::TSet_AntiAliasing()
{ 
	this->type = TSetType::antiAliasing; 
}