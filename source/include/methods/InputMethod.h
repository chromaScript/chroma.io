#ifndef INPUTMETHOD_H
#define INPUTMETHOD_H

#include "../tool/ToolSettings.h"
#include "../tool/toolSettings/ToolSettings_Forward.h"
#include "../entities/visuals/VertexData.h"
#include "MethodType.h"
#include "../input/keys.h"

class Application;
class Tool;

#include <vector>
#include <string>

class InputMethod
{
private:
protected:
	MethodType id;
	std::string name = "";
	std::shared_ptr<Tool> owner;
	// InputData is an internal management structure for storing incoming mouse events
	// This allows input methods to hold/wait on adding new anchor points, and still
	// be able to recall the input history for calculations
	InputData data;
	int splineIDCount = 0;
	int anchorIDCount = 0;
	size_t inputCount = 0;
	int maxBufferLength = 5;
	float startAngle = -360.0f;
	float startExtrude = 0.0f;
	float currentLength = 0.0f;
	glm::vec3 lastCursor = glm::vec3(0);
	int storedEntityCount = 0;
public:
	MethodType type = MethodType::in_noInput;
	// SplineData is the public spline data management structure that is used internally
	// by input methods to perform more complex operations.
	VertexData splineData;
	// VertexData is the public vertex data management structure that passes through
	// the output method to the Fragment being rendered
	VertexData fragData;
	// The interest mask stores a vector of valid tool settings for the input method
	// Only when both the output and input have a tool setting is it available to modify
	std::vector<TSetType> interestMask;
	// The control mode stores the style of input control for the tool. Many tools share
	// the control scheme for their input stream, and this will expose the settings to
	// customize that control scheme
	TSetType controlScheme;
	TSet_ContinuousControl continuous;
	TSet_DragControl drag;
	TSet_OnePointControl onePoint;
	TSet_TwoPointControl twoPoint;
	TSet_ThreePointControl threePoint;
	// Methods
	InputMethod(MethodType id, TSetType controlScheme, std::shared_ptr<Tool> owner) : id(id), owner(owner), controlScheme(controlScheme) {};
	MethodType getID() { return id; }
	InputData getData() { return data; }
	VertexData getFragData() { return fragData; }
	virtual InputHandlerFlag move(Application* sender, Input dat) = 0;
	virtual InputHandlerFlag click(Application* sender, Input dat) = 0;
	virtual InputHandlerFlag key(Application* sender, Input dat, Keybind key, InputAction action, InputModKey modKeys) = 0;
	bool continuousMove(Application* sender, Input dat,
		TSet_ContinuousControl* continuousControl,
		TSet_Smoothing* smoothing,
		VertexData* target, float vertexSpacing,
		glm::vec3& outPos, glm::vec3& outDir);
	bool continuousHover(Application* sender, Input dat,
		TSet_ContinuousControl* continuousControl, TSet_Smoothing* smoothing,
		VertexData* target, float vertexSpacing,
		glm::vec3& outPos, glm::vec3& outDir);
	bool dragMove(Application* sender, Input dat,
		TSet_DragControl* dragControl,
		glm::vec3& cursorPos);
	bool onePointMove(Application* sender, Input dat,
		TSet_OnePointControl* onePointControl, 
		glm::vec3 &cursorPos);
	void resetInput(Application* sender, Input dat, glm::vec3 &point, glm::vec3 &dir);
	std::string getName() { return name; }
	void addInputData(Input dat);
};

#endif