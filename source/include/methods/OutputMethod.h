#ifndef OUTPUTMETHOD_H
#define OUTPUTMETHOD_H

#include "../structs.h"
#include "../ToolSettings.h"
#include "../toolSettings/ToolSettings_Forward.h"
class Tool;
class Application;

#include <vector>
#include <string>

class OutputMethod
{
private:
protected:
	int ID;
	std::string name = "";
	std::shared_ptr<Tool> owner;
	//InputData data;
public:
	MethodType type = MethodType::noOutput;
	std::vector<TSetType> interestMask;
	OutputMethod(int id, std::shared_ptr<Tool> owner) : ID(id), owner(owner) {};
	int getID() { return ID; }
	//InputData getData() { return data; }
	virtual void preview(Application* sender, VertexData* fragDat) = 0;
	virtual void finalize(Application* sender, VertexData* fragDat) = 0;
	virtual void postprocess(Application* sender, VertexData* fragDat) = 0;
	std::string getName() { return name; }
};

#endif