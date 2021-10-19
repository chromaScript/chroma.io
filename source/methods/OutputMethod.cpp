#include "../include/methods/OutputMethod.h"
#include "../include/tool/ToolSettings.h"
#include "../include/tool/toolSettings/ToolSettings_Forward.h"
#include "../include/tool/Tool.h"

#ifndef APPLICATION_H
#include "../include/Application.h"
#endif

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/rotate_vector.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/rotate_vector.hpp>

#include <iostream>
#include <algorithm>
#include <iostream>

OutputMethod::OutputMethod(MethodType id, std::shared_ptr<Tool> owner) : id(id), owner(owner)
{

}

int OutputMethod::increaseEntityCount()
{
	entityCount++;
	if (entityCount > 82861) { entityCount = 1; }
	return entityCount;
}