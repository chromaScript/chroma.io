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

	glGenVertexArrays(1, &inputPointVAO);
	glGenBuffers(1, &inputPointVBO);
	glGenTextures(1, &inputPointTEX0);
	// Set vertex attributes
	glBindVertexArray(inputPointVAO);
	glBindBuffer(GL_ARRAY_BUFFER, inputPointVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, inputPointStride, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, inputPointStride, (void*)inputPointOffset_dir);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, inputPointStride, (void*)inputPointOffset_uv);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, inputPointStride, (void*)inputPointOffset_color);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, inputPointStride, (void*)inputPointOffset_size);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, inputPointStride, (void*)inputPointOffset_shape);
	glEnableVertexAttribArray(5);
	glBindVertexArray(0);

	glGenVertexArrays(1, &inputLineVAO);
	glGenBuffers(1, &inputLineVBO);
	glGenTextures(1, &inputLineTEX0);
	// Set vertex attributes
	glBindVertexArray(inputLineVAO);
	glBindBuffer(GL_ARRAY_BUFFER, inputLineVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, inputPointStride, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, inputPointStride, (void*)inputPointOffset_dir);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, inputPointStride, (void*)inputPointOffset_uv);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, inputPointStride, (void*)inputPointOffset_color);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, inputPointStride, (void*)inputPointOffset_size);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, inputPointStride, (void*)inputPointOffset_shape);
	glEnableVertexAttribArray(5);
	glBindVertexArray(0);
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
bool Visualizer::setPreview_inputLines(bool set)
{
	enablePreview_inputLines = set;
	return enablePreview_inputLines;
}
bool Visualizer::setPreview_inputPoints(bool set)
{
	enablePreview_inputPoints = set;
	return enablePreview_inputPoints;
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
		previewLayers.erase(layer);
	}
}

void Visualizer::updateLayerObject(unsigned int layer, size_t index, PreviewObj obj)
{
	if (previewLayers.count(layer) == 1 && previewLayers.at(layer).first.size() > index) {
		previewLayers.at(layer).first[index] = obj;
		dirtyVertexData(layer);
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
}

void Visualizer::updateVertexData_inputPoints(glm::ivec2* windowDimensions)
{
	std::vector<PreviewObj> objList;
	for (auto const& item : previewLayers) {
		if (item.first >= inputPointsRange.first && item.first < inputPointsRange.first + inputPointsRange.second) {
			objList.insert(objList.end(), item.second.first.begin(), item.second.first.end());
		}
	}
	if (objList.size() > 0) {
		float* vertexData = new float[inputPointAttribs * objList.size()];
		for (int i = 0; i < objList.size(); i++) {
			vertexData[(i * inputPointAttribs) + 0] = objList[i].p1.x;
			vertexData[(i * inputPointAttribs) + 1] = objList[i].p1.y;
			vertexData[(i * inputPointAttribs) + 2] = objList[i].p1.z;
			vertexData[(i * inputPointAttribs) + 3] = objList[i].p2.x;
			vertexData[(i * inputPointAttribs) + 4] = objList[i].p2.y;
			vertexData[(i * inputPointAttribs) + 5] = objList[i].p2.z;
			vertexData[(i * inputPointAttribs) + 6] = 0.0f;
			vertexData[(i * inputPointAttribs) + 7] = 0.0f;
			vertexData[(i * inputPointAttribs) + 8] = objList[i].color.r;
			vertexData[(i * inputPointAttribs) + 9] = objList[i].color.g;
			vertexData[(i * inputPointAttribs) + 10] = objList[i].color.b;
			vertexData[(i * inputPointAttribs) + 11] = objList[i].color.a;
			vertexData[(i * inputPointAttribs) + 12] =
				((objList[i].size / (float)windowDimensions->x) + 
				(objList[i].size / (float)windowDimensions->y)) / 2.0f;
			vertexData[(i * inputPointAttribs) + 13] = (float)static_cast<int>(objList[i].shape);
		}
		// Bind vertex array and buffers with data
		glBindVertexArray(inputPointVAO);
		glBindBuffer(GL_ARRAY_BUFFER, inputPointVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 14 * objList.size(), vertexData, GL_STATIC_DRAW);
		glBindVertexArray(0);
		delete[]vertexData;
	}
	else {
		glBindVertexArray(inputPointVAO);
		glBindBuffer(GL_ARRAY_BUFFER, inputPointVBO);
		glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW);
		glBindVertexArray(0);
	}
	inputPointsDirty = false;
}

void Visualizer::updateVertexData_inputLines(glm::ivec2* windowDimensions)
{
	std::vector<PreviewObj> objList;
	for (auto const& item : previewLayers) {
		if (item.first >= inputLinesRange.first && item.first < inputLinesRange.first + inputLinesRange.second) {
			objList.insert(objList.end(), item.second.first.begin(), item.second.first.end());
		}
	}
	if (objList.size() > 0) {
		float* vertexData = new float[inputPointAttribs * objList.size()];
		for (int i = 0; i < objList.size(); i++) {
			vertexData[(i * inputPointAttribs) + 0] = objList[i].p1.x;
			vertexData[(i * inputPointAttribs) + 1] = objList[i].p1.y;
			vertexData[(i * inputPointAttribs) + 2] = objList[i].p1.z;
			vertexData[(i * inputPointAttribs) + 3] = objList[i].p2.x;
			vertexData[(i * inputPointAttribs) + 4] = objList[i].p2.y;
			vertexData[(i * inputPointAttribs) + 5] = objList[i].p2.z;
			vertexData[(i * inputPointAttribs) + 6] = 0.0f;
			vertexData[(i * inputPointAttribs) + 7] = 0.0f;
			vertexData[(i * inputPointAttribs) + 8] = objList[i].color.r;
			vertexData[(i * inputPointAttribs) + 9] = objList[i].color.g;
			vertexData[(i * inputPointAttribs) + 10] = objList[i].color.b;
			vertexData[(i * inputPointAttribs) + 11] = objList[i].color.a;
			vertexData[(i * inputPointAttribs) + 12] =
				((objList[i].size / (float)windowDimensions->x) +
					(objList[i].size / (float)windowDimensions->y)) / 2.0f;
			vertexData[(i * inputPointAttribs) + 13] = (float)static_cast<int>(objList[i].shape);
		}
		// Bind vertex array and buffers with data
		glBindVertexArray(inputLineVAO);
		glBindBuffer(GL_ARRAY_BUFFER, inputLineVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 14 * objList.size(), vertexData, GL_STATIC_DRAW);
		glBindVertexArray(0);
		delete[]vertexData;
	}
	else {
		glBindVertexArray(inputLineVAO);
		glBindBuffer(GL_ARRAY_BUFFER, inputLineVBO);
		glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW);
		glBindVertexArray(0);
	}
	inputPointsDirty = false;
}

void Visualizer::draw(ShaderTransform* xform, glm::ivec2* windowDimensions)
{
	if (!enablePreview_all || previewLayers.size() == 0) { return; }
	if (enablePreview_inputPoints && inputPointsDirty) { updateVertexData_inputPoints(windowDimensions); }
	if (enablePreview_inputLines && inputLinesDirty) { updateVertexData_inputLines(windowDimensions); }
	for (auto const& item : previewLayers) {
		if (item.first >= inputLinesRange.first && item.first < inputLinesRange.first + inputLinesRange.second) {
			if (enablePreview_inputLines) {
				drawInputLines(xform, &item.second.first, item.second.second);
			}
		}
		else if (item.first >= inputPointsRange.first && item.first < inputPointsRange.first + inputPointsRange.second) {
			if (enablePreview_inputPoints) {
				drawInputPoints(xform, &item.second.first, item.second.second);
			}
		}
	}
}

void Visualizer::drawInputPoints(ShaderTransform* xform, const std::vector<PreviewObj>* layer, BlendMode blend)
{
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(xform->m, glm::vec3(0.0f));
	previewShader->use();
	previewShader->setMat4("projection", xform->p);
	previewShader->setMat4("view", xform->v);
	previewShader->setMat4("model", modelMatrix);

	glBindVertexArray(inputPointVAO);
	glDrawArrays(GL_POINTS, 0, layer->size());
	glBindVertexArray(0);
}

void Visualizer::drawInputLines(ShaderTransform* xform, const std::vector<PreviewObj>* layer, BlendMode blend)
{
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(xform->m, glm::vec3(0.0f));
	previewShader->use();
	previewShader->setMat4("projection", xform->p);
	previewShader->setMat4("view", xform->v);
	previewShader->setMat4("model", modelMatrix);

	glBindVertexArray(inputLineVAO);
	glDrawArrays(GL_POINTS, 0, layer->size());
	glBindVertexArray(0);
}