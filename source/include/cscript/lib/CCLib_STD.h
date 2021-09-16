#ifndef CCLIB_STD
#define CCLIB_STD

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
class CStd_fVecSize : public CFunction
{
public:
	CStd_fVecSize(std::shared_ptr<CEnvironment> funcEnv);
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
class CStd_fMakeKeySig : public CFunction
{
public:
	CStd_fMakeKeySig(std::shared_ptr<CEnvironment> funcEnv);
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
class CStd_fTriggerStoredEvent : public CFunction
{
public:
	std::shared_ptr<Application> app;
	CStd_fTriggerStoredEvent(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<Application> app);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};

#endif