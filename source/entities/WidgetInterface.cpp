#include "../include/entities/WidgetInterface.h"

#include "../include/entities/Widget.h"
#include "../include/cscript/CCallable.h"
#include "../include/cscript/CInterpreter.h"
#include "../include/cscript/CToken.h"
#include "../include/cscript/CStmt.h"
#include "../include/cscript/CExpr.h"

#include <memory>
#include <map>
#include <vector>
#include <string>
#include <deque>

void WidgetInterface::attachTextInterface(Widget* widget)
{
	widget->callbackMap.insert(std::pair<std::string, std::vector<std::shared_ptr<CStmt>>>(
		"onclick", 
		{ createTextClickEvent() }));
}

std::shared_ptr<CStmt> WidgetInterface::createTextClickEvent()
{
	// Make the token for the lookup
	std::shared_ptr<CToken> line = std::make_shared<CToken>(CTokenType::IDENTIFIER, "@textClick", -1);
	// Craft the pseudo-expression
	std::vector<std::shared_ptr<CExpr>> args = {};
	std::shared_ptr<CExpr> callee = std::make_shared<CExpr_Variable>(line);
	std::shared_ptr<CExpr> call = std::make_shared<CExpr_Call>(callee, args, nullptr, line);
	// Return the callback binding
	return std::make_shared<CStmt_Expression>(call, line);
}