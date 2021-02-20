#include "../include/entities/Fragment.h"

// Constructor / Destructor
Fragment::Fragment()
{

}
Fragment::~Fragment()
{

}
// Data Functions (For additional texture data)
// Transform Function (Specific to Fragment)
void Fragment::updateTransformByAnchors()
{
	int size = (int)fragData.anchors.size();
	if (size == 0) { return; } // Exit early on bad call
	int i = 0;
	int minX = 0, maxX = 0, minY = 0, maxY = 0;
	float qX, qY;
	for (i; i < size; i++)
	{
		qX = fragData.anchors[i].pos.x;
		qY = fragData.anchors[i].pos.y;
		if (minX == 0 && maxX == 0)
		{
			minX = (int)qX;
			maxX = (int)qX;
		}
		else
		{
			if (qX < minX) { minX = (int)qX; }
			if (qX > maxX) { maxX = (int)qX; }
		}
		if (minY == 0 && maxY == 0)
		{
			minY = (int)qY;
			maxY = (int)qY;
		}
		else
		{
			if (qY < minY) { minY = (int)qY; }
			if (qY > maxY) { maxY = (int)qY; }
		}
	}
	// Set the transform data
	this->transform = EntityTransform(
		glm::vec3(float(minX), float(minY), 0.0f),
		Bounds(0, 0, maxX * 2, maxY * 2),
		0.0f);
}