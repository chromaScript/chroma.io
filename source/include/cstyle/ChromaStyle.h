#ifndef CHROMASTYLE_H
#define CHROMASTYLE_H

#include "SForward.h"
class SLexer;
class SParser;
class SResolver;
class SInterpreter;
class WidgetStyle;

class ChromaScript;
class Application;
class WPlugin;

#include <vector>
#include <filesystem>
#include <string>
#include <iostream>
#include <memory>
#include <map>

class ChromaStyle : public std::enable_shared_from_this<ChromaStyle> {
private:
	//
	std::shared_ptr<SLexer> lexer;
	std::shared_ptr<SParser> parser;
	std::shared_ptr<SResolver> resolver;
	std::shared_ptr<SInterpreter> interpreter;

	//
	std::vector<std::shared_ptr<SStmt>> make(std::filesystem::path scriptPath, std::string _namespace, std::filesystem::path rootDir);
	//
	void build(std::vector<std::shared_ptr<SStmt>> statements, std::string _namespace, std::filesystem::path rootDir);
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
	ChromaStyle(std::shared_ptr<Application> owner, std::shared_ptr<ChromaScript> scriptConsole);
	//
	void entry(std::vector<std::filesystem::path> paths, std::string _namespace, std::filesystem::path rootDir);
	void entry(std::map<std::filesystem::path, std::shared_ptr<WPlugin>> pathMap);
	//
	std::shared_ptr<WidgetStyle> create(std::string styleString, std::string _namespace, std::filesystem::path rootDir);
	//
	void initializeConsole();
	//
	void ping(double time, std::string message);
	//
	void error(std::string message);
	void error(size_t line, std::string message);
	void error(std::shared_ptr<SToken> line, std::string message);
	//
	void warning(std::string message);
	void warning(size_t line, std::string message);
	void warning(std::shared_ptr<SToken> line, std::string message);
};

#endif