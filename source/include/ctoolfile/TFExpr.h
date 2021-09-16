#ifndef TFEXPR_H
#define TFEXPR_H

#include "TFForward.h"
#include "TFInterpreter.h"
#include "TFToken.h"
#include "TFVisitors.h"
#include "../cscript/CObject.h"

#include <memory>
#include <string>

class TFExpr
{
public:
	virtual void accept(TFExprVisitor_Abstract<void>& visitor) = 0;
	virtual std::shared_ptr<CObject> acceptInterpreter(TFExprVisitor_Abstract<std::shared_ptr<CObject>>& visitor) = 0;
};

// Child Classes
class TFExpr_Value : public TFExpr, public std::enable_shared_from_this<TFExpr_Value>
{
public:
	std::shared_ptr<TFToken> line;
	std::shared_ptr<CObject> value;
	TFExpr_Value(std::shared_ptr<CObject> value, std::shared_ptr<TFToken> line);
	virtual void accept(TFExprVisitor_Abstract<void>& visitor);
	virtual std::shared_ptr<CObject> acceptInterpreter(TFExprVisitor_Abstract<std::shared_ptr<CObject>>& visitor);
};
class TFExpr_Name : public TFExpr, public std::enable_shared_from_this<TFExpr_Name>
{
public:
	std::shared_ptr<TFToken> line;
	int settingsID;
	int settingSubID;
	std::string settingsName;
	TFExpr_Name(int settingsID, int settingSubID, std::string name, std::shared_ptr<TFToken> line);
	virtual void accept(TFExprVisitor_Abstract<void>& visitor);
	virtual std::shared_ptr<CObject> acceptInterpreter(TFExprVisitor_Abstract<std::shared_ptr<CObject>>& visitor);
};

#endif