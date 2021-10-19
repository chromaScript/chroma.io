#include "../../include/entities/layers/LayerProperties.h"

/*
// Layer Structs
*/
// LayerProperties is used to hold various default type layer properties like ID, Z-Index, Group ID, Parent/Clip ID, Alpha/Pixel/Position Lock, etc.
LayerProperties::LayerProperties()
{
	alphaLock = false;
	dataLock = false;
	positionLock = false;
	mergeLock = false;
}
// GroupProperties is used to hold group information
