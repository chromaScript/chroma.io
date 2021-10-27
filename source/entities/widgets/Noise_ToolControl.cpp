#include "../../include/gladHelper.h"

#include "../../include/entities/widgets/Noise_ToolControl.h"
#include "../../include/entities/widgets/WidgetStyle.h"
#include "../../include/entities/widgets/WidgetInterface.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "../../include/entities/UserInterface.h"
#include "../../include/tool/Tool.h"
#include "../../include/tool/ToolSettings.h"
#include "../../include/tool/toolSettings/ToolSettings_Forward.h"

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

extern std::shared_ptr<Application> chromaIO;

//Constructor

Noise_ToolControl::Noise_ToolControl(std::map<std::string, std::string> basicAttribs,
	std::weak_ptr<Widget> parent,
	std::weak_ptr<WidgetStyle> style,
	std::shared_ptr<Shader> shader) : Widget(style, shader)
{
	this->type = LTokenType::T_NOISE;
	maxChildren = 0;

	if (!parent.expired()) { this->parentWidget = parent; }
	if (basicAttribs.count("id")) { this->idAttrib = basicAttribs.at("id"); }
	if (basicAttribs.count("name")) { this->nameAttrib = basicAttribs.at("name"); }
	if (basicAttribs.count("img")) { this->imgAttrib = basicAttribs.at("img"); }
	if (basicAttribs.count("value")) { this->valueAttrib = basicAttribs.at("value"); }
	if (basicAttribs.count("innerContent")) { this->innerContent = basicAttribs.at("innerContent"); }

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

void Noise_ToolControl::initializeWidget(std::shared_ptr<UI> ui)
{
	// Nothing
}

void Noise_ToolControl::bindToolSettings(std::weak_ptr<Tool> tool, int settingSig, int subSig)
{
	if (tool.expired()) { return; }
	boundTool = tool;
	boundSettingID = settingSig;
	boundSettingSubSigID = subSig;
	noiseSettings = boundTool.lock()->getSetting(settingSig, subSig)->getNoise(settingSig, subSig);
}

void Noise_ToolControl::buildWidget()
{
	setBounds_Widget();
	setVertData_Widget();
	bindBuffers();
	bindTexture(texSizeX, texSizeY);
}
void Noise_ToolControl::clearData()
{
	// Nothing to do
}

bool Noise_ToolControl::updateZoom(float value)
{
	zoom = clampf(value, 0.01f, 600.0f);
	std::cout << zoom << std::endl;
	return true;
}

// Dimension Functions
glm::ivec2 Noise_ToolControl::getSizeByChildren()
{
	glm::ivec2 report = setSizeByParent();
	sizeX = report.x;
	sizeY = report.y;
	return report;
}
glm::ivec2 Noise_ToolControl::getSizeByParent()
{
	glm::ivec2 report = setSizeByParent();
	sizeX = report.x;
	sizeY = report.y;
	return report;
}
// Report the location of child widget relative to it's siblings (Per-Widget behavior, this is how a widget
// manages it's children locations.
glm::ivec2 Noise_ToolControl::findWidgetLocation(std::shared_ptr<Widget> childWidget)
{
	// The graph should only have natively-placed widgets as it's children. For now
	// use the default horizontal Arrangement, as that has all the rules needed.
	return horizontalArrangement(childWidget);
}

// Render Functions
void Noise_ToolControl::drawSelf(ShaderTransform* xform)
{
	
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
		glm::vec4 boxShadowColor = style.boxShadowColor.makeVec4();
		screenLocation = getScreenLocation(style.boxShadowSizeX, style.boxShadowSizeY, true);
		shader->setBool("drawBoxShadow", true);
		shader->setBool("drawNoise", false);
		shader->setVec2("screenLocation", screenLocation);
		shader->setInt("texture1", 0);
		// Set Box Shadow Color
		shader->setVec4("boxShadowColor", boxShadowColor);
		// Set the Overflow Values
		shader->setInt("useOverflow", overflowValue);
		shader->setVec4("overflowBox", overflowBoxMask);
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TEX0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	shader->use();
	screenLocation = getScreenLocation(0, 0, true);
	shader->setVec2("screenLocation", screenLocation);
	shader->setInt("texture1", 0);

	shader->setBool("drawBoxShadow", false);

	if (noiseSettings != nullptr)
	{
		shader->setBool("drawNoise", true);
		int noiseMode = 0;
		switch (noiseSettings->noiseMode)
		{
		case TSetProp::perlin: noiseMode = 0; break;
		case TSetProp::simplex: noiseMode = 1; break;
		case TSetProp::voroni: noiseMode = 2; break;
		case TSetProp::random: noiseMode = 3; break;
		case TSetProp::vorotwist: noiseMode = 4; break;
		case TSetProp::voroblur: noiseMode = 5; break;
		case TSetProp::canvas: noiseMode = 6; break;
		case TSetProp::cellularA: noiseMode = 7; break;
		case TSetProp::cellularB: noiseMode = 8; break;
		case TSetProp::halftoneDots: noiseMode = 9; break;
		case TSetProp::maze: noiseMode = 10; break;
		case TSetProp::round: noiseMode = 11; break;
		case TSetProp::triangles: noiseMode = 12; break;
		default: noiseMode = 0;	 
		}
		shader->setInt("noiseMode", noiseMode);
		glm::vec2 noiseScale = noiseSettings->getFinalScale();
		shader->setFloat("widgetZoom", zoom / 100.0f);
		shader->setInt("scaleCanvas", (int)noiseSettings->scaleToCanvas);
		shader->setVec2("scale", noiseScale);
		shader->setFloat("skew", noiseSettings->skew);
		shader->setFloat("seedA", (float)noiseSettings->seed);
		shader->setFloat("factorA", noiseSettings->factorA);
		shader->setFloat("factorB", noiseSettings->factorB);
		shader->setFloat("factorC", noiseSettings->factorC);
		shader->setBool("rotateNoise", noiseSettings->rotateNoise);
		shader->setFloat("rotation", noiseSettings->rotation * (MATH_PI / 180.0f));
		shader->setFloat("brightness", noiseSettings->brightness);
		shader->setFloat("contrast", noiseSettings->contrast);
		shader->setFloat("rangeMin", noiseSettings->valueRangeMin);
		shader->setFloat("rangeMax", noiseSettings->valueRangeMax);
		shader->setFloat("rangeSmooth", noiseSettings->valueRangeSmooth);
		shader->setFloat("blackLevel", noiseSettings->blackValue);
		shader->setFloat("whiteLevel", noiseSettings->whiteValue);
		shader->setInt("invert", (int)noiseSettings->invertValue);
		shader->setInt("flipX", (int)noiseSettings->flipX);
		shader->setInt("flipY", (int)noiseSettings->flipY);
		shader->setBool("lockPivot", noiseSettings->useOriginOffset);
	}
	else
	{
		shader->setBool("drawNoise", false);
		
	}
	// Set the Overflow Values
	shader->setInt("useOverflow", overflowValue);
	shader->setVec4("overflowBox", overflowBoxMask);
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TEX0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}