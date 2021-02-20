#ifndef CLITERAL_H
#define CLITERAL_H

#include "CEnums.h"
#include "CToken.h"

#include <memory>
#include <string>

class CLiteral
{
public:
	CLiteral();
	CLiteral(CLiteralTypes type, std::string litName);
	CLiteral(std::shared_ptr<CToken> token);
	std::string litName;
	CLiteralTypes type;
};

#endif