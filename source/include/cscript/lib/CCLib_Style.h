#ifndef CCLIB_STYLE
#define CCLIB_STYLE

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

class CStd_cStyle : public CClass
{
public:
	CStd_cStyle(std::shared_ptr<CEnvironment> classEnv);
	virtual std::string toString();
};

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Style Class Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

class CStd_cfSetProperty_byName : public CFunction
{
public:
	CStd_cfSetProperty_byName(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfGetProperty_byName : public CFunction
{
public:
	CStd_cfGetProperty_byName(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};

#endif