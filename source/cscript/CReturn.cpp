#include "../include/cscript/CReturn.h"

#include <vector>
#include <memory>

CReturn::CReturn()
{
	returnCount = 0;
}
CReturn::CReturn(std::vector<int> anticipatedReturns, std::vector<int> countedReturns)
{
	this->anticipatedReturns = anticipatedReturns;
	this->countedReturns = countedReturns;
	this->returnCount = 0;
}
//
bool CReturn::checkReturns()
{
	int failCount = 0;
	for (size_t i = 0; i < anticipatedReturns.size(); i++)
	{
		if (countedReturns[i] < anticipatedReturns[i])
		{
			failCount++;
		}
	}
	if (failCount > 0 && nestedReturns.size() != 0)
	{
		return checkNestedReturns();
	}
	else
	{
		return !(bool)failCount;
	}
}
bool CReturn::checkNestedReturns()
{
	int nestedFailCount = 0;

	for (size_t i = 0; i < nestedReturns.size(); i++)
	{
		if (!nestedReturns[i].get()->checkReturns())
		{
			nestedFailCount++;
		}
	}
	return !(bool)nestedFailCount;
}