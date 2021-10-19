#ifndef CHROMASCRIPT_H
#define CHROMASCRIPT_H

#include "CForward.h"
class Application;
class CLexer;
class CParser;
class CResolver;
class CTypeWizard;
class COptimizerRed;
class WPlugin;

#include <vector>
#include <filesystem>
#include <string>
#include <iostream>
#include <memory>
#include <map>

class ChromaScript : public std::enable_shared_from_this<ChromaScript> {
private:
	//
	std::shared_ptr<Application> owner = nullptr;
	//
	std::vector<std::shared_ptr<CExpr>> expressions;
	std::vector<std::shared_ptr<CStmt>> statements;
	//
	std::shared_ptr<CLexer> lexer;
	std::shared_ptr<CParser> parser;
	std::shared_ptr<CInterpreter> interpreter;
	std::shared_ptr<CResolver> resolver;
	std::shared_ptr<CTypeWizard> typeWizard;
	std::shared_ptr<COptimizerRed> optimizerRed;
	//
	std::vector<std::shared_ptr<CStmt>> build(std::filesystem::path scriptPath, std::string _namespace, bool isExpr);
	std::vector<std::shared_ptr<CStmt>> build(std::string script, std::string _namespace, bool isExpr);
	//
	void compile(std::vector<std::shared_ptr<CStmt>> statements, std::string _namespace);
	//
	
	std::shared_ptr<CObject> execute(std::vector<std::shared_ptr<CStmt>> statements, std::string _namespace);
	//
	void loadLibraries(std::shared_ptr<CEnvironment> env);
	//
	void report(std::string message);
	bool hadError = false;
	//
	void alert(std::string message);
protected:
public:
	//
	std::shared_ptr<CEnvironment> global = nullptr;
	//
	ChromaScript(std::shared_ptr<Application> owner);
	//
	void entry(std::vector<std::filesystem::path> paths, std::string _namespace, std::filesystem::path rootDir);
	void entry(std::map<std::filesystem::path, std::shared_ptr<WPlugin>> pathMap);
	void entry(std::string script, std::string _namespace, std::filesystem::path rootDir);
	std::shared_ptr<CObject> entry(std::string script, std::string _namespace, bool isExpr);
	std::vector<std::shared_ptr<CStmt>> entry_compileOnly(std::string script, std::string _namespace, bool isExpr);
	//
	void run(std::vector<std::shared_ptr<CStmt>> statements, std::string _namespace);
	//
	void initializeConsole();
	void cleanEngine();
	std::shared_ptr<CInterpreter> getInterpreter() { return interpreter; }
	//
	void ping(double time, std::string message);
	void print(std::string message);
	//
	void error(std::string message);
	void error(size_t line, std::string message);
	void error(std::shared_ptr<CToken> line, std::string message);
	//
	void warning(std::string message);
	void warning(size_t line, std::string message);
	void warning(std::shared_ptr<CToken> line, std::string message);
	//
	bool insertWidgetTableID(std::string id, std::string rootName);
	void insertWidgetTableClass(std::string className, std::string rootName);
	bool insertPrototypeTableID(std::string id);
	//
	std::shared_ptr<CObject> copyObj(std::shared_ptr<CObject> right, std::string name);
	//
	void printObj(std::shared_ptr<CObject> obj);
	std::string toString(std::shared_ptr<CObject> obj);
	double toNum(std::shared_ptr<CObject> obj);
	std::string numToString(double d);
	std::string numToString(float f);
	std::string boolToString(bool b);
	std::string vecToString(std::shared_ptr<CObject> obj);
	std::string arrayToString(std::shared_ptr<CObject> obj);
};

#endif