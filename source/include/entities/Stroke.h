#ifndef STROKE_H
#define STROKE_H

#include "Fragment.h"

#include "../structs.h"
#include "../SharedHelper.h"
#include "../Color.h"
#include "../ToolSettings.h"

#include <memory>

class Stroke : public Fragment
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

	// Tool / Brush settings Note: will directly read from the associated tool later
	int tipSize = 12;
	float tipSpacing = 1.0f;
	float strokeAlpha = 1.0f; // This is the maximum opacity value for the stroke, in which opacityByPressure will operate
	CColor strokeColor = red;
	bool opacityByPressure = true;
	bool sizeByPressure = true;
	bool rotationByDirection = true;
	// Note: May want to add additional textureID slots to this for Dual Brush?

	// Stroke Data
	std::vector<StrokeShard> shards;

	// Shard Management
	int lastShardID = 0;
	float storedLength = 0.0f;

	// Rendering Management
	bool drawShapeLines = false;
	bool drawBoundsLines = false;
	float* lineLoop = nullptr;
	int lineLoopLen = 0;
	int lineSize = 18;
	unsigned int linesVAO = 0, linesVBO = 0;

	// Inherited Tool Settings
	TSet_Basic basic;
	TSet_Image image;
	TSet_Alpha alpha;

public:
	// Constructor / Destructor
	Stroke();
	Stroke(std::shared_ptr<Shader> shader, TSet_Basic* basicPtr, TSet_Image* imagePtr, TSet_Alpha* alphaPtr);
	void initializeDebugData();
	~Stroke();

	std::shared_ptr<Shader> frameShader;
	std::shared_ptr<Shader> compositeShader;
	std::shared_ptr<Shader> compositeFrameShader;
	std::shared_ptr<Shader> debugLineShader;

	// Shard Properties (Usually taken from the tool settings)
	void setTipProperties(int size, float spacing);
	void setRGBA(float r, float g, float b, float a);
	int getTipSize() { return tipSize; }

	// Shard Modification Functions
	CColor modulateColor(float scalar);
	float modulateOpacity(float scalar);
	glm::vec4 modulateScale(float scalar);
	//glm::vec3 lerpDir(glm::vec3 dirA, glm::vec3 dirB, float t);

	// Container Functions
	void createNewShard(int ID, glm::vec3 pos, glm::vec3 dir, glm::vec4 scale,
		CColor color, float opacity, float pressure, float rotation, float tiltx, float tilty, float velocity);

	// Shard Management Functions
	//void processNewAnchor();
	void processNewAnchor();
	void rebuildFragmentShards();
	void rebuildAnchorShards(int anchorID);

	// Data Functions
	void setVertData_stroke(float size);
	void setVertData_composite(int width, int height);
	void setCompositePos(float x, float y);
	void reframeCompositeData(float widthNew, float heightNew, float xposNew, float yposNew);

	// Render Functions
	void setBoundsDraw(EntityTransform transform);
	void disableBoundsDraw();
	void setLineDraw(std::vector<glm::vec3> lines);
	void disableLineDraw();
	virtual void render(ShaderTransform xform, unsigned int targetBuffer);
	virtual void draw(ShaderTransform xform);
	void drawDebugData(ShaderTransform xform);
};

#endif