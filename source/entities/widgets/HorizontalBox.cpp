#include "../../include/gladHelper.h"

#include "../../include/entities/widgets/HorizontalBox.h"
#include "../../include/entities/widgets/WidgetStyle.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

//Constructor

HorizontalBox::HorizontalBox(std::map<std::string, std::string> basicAttribs,
	std::weak_ptr<Widget> parent,
	std::weak_ptr<WidgetStyle> style,
	std::shared_ptr<Shader> shader) : Widget(style, shader)
{
	this->type = LTokenType::H_BOX;
	if (!parent.expired()) { this->parentWidget = parent; }
	if (basicAttribs.count("id")) { this->idAttrib = basicAttribs.at("id"); }
	if (basicAttribs.count("name")) { this->nameAttrib = basicAttribs.at("name"); }
	if (basicAttribs.count("img")) { this->imgAttrib = basicAttribs.at("img"); }
	if (basicAttribs.count("value")) { this->valueAttrib = basicAttribs.at("value"); }
	if (basicAttribs.count("innerContent")) { this->innerContent = basicAttribs.at("innerContent"); }

	maxChildren = 200;
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

void HorizontalBox::buildWidget()
{
	setBounds_Widget();
	setVertData_Widget();
	bindBuffers();
	bindTexture(texSizeX, texSizeY);
}
void HorizontalBox::clearData()
{
	// Nothing to do
}

// Dimension Functions
glm::ivec2 HorizontalBox::getSizeByChildren()
{
	glm::ivec2 report = setSizeByParent();
	sizeX = report.x;
	sizeY = report.y;
	return report;
}
glm::ivec2 HorizontalBox::getSizeByParent()
{
	glm::ivec2 report = setSizeByParent();
	sizeX = report.x;
	sizeY = report.y;
	return report;
}
// Report the location of child widget relative to it's siblings (Per-Widget behavior, this is how a widget
// manages it's children locations.
glm::ivec2 HorizontalBox::findWidgetLocation(std::shared_ptr<Widget> childWidget)
{
	return horizontalArrangement(childWidget);
}

// Render Functions
void HorizontalBox::drawSelf(ShaderTransform* xform)
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
		shader->setBool("useBackgroundColor", true);
		shader->setVec3("backgroundColor", backgroundColor);
		shader->setFloat("backgroundAlpha", style.boxShadowAlpha);
		// Set Fill Color
		shader->setBool("useFillColor", false);
		shader->setVec3("fillColor", fillColor);
		shader->setFloat("fillAlpha", style.fillAlpha);
		// Set the Overflow Values
		shader->setInt("useOverflow", overflowValue);
		shader->setVec4("overflowBox", overflowBoxMask);
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
	shader->setBool("useBackgroundColor", useBGColor);
	shader->setVec3("backgroundColor", backgroundColor);
	shader->setFloat("backgroundAlpha", style.backgroundAlpha);
	// Set Fill Color
	shader->setBool("useFillColor", (style.fillAlpha > 0) ? true : false);
	shader->setVec3("fillColor", fillColor);
	shader->setFloat("fillAlpha", style.fillAlpha);
	// Set the Overflow Values
	shader->setInt("useOverflow", overflowValue);
	shader->setVec4("overflowBox", overflowBoxMask);
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TEX0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}