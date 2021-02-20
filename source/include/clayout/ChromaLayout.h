#ifndef CHROMALAYOUT_H
#define CHROMALAYOUT_H

#include "LForward.h"
class LLexer;
class LParser;
class LResolver;
class LTypeWizard;
class LInterpreter;

class ChromaScript;
class Application;
class WPlugin;

#include <vector>
#include <filesystem>
#include <string>
#include <iostream>
#include <memory>
#include <map>

class ChromaLayout : public std::enable_shared_from_this<ChromaLayout> {
private:
public:
	//
	std::shared_ptr<LLexer> lexer;
	std::shared_ptr<LParser> parser;
	std::shared_ptr<LResolver> resolver;
	std::shared_ptr<LInterpreter> interpreter;

	//
	std::vector<std::shared_ptr<LStmt>> build(std::filesystem::path scriptPath, std::string _namespace);
	//
	void construct(std::vector<std::shared_ptr<LStmt>> statements, std::string _namespace, std::filesystem::path rootDir);
	//
	void report(std::string message);
	bool hadError = false;
	//
	void alert(std::string message);
public:
	//
	std::shared_ptr<ChromaScript> scriptConsole = nullptr;
	std::shared_ptr<Application> owner = nullptr;
	//
	ChromaLayout(std::shared_ptr<Application> owner, std::shared_ptr<ChromaScript> scriptConsole);
	//
	void entry(std::vector<std::filesystem::path> paths, std::string _namespace, std::filesystem::path rootDir);
	void entry(std::map<std::filesystem::path, std::shared_ptr<WPlugin>> pathMap);
	//
	void create();
	//
	void initializeConsole();
	//
	void ping(double time, std::string message);
	//
	void error(std::string message);
	void error(size_t line, std::string message);
	void error(std::shared_ptr<LToken> line, std::string message);
	//
	void warning(std::string message);
	void warning(size_t line, std::string message);
	void warning(std::shared_ptr<LToken> line, std::string message);
};

#endif