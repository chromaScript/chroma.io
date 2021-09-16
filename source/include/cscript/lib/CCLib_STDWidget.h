#ifndef CCLIB_STDWIDGET
#define CCLIB_STDWIDGET

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

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Internal Built-In Widget Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

class CInt_TextClick : public CFunction
{
public:
	std::shared_ptr<UI> ui;
	CInt_TextClick(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Standard Library Global Widget Operator Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////


class CStd_fGetParent : public CFunction
{
public:
	std::shared_ptr<UI> ui;
	CStd_fGetParent(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_fGetRoot : public CFunction
{
public:
	std::shared_ptr<UI> ui;
	CStd_fGetRoot(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_fSetProperty : public CFunction
{
public:
	std::shared_ptr<UI> ui;
	CStd_fSetProperty(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_fGetProperty : public CFunction
{
public:
	std::shared_ptr<UI> ui;
	CStd_fGetProperty(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_fSetChildProperty_byClass : public CFunction
{
public:
	std::shared_ptr<UI> ui;
	CStd_fSetChildProperty_byClass(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_fSetChildProperty_byType : public CFunction
{
public:
	std::shared_ptr<UI> ui;
	CStd_fSetChildProperty_byType(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_fGetChildProperty_byType : public CFunction
{
public:
	std::shared_ptr<UI> ui;
	CStd_fGetChildProperty_byType(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_fSetChildProperty_byID : public CFunction
{
public:
	std::shared_ptr<UI> ui;
	CStd_fSetChildProperty_byID(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_fSetChildProperty_byName : public CFunction
{
public:
	std::shared_ptr<UI> ui;
	CStd_fSetChildProperty_byName(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_fSetClassProperty : public CFunction
{
public:
	std::shared_ptr<UI> ui;
	CStd_fSetClassProperty(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_fSetClassProperty_excludingActive : public CFunction
{
public:
	std::shared_ptr<UI> ui;
	CStd_fSetClassProperty_excludingActive(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_fSetClassProperty_excludingID : public CFunction
{
public:
	std::shared_ptr<UI> ui;
	CStd_fSetClassProperty_excludingID(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_fResetProperty : public CFunction
{
public:
	std::shared_ptr<UI> ui;
	CStd_fResetProperty(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_fResetClassProperty : public CFunction
{
public:
	std::shared_ptr<UI> ui;
	CStd_fResetClassProperty(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};

#endif