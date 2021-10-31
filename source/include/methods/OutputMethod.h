#ifndef OUTPUTMETHOD_H
#define OUTPUTMETHOD_H

#include "../tool/ToolSettings.h"
#include "../tool/toolSettings/ToolSettings_Forward.h"
#include "../entities/visuals/VertexData.h"
#include "../methods/MethodType.h"

class Tool;
class Application;

#include <vector>
#include <string>

class OutputMethod
{
private:
protected:
	MethodType id = MethodType::out_noOutput;
	std::string name = "";
	std::shared_ptr<Tool> owner;
	//InputData data;
public:
	int entityCount = 0;
	MethodType type = MethodType::out_noOutput;
	std::vector<TSetType> interestMask;
	OutputMethod(MethodType id, std::shared_ptr<Tool> owner);
	MethodType getID() { return id; }
	virtual void preview(Application* sender, VertexData* vertexData, InputHandlerFlag action) = 0;
	virtual void finalize(Application* sender, VertexData* fragDat) = 0;
	virtual void postprocess(Application* sender, VertexData* fragDat) = 0;
	std::string getName() { return name; }
	int increaseEntityCount();
};

#endif