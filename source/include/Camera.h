#ifndef CAMERA_H
#define CAMERA_H

#include <glm.hpp>

#include "math/transform.h"

class Camera
{
private:
	float maxZoom = 0.0f;
	float zoomSens = 0.0f;
	float panSens = 0.0f;
	float rollSens = 0.0f;
	float zoomSpeed = 0.0f;
	float panSpeed = 0.0f;
	float rollSpeed = 0.0f;
	float momentum = 0.0f;
	glm::vec2 slideDir = glm::vec2(0.0f, 0.0f);
	float friction = 0.998f;
	float decayHigh = 0.1f;
	float decayLow = 0.02f;
	float momentumCap = 2400.0f;
	float yaw = -90.0f;
	float pitch = 0.0f;
	//float roll = 90.0f;
	float roll = 0.0f;
	glm::vec3 pos = glm::vec3(0.0f);
	glm::vec3 centerPos = glm::vec3(0.0f);
	float xoffset = 0.0f;
	float yoffset = 0.0f;
	ObjectTransform camXform = ObjectTransform();
	float orthoX = 0.0f;
	float orthoY = 0.0f;
	float orthoZoom = 0.0f;
	float clipStart = 0.0f;
	float clipEnd = 0.0f;
	// Transform Matricies (Typically given to shaders)
	ShaderTransform transform = ShaderTransform();

protected:
public:
	Camera(float zSens, float pSens, float rSens, float winRatio, glm::ivec2 winDimensions);
	~Camera();
	// Update Camera Settings
	void setSensitivity(float zSens, float pSens, float rSens);
	void updateSpeed(float delta);
	// Update Camera Position & Direction
	void setPosition(float factor);
	void setPosition(float x, float y);
	void setPosition(glm::vec3 pos1, glm::vec3 pos2);
	void setPosition(glm::vec3 vec);
	void setCenterPosition(glm::vec3 vec);
	void centerToCanvas(glm::ivec2 dimensions);
	void setDirections(glm::vec3 fwdvec, float rollAngle);
	void setRoll(float r);
	void addRoll(float r);
	// Camera Physics Functions
	void addMomentum(glm::vec2 dir, float amount);
	void updateMomentum(bool shouldStop, float delta);
	void clearMomentum();
	// Set Camera Ortho Projection
	void zoomExtents(glm::ivec2 dimensions);
	void setOrthoXY(float wRatio);
	void setMaxZoom(float width, float height);
	void setOrthoZoom(float z);
	void setClippingRange(float near, float far);
	// Set Shader Transforms
	void setProjection();
	void setView();
	void setModel();
	glm::vec3 translateWorldPos(glm::vec3 pos);
	// Return Functions
	float getZoomSens();
	float getPanSens();
	float getRollSens();
	float getMaxZoom();
	float getOrthoZoom();
	float getRoll();
	glm::mat4 getView();
	glm::mat4 getProjection();
	glm::mat4 getModel();
	ShaderTransform* getShaderTransform();
	glm::vec3 getPosition();
	glm::vec3 getCenterPosition();
	ObjectTransform* getTransform();
};

#endif