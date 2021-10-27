#ifndef VISUALIZER_H
#define VISUALIZER_H

#include "PreviewObj.h"
#include "../../Shader.h"
#include "../../math/blend.h"
#include "../../math/transform.h"

#include <glm.hpp>
#include <memory>
#include <map>

enum class PreviewLayerType : int {
	inputPoint = 0,
	inputLine = 1
};

class Visualizer
{
private:
	bool enablePreview_all = true;
	bool enablePreview_inputLines = false;
	bool enablePreview_inputPoints = false;
	std::shared_ptr<Shader> previewShader = nullptr;

	const int inputPointAttribs = 14;
	const int inputPointStride = inputPointAttribs * sizeof(float);
	const int inputPointOffset_dir = 3 * sizeof(float);
	const int inputPointOffset_uv = 6 * sizeof(float);
	const int inputPointOffset_color = 8 * sizeof(float);
	const int inputPointOffset_size = 12 * sizeof(float);
	const int inputPointOffset_shape = 13 * sizeof(float);
	unsigned int inputPointVAO = 0; 
	unsigned int inputPointVBO = 0;
	unsigned int inputPointTEX0 = 0;
	bool inputPointsDirty = true;
	//
	unsigned int inputLineVAO = 0;
	unsigned int inputLineVBO = 0;
	unsigned int inputLineTEX0 = 0;
	bool inputLinesDirty = true;
	//
	std::pair<unsigned int, unsigned int> inputLinesRange = std::pair<unsigned int, unsigned int>(5000, 1000);
	std::vector<unsigned int> inputLinesAlloc;
	std::pair<unsigned int, unsigned int> inputPointsRange = std::pair<unsigned int, unsigned int>(6000, 1000);
	std::vector<unsigned int> inputPointsAlloc;

	std::map<unsigned int, std::pair<std::vector<PreviewObj>, BlendMode>> previewLayers;
public:
	Visualizer();
	void setPreviewShader(std::shared_ptr<Shader> previewShader);
	bool setPreview_all(bool set);
	bool setPreview_inputLines(bool set);
	bool setPreview_inputPoints(bool set);
	unsigned int requestNewLayer(PreviewLayerType containerType);
	// toggleLayer
	// pauseLayer
	unsigned int addLayer(unsigned int layer, size_t initialSize, BlendMode blendMode);
	void addLayerObject(unsigned int layer, PreviewObj obj);
	void updateLayerObject(unsigned int layer, size_t index, PreviewObj obj);
	void dirtyVertexData(unsigned int layer);
	void updateVertexData_inputPoints(glm::ivec2* windowDimensions);
	void updateVertexData_inputLines(glm::ivec2* windowDimensions);
	void removeLayer(unsigned int layer);
	// removeLayerObject
	// toggleDraw
	// pauseDraw
	// clearLayers
	void draw(ShaderTransform* xform, glm::ivec2* windowDimensions);
	void drawInputPoints(ShaderTransform* xform, const std::vector<PreviewObj>* layer, BlendMode blend);
	void drawInputLines(ShaderTransform* xform, const std::vector<PreviewObj>* layer, BlendMode blend);
};
#endif // !VISUALIZER_H
