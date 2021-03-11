#ifndef VISUALENTITY_H
#define VISUALENTITY_H

#include <glm.hpp>

#include "../Color.h"
#include "../structs.h"
#include "../Shader.h"

class VisualEntity
{
private:
	
protected:
	// Constructor / Destructor
	VisualEntity() {}; // VisualEntity Constructor is blank. The final class in the visualEntity chain should have a specialized constructor
	~VisualEntity();

	// Property Variables
	long UEID = 0; // Unique Entity ID
	std::string name; // Object Display Name
	glm::vec4 highlight = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f); // Highlight color for UI
	EntityTransform transform; // Position (XYZ): P2/P1 Corners (width, height, 0.0, 0.0): Roll Angle (z-axis)
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	
	// Render Properties
	int channelCount = 0;
	BlendModes blendMode = BlendModes::normal;
	std::shared_ptr<Shader> shader;

	// Data variables
	unsigned char* data = nullptr; // Raster data for the layer (Non-recoverable with undo/history)
	unsigned char* composite = nullptr; // Preview data for the layer (Composite is made when layer is out of focus, for preview/faster rendering)

	// Entity Vertex Data/Arrangement 
	float vertData[20] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	static const int vertData_size = sizeof(vertData);
	unsigned int vertArrangement[6] = 
	{
		0, 1, 3,  // First Triangle
		1, 2, 3   // Second Triangle
	};
	static const int vertArrangement_size = sizeof(vertArrangement);
	const int attribStride = 5 * sizeof(float);
	const int tAttribOffset = 3 * sizeof(float);

	// Buffer IDs
	unsigned int VAO = 0; // Vertex Array Object ID
	unsigned int VBO = 0; // Vertex Buffer Object ID
	unsigned int EBO = 0; // Elements Buffer Object ID
	unsigned int TEX0 = 0; // Texture Buffer Object ID
public:
	// UEID functions
	void generateUEID();
	long getUEID();

	// Property Functions
	std::string getName();
	glm::vec4 getHighlightColor();

	// Transform functions
	void setTransform(glm::vec3 pos, Bounds rect, float angle);
	void setTransform(glm::vec3 pos);
	void setTransform(Bounds rect);
	void setTransform(float angle);
	void setTransform(int x1, int y1, int x2, int y2);
	EntityTransform getTransform();

	// Dimension functions
	glm::ivec2 getDimensions();
	float getRatio();
	int getiArea();
	virtual void setVertData();

	// Data Functions
	void initializeData(int area);
	void fillData(int area, CColor_uc fill);
	void setData_toImage(const char* texPath, int *width, int *height);
	void clearData(int area);

	// Blend functions
	void setBlendMode(BlendModes mode);
	BlendModes getBlendMode();

	// Render Functions
	void setShader(std::shared_ptr<Shader> program);
	std::shared_ptr<Shader> getShader();
	void generateBuffers(unsigned int* va, unsigned int* vb, unsigned int* eb, unsigned int* tx0);
	void generateBuffers(unsigned int* va, unsigned int* vb, unsigned int* eb);
	virtual void bindBuffers();
	virtual void bindTexture(int width, int height);
	virtual void render(ShaderTransform xform, unsigned int targetBuffer) = 0;
	virtual void draw(ShaderTransform xform) = 0;
};

#endif