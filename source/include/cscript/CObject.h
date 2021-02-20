#ifndef COBJECTS_H
#define COBJECTS_H

#include "CEnums.h"
#include "CForward.h"
#include "CStmt.h"
#include "CCallable.h"
#include "CLiteral.h"

#include <glm.hpp>

class Application;
class UI;
class Toolbox;
class Widget;

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <variant>

class CInterpreter;

class CObject
{
public:
	CLiteral objType;
	std::string objName = "";
	std::variant<
		nullptr_t, 
		std::shared_ptr<std::vector<std::shared_ptr<CObject>>>,
		std::shared_ptr<CFunction>,
		std::shared_ptr<CClass>,
		std::shared_ptr<CInstance>, 
		double,
		glm::dvec2,
		glm::dvec3,
		glm::dvec4,
		std::string,
		bool,
		std::shared_ptr<Application>,
		std::shared_ptr<UI>,
		std::shared_ptr<Toolbox>,
		std::weak_ptr<Widget>,
		std::shared_ptr<WidgetStyle>> obj;

	CObject();
	//~CObject();
	CObject(const CObject& cobject);
	CObject(std::shared_ptr<CObject> object);
	CObject(std::shared_ptr<CToken> typeToken, std::string objName);
	CObject(CLiteral type, std::string objName);
	CObject(CLiteralTypes type, std::shared_ptr<std::vector<std::shared_ptr<CObject>>> value);
	CObject(double value);
	CObject(std::string value);
	CObject(std::vector<std::string> values);
	CObject(bool value);
	CObject(glm::dvec2 value);
	CObject(glm::dvec3 value);
	CObject(glm::dvec4 value);
	CObject(std::shared_ptr<Application> app);
	CObject(std::shared_ptr<UI> ui);
	CObject(std::shared_ptr<Toolbox> toolbox);
	CObject(std::shared_ptr<WidgetStyle> style);
	CObject(std::weak_ptr<Widget> widget);
	CObject(nullptr_t ptr);
	//
	CObject(CCallableTypes libFuncName, std::shared_ptr<CEnvironment> env, std::shared_ptr<Application> app);
	CObject(CCallableTypes libFuncName, std::shared_ptr<CEnvironment> env);
	CObject(CCallableTypes libClassName, std::shared_ptr<Application> app);
	CObject(CCallableTypes libClassName, std::shared_ptr<WidgetStyle> style);
	CObject(std::shared_ptr<CStmt_Function> functionStmt, std::shared_ptr<CEnvironment> funcEnv);
	CObject(
		std::shared_ptr<CEnvironment> currentEnv,
		std::string instanceName,
		std::shared_ptr<CClass> classTemplate);
	CObject(
		std::shared_ptr<CEnvironment> currentEnv,
		std::string instanceName,
		std::shared_ptr<CInstance> instanceCopy);
	CObject(std::shared_ptr<CStmt_Class> classStmt,
		std::string name,
		std::shared_ptr<CEnvironment> classEnv,
		std::shared_ptr<CClass> superClass);
};



#endif