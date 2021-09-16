#ifndef CCLIB_TOOLBOX
#define CCLIB_TOOLBOX

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

class CStd_cToolbox : public CClass
{
public:
	CStd_cToolbox(std::shared_ptr<CEnvironment> classEnv, std::shared_ptr<Application> app);
	virtual std::string toString();
};

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Toolbox Class Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

class CStd_cfCheckValidIOCombination : public CFunction
{
public:
	CStd_cfCheckValidIOCombination(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfCheckValidControlScheme : public CFunction
{
public:
	CStd_cfCheckValidControlScheme(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfCreateNewTool : public CFunction
{
public:
	CStd_cfCreateNewTool(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfCheckActiveToolSettingsMask : public CFunction
{
public:
	CStd_cfCheckActiveToolSettingsMask(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfSetActiveToolProp : public CFunction
{
public:
	CStd_cfSetActiveToolProp(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfGetActiveToolProp : public CFunction
{
public:
	CStd_cfGetActiveToolProp(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfSetActiveToolInterest : public CFunction
{
public:
	CStd_cfSetActiveToolInterest(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfGetActiveToolInterest : public CFunction
{
public:
	CStd_cfGetActiveToolInterest(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfExportAllTools : public CFunction
{
public:
	CStd_cfExportAllTools(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};

#endif