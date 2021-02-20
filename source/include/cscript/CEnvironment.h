#ifndef CENVIRONMENT_H
#define CENVIRONMENT_H

#include "CForward.h"
class CObject;
class CToken;
class ChromaScript;
class SToken;

#include <string>
#include <vector>
#include <map>
#include <memory>

class CEnvironment : public std::enable_shared_from_this<CEnvironment>
{
private:
	//
	std::shared_ptr<ChromaScript> console = nullptr;
	//
	std::shared_ptr<CEnvironment> findEnvironment(std::string lookup);
public:
	//
	std::string _namespace;
	std::map<std::string, std::shared_ptr<CObject>> values;
	std::shared_ptr<CEnvironment> enclosing;
	std::vector<std::shared_ptr<CEnvironment>> scopes;
	std::shared_ptr<CObject> returnValue = nullptr;
	//
	CEnvironment();
	CEnvironment(std::shared_ptr<ChromaScript> console, std::string _namespace);
	CEnvironment(std::shared_ptr<CEnvironment> enclosing, std::shared_ptr<ChromaScript> console, std::string _namespace);
	//
	void setConsole(std::shared_ptr<ChromaScript> console) { this->console = console; }
	//
	std::shared_ptr<CEnvironment> fetchEnvironment(std::string lookup);
	std::shared_ptr<CEnvironment> lookupEnvironment(std::shared_ptr<CExpr_Scope> scopeExpr);
	std::shared_ptr<CEnvironment> lookupEnvironment(std::string lookup, bool allowNew);
	std::shared_ptr<CEnvironment> lookupEnvironment(std::vector<std::string> scopeStack, std::string name);
	std::shared_ptr<CEnvironment> lookupEnvironment(std::vector<std::shared_ptr<CToken>> scopeStack, std::string lookup);
	//
	std::shared_ptr<CEnvironment> virtualize(std::string _namespaceNew, std::shared_ptr<CEnvironment> targetEnclosing);
	void copyData(std::shared_ptr<CEnvironment> targetEnclosing, std::shared_ptr<CEnvironment> target);
	//
	std::vector<std::string> fetchValueNames();
	//
	int declare(std::string name);
	int define(std::string name, std::shared_ptr<CObject> value);
	std::shared_ptr<CObject> get(std::string name);
	std::shared_ptr<CObject> get(std::vector<std::string> scopeStack, std::string name);
	std::shared_ptr<CObject> get(std::vector<std::shared_ptr<CToken>> scopeStack, std::string name);
	std::shared_ptr<CObject> get(std::vector<std::shared_ptr<SToken>> scopeStack, std::string name);
	std::shared_ptr<CEnvironment> getEnvironment(std::string name);
};

#endif