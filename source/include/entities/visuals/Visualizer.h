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
	inputLine = 1,
	inputBounds = 2,
	inputCurves = 3
};

class Visualizer
{
private:
	bool enablePreview_all = true;
	std::shared_ptr<Shader> previewShader_pointLines = nullptr;
	std::shared_ptr<Shader> previewShader_curves = nullptr;
	//
	const int inputTypeAAttribs = 14;
	const int inputTypeAStride = inputTypeAAttribs * sizeof(float);
	const int inputTypeAOffset_dir = 3 * sizeof(float);
	const int inputTypeAOffset_uv = 6 * sizeof(float);
	const int inputTypeAOffset_color = 8 * sizeof(float);
	const int inputTypeAOffset_size = 12 * sizeof(float);
	const int inputTypeAOffset_shape = 13 * sizeof(float);
	//
	const int inputTypeBAttribs = 16;
	const int inputTypeBStride = inputTypeBAttribs * sizeof(float);
	const int inputTypeBOffset_startHandle = 2 * sizeof(float);
	const int inputTypeBOffset_endHandle = 4 * sizeof(float);
	const int inputTypeBOffset_endPos = 6 * sizeof(float);
	const int inputTypeBOffset_boundsP1 = 8 * sizeof(float);
	const int inputTypeBOffset_boundsP2 = 10 * sizeof(float);
	const int inputTypeBOffset_boundsP3 = 12 * sizeof(float);
	const int inputTypeBOffset_boundsP4 = 14 * sizeof(float);
	//
	unsigned int inputPointVAO = 0; 
	unsigned int inputPointVBO = 0;
	unsigned int inputPointTEX0 = 0;
	bool inputPointsDirty = true;
	bool enablePreview_inputPoints = false;
	//
	unsigned int inputLineVAO = 0;
	unsigned int inputLineVBO = 0;
	unsigned int inputLineTEX0 = 0;
	bool inputLinesDirty = true;
	bool enablePreview_inputLines = false;
	//
	unsigned int inputBoundsVAO = 0;
	unsigned int inputBoundsVBO = 0;
	unsigned int inputBoundsTEX0 = 0;
	bool inputBoundsDirty = true;
	bool enablePreview_inputBounds = false;
	//
	unsigned int inputCurvesVAO = 0;
	unsigned int inputCurvesVBO = 0;
	unsigned int inputCurvesTEX0 = 0;
	bool inputCurvesDirty = true;
	bool enablePreview_inputCurves = false;
	//
	std::pair<unsigned int, unsigned int> inputCurvesRange = std::pair<unsigned int, unsigned int>(10000, 1000);
	std::vector<unsigned int> inputCurvesAlloc;
	std::pair<unsigned int, unsigned int> inputLinesRange = std::pair<unsigned int, unsigned int>(11000, 1000);
	std::vector<unsigned int> inputLinesAlloc;
	std::pair<unsigned int, unsigned int> inputPointsRange = std::pair<unsigned int, unsigned int>(12000, 1000);
	std::vector<unsigned int> inputPointsAlloc;
	std::pair<unsigned int, unsigned int> inputBoundsRange = std::pair<unsigned int, unsigned int>(18000, 1000);
	std::vector<unsigned int> inputBoundsAlloc;

	std::map<unsigned int, std::pair<std::vector<PreviewObj>, BlendMode>> previewLayers;
public:
	Visualizer();
	//
	void releaseBuffers();
	void generateBuffers();
	void bindAttribs_typeA(unsigned int* VAO, unsigned int* VBO);
	void bindAttribs_typeB(unsigned int* VAO, unsigned int* VBO);
	void setPreviewShaders(
		std::shared_ptr<Shader> previewShader_pointLines, std::shared_ptr<Shader> previewShader_curves);
	//
	bool setPreview_all(bool set);
	bool setPreview(PreviewLayerType select, bool set);
	unsigned int requestNewLayer(PreviewLayerType containerType);
	// toggleLayer
	// pauseLayer
	unsigned int addLayer(unsigned int layer, size_t initialSize, BlendMode blendMode);
	void addLayerObject(unsigned int layer, PreviewObj obj);
	void updateLayerObject(unsigned int layer, size_t index, PreviewObj obj);
	void putLayerObject(unsigned int layer, size_t index, PreviewObj obj);
	void putBoundsObject(unsigned int layer, EntityTransform transform);
	void dirtyVertexData(unsigned int layer);
	void updateVertexData_inputPoints(glm::ivec2* windowDimensions);
	void updateVertexData_inputLines(glm::ivec2* windowDimensions);
	void updateVertexData_inputBounds(glm::ivec2* windowDimensions);
	void updateVertexData_inputCurves(glm::ivec2* windowDimensions);
	bool generateVertexData_typeA(unsigned int* VAO, unsigned int* VBO, 
		glm::ivec2* windowDimensions, std::vector<PreviewObj>* objList);
	bool generateVertexData_typeB(unsigned int* VAO, unsigned int* VBO,
		glm::ivec2* windowDimensions, std::vector<PreviewObj>* objList);
	void removeLayer(unsigned int layer);
	void trimLayer(unsigned int layer, size_t size);
	// removeLayerObject
	// toggleDraw
	// pauseDraw
	void resetVisualizer();
	void clearLayers();
	void draw(ShaderTransform* xform, glm::ivec2* windowDimensions);
	void drawInputData_typeA(
		unsigned int* VAO, ShaderTransform* xform, 
		glm::ivec2* windowDimensions, 
		const std::vector<PreviewObj>* layer, BlendMode blend);
	void drawInputData_typeB(
		unsigned int* VAO, ShaderTransform* xform, 
		glm::ivec2* windowDimensions, 
		const std::vector<PreviewObj>* layer, BlendMode blend);
};
#endif // !VISUALIZER_H
