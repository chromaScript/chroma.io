#include "../../include/gladHelper.h"

#include "../../include/entities/widgets/GradientBox.h"
#include "../../include/entities/widgets/WidgetStyle.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

//Constructor
GradientBox::GradientBox(std::map<std::string, std::string> basicAttribs,
	std::weak_ptr<Widget> parent,
	std::weak_ptr<WidgetStyle> style,
	std::shared_ptr<Shader> shader) : Widget(style, shader)
{
	this->type = LTokenType::GRADIENT_BOX;
	if (!parent.expired()) { this->parentWidget = parent; }
	if (basicAttribs.count("id")) { this->idAttrib = basicAttribs.at("id"); }
	if (basicAttribs.count("name")) { this->nameAttrib = basicAttribs.at("name"); }
	if (basicAttribs.count("img")) { this->imgAttrib = basicAttribs.at("img"); }
	if (basicAttribs.count("value")) { this->valueAttrib = basicAttribs.at("value"); }
	if (basicAttribs.count("innerContent")) { this->innerContent = basicAttribs.at("innerContent"); }

	maxChildren = 24;
	useBGColor = true;
	generateBuffers(&(this->gbVAO), &(this->gbVBO), &(this->gbEBO), &(this->gbTEX0));
}

void GradientBox::buildWidget()
{
	setBounds_Widget();
	setVertData_gradientBox();
	// Bind vertex array and buffers with data
	glBindVertexArray(gbVAO);
	glBindBuffer(GL_ARRAY_BUFFER, gbVBO);
	glBufferData(GL_ARRAY_BUFFER, gradientBoxData_size, gradientBoxData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gbEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertArrangement_size, vertArrangement, GL_STATIC_DRAW);
	// Set vertex position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, gbAttribStride, (void*)0);
	glEnableVertexAttribArray(0);
	// Set vertex color attribute
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, gbAttribStride, (void*)(size_t)gbVCAttribOffset);
	glEnableVertexAttribArray(1);
	// Set texture coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, gbAttribStride, (void*)(size_t)gbTCAttribOffset);
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
}

void GradientBox::clearData()
{
	glDeleteTextures(1, &this->gbTEX0);
	glDeleteBuffers(1, &this->gbVBO);
	glDeleteBuffers(1, &this->gbEBO);
	glDeleteVertexArrays(1, &this->gbVAO);
}

void GradientBox::setVertData_gradientBox()
{
	float ox = (2 * (float(transform.boundBox.x1) / WINDOW_WIDTH)) - 1;
	float oy = (2 * (float(WINDOW_HEIGHT - transform.boundBox.y1) / WINDOW_HEIGHT)) - 1;
	float vx = (2 * (float(transform.boundBox.x2) / WINDOW_WIDTH)) - 1;
	float vy = (2 * (float(WINDOW_HEIGHT - transform.boundBox.y2) / WINDOW_HEIGHT)) - 1;
	float ratio = (float)sizeX / (float)sizeY;
	float tx;
	float ty;
	if (ratio >= 1.0f)
	{
		tx = 1.0f;
		ty = tx / ratio;
	}
	else
	{
		ty = 1.0f;
		tx = ty * ratio;
	}
	// Now create a new float array with these bounds
	CColor vc1 = CColor(); CColor vc2 = CColor(); CColor vc3 = CColor(); CColor vc4 = CColor();
	for (int i = 1; i < 5; i++)
	{
		if (style.vertexColor.count(i) == 1)
		{
			switch (i)
			{
			case 1: vc1 = style.vertexColor.at(i); break;
			case 2: vc2 = style.vertexColor.at(i); break;
			case 3: vc3 = style.vertexColor.at(i); break;
			case 4: vc4 = style.vertexColor.at(i); break;
			}
		}
	}
	float newData[36] = {
		// The top and bottom are normal texture coords because this renders in respect
		// to the screen space coordinates
		// positions	// vertex color					// texture coords - 
		vx, oy, 0.0f,	vc2.r,vc2.g,vc2.b,vc2.a,		1.0f, 1.0f, // top right
		vx, vy,	0.0f,	vc3.r,vc3.g,vc3.b,vc3.a,		1.0f, 0.0f, // bottom right
		ox, vy, 0.0f,	vc4.r,vc4.g,vc4.b,vc4.a,		0.0f, 0.0f,  // bottom left
		ox, oy, 0.0f,	vc1.r,vc1.g,vc1.b,vc1.a,		0.0f, 1.0f // top left
	};
	// Overwrite the old data with the new data
	for (int i = 0; i < 36; i++)
	{
		gradientBoxData[i] = newData[i];
	}
}

// Dimension Functions
glm::ivec2 GradientBox::getSizeByChildren()
{
	glm::ivec2 report = setSizeByParent();
	sizeX = report.x;
	sizeY = report.y;
	return report;
}
glm::ivec2 GradientBox::getSizeByParent()
{
	glm::ivec2 report = setSizeByParent();
	sizeX = report.x;
	sizeY = report.y;
	return report;
}
// Report the location of child widget relative to it's siblings (Per-Widget behavior, this is how a widget
// manages it's children locations.
glm::ivec2 GradientBox::findWidgetLocation(std::shared_ptr<Widget> childWidget)
{
	return horizontalArrangement(childWidget);
}

// Render Functions
void GradientBox::drawSelf(ShaderTransform* xform)
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
		glBindVertexArray(gbVAO);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, TEX0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	shader->use();
	backgroundColor = style.backgroundColor.makeVec3();
	screenLocation = getScreenLocation(0, 0, true);
	shader->setVec2("screenLocation", screenLocation);
	shader->setInt("texture1", 0);
	// Set Background Color
	shader->setBool("useBackgroundColor", true);
	shader->setVec3("backgroundColor", backgroundColor);
	shader->setFloat("backgroundAlpha", style.backgroundAlpha);
	// Set Fill Color
	shader->setBool("useFillColor", false);
	shader->setVec3("fillColor", fillColor);
	shader->setFloat("fillAlpha", style.fillAlpha);
	// Set the Overflow Values
	shader->setInt("useOverflow", overflowValue);
	shader->setVec4("overflowBox", overflowBoxMask);
	glBindVertexArray(gbVAO);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, TEX0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}