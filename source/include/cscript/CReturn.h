#ifndef CRETURN_H
#define CRETURN_H

#include "CForward.h"

#include <vector>
#include <memory>

class CReturn
{
public:
	CReturn();
	CReturn(std::vector<int> anticipatedReturns, std::vector<int> countedReturn);
	//
	int returnCount;
	std::vector<int> anticipatedReturns;
	std::vector<int> countedReturns;
	size_t stride = 0;
	std::vector<std::shared_ptr<CReturn>> nestedReturns;
	//
	bool checkReturns();
	bool checkNestedReturns();
};

#endif