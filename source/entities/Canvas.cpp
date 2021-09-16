#include "../include/entities/Canvas.h"

#include "../include/Color.h"

#include <glad/glad.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>

#ifndef APPLICATION_H
#include "../include/Application.h"
#endif
#include "../include/entities/DataLayer.h"

#include <memory>

extern std::shared_ptr<Application> chromaIO;

// Constructor / Destructor
Canvas::Canvas(std::string documentName, int width, int height, std::shared_ptr<Shader> shader)
{
	// Properties
	generateUEID();
	this->name = documentName;
	highlight = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	// Transforms, VertData & PxData
	setTransform(0.0f);
	setTransform(glm::vec3(0.0f, 0.0f, 0.0f));
	setTransform(0, 0, width, height);
	setVertData();
	initializeData(width * height);
	clearData(width * height);
	// Shader & Render
	this->shader = shader;
	generateBuffers(&(this->VAO), &(this->VBO), &(this->EBO), &(this->TEX0));
	bindBuffers();
	bindTexture(transform.boundBox.x2, transform.boundBox.y2);
	compositeFrameShader = chromaIO.get()->getCompositeFrameShader();
	compositeShader = chromaIO.get()->getCompositeShader();
	std::cout << "NEW::CANVAS::UEID=" << UEID
		<< "::NAME=" << documentName.c_str() << std::endl;
	// Set up the render target buffer
	glGenFramebuffers(1, &renderBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, renderBuffer);
	// create a color attachment texture
	glGenTextures(1, &renderColorBuffer);
	glBindTexture(GL_TEXTURE_2D, renderColorBuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, transform.boundBox.x2, transform.boundBox.y2, 0, GL_RGBA, GL_FLOAT, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderColorBuffer, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
};

Canvas::~Canvas()
{
	glDeleteBuffers(1, &renderBuffer);
	glDeleteTextures(1, &renderColorBuffer);
	layers.shrink_to_fit();
	compositeFrameShader.reset();
	compositeShader.reset();
}

// Canvas Properties Functions

// Container Functions
std::weak_ptr<Layer> Canvas::newDataLayer(glm::ivec2 dimensions, std::string name, CColor fill, std::shared_ptr<Shader> shader)
{
	layers.emplace_back(std::make_shared<DataLayer>(dimensions, name, fill, shader));
	setActiveLayer(layers.back());
	return activeLayer;
}
void Canvas::setActiveLayer(std::weak_ptr<Layer> select)
{
	activeLayer = select;
}
std::weak_ptr<Layer> Canvas::getActiveLayer()
{
	return activeLayer;
}
bool Canvas::close(bool saveBeforeExit)
{
	if (saveBeforeExit)
	{
		chromaIO.get()->renderCanvas_toFile(nullptr);
	}
	for (std::shared_ptr<Layer> layer : layers)
	{
		layer.get()->clearLayer();
		layer.get()->deleteBuffers();
		layer.reset();
	}
	activeLayer.reset();
	layers.clear();
	return true;
}

// Render Functions
// Activate the shader program, Bind the vertex and texture to use, then Draw Elements (Rectangle)
void Canvas::draw(ShaderTransform xform)
{
	shader->use();
	shader->setInt("texture1", 0);
	shader->setFloat("camZoom", xform.zoom);
	shader->setMat4("projection", xform.p);
	shader->setMat4("view", xform.v);
	shader->setMat4("model", xform.m);
	shader->setInt("checkerSize", checkerSize);
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TEX0);
	// Insert camera transforms later here
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
// Loop through renders for layers
void Canvas::drawLayers(ShaderTransform xform)
{
	for (std::shared_ptr<Layer> layer : layers)
	{
		layer.get()->draw(xform);
	}
}
// Render to file output
float* Canvas::renderCanvas()
{
	render(ShaderTransform(), renderBuffer);
	renderData = new float[(size_t)transform.boundBox.x2 * (size_t)transform.boundBox.y2 * 4];
	glBindFramebuffer(GL_FRAMEBUFFER, renderBuffer);
	glReadPixels(0, 0, transform.boundBox.x2, transform.boundBox.y2, GL_RGBA, GL_FLOAT, renderData);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return renderData;
}
void Canvas::deleteRenderData()
{
	if (sizeof(renderData) > sizeof(float))
	{
		delete[] renderData;
	}
	else
	{
		delete renderData;
	}
}
// Render
void Canvas::render(ShaderTransform xform, unsigned int targetBuffer)
{
	/*
	shader->use();
	shader->setInt("texture1", 0);
	shader->setFloat("camZoom", xform.zoom);
	shader->setMat4("projection", xform.p);
	shader->setMat4("view", xform.v);
	shader->setMat4("model", xform.m);
	shader->setInt("checkerSize", checkerSize);
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TEX0);
	// Insert camera transforms later here
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	*/
	glm::mat4 projection = glm::ortho(float(-transform.boundBox.x2), 0.0f, float(transform.boundBox.y2), 0.0f, -10.0f, 10.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-transform.boundBox.x2, transform.boundBox.y2, 0.0f));
	glViewport(0, 0, transform.boundBox.x2, transform.boundBox.y2);

	glBindFramebuffer(GL_FRAMEBUFFER, renderBuffer);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	
	for (std::shared_ptr<Layer> layer : layers)
	{
		layer.get()->render(ShaderTransform(projection, view, model), renderBuffer);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, chromaIO.get()->getWindowWidth(), chromaIO.get()->getWindowHeight());
}