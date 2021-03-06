#include "../include/entities/VerticalBox.h"
#include "../include/entities/WidgetStyle.h"

#include <glad/glad.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

//Constructor
VerticalBox::VerticalBox(std::map<std::string, std::string> basicAttribs,
	std::weak_ptr<Widget> parent,
	std::weak_ptr<WidgetStyle> style,
	std::shared_ptr<Shader> shader) : Widget(style, shader)
{
	this->type = LTokenType::V_BOX;
	this->parentWidget = parent;
	if (basicAttribs.count("id")) { this->idAttrib = basicAttribs.at("id"); }
	if (basicAttribs.count("name")) { this->nameAttrib = basicAttribs.at("name"); }
	if (basicAttribs.count("img")) { this->imgAttrib = basicAttribs.at("img"); }
	if (basicAttribs.count("value")) { this->valueAttrib = basicAttribs.at("value"); }
	if (basicAttribs.count("innerContent")) { this->innerContent = basicAttribs.at("innerContent"); }
	maxChildren = 200;
	if (style.lock().get()->backgroundTexture.string().size() != 0)
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

void VerticalBox::buildWidget()
{
	setBounds_Widget();
	setVertData_Widget();
	bindBuffers();
	bindTexture(texSizeX, texSizeY);
}

// Dimension Functions
glm::ivec2 VerticalBox::getSizeByChildren()
{
	glm::ivec2 report = setSizeByParent();
	sizeX = report.x;
	sizeY = report.y;
	return report;
}
glm::ivec2 VerticalBox::getSizeByParent()
{
	glm::ivec2 report = setSizeByParent();
	sizeX = report.x;
	sizeY = report.y;
	return report;
}
// Report the location of child widget relative to it's siblings (Per-Widget behavior, this is how a widget
// manages it's children locations.
glm::ivec2 VerticalBox::findWidgetLocation(std::shared_ptr<Widget> childWidget)
{
	return verticalArrangement(childWidget);
}

// Render Functions
void VerticalBox::drawSelf(ShaderTransform xform)
{
	glm::vec3 backgroundColor;
	glm::vec3 fillColor = style.fillColor.makeVec3();
	glm::vec2 screenLocation;
	// Draw the drop shadow if applicable
	if (style.boxShadowSizeX != 0 || style.boxShadowSizeY != 0)
	{
		// Reuse backgroundColor for boxShadow
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
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TEX0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}