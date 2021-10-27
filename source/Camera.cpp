#include "include/Camera.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/rotate_vector.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/quaternion.hpp>

#include <iostream>

// Default Constructor, Note: Ratio must be window ratio, not canvas ratio
Camera::Camera(float zSens, float pSens, float rSens, float winRatio, glm::ivec2 winDimensions) : zoomSens(zSens), panSens(pSens), rollSens(rSens)
{
	setCenterPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	centerToCanvas(winDimensions);
	setPosition(glm::vec3(0.0f, 0.0f, 1.0f));
	setOrthoXY(winRatio);
	setMaxZoom(1024.0f, 1024.0f);
	setOrthoZoom(1024.0f);
	setClippingRange(0.01f, 100.0f);
	setDirections(glm::vec3(0.0f, 0.0f, -1.0f), roll);
	setProjection();
	setPosition(float(winDimensions.x * winDimensions.y));
}
Camera::~Camera()
{

}
// Camera Settings Functions
void Camera::setSensitivity(float zSens, float pSens, float rSens)
{
	zoomSens = zSens;
	panSens = pSens;
	rollSens = rSens;
}

void Camera::updateSpeed(float delta)
{
	zoomSpeed = zoomSens * delta;
	panSpeed = panSens * delta;
	rollSpeed = rollSens * delta;
}

float Camera::getZoomSens() { return zoomSens; };
float Camera::getPanSens() { return panSens; };
float Camera::getRollSens() { return rollSens; };

// Camera Position & World Directions
// Set camera position by reduction (Used for window resize)
void Camera::setPosition(float factor)
{
	pos.x = pos.x / factor;
	pos.y = pos.y / factor;
	setView();
	setModel();
}
// Set camera position by adding to current position (Used for camera movement)
void Camera::setPosition(float x, float y)
{
	// Warning: Panning while rotated goes in the direction of the camera up/right, not screen up/right
	if (isnan(x) || isnan(y))
	{
		return;
	}
	else
	{
		pos.x = pos.x + (x * 1);
		pos.y = pos.y + (y * 1);
		setView();
		setModel();
	}
}
void Camera::setPosition(glm::vec3 pos1, glm::vec3 pos2)
{
	// Warning: Panning while rotated goes in the direction of the camera up/right, not screen up/right
	if (isnan(pos1.x) || isnan(pos1.y) || isnan(pos2.x) || isnan(pos2.y))
	{
		return;
	}
	else
	{
		glm::quat rotation = glm::quat(glm::vec3(0, 0, glm::radians(roll)));
		glm::vec3 rotated = pos1 + (rotation * (pos2 - pos1));
		pos.x += pos1.x - rotated.x;
		pos.y += pos1.y - rotated.y;
		setView();
		setModel();
	}
}

// Set camera position by new vector override
void Camera::setPosition(glm::vec3 vec) 
{ 
	pos = vec; 
	setView();
	setModel();
}
// Set the center position of the camera (center overed canvas)
void Camera::centerToCanvas(glm::ivec2 dimensions)
{
	float x = (float)dimensions.x / 2;
	float y = (float)dimensions.y / 2;
	float z = pos.z;
	std::cout << "CAMERA::CENTERTOCANVAS::X=" << x << "::Y=" << y << std::endl;
	setCenterPosition(glm::vec3(x, y, z));
	setView();
	setModel();
}
void Camera::setCenterPosition(glm::vec3 vec) { centerPos = vec; }

// Camera Rotation
void Camera::setDirections(glm::vec3 fwdvec, float rollAngle)
{
	camXform.fwd = fwdvec;
	float y = sin(glm::radians(rollAngle));
	float x = cos(glm::radians(rollAngle));
	glm::vec3 rotUp = glm::vec3(x, y, 0.0f);
	camXform.up = glm::normalize(rotUp);
	camXform.right = glm::normalize(glm::cross(camXform.up, camXform.fwd));
}
void Camera::setRoll(float r)
{
	roll = r;
	setView();
	setModel();
}
void Camera::addRoll(float r)
{
	roll = roll - r;
	if (roll >= 180.0f)
	{
		roll = -179.8f;
	}
	else if (roll <= -179.8f)
	{
		roll = 180.0f;
	}
	else
	{
		roll = roll;
	}
	setModel();
	setView();
}
float Camera::getRoll() { return roll; }
ObjectTransform* Camera::getTransform() { return &camXform; }

// Camera Physics Functions
void Camera::addMomentum(glm::vec2 dir, float amount)
{
	// Simple physics simulation, no need to combine vectors. Each new momentum clears existing momentum
	clearMomentum();
	slideDir = dir;
	if (amount >= momentumCap)
	{
		momentum = momentumCap;
	}
	else
	{
		momentum += amount;
	}
}
void Camera::updateMomentum(bool shouldStop, float delta)
{
	if (shouldStop == false && momentum != 0)
	{
		float x = (float)delta * (momentum * slideDir.x);
		float y = (float)delta * (momentum * slideDir.y);
		setPosition(x, y);
		float decay = decayHigh + ((momentum / momentumCap) * (decayLow - decayHigh));
		momentum = momentum - (delta * momentum * (friction + decay));
		//std::cout << "CAMERA::MOMENTUM=" << momentum << std::endl;
		if (momentum <= 18.0f)
		{
			clearMomentum();
		}
	}
	else
	{
		clearMomentum();
	}
}
void Camera::clearMomentum()
{
	slideDir = glm::vec2(0.0f, 0.0f);
	momentum = 0;
}
// Set Ortho perspective properties, Note: orthoZoom should fall between about 1.0 to 10
void Camera::setOrthoXY(float wRatio)
{
	if (wRatio >= 1.0f) // Window is landscape
	{ 
		orthoX = 1.0f;
		orthoY = orthoX / wRatio;
	}
	else // Window is portrait
	{
		orthoY = 1.0f;
		orthoX = orthoY * wRatio;
	}
}

void Camera::zoomExtents(glm::ivec2 dimensions)
{
	centerToCanvas(dimensions);
	setRoll(0.0f);
	setOrthoZoom(float(glm::max(dimensions.x, dimensions.y)) / 1.75f);
	setPosition(glm::vec3(0.0f, 0.0f, 1.0f));
}

void Camera::setMaxZoom(float width, float height)
{
	maxZoom = glm::max(width, height) * 10.0f;
}
float Camera::getMaxZoom()
{
	return maxZoom;
}

void Camera::setOrthoZoom(float z)
{
	// Warning: Need to update this later when camera movement is enabled, and properly assess the clamp ranges
	if (z > maxZoom)
	{
		transform.zoom = maxZoom;
	}
	else if (z < 10.0f)
	{
		transform.zoom = 10.0f;
	}
	else
	{
		transform.zoom = z;
	}
	setProjection();
}
void Camera::setClippingRange(float camNear, float camFar)
{
	clipStart = camNear;
	clipEnd = camFar;
}
float Camera::getOrthoZoom() { return transform.zoom; }

// Set transform matricies for draw calls
void Camera::setProjection()
{
	transform.p = glm::ortho(
		orthoX * transform.zoom * -1.0f,
		orthoX * transform.zoom,
		orthoY * transform.zoom * -1.0f,
		orthoY * transform.zoom,
		clipStart, clipEnd);
}
void Camera::setView()
{
	transform.v = glm::lookAt(pos,
		pos + camXform.fwd,
		glm::vec3(0.0f, 1.0f, 0.0f));
}
void Camera::setModel()
{
	// Note: Model transform uses the location of the camera in world space (position)
	transform.m = glm::mat4(1.0f);
	// Add the XY offset
	transform.m = glm::translate(transform.m, glm::vec3(-xoffset, -yoffset, 0.0f));
	// Add the camera rotation
	transform.m = glm::rotate(transform.m, glm::radians(roll), glm::vec3(0.0f, 0.0f, 1.0f));
	// Add the center XY offset
	transform.m = glm::translate(transform.m, glm::vec3(-centerPos.x, -centerPos.y, 0.0f));
}
glm::vec3 Camera::translateWorldPos(glm::vec3 point)
{
	// Note: Model transform uses the location of the camera in world space (position)
	glm::vec4 vec = glm::vec4(point.x, point.y, point.z, 1.0f);
	glm::mat4 matrix = glm::mat4(1.0f);
	matrix = glm::translate(matrix, glm::vec3(centerPos.x, centerPos.y, 0.0f));
	matrix = glm::rotate(matrix, glm::radians(-roll), glm::vec3(0.0f, 0.0f, 1.0f));
	matrix = glm::translate(matrix, glm::vec3(pos));
	vec = matrix * vec;
	glm::vec3 outVec = glm::vec3(vec.x, vec.y, vec.z);
	return outVec;
}
glm::vec3 Camera::getPosition() { return pos; }
glm::vec3 Camera::getCenterPosition() { return centerPos; }
glm::mat4 Camera::getView() { return transform.v; }
glm::mat4 Camera::getProjection() { return transform.p; }
glm::mat4 Camera::getModel() { return transform.m; }
ShaderTransform* Camera::getShaderTransform() { return &transform; }

