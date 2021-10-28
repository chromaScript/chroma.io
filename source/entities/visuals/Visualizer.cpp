#include "../../include/gladHelper.h"
#include "../../include/math/string.h"

#include "../../include/entities/visuals/Visualizer.h"

Visualizer::Visualizer()
{
	this->previewShader = previewShader;
	inputLinesAlloc.reserve(100);
	for (unsigned int i = inputLinesRange.first; i < inputLinesRange.first + 100; i++) { inputLinesAlloc.push_back(i); }
	inputPointsAlloc.reserve(100);
	for (unsigned int i = inputPointsRange.first; i < inputPointsRange.first + 100; i++) { inputPointsAlloc.push_back(i); }
	inputBoundsAlloc.reserve(100);
	for (unsigned int i = inputBoundsRange.first; i < inputBoundsRange.first + 100; i++) { inputBoundsAlloc.push_back(i); }

	generateBuffers();
}

void Visualizer::releaseBuffers()
{
	glDeleteVertexArrays(1, &inputPointVAO);
	glDeleteBuffers(1, &inputPointVBO);
	glDeleteVertexArrays(1, &inputLineVAO);
	glDeleteBuffers(1, &inputLineVBO);
	glDeleteVertexArrays(1, &inputBoundsVAO);
	glDeleteBuffers(1, &inputBoundsVBO);
}

void Visualizer::generateBuffers()
{
	glm::ivec2 emptyDimensions = glm::ivec2(0);
	glGenVertexArrays(1, &inputPointVAO);
	glGenBuffers(1, &inputPointVBO);
	// Set vertex attributes
	bindAttribs_typeA(&inputPointVAO, &inputPointVBO);
	generateVertexData_typeA(&inputPointVAO, &inputPointVBO, &emptyDimensions, nullptr);

	glGenVertexArrays(1, &inputLineVAO);
	glGenBuffers(1, &inputLineVBO);
	// Set vertex attributes
	bindAttribs_typeA(&inputLineVAO, &inputLineVBO);
	generateVertexData_typeA(&inputLineVAO, &inputLineVBO, &emptyDimensions, nullptr);

	glGenVertexArrays(1, &inputBoundsVAO);
	glGenBuffers(1, &inputBoundsVBO);
	// Set vertex attributes
	bindAttribs_typeA(&inputBoundsVAO, &inputBoundsVBO);
	generateVertexData_typeA(&inputBoundsVAO, &inputBoundsVBO, &emptyDimensions, nullptr);
}

void Visualizer::bindAttribs_typeA(unsigned int* VAO, unsigned int* VBO)
{
	// Set vertex attributes
	glBindVertexArray(*VAO);
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, inputTypeAStride, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, inputTypeAStride, (void*)inputTypeAOffset_dir);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, inputTypeAStride, (void*)inputTypeAOffset_uv);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, inputTypeAStride, (void*)inputTypeAOffset_color);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, inputTypeAStride, (void*)inputTypeAOffset_size);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, inputTypeAStride, (void*)inputTypeAOffset_shape);
	glEnableVertexAttribArray(5);
	glBindVertexArray(0);
}

void Visualizer::resetVisualizer()
{
	clearLayers();
	releaseBuffers();
	generateBuffers();
}

void Visualizer::setPreviewShader(std::shared_ptr<Shader> previewShader)
{
	this->previewShader = previewShader;
}

bool Visualizer::setPreview_all(bool set)
{
	enablePreview_all = set;
	return enablePreview_all;
}
bool Visualizer::setPreview(PreviewLayerType select, bool set)
{
	switch (select) {
	case PreviewLayerType::inputPoint: 
		enablePreview_inputPoints = set; 
		if (set) { inputPointsDirty = true; }
		return enablePreview_inputPoints; break;
	case PreviewLayerType::inputLine: 
		enablePreview_inputLines = set; 
		if (set) { inputLinesDirty = true; }
		return enablePreview_inputLines; break;
	case PreviewLayerType::inputBounds: 
		enablePreview_inputBounds = set; 
		if (set) { inputBoundsDirty = true; }
		return enablePreview_inputBounds; break;
	}
}

unsigned int Visualizer::requestNewLayer(PreviewLayerType containerType)
{
	unsigned int layerID = 0;
	if (containerType == PreviewLayerType::inputPoint) {
		if (inputPointsAlloc.size() > 0) {
			layerID = inputPointsAlloc.front();
			inputPointsAlloc.erase(inputPointsAlloc.begin());
		}
	}
	else if (containerType == PreviewLayerType::inputLine) {
		if (inputLinesAlloc.size() > 0) {
			layerID = inputLinesAlloc.front();
			inputLinesAlloc.erase(inputLinesAlloc.begin());
		}
	}
	else if (containerType == PreviewLayerType::inputBounds) {
		if (inputBoundsAlloc.size() > 0) {
			layerID = inputBoundsAlloc.front();
			inputBoundsAlloc.erase(inputBoundsAlloc.begin());
		}
	}
	return layerID;
}

unsigned int Visualizer::addLayer(unsigned int layer, size_t initialSize, BlendMode blendMode)
{
	if (previewLayers.count(layer) == 1) { return 0; }
	std::vector<PreviewObj> layerObj;
	layerObj.reserve(initialSize);
	for (int i = 0; i < initialSize; i++) { layerObj.push_back(PreviewObj()); }
	previewLayers.insert(
		std::pair<unsigned int, std::pair<std::vector<PreviewObj>, BlendMode>>(
			layer, std::pair<std::vector<PreviewObj>, BlendMode>(layerObj, blendMode)
			));
	dirtyVertexData(layer);
	return layer;
}

void Visualizer::addLayerObject(unsigned int layer, PreviewObj obj)
{
	if (previewLayers.count(layer) == 1) {
		previewLayers.at(layer).first.push_back(obj);
	}
}

void Visualizer::removeLayer(unsigned int layer)
{
	if (previewLayers.count(layer) == 1) {
		if (layer >= inputLinesRange.first && layer < inputLinesRange.first + inputLinesRange.second) {
			inputLinesAlloc.push_back(layer);
		}
		else if (layer >= inputPointsRange.first && layer < inputPointsRange.first + inputPointsRange.second) {
			inputPointsAlloc.push_back(layer);
		}
		else if (layer >= inputBoundsRange.first && layer < inputBoundsRange.first + inputBoundsRange.second) {
			inputBoundsAlloc.push_back(layer);
		}
		previewLayers.erase(layer);
	}
}
void Visualizer::clearLayers()
{
	for (auto const& layer : previewLayers) {
		removeLayer(layer.first);
	}
	glm::ivec2 emptyDimensions = glm::ivec2(0);
	//generateVertexData_typeA(&inputPointVAO, &inputPointVBO, &emptyDimensions, nullptr);
	//generateVertexData_typeA(&inputLineVAO, &inputLineVBO, &emptyDimensions, nullptr);
	//generateVertexData_typeA(&inputBoundsVAO, &inputBoundsVBO, &emptyDimensions, nullptr);
	previewLayers.clear();
	inputPointsDirty = inputLinesDirty = inputBoundsDirty = true;
	enablePreview_inputPoints = enablePreview_inputLines = enablePreview_inputBounds = false;
}

void Visualizer::updateLayerObject(unsigned int layer, size_t index, PreviewObj obj)
{
	if (previewLayers.count(layer) == 1 && previewLayers.at(layer).first.size() > index) {
		previewLayers.at(layer).first[index] = obj;
		dirtyVertexData(layer);
	}
}
void Visualizer::putLayerObject(unsigned int layer, size_t index, PreviewObj obj)
{
	if (previewLayers.count(layer) == 1) {
		if (previewLayers.at(layer).first.size() > index) {
			previewLayers.at(layer).first[index] = obj;
			dirtyVertexData(layer);
		}
		else {
			addLayerObject(layer, obj);
			dirtyVertexData(layer);
		}
	}
}
void Visualizer::putBoundsObject(unsigned int layer, EntityTransform transform)
{
	if (previewLayers.count(layer) == 1) {
		float rad = glm::radians(transform.roll);
		glm::vec3 boundsDir = glm::vec3(cos(rad), sin(rad), 0.0f);
		glm::quat rotation = glm::quat(glm::vec3(0.0f, 0.0f, rad));
		glm::vec3 origin = transform.origin;
		glm::vec3 p1 = origin + (rotation * (transform.bounds.p1 - origin));
		glm::vec3 p2 = origin + (rotation * (transform.bounds.p2 - origin));
		glm::vec3 p3 = origin + (rotation * (transform.bounds.p3 - origin));
		glm::vec3 p4 = origin + (rotation * (transform.bounds.p4 - origin));
		// Create Lines
		putLayerObject(layer, 0,
			PreviewObj(0, p1, p2, blue, ShapeType::line, 1.0f));
		putLayerObject(layer, 1,
			PreviewObj(0, p2, p3, blue, ShapeType::line, 1.0f));
		putLayerObject(layer, 2,
			PreviewObj(0, p3, p4, blue, ShapeType::line, 1.0f));
		putLayerObject(layer, 3,
			PreviewObj(0, p4, p1, blue, ShapeType::line, 1.0f));
		// Create Points
		putLayerObject(layer, 4,
			PreviewObj(4, p1, boundsDir, blue, ShapeType::square, 6.0f));
		putLayerObject(layer, 5,
			PreviewObj(4, p2, boundsDir, blue, ShapeType::square, 6.0f));
		putLayerObject(layer, 6,
			PreviewObj(4, p3, boundsDir, blue, ShapeType::square, 6.0f));
		putLayerObject(layer, 7,
			PreviewObj(4, p4, boundsDir, blue, ShapeType::square, 6.0f));
	}
}

void Visualizer::dirtyVertexData(unsigned int layer)
{
	if (layer >= inputLinesRange.first && layer < inputLinesRange.first + inputLinesRange.second) {
		inputLinesDirty = true;
	}
	else if (layer >= inputPointsRange.first && layer < inputPointsRange.first + inputPointsRange.second) {
		inputPointsDirty = true;
	}
	else if (layer >= inputBoundsRange.first && layer < inputBoundsRange.first + inputBoundsRange.second) {
		inputBoundsDirty = true;
	}
}

void Visualizer::updateVertexData_inputPoints(glm::ivec2* windowDimensions)
{
	std::vector<PreviewObj> objList;
	for (auto const& item : previewLayers) {
		if (item.first >= inputPointsRange.first && item.first < inputPointsRange.first + inputPointsRange.second) {
			objList.insert(objList.end(), item.second.first.begin(), item.second.first.end());
		}
	}
	bool result = generateVertexData_typeA(&inputPointVAO, &inputPointVBO, windowDimensions, &objList);
	if (result) { inputPointsDirty = false; }
}

void Visualizer::updateVertexData_inputLines(glm::ivec2* windowDimensions)
{
	std::vector<PreviewObj> objList;
	for (auto const& item : previewLayers) {
		if (item.first >= inputLinesRange.first && item.first < inputLinesRange.first + inputLinesRange.second) {
			objList.insert(objList.end(), item.second.first.begin(), item.second.first.end());
		}
	}
	bool result = generateVertexData_typeA(&inputLineVAO, &inputLineVBO, windowDimensions, &objList);
	if (result) { inputLinesDirty = false; }
}

void Visualizer::updateVertexData_inputBounds(glm::ivec2* windowDimensions)
{
	std::vector<PreviewObj> objList;
	for (auto const& item : previewLayers) {
		if (item.first >= inputBoundsRange.first && item.first < inputBoundsRange.first + inputBoundsRange.second) {
			objList.insert(objList.end(), item.second.first.begin(), item.second.first.end());
		}
	}
	bool result = generateVertexData_typeA(&inputBoundsVAO, &inputBoundsVBO, windowDimensions, &objList);
	if (result) { inputBoundsDirty = false; }
}

bool Visualizer::generateVertexData_typeA(unsigned int* VAO, unsigned int* VBO, glm::ivec2* windowDimensions, std::vector<PreviewObj>* objList)
{
	if (objList != nullptr && objList->size() > 0) {
		std::vector<PreviewObj>& list = *objList;
		float* vertexData = new float[inputTypeAAttribs * objList->size()];
		for (int i = 0; i < objList->size(); i++) {
			vertexData[(i * inputTypeAAttribs) + 0] = list[i].p1.x;
			vertexData[(i * inputTypeAAttribs) + 1] = list[i].p1.y;
			vertexData[(i * inputTypeAAttribs) + 2] = list[i].p1.z;
			vertexData[(i * inputTypeAAttribs) + 3] = list[i].p2.x;
			vertexData[(i * inputTypeAAttribs) + 4] = list[i].p2.y;
			vertexData[(i * inputTypeAAttribs) + 5] = list[i].p2.z;
			vertexData[(i * inputTypeAAttribs) + 6] = 0.0f;
			vertexData[(i * inputTypeAAttribs) + 7] = 0.0f;
			vertexData[(i * inputTypeAAttribs) + 8] = list[i].color.r;
			vertexData[(i * inputTypeAAttribs) + 9] = list[i].color.g;
			vertexData[(i * inputTypeAAttribs) + 10] = list[i].color.b;
			vertexData[(i * inputTypeAAttribs) + 11] = list[i].color.a;
			vertexData[(i * inputTypeAAttribs) + 12] =
				((list[i].size / (float)windowDimensions->x) +
					(list[i].size / (float)windowDimensions->y)) / 2.0f;
			vertexData[(i * inputTypeAAttribs) + 13] = (float)static_cast<int>(list[i].shape);
		}
		// Bind vertex array and buffers with data
		glBindVertexArray(*VAO);
		glBindBuffer(GL_ARRAY_BUFFER, *VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 14 * objList->size(), vertexData, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		delete[]vertexData;
		return true;
	}
	else {
		glBindVertexArray(*VAO);
		glBindBuffer(GL_ARRAY_BUFFER, *VBO);
		glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		return false;
	}
}

void Visualizer::draw(ShaderTransform* xform, glm::ivec2* windowDimensions)
{
	if (!enablePreview_all || previewLayers.size() == 0) { return; }
	for (auto const& item : previewLayers) {
		if (item.first >= inputLinesRange.first && item.first < inputLinesRange.first + inputLinesRange.second) {
			if (inputLinesDirty) { updateVertexData_inputLines(windowDimensions); }
			if (enablePreview_inputLines && !inputLinesDirty) {
				drawInputData_typeA(&inputLineVAO, xform, &item.second.first, item.second.second);
			}
		}
		else if (item.first >= inputPointsRange.first && item.first < inputPointsRange.first + inputPointsRange.second) {
			if (inputPointsDirty) { updateVertexData_inputPoints(windowDimensions); }
			if (enablePreview_inputPoints && !inputPointsDirty) {
				drawInputData_typeA(&inputPointVAO, xform, &item.second.first, item.second.second);
			}
		}
		else if (item.first >= inputBoundsRange.first && item.first < inputBoundsRange.first + inputBoundsRange.second) {
			if (inputBoundsDirty) { updateVertexData_inputBounds(windowDimensions); }
			if (enablePreview_inputBounds && !inputBoundsDirty) {
				drawInputData_typeA(&inputBoundsVAO, xform, &item.second.first, item.second.second);
			}
		}
	}
}

void Visualizer::drawInputData_typeA(unsigned int* VAO, ShaderTransform* xform, const std::vector<PreviewObj>* layer, BlendMode blend)
{
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(xform->m, glm::vec3(0.0f));
	previewShader->use();
	previewShader->setMat4("projection", xform->p);
	previewShader->setMat4("view", xform->v);
	previewShader->setMat4("model", modelMatrix);
	glBindVertexArray(*VAO);
	glDrawArrays(GL_POINTS, 0, layer->size());
	glBindVertexArray(0);
}