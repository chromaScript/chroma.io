#include "../../include/gladHelper.h"

#include "../../include/entities/widgets/Graph_ToolControl.h"
#include "../../include/entities/widgets/WidgetStyle.h"
#include "../../include/entities/widgets/WidgetInterface.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "../../include/Application.h"
#include "../../include/entities/UserInterface.h"
#include "../../include/tool/Tool.h"
#include "../../include/tool/ToolSettings.h"
#include "../../include/tool/toolSettings/ToolSettings_Forward.h"

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

extern std::shared_ptr<Application> chromaIO;

//Constructor

Graph_ToolControl::Graph_ToolControl(std::map<std::string, std::string> basicAttribs,
	std::weak_ptr<Widget> parent,
	std::weak_ptr<WidgetStyle> style,
	std::shared_ptr<Shader> shader) : Widget(style, shader)
{
	this->type = LTokenType::T_GRAPH;
	maxChildren = 16;
	maxHandles = 14;

	if (!parent.expired()) { this->parentWidget = parent; }
	if (basicAttribs.count("id")) { this->idAttrib = basicAttribs.at("id"); }
	if (basicAttribs.count("name")) { this->nameAttrib = basicAttribs.at("name"); }
	if (basicAttribs.count("img")) { this->imgAttrib = basicAttribs.at("img"); }
	if (basicAttribs.count("value")) { this->valueAttrib = basicAttribs.at("value"); }
	if (basicAttribs.count("innerContent")) { this->innerContent = basicAttribs.at("innerContent"); }

	topSliderStyle = std::make_shared<WidgetStyle>();
	bottomSliderStyle = std::make_shared<WidgetStyle>();
	handleStyle = std::make_shared<WidgetStyle>();

	handleStyle.get()->position = topSliderStyle.get()->position = bottomSliderStyle.get()->position = UI_POSITION_ABSOLUTE;
	handleStyle.get()->visibility = topSliderStyle.get()->visibility = bottomSliderStyle.get()->visibility = UI_VISIBILITY_SHOW;
	handleStyle.get()->backgroundColor = topSliderStyle.get()->backgroundColor = bottomSliderStyle.get()->backgroundColor = CColor(0.62f, 0.62f, 0.62f, 1.0f);
	handleStyle.get()->backgroundAlpha = topSliderStyle.get()->backgroundAlpha = bottomSliderStyle.get()->backgroundAlpha = 1.0f;
	handleStyle.get()->minX = handleStyle.get()->minY = topSliderStyle.get()->minX = bottomSliderStyle.get()->minX = 6.0f;
	topSliderStyle.get()->minY = bottomSliderStyle.get()->minY = 10.0f;
	topSliderStyle.get()->offset.top = int(-topSliderStyle.get()->minY);
	
	if (!style.expired() && style.lock().get()->backgroundTexture.string().size() != 0)
	{
		std::string str = style.lock().get()->backgroundTexture.string();
		setData_toImage(str.c_str(), &texSizeX, &texSizeY);
		useBGColor = false;
	}
	else
	{
		useBGColor = true;
	}
	generateBuffers(&(this->VAO), &(this->VBO), &(this->EBO), &(this->TEX0));
}

void Graph_ToolControl::initializeWidget(std::shared_ptr<UI> ui)
{
	isInitialized = true;
	// Attach Main Widget Interface Functions
	attachGraphInterface(this, WidgetInterfaceNames::TGraph_Main);

	// Create Bottom Bound Slider
	std::map<std::string, std::string> basicAttribsBottom = {
		{"name", "tg_botSlider"},
		{"value", "0"}
	};
	bottomSliderStyle.get()->offset.top = this->getSizeByParent().y;
	bottomSliderStyle.get()->offset.left = int(0 - (bottomSliderStyle.get()->minX / 2.0f));
	childWidgets.push_back(ui.get()->createWidget(
		LTokenType::H_BOX,
		basicAttribsBottom,
		weak_from_this(),
		bottomSliderStyle.get()->weak_from_this(),
		chromaIO.get()->getWidgetShader()));
	childWidgets.back().get()->genIndexA = -1;
	childWidgets.back().get()->_namespace = this->_namespace;
	attachGraphInterface(childWidgets.back().get(), WidgetInterfaceNames::TGraph_BottomSlider);

	// Create Top Bound Slider
	std::map<std::string, std::string> basicAttribsTop = {
		{"name", "tg_topSlider"},
		{"value", "1"}
	};
	topSliderStyle.get()->offset.left = int(this->getSizeByParent().x - (topSliderStyle.get()->minX / 2.0f));

	childWidgets.push_back(ui.get()->createWidget(
		LTokenType::H_BOX,
		basicAttribsTop,
		weak_from_this(),
		topSliderStyle.get()->weak_from_this(),
		chromaIO.get()->getWidgetShader()));
	childWidgets.back().get()->genIndexA = -2;
	childWidgets.back().get()->_namespace = this->_namespace;
	attachGraphInterface(childWidgets.back().get(), WidgetInterfaceNames::TGraph_TopSlider);

	updateGraphData(ui);
}

std::weak_ptr<Widget> Graph_ToolControl::addGraphHandleWidget(std::shared_ptr<UI> ui, int topOffset, int leftOffset, bool doUpdate)
{
	handleStyle.get()->offset.top = topOffset;
	handleStyle.get()->offset.left = leftOffset;
	// Create A New Handle Widget
	std::map<std::string, std::string> basicAttribsHandle = {
		{"name", "tg_handle"}
	};

	childWidgets.push_back(ui.get()->createWidget(
		LTokenType::IMAGE,
		basicAttribsHandle,
		weak_from_this(),
		handleStyle.get()->weak_from_this(),
		chromaIO.get()->getWidgetShader()));
	childWidgets.back().get()->genIndexA = (handleCount == 0) ? 0 : -4;
	childWidgets.back().get()->_namespace = this->_namespace;
	attachGraphInterface(childWidgets.back().get(), WidgetInterfaceNames::TGraph_Handle);
	
	// Handle automatic updating for adding new handles. If calling with !doUpdate, callee must determine updating behavior.
	if (doUpdate) {
		ui.get()->requestWidgetHierarchyRebuild(weak_from_this());
		ui.get()->clearRebuildRequests();
		handleCount++;

		updateGraphData(ui);

		// Relocate widgets
		int closeA = 0;
		float distA = (float)INT_MAX;
		int insertIndex = 0;
		if (handleCount > 1)
		{
			glm::ivec2 loc = childWidgets.back().get()->location;
			glm::vec2 normalized = glm::vec2(
				clampf((loc.x + int((float)childWidgets.back().get()->getSize().x / 2.0f)) / (float)sizeX, 0.0f, 1.0f),
				clampf(1.0f - ((loc.y + int((float)childWidgets.back().get()->getSize().x / 2.0f)) / (float)sizeX), 0.0f, 1.0f));
			bool foundFirst = false;
			for (int i = 0; i < handleCount + 1; i++)
			{
				glm::vec4 segment = glm::vec4(
					curvePoints[(i * 2)], curvePoints[(i * 2) + 1],
					curvePoints[(i * 2) + 2], curvePoints[(i * 2) + 3]);
				float dist = distancePointLine2D(normalized, segment);
				if (dist < distA)
				{
					distA = dist; closeA = i;
				}
			}
			for (std::shared_ptr<Widget> child : childWidgets)
			{
				if (child.get()->genIndexA >= 0 && child.get()->genIndexA >= closeA)
				{
					child.get()->genIndexA++;
				}
				else if (child.get()->genIndexA == -4) { child.get()->genIndexA = closeA; }
				std::cout << child.get()->genIndexA << std::endl;
			}
			updateGraphData(ui);
		}
	}

	return childWidgets.back().get()->weak_from_this();
}

void Graph_ToolControl::removeHandleWidget(std::shared_ptr<UI> ui, int handleNumber, bool doUpdate)
{
	std::string randTag = randomNamespace(48);
	for (std::shared_ptr<Widget> child : childWidgets)
	{
		if (child.get()->genIndexA == handleNumber)
		{
			child.get()->idAttrib = randTag;
			child.get()->genIndexA = -4;
			chromaIO.get()->scriptConsole.get()->insertWidgetTableID(randTag, this->getRoot().lock()->rootId);
			break;
		}
	}
	ui.get()->requestWidgetDeletion(nullptr, randTag);
	// Perform updates for when handling one deletion at a time. Otherwise for multi-delete actions,
	// leave handleCount behavior to that function.
	if (doUpdate)
	{
		for (std::shared_ptr<Widget> child : childWidgets)
		{
			if (child.get()->genIndexA >= 0 && child.get()->genIndexA > handleNumber)
			{
				child.get()->genIndexA--;
			}
		}
		handleCount--;
		updateGraphData(ui);
	}
}

void Graph_ToolControl::removeHandleWidget(std::shared_ptr<UI> ui, std::shared_ptr<Widget> child)
{
	std::string randTag = randomNamespace(48);
	child.get()->idAttrib = randTag;
	chromaIO.get()->scriptConsole.get()->insertWidgetTableID(randTag, this->getRoot().lock()->rootId);
	ui.get()->requestWidgetDeletion(nullptr, randTag);
}

void Graph_ToolControl::updateGraphData(std::shared_ptr<UI> ui)
{
	for (std::shared_ptr<Widget> child : childWidgets)
	{
		if (child.get()->nameAttrib == "tg_topSlider")
		{
			curveEnd = glm::vec2(clampf(((float)child.get()->style.offset.left + ((float)child.get()->getSize().x / 2.0f)) / (float)sizeX, 0.0f, 1.0f), 1.0f);
		}
		else if (child.get()->nameAttrib == "tg_botSlider")
		{
			curveStart = glm::vec2(clampf(((float)child.get()->style.offset.left + ((float)child.get()->getSize().x / 2.0f)) / (float)sizeX, 0.0f, 1.0f), 0.0f);
		}
		else if (child.get()->nameAttrib.find("tg_handle") != std::string::npos && child.get()->genIndexA != -4)
		{
			int handleID = child.get()->genIndexA + 1;
			glm::ivec2 loc = child.get()->location;
			curvePoints[((size_t)handleID * 2)] = clampf((loc.x + int((float)child.get()->getSize().x / 2.0f)) / (float)sizeX, 0.0f, 1.0f);
			curvePoints[((size_t)handleID * 2) + 1] = clampf(1.0f - ((loc.y + int((float)child.get()->getSize().x / 2.0f)) / (float)sizeX), 0.0f, 1.0f);
		}
	}
	curvePoints[0] = curveStart.x; curvePoints[1] = curveStart.y;
	curvePoints[((handleCount + 1) * 2)] = curveEnd.x;
	curvePoints[((handleCount + 1) * 2) + 1] = curveEnd.y;
	// Note: Should move this copyWidgetData to elsewhere. Consider helper function that validates and updates if valid.
	if (graphSettings != nullptr) { graphSettings->copyWidgetData(this); }
}

void Graph_ToolControl::copyGraphData(std::shared_ptr<UI> ui)
{
	if (graphSettings == nullptr) { return; }
	curveStart = graphSettings->start;
	curveEnd = graphSettings->end;
	// Copy Curves Point Data
	int newHandleCount = graphSettings->handleCount;
	curvePoints[0] = curveStart.x; curvePoints[1] = curveStart.y;
	for (int i = 1; i < 15; i++)
	{
		curvePoints[(i * 2)] = graphSettings->controlPoints[(i * 2)];
		curvePoints[(i * 2) + 1] = graphSettings->controlPoints[(i * 2) + 1];
	}
	curvePoints[((newHandleCount + 1) * 2)] = curveEnd.x;
	curvePoints[((newHandleCount + 1) * 2) + 1] = curveEnd.y;
	std::shared_ptr<CInterpreter> interpreter = chromaIO.get()->scriptConsole.get()->getInterpreter();
	for (std::shared_ptr<Widget> child : childWidgets)
	{
		if (child.get()->nameAttrib == "tg_topSlider")
		{
			int maxLeft = sizeX;
			int sliderWidth = child.get()->getSize().x;
			int xLoc = (int)clampf((curveEnd.x * sizeX) - (sliderWidth / 2.0f), -(sliderWidth / 2.0f), maxLeft - (sliderWidth / 2.0f));
			child.get()->setProperty(interpreter, "left", std::make_shared<CObject>(double(xLoc)));
		}
		else if (child.get()->nameAttrib == "tg_botSlider")
		{
			int maxLeft = sizeX;
			int sliderWidth = child.get()->getSize().x;
			int xLoc = (int)clampf((curveStart.x * sizeX) - (sliderWidth / 2.0f), -(sliderWidth / 2.0f), maxLeft - (sliderWidth / 2.0f));
			child.get()->setProperty(interpreter, "left", std::make_shared<CObject>(double(xLoc)));
		}
	}
	// New Handle Count is 0 and Current Handle Widgets is 0
	if (newHandleCount == 0 && childWidgets.size() == 2)
	{
		// Do Nothing
		handleCount = 0;
		return;
	}
	// New handle count is 0 and Current Handle Widgets is greater than 0
	else if (newHandleCount == 0 && childWidgets.size() > 2)
	{
		// Delete all handle widgets
		for (std::shared_ptr<Widget> child : childWidgets)
		{
			if (child.get()->nameAttrib.find("tg_handle") != std::string::npos)
			{
				removeHandleWidget(ui, child);
			}
		}
		handleCount = 0;
		return;
	}
	// New handle count is greater than 0
	else
	{
		// Wipe the handle count attribute
		for (std::shared_ptr<Widget> child : childWidgets)
		{
			if (child.get()->nameAttrib.find("tg_handle") != std::string::npos)
			{
				child.get()->genIndexA = -4;
			}
		}
		int maxLeft = sizeX;
		int maxTop = sizeY;
		// Reset handleCount and loop through the widgets.
		// Use existing widgets and copy the new data to their attributes
		handleCount = 0;
		for (std::shared_ptr<Widget> child : childWidgets)
		{
			if (child.get()->nameAttrib.find("tg_handle") != std::string::npos && child.get()->genIndexA == -4 && handleCount < newHandleCount)
			{
				int sliderWidth = child.get()->getSize().x;
				int xLoc = (int)clampf(
					(curvePoints[(handleCount + 1) * 2] * maxLeft) - (sliderWidth / 2.0f), 
					-(sliderWidth / 2.0f), 
					maxLeft - (sliderWidth / 2.0f));
				int yLoc = (int)clampf(
					((1.0f - curvePoints[((handleCount + 1) * 2) + 1]) * maxTop) - (sliderWidth / 2.0f), 
					-(sliderWidth / 2.0f), 
					maxTop - (sliderWidth / 2.0f));
				child.get()->setProperty(interpreter, "left", std::make_shared<CObject>(double(xLoc)));
				child.get()->setProperty(interpreter, "top", std::make_shared<CObject>(double(yLoc)));
				child.get()->genIndexA = handleCount;
				handleCount++;
			}
		}

		// Handle if there are excess widgets
		if (handleCount == newHandleCount && (int)childWidgets.size() > handleCount + 2)
		{
			// Delete extra widgets
			for (std::shared_ptr<Widget> child : childWidgets)
			{
				if (child.get()->nameAttrib.find("tg_handle") != std::string::npos && child.get()->genIndexA == -4)
				{
					removeHandleWidget(ui, child);
				}
			}
		}
		// Handle if there are fewer handle widgets than needed
		else if (handleCount < newHandleCount)
		{
			// Add new widgets
			for (int i = handleCount; i < newHandleCount; i++)
			{
				int sliderWidth = handleStyle.get()->minX;
				int xLoc = (int)clampf(
					(curvePoints[(handleCount + 1) * 2] * maxLeft) - (sliderWidth / 2.0f),
					-(sliderWidth / 2.0f),
					maxLeft - (sliderWidth / 2.0f));
				int yLoc = (int)clampf(
					((1.0f - curvePoints[((handleCount + 1) * 2) + 1]) * maxTop) - (sliderWidth / 2.0f),
					-(sliderWidth / 2.0f),
					maxTop - (sliderWidth / 2.0f));
				addGraphHandleWidget(ui, yLoc, xLoc, false);
				childWidgets.back().get()->genIndexA = handleCount;
				handleCount++;
			}
		}
		// Finally do manual updates for the widget
		ui.get()->requestWidgetHierarchyRebuild(weak_from_this());
		ui.get()->clearRebuildRequests();

		updateGraphData(ui);
	}
}

bool Graph_ToolControl::canAddHandle()
{
	if (handleCount < maxHandles) { return true; }
	return false;
}

void Graph_ToolControl::bindToolSettings(std::weak_ptr<Tool> tool, int settingSig, int subSig)
{
	if (graphSettings != nullptr)
	{
		graphSettings->copyWidgetData(this);
	}
	if (settingSig == -1)
	{
		for (std::shared_ptr<Widget> child : childWidgets)
		{
			if (child.get()->nameAttrib.find("tg_handle") != std::string::npos)
			{
				child.get()->genIndexA != -4;
			}
		}
		boundTool.reset();
		boundSettingID = -1;
		boundSettingSubSigID = -1;
		graphSettings = nullptr;
		return;
	}
	else if (tool.expired()) { return; }

	boundTool = tool;
	boundSettingID = settingSig;
	boundSettingSubSigID = subSig;
	graphSettings = boundTool.lock()->getSetting(settingSig, subSig)->getGraph(settingSig, subSig);
	copyGraphData(chromaIO.get()->ui);
}

void Graph_ToolControl::buildWidget()
{
	setBounds_Widget();
	setVertData_Widget();
	bindBuffers();
	bindTexture(texSizeX, texSizeY);
	updateGraphData(chromaIO.get()->ui);
}
void Graph_ToolControl::clearData()
{
	// Nothing to do
}

bool Graph_ToolControl::callSpecialFunction(std::string funcName, std::vector<std::shared_ptr<CObject>> args)
{
	if (funcName == "reset")
	{
		return resetGraph();
	}
	else if (funcName == "mirror")
	{
		return mirrorGraph();
	}
	else if (funcName == "move")
	{
		if (args.size() < 2) { return false; }
		if (args[0].get()->objType.type != CLiteralTypes::_CNumber) { return false; }
		if (args[1].get()->objType.type != CLiteralTypes::_CNumber) { return false; }
		return moveGraph((float)std::get<double>(args[0].get()->obj), (float)std::get<double>(args[1].get()->obj));
	}
	else if (funcName == "scale")
	{
		if (args.size() < 1) { return false; }
		if (args[0].get()->objType.type != CLiteralTypes::_CNumber) { return false; }
		return scaleGraph((float)std::get<double>(args[0].get()->obj));
	}
	return false;
}

bool Graph_ToolControl::resetGraph()
{
	graphSettings->resetGraph();
	copyGraphData(chromaIO.get()->ui);
	return true;
}

bool Graph_ToolControl::mirrorGraph()
{
	graphSettings->copyWidgetData(this);
	graphSettings->flipGraph();
	copyGraphData(chromaIO.get()->ui);
	return true;
}

bool Graph_ToolControl::moveGraph(float amountX, float amountY)
{
	graphSettings->copyWidgetData(this);
	graphSettings->shiftGraph(amountX, amountY);
	copyGraphData(chromaIO.get()->ui);
	return true;
}

bool Graph_ToolControl::scaleGraph(float amount)
{
	graphSettings->copyWidgetData(this);
	graphSettings->scaleGraph(amount);
	copyGraphData(chromaIO.get()->ui);
	return true;
}

// Dimension Functions
glm::ivec2 Graph_ToolControl::getSizeByChildren()
{
	glm::ivec2 report = setSizeByParent();
	sizeX = report.x;
	sizeY = report.y;
	return report;
}
glm::ivec2 Graph_ToolControl::getSizeByParent()
{
	glm::ivec2 report = setSizeByParent();
	sizeX = report.x;
	sizeY = report.y;
	return report;
}
// Report the location of child widget relative to it's siblings (Per-Widget behavior, this is how a widget
// manages it's children locations.
glm::ivec2 Graph_ToolControl::findWidgetLocation(std::shared_ptr<Widget> childWidget)
{
	// The graph should only have natively-placed widgets as it's children. For now
	// use the default horizontal Arrangement, as that has all the rules needed.
	return horizontalArrangement(childWidget);
}

// Render Functions
void Graph_ToolControl::drawSelf(ShaderTransform* xform)
{
	glm::vec3 backgroundColor;
	glm::vec3 fillColor = style.fillColor.makeVec3();
	glm::vec2 screenLocation;
	int overflowValue = (applyOverflowMask && !overflowTarget.expired() && overflowTarget.lock()->style.zIndex >= style.zIndex) ? 0 : 1;
	glm::vec4 overflowBoxMask = glm::vec4(0.0f);
	// Calculate the overflowMask if needed (Note that the box is pre-calculated, but given an offset per-frame)
	// offsetBox is given as BottomLeft.xy, TopRight.xy
	if (overflowValue == 0)
	{
		overflowBoxMask = calculateOverflowMask();
	}
	// Draw the drop shadow if applicable
	if (style.boxShadowSizeX != 0 || style.boxShadowSizeY != 0)
	{
		// Reuse backgroundColor variable for boxShadowColor
		shader->use();
		backgroundColor = style.boxShadowColor.makeVec3();
		screenLocation = getScreenLocation(style.boxShadowSizeX, style.boxShadowSizeY, true);
		shader->setVec2("screenLocation", screenLocation);
		shader->setInt("texture1", 0);
		// Set Box Shadow Color
		shader->setVec3("backgroundColor", backgroundColor);
		shader->setFloat("backgroundAlpha", style.boxShadowAlpha);
		// Set the Overflow Values
		shader->setInt("useOverflow", overflowValue);
		shader->setVec4("overflowBox", overflowBoxMask);
		// Set Vertex Locations
		shader->setInt("pointsCount", handleCount + 1);
		shader->setFloatArray("curvePoints", 32, curvePoints);
		shader->setVec2("curveStart", curveStart);
		shader->setVec2("curveEnd", curveEnd);
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TEX0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	shader->use();
	backgroundColor = style.backgroundColor.makeVec3();
	screenLocation = getScreenLocation(0, 0, true);
	shader->setVec2("screenLocation", screenLocation);
	shader->setInt("texture1", 0);
	// Set Background Color
	shader->setVec3("backgroundColor", backgroundColor);
	shader->setFloat("backgroundAlpha", style.backgroundAlpha);
	// Set the Overflow Values
	shader->setInt("useOverflow", overflowValue);
	shader->setVec4("overflowBox", overflowBoxMask);
	// Set Vertex Locations
	shader->setInt("pointsCount", handleCount + 1);
	shader->setFloatArray("curvePoints", 32, curvePoints);
	shader->setVec2("curveStart", curveStart);
	shader->setVec2("curveEnd", curveEnd);
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TEX0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}