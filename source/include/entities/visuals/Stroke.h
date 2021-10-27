#ifndef STROKE_H
#define STROKE_H

#include "Fragment.h"

#include "StrokeShard.h"
#include "../../math/Color.h"
#include "../../tool/ToolSettings.h"
#include "../../tool/toolSettings/ToolSettings_Forward.h"

class Layer;
class Tool;

#include <memory>

class Stroke : public Fragment, public std::enable_shared_from_this<Stroke>
{
private:
protected:
	// Composite vert and texture data
	float compositeVerts[20] = {
		// positions			// texCoords
		-1.0f,  1.0f, 0.0f,		0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f,		0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,		1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,		0.0f, 1.0f,

	};
	glm::ivec3 compositePos = glm::ivec3(0, 0, 0);
	glm::ivec2 compositeSize = glm::ivec2(0, 0);
	unsigned int compVAO = 0, compEBO = 0, compVBO = 0, compBuffer = 0, compColorBuffer = 0;
	unsigned int compTempVAO = 0, compTempEBO = 0, compTempVBO = 0, compTempBuffer = 0, compTempColorBuffer = 0;
	int compMinX = 0, compMaxX = 0, compMinY = 0, compMaxY = 0;

	// Temporarily testing variables for testing stroke opacity & framebuffering
	float quadVertices[24] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	unsigned int quadVAO = 0, quadVBO = 0;
	unsigned int tempVAO = 0, tempVBO = 0;
	unsigned int tempbuffer = 0;
	unsigned int framebuffer = 0;
	unsigned int tempColorbuffer = 0;
	unsigned int frameColorbuffer = 0;
	unsigned int tempRBO = 0;
	unsigned int rbo = 0;
	float* strokeImageData = nullptr;
	float* tempImageData = nullptr;
	int compositePadding = 0;

	// Stroke Data
	int dataID = 0;
	std::vector<char> serialSettingsData;
	std::vector<StrokeShard> shards;
	float shardValue_min = 1.0f;
	float shardValue_max = 0.0f;
	glm::vec3 averageDir = glm::vec3(0.0f);
	glm::vec2 totalPos = glm::vec2(0.0f);
	glm::vec3 averagePos = glm::vec3(0.0f);

	bool rebuildBounds = true;
	float maskDir_cutoffFactor = 10.0f;
	float maskDir_spacingFactor = 8.0f;
	TSetProp maskDir_cutoffMode = TSetProp::null;
	glm::vec2 fillX = glm::vec2(0.0f);
	glm::vec2 fillY = glm::vec2(0.0f);
	glm::vec3 center = glm::vec3(0.0f);
	glm::vec3 dir_average = glm::vec3(0.0f);
	glm::vec3 dirIntersectA = glm::vec3(0.0f);
	glm::vec3 dirIntersectB = glm::vec3(0.0f);
	float stretchLength = 0.0f;
	bool isPortrait = false;

	// Shard Management
	int lastShardID = 0;
	int storedEntityCount = 0;
	int storedSplineCount = 0;
	float storedLength = 0.0f;

	// Rendering Management
	bool drawShapeLines = false;
	float* lineLoop = nullptr;
	int lineLoopLen = 0;
	int lineSize = 18;
	unsigned int linesVAO = 0, linesVBO = 0;

	// Inherited Tool Settings
	TSet_Basic basic;
	TSet_Image image;
	TSet_Character character;
	TSet_Alpha alpha;
	TSet_Color color;
	TSet_Scatter scatter;
	TSet_Effects effects;

public:
	// Constructor / Destructor
	Stroke();
	Stroke(std::shared_ptr<Layer> container, std::shared_ptr<Shader> shader, std::shared_ptr<Tool> tool, int entityCount);
	void initializeDebugData();
	~Stroke();

	std::weak_ptr<Layer> container;

	std::shared_ptr<Shader> frameShader;
	std::shared_ptr<Shader> compositeShader;
	std::shared_ptr<Shader> compositeFrameShader;
	std::shared_ptr<Shader> debugLineShader;

	// Shard Modification Functions
	void updateGenData(int lastShard, glm::vec3 pos, glm::vec3 dir, float count, float trueSpacing, float tipSize);

	// Container Functions
	void generateShards(int& shardCount, int& lastShardID, glm::vec3 pos, glm::vec3 dir, glm::vec4 scale, Input input);
	void createNewShard(int* ID, glm::vec3* pos, glm::vec3* dir, glm::vec4* scale,
		glm::vec2* outVert0, glm::vec2* outVert1, glm::vec2* outVert2, glm::vec2* outVert3,
		CColor* color, float* opacity, float* flow, Input* input);

	// Shard Management Functions
	void processNewAnchor();
	void batchProcessNewAnchor();
	void rebuildFragmentShards();
	void rebuildAnchorShards(int anchorID);

	// Data Functions
	void setVertData_stroke(float size, float scaleX, float scaleY);
	void setVertData_composite(int width, int height);
	void setCompositePos(float x, float y);
	void reframeCompositeData(float widthNew, float heightNew, float xposNew, float yposNew);
	void cleanup_stroke();

	// Render Functions
	void setCompositeShaderUniforms(ShaderTransform* xform);
	void setLineDraw(std::vector<glm::vec3> lines);
	void disableLineDraw();
	virtual void render(ShaderTransform* xform, unsigned int targetBuffer);
	virtual void draw(ShaderTransform* xform);
	void drawDebugData(ShaderTransform* xform);
};

#endif