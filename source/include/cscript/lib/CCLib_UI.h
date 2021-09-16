#ifndef CCLIB_UI
#define CCLIB_UI

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

class CStd_cUi : public CClass
{
public:
	CStd_cUi(std::shared_ptr<CEnvironment> classEnv, std::shared_ptr<Application> app);
	virtual std::string toString();
};

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Ui Class Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

class CStd_cfCheckWidgetIDTable : public CFunction
{
public:
	CStd_cfCheckWidgetIDTable(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfMoveRootToFront : public CFunction
{
public:
	CStd_cfMoveRootToFront(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfSetActivePopup : public CFunction
{
public:
	CStd_cfSetActivePopup(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfClearPopup : public CFunction
{
public:
	CStd_cfClearPopup(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfPreventBlurCallback : public CFunction
{
public:
	CStd_cfPreventBlurCallback(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfPreventFocusCallback : public CFunction
{
public:
	CStd_cfPreventFocusCallback(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfSetFocus_byID : public CFunction
{
public:
	CStd_cfSetFocus_byID(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfClearFocus : public CFunction
{
public:
	CStd_cfClearFocus(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfGetWidget_byID : public CFunction
{
public:
	CStd_cfGetWidget_byID(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfDeleteWidget_byID : public CFunction
{
public:
	CStd_cfDeleteWidget_byID(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfSortTargetWidgetChildren_byMacro : public CFunction
{
public:
	CStd_cfSortTargetWidgetChildren_byMacro(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfSetFGColor_HSL : public CFunction
{
public:
	CStd_cfSetFGColor_HSL(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfSetBGColor_HSL : public CFunction
{
public:
	CStd_cfSetBGColor_HSL(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfResetFGBGColor : public CFunction
{
public:
	CStd_cfResetFGBGColor(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfSwapFGBGColor : public CFunction
{
public:
	CStd_cfSwapFGBGColor(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfCreateNewDocument : public CFunction
{
public:
	CStd_cfCreateNewDocument(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_cfCloseDocument : public CFunction
{
public:
	CStd_cfCloseDocument(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};

#endif