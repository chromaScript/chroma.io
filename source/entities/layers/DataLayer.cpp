#include "../../include/entities/layers/DataLayer.h"
#include "../../include/math/Color.h"

#include "../../include/gladHelper.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <iostream>

extern std::shared_ptr<Shader> layerShader;

DataLayer::DataLayer(glm::ivec2 dimensions, std::string name, CColor fill, std::shared_ptr<Shader> shader) 
	: Layer()
{
	generateUEID();
	this->name = name;
	highlight = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	setTransform(glm::vec3(0.0f, 0.0f, 0.0f), Bounds(0, 0, dimensions.x, dimensions.y), 0.0f);
	setTransform(0, 0, dimensions.x, dimensions.y);
	setVertData(false);
	initializeData(dimensions.x * dimensions.y);
	fillData(dimensions.x * dimensions.y, fill.makeCColor_uc());
	data[0] = 255; data[8] = 255;
	data[1] = 255; data[9] = 255;
	clearLocks();
	this->shader = shader;
	generateBuffers(&(this->VAO), &(this->VBO), &(this->EBO), &(this->TEX0));
	bindBuffers();
	bindTexture(transform.boundBox.x2, transform.boundBox.y2);

	std::cout << "NEW::DATALAYER::UEID=" << UEID 
		<< "::VBO=" << this->VBO
		<< "::NAME=" << name.c_str() 
		<< "::COLOR=" << static_cast<int>(fill.r) << ", "
		<< static_cast<int>(fill.g) << ", "
		<< static_cast<int>(fill.b) << ", " << std::endl;
}

void DataLayer::draw(ShaderTransform* xform)
{
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(xform->m, transform.pos);
	shader->use();
	shader->setInt("texture1", 0);
	shader->setMat4("projection", xform->p);
	shader->setMat4("view", xform->v);
	shader->setMat4("model", modelMatrix);
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TEX0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	if ((int)children.size() != 0)
	{
		for (std::weak_ptr<Fragment> child : children)
		{
			auto frag = child.lock();
			frag->draw(xform);
		}
	}
}

void DataLayer::render(ShaderTransform* xform, unsigned int targetBuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, targetBuffer);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(xform->m, transform.pos);
	shader->use();
	shader->setInt("texture1", 0);
	shader->setMat4("projection", xform->p);
	shader->setMat4("view", xform->v);
	shader->setMat4("model", modelMatrix);
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TEX0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	if (children.size() != 0)
	{
		for (std::weak_ptr<Fragment> child : children)
		{
			auto frag = child.lock();
			frag->render(xform, targetBuffer);
		}
	}
}