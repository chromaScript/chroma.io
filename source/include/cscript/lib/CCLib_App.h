#ifndef CCLIB_APP
#define CCLIB_APP

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

class CStd_cApp : public CClass
{
public:
	CStd_cApp(std::shared_ptr<CEnvironment> classEnv, std::shared_ptr<Application> app);
	virtual std::string toString();
};

////////////////////////////////////////////////////////////////////////////////////////////////
//
// App Class Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

class CStd_cfExit : public CFunction
{
public:
	CStd_cfExit(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfSaveBMP : public CFunction
{
public:
	CStd_cfSaveBMP(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfGetWindowSize : public CFunction
{
public:
	CStd_cfGetWindowSize(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfGetWindowPosition : public CFunction
{
public:
	CStd_cfGetWindowPosition(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfMaximize : public CFunction
{
public:
	CStd_cfMaximize(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfMinimize : public CFunction
{
public:
	CStd_cfMinimize(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfSetCursor : public CFunction
{
public:
	CStd_cfSetCursor(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfIsValidKeybind_Tool : public CFunction
{
public:
	CStd_cfIsValidKeybind_Tool(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfIsValidKeybind_AlphaOnly : public CFunction
{
public:
	CStd_cfIsValidKeybind_AlphaOnly(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfIsValidKeybind_SymbolOrChar : public CFunction
{
public:
	CStd_cfIsValidKeybind_SymbolOrChar(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfIsValidKeybind_ModOnly : public CFunction
{
public:
	CStd_cfIsValidKeybind_ModOnly(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfMoveAppWindow : public CFunction
{
public:
	CStd_cfMoveAppWindow(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfResizeAppWindow : public CFunction
{
public:
	CStd_cfResizeAppWindow(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfZoomExtents : public CFunction
{
public:
	CStd_cfZoomExtents(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};

#endif