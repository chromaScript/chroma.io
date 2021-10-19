#ifndef NOISE_TOOLCONTROL_H
#define NOISE_TOOLCONTROL_H

#include "Widget.h"
#include "WidgetInterface.h"
#include "../../Shader.h"
#include "../../math/transform.h"

class UI;
class Tool;
class TSetNoise;

#include <glm.hpp>
#include <map>

class Noise_ToolControl : public Widget, public WidgetInterface
{
private:
protected:
public:
	std::weak_ptr<Tool> boundTool;
	TSetNoise* noiseSettings = nullptr;

	int boundSettingID = -1;
	int boundSettingSubSigID = -1;
	float zoom = 10.0f;
	
	// Constructors
	Noise_ToolControl(std::map<std::string, std::string> basicAttribs,
		std::weak_ptr<Widget> parent,
		std::weak_ptr<WidgetStyle> style,
		std::shared_ptr<Shader> shader);
	void bindToolSettings(std::weak_ptr<Tool> tool, int settingSig, int subSig);
	virtual void buildWidget();
	void initializeWidget(std::shared_ptr<UI> ui);
	virtual void clearData();
	bool updateZoom(float value);

	// Special Function

	// Noise Functions


	// Dimension Functions
	virtual glm::ivec2 getSizeByChildren();
	virtual glm::ivec2 getSizeByParent();
	virtual glm::ivec2 findWidgetLocation(std::shared_ptr<Widget> childWidget);

	// Render Function
	virtual void drawSelf(ShaderTransform xform);
};

#endif