#ifndef CCLIB_STDWIDGETINTERFACE_H
#define CCLIB_STDWIDGETINTERFACE_H

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
// Line/Paragraph Functions
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
// Graph ToolController Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

class CInt_TGraph_Hover : public CFunction
{
public:
	std::shared_ptr<UI> ui;
	std::weak_ptr<Widget> activeDragTarget;
	bool blockNewHandle = false;
	CInt_TGraph_Hover(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};

class CInt_TGraph_Drag : public CFunction
{
public:
	std::shared_ptr<UI> ui;
	std::weak_ptr<Widget> activeDragTarget;
	bool blockNewHandle = false;
	CInt_TGraph_Drag(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui);
	virtual std::shared_ptr<CObject> call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments);
	virtual std::string toString();
};

#endif