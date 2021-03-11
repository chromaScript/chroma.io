#ifndef CCALLABLE_LIB
#define CCALLABLE_LIB

#include "CEnums.h"
#include "CForward.h"
#include "CToken.h"
#include "CCallable.h"

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
// Standard Library Utility Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

class CStd_fClock : public CFunction
{
public:
	CStd_fClock(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_fToString : public CFunction
{
public:
	CStd_fToString(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_fToNum : public CFunction
{
public:
	CStd_fToNum(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_fKeyToString : public CFunction
{
public:
	CStd_fKeyToString(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_fBindTimerEvent : public CFunction
{
public:
	std::shared_ptr<Application> app;
	CStd_fBindTimerEvent(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<Application> app);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_fCancelTimerEvent : public CFunction
{
public:
	std::shared_ptr<Application> app;
	CStd_fCancelTimerEvent(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<Application> app);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Standard Library Collision Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

class CStd_fGetMousePos : public CFunction
{
public:
	std::shared_ptr<Application> app;
	CStd_fGetMousePos(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<Application> app);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_fGetMousePos_relative : public CFunction
{
public:
	std::shared_ptr<Application> app;
	CStd_fGetMousePos_relative(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<Application> app);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_fSweepTestRoot_byID : public CFunction
{
public:
	std::shared_ptr<UI> ui;
	CStd_fSweepTestRoot_byID(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_fHitTestWidget_byID : public CFunction
{
public:
	std::shared_ptr<UI> ui;
	CStd_fHitTestWidget_byID(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Standard Library Math Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

class CStd_fFibonacci : public CFunction
{
public:
	CStd_fFibonacci(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
	double fib(double f);
};
class CStd_fLineIntersect2D : public CFunction
{
public:
	CStd_fLineIntersect2D(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_fFloor : public CFunction
{
public:
	CStd_fFloor(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_fClamp : public CFunction
{
public:
	CStd_fClamp(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_fLerp : public CFunction
{
public:
	CStd_fLerp(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_fRGB_toHSL : public CFunction
{
public:
	CStd_fRGB_toHSL(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_fHSL_toRGB : public CFunction
{
public:
	CStd_fHSL_toRGB(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_fHueByDegree_HSL : public CFunction
{
public:
	CStd_fHueByDegree_HSL(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_fSaturationByPercentDegree_HSL : public CFunction
{
public:
	class CStd_fSaturationByPercentDegree_HSL(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};
class CStd_fValueByPercent_HSL : public CFunction
{
public:
	class CStd_fValueByPercent_HSL(std::shared_ptr<CEnvironment> funcEnv);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};


////////////////////////////////////////////////////////////////////////////////////////////////
//
// Standard Library Callback Binding Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

class CStd_fBindCallbackEvent : public CFunction
{
public:
	std::shared_ptr<Application> app;
	CStd_fBindCallbackEvent(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<Application> app);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Standard Library Global Widget Operator Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Standard Library Classes
//
////////////////////////////////////////////////////////////////////////////////////////////////

class CStd_cApp : public CClass
{
public:
	CStd_cApp(std::shared_ptr<CEnvironment> classEnv, std::shared_ptr<Application> app);
	virtual std::string toString();
};
class CStd_cUi : public CClass
{
public:
	CStd_cUi(std::shared_ptr<CEnvironment> classEnv, std::shared_ptr<Application> app);
	virtual std::string toString();
};
class CStd_cDocument : public CClass
{
public:
	CStd_cDocument(std::shared_ptr<CEnvironment> classEnv, std::shared_ptr<Application> app);
	virtual std::string toString();
};
class CStd_cToolbox : public CClass
{
public:
	CStd_cToolbox(std::shared_ptr<CEnvironment> classEnv, std::shared_ptr<Application> app);
	virtual std::string toString();
};
class CStd_cWidget : public CClass
{
public:
	CStd_cWidget(std::shared_ptr<CEnvironment> classEnv);
	virtual std::string toString();
};
class CStd_cStyle : public CClass
{
public:
	CStd_cStyle(std::shared_ptr<CEnvironment> classEnv);
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

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Ui Class Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Widget Class Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

class CStd_cfSetProperty : public CFunction
{
public:
	CStd_cfSetProperty(std::shared_ptr<CEnvironment> funcEnv);
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