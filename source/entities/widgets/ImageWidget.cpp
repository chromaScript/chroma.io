#include "../../include/gladHelper.h"

#include "../../include/entities/widgets/ImageWidget.h"
#include "../../include/entities/widgets/WidgetStyle.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

//Constructor
ImageWidget::ImageWidget(std::map<std::string, std::string> basicAttribs,
	std::weak_ptr<Widget> parent,
	std::weak_ptr<WidgetStyle> style,
	std::shared_ptr<Shader> shader) : Widget(style, shader)
{
	this->type = LTokenType::IMAGE;
	this->parentWidget = parent;
	if (basicAttribs.count("id")) { this->idAttrib = basicAttribs.at("id"); }
	if (basicAttribs.count("name")) { this->nameAttrib = basicAttribs.at("name"); }
	if (basicAttribs.count("img")) { this->imgAttrib = basicAttribs.at("img"); }
	if (basicAttribs.count("value")) { this->valueAttrib = basicAttribs.at("value"); }
	if (basicAttribs.count("innerContent")) { this->innerContent = basicAttribs.at("innerContent"); }
	maxChildren = 1;
	if (style.expired())
	{
		// Just in case the copy was nullptr, avoid runtime errors from access violation
		this->style = WidgetStyle();
	}
	if (this->style.backgroundTexture.string().size() != 0)
	{
		std::string str = this->style.backgroundTexture.string();
		setData_toImage(str.c_str(), &texSizeX, &texSizeY);
		useBGColor = false;
	}
	else
	{
		useBGColor = true;
	}
	generateBuffers(&(this->VAO), &(this->VBO), &(this->EBO), &(this->TEX0));
}

void ImageWidget::buildWidget()
{
	setBounds_Widget();
	setVertData_Widget();
	bindBuffers();
	bindTexture(texSizeX, texSizeY);
}
void ImageWidget::clearData()
{
	// Nothing to do
}

// Dimension Functions
glm::ivec2 ImageWidget::getSizeByChildren()
{
	glm::ivec2 report = setSizeByParent();
	sizeX = report.x;
	sizeY = report.y;
	return report;
}
glm::ivec2 ImageWidget::getSizeByParent()
{
	glm::ivec2 report = setSizeByParent();
	sizeX = report.x;
	sizeY = report.y;
	return report;
}
// Report the location of child widget relative to it's siblings (Per-Widget behavior, this is how a widget
// manages it's children locations.
glm::ivec2 ImageWidget::findWidgetLocation(std::shared_ptr<Widget> childWidget)
{
	// Image should have no child widgets, so this function should not ever get called.
	// But if it does, return the pxPosition style attribute.
	glm::ivec2 childLoc;
	childLoc.x = childWidget.get()->style.getOffset().left;
	childLoc.y = childWidget.get()->style.getOffset().top;
	return childLoc;
}

// Render Functions
void ImageWidget::drawSelf(ShaderTransform xform)
{
	if (nameAttrib == "tg_handle0")
	{
		int k = 5;
	}
	glm::vec3 backgroundColor = glm::vec3(0.0f);
	glm::vec3 fillColor = style.fillColor.makeVec3();
	glm::vec2 screenLocation = glm::vec2(0.0f);
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
		shader->use();
		backgroundColor = style.boxShadowColor.makeVec3();
		screenLocation = getScreenLocation(style.boxShadowSizeX, style.boxShadowSizeY, true);
		shader->setVec2("screenLocation", screenLocation);
		shader->setInt("texture1", 0);
		// Set Box Shadow Color
		if (useBGColor)
		{
			// Set Background Color
			shader->setBool("useBackgroundColor", true);
			shader->setVec3("backgroundColor", backgroundColor);
			shader->setFloat("backgroundAlpha", style.boxShadowAlpha);
			// Set Fill Color
			shader->setBool("useFillColor", false);
			shader->setVec3("fillColor", fillColor);
			shader->setFloat("fillAlpha", style.fillAlpha);
		}
		else
		{
			// Set Background Color
			shader->setBool("useBackgroundColor", false);
			shader->setVec3("backgroundColor", backgroundColor);
			shader->setFloat("backgroundAlpha", (style.backgroundColor.a > style.backgroundAlpha) ? style.backgroundColor.a : style.backgroundAlpha);
			// Set Fill Color
			shader->setBool("useFillColor", true);
			shader->setVec3("fillColor", backgroundColor);
			shader->setFloat("fillAlpha", style.boxShadowAlpha);
		}
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
	shader->setFloat("backgroundAlpha", (style.backgroundColor.a > style.backgroundAlpha) ? style.backgroundColor.a : style.backgroundAlpha);
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