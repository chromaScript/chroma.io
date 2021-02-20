#ifndef INPUTMETHOD_H
#define INPUTMETHOD_H

#include "../structs.h"
#include "../ToolSettings.h"

class Application;
class Tool;

#include <vector>

class InputMethod
{
private:
protected:
	int ID;
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
public:
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
	InputMethod(int id, TSetType controlScheme, std::shared_ptr<Tool> owner) : ID(id), owner(owner), controlScheme(controlScheme) {};
	int getID() { return ID; }
	InputData getData() { return data; }
	VertexData getFragData() { return fragData; }
	virtual int move(Application* sender, MouseEvent dat) = 0;
	virtual int click(Application* sender, MouseEvent dat) = 0;
	bool continuousMove(Application* sender, MouseEvent dat,
		TSet_ContinuousControl* lineControl, 
		TSet_Smoothing* smoothing,
		bool useSplineData,
		glm::vec3& outPos, glm::vec3& outDir);
	bool dragMove(Application* sender, MouseEvent dat, 
		TSet_DragControl* dragControl,
		glm::vec3& cursorPos);
	bool onePointMove(Application* sender, MouseEvent dat, 
		TSet_OnePointControl* onePointControl, 
		glm::vec3 &cursorPos);
	void resetInput(Application* sender, MouseEvent dat, glm::vec3 &point, glm::vec3 &dir);
};

#endif