#ifndef CCLIB_WIDGET
#define CCLIB_WIDGET

#include "../CCallable_Lib.h"
#include "../CEnums.h"
#include "../CForward.h"
#include "../CToken.h"
#include "../CCallable.h"

class Application;
class UI;
class Widget;

#include <string>
#include <vector>
#include <memory>

class CStd_cWidget : public CClass
{
public:
	CStd_cWidget(std::shared_ptr<CEnvironment> classEnv);
	virtual std::string toString();
};

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Widget Class Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

class CStd_cfRebuildOutofBoundsWidgets : public CFunction
{
public:
	CStd_cfRebuildOutofBoundsWidgets(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfSetProperty : public CFunction
{
public:
	CStd_cfSetProperty(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfSetChildProperty_byClass : public CFunction
{
public:
	CStd_cfSetChildProperty_byClass(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfGetChildrenProperties : public CFunction
{
public:
	CStd_cfGetChildrenProperties(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfGetProperty : public CFunction
{
public:
	CStd_cfGetProperty(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfSetPosition : public CFunction
{
public:
	CStd_cfSetPosition(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfSetChildOrder_byID : public CFunction
{
public:
	CStd_cfSetChildOrder_byID(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfAddChildWidget_Prototype : public CFunction
{
public:
	CStd_cfAddChildWidget_Prototype(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfRemoveChildWidget_byID : public CFunction
{
public:
	CStd_cfRemoveChildWidget_byID(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfRemoveChildWidgets_byClass : public CFunction
{
public:
	CStd_cfRemoveChildWidgets_byClass(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};

#endif