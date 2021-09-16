#ifndef CCLIB_DOCUMENT
#define CCLIB_DOCUMENT

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

class CStd_cDocument : public CClass
{
public:
	CStd_cDocument(std::shared_ptr<CEnvironment> classEnv, std::shared_ptr<Application> app);
	virtual std::string toString();
};

#endif