#ifndef GRAPH_TOOLCONTROL_H
#define GRAPH_TOOLCONTROL_H

#include "Widget.h"
#include "WidgetInterface.h"

class UI;
class Tool;
class TSetGraph;

#include <glm.hpp>
#include <map>

class Graph_ToolControl : public Widget, public WidgetInterface
{
private:
	bool isInitialized = false;

	int maxHandles = 0;
	int handleCount = 0;
	

	glm::vec2 curveStart = glm::vec2(0.0f);
	glm::vec2 curveEnd = glm::vec2(0.0f);
	float curvePoints[32] = {
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
	};

protected:
public:
	std::weak_ptr<Tool> boundTool;
	TSetGraph* graphSettings = nullptr;

	int boundSettingID = -1;
	int boundSettingSubSigID = -1;

	std::shared_ptr<WidgetStyle> topSliderStyle;
	std::shared_ptr<WidgetStyle> bottomSliderStyle;
	std::shared_ptr<WidgetStyle> handleStyle;

	// Constructors
	Graph_ToolControl(std::map<std::string, std::string> basicAttribs,
		std::weak_ptr<Widget> parent,
		std::weak_ptr<WidgetStyle> style,
		std::shared_ptr<Shader> shader);
	void bindToolSettings(std::weak_ptr<Tool> tool, int settingSig, int subSig);
	virtual void buildWidget();
	void initializeWidget(std::shared_ptr<UI> ui);
	virtual void clearData();

	// Graph Functions
	std::weak_ptr<Widget> addGraphHandleWidget(std::shared_ptr<UI> ui, int topOffset, int leftOffset, bool doUpdate);
	void removeHandleWidget(std::shared_ptr<UI> ui, int handleNumber, bool doUpdate);
	void removeHandleWidget(std::shared_ptr<UI> ui, std::shared_ptr<Widget> child);
	void updateGraphData(std::shared_ptr<UI> ui);
	void copyGraphData(std::shared_ptr<UI> ui);
	bool canAddHandle();
	bool resetGraph();
	bool mirrorGraph();
	bool moveGraph(float amountX, float amountY);
	bool scaleGraph(float amount);

	// Special Function
	bool callSpecialFunction(std::string funcName, std::vector<std::shared_ptr<CObject>> args);

	glm::vec2 getStartPoint() { return curveStart; }
	glm::vec2 getEndPoint() { return curveEnd; }
	float* getCurvePoints() { return curvePoints; }
	int getHandleCount() { return handleCount; }

	// Dimension Functions
	virtual glm::ivec2 getSizeByChildren();
	virtual glm::ivec2 getSizeByParent();
	virtual glm::ivec2 findWidgetLocation(std::shared_ptr<Widget> childWidget);

	// Render Function
	virtual void drawSelf(ShaderTransform* xform);
};

#endif