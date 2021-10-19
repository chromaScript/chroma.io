#include "../../include/entities/visuals/FragmentAnchor.h"
#include "../../include/input/input.h"
#include "../../include/math/transform.h"

FragmentAnchor::FragmentAnchor()
{

}
FragmentAnchor::FragmentAnchor(const FragmentAnchor& copy)
{
	this->ID = copy.ID;
	this->pos = copy.pos;
	this->dir = copy.dir;
	this->dirInterpFactor = copy.dirInterpFactor;
	this->headType = copy.headType;
	this->headControl = copy.headControl;
	this->headHandle = copy.headHandle;
	this->tailType = copy.tailType;
	this->tailControl = copy.tailControl;
	this->tailHandle = copy.tailHandle;
	this->handleRelationship = copy.handleRelationship;
	this->input = copy.input;
}
FragmentAnchor::FragmentAnchor(int ID, glm::vec3 pos, glm::vec3 dir,
	HandleType headType, bool headControl, glm::vec3 headHandle,
	HandleType tailType, bool tailControl, glm::vec3 tailHandle,
	HandleRel handleRelationship,
	Input input)
{
	this->ID = ID;
	this->pos = pos;
	this->dir = dir;
	this->headType = headType;
	this->headControl = headControl;
	this->headHandle = headHandle;
	this->tailType = tailType;
	this->tailControl = tailControl;
	this->tailHandle = tailHandle;
	this->handleRelationship = handleRelationship;
	this->input = input;
}
FragmentAnchor::FragmentAnchor(int ID, glm::vec3 pos, glm::vec3 dir, float dirInterpFactor,
	HandleType headType, bool headControl, glm::vec3 headHandle,
	HandleType tailType, bool tailControl, glm::vec3 tailHandle,
	HandleRel handleRelationship,
	Input input)
{
	this->ID = ID;
	this->pos = pos;
	this->dir = dir;
	this->dirInterpFactor = dirInterpFactor;
	this->headType = headType;
	this->headControl = headControl;
	this->headHandle = headHandle;
	this->tailType = tailType;
	this->tailControl = tailControl;
	this->tailHandle = tailHandle;
	this->handleRelationship = handleRelationship;
	this->input = input;
}
void FragmentAnchor::copyStylusData(FragmentAnchor* target)
{
	this->input = target->input;
}
void FragmentAnchor::swapStylusData(FragmentAnchor* target)
{
	Input inputDataT = this->input;
	copyStylusData(target);
	target->input = inputDataT;
}
void FragmentAnchor::clearStylusData()
{
	input.resetStylus();
}