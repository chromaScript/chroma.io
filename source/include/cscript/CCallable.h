#ifndef CCALLABLE_H
#define CCALLABLE_H

#include "CEnums.h"
#include "CForward.h"
#include "CToken.h"


#include <string>
#include <vector>
#include <memory>

//
class CCallable
{
public:
	CCallableTypes type = CCallableTypes::_CUndefined;
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};

//
class CClass : public CCallable
{
public:
	std::shared_ptr<CStmt_Class> classDeclaration = nullptr;
	std::shared_ptr<CClass> superClass = nullptr;
	std::shared_ptr<CEnvironment> classEnv = nullptr;
	std::string name;
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
//
class CInstance : public CCallable
{
public:
	std::string className;
	std::shared_ptr<CEnvironment> instanceEnv;
	CInstance(std::string className, std::shared_ptr<CEnvironment> instanceEnv);
	std::shared_ptr<CObject> get(std::string lookup);
	void set(CToken name, std::shared_ptr<CObject> value);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};

class CUser_Class : public CClass
{
public:
	CUser_Class(std::shared_ptr<CStmt_Class> classDeclaration, 
		std::string name, 
		CCallableTypes type,
		std::shared_ptr<CEnvironment> classEnv,
		std::shared_ptr<CClass> superClass);
	virtual std::string toString();
};

//
class CFunction : public CCallable
{
public:
	std::shared_ptr<CStmt_Function> funcDeclaration = nullptr;
	std::shared_ptr<CEnvironment> funcEnv = nullptr;
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};

class CUser_Function : public CFunction
{
public:
	CUser_Function(std::shared_ptr<CStmt_Function> funcDeclaration, CCallableTypes type, std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};

#endif