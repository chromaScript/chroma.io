#ifndef CHROMATOOLFILE_H
#define CHROMATOOLFILE_H

#include "TFForward.h"
class Application;
class TFLexer;
class TFParser;
class TFInterpreter;
class Toolbox;
class Tool;
class TFSettingsWriter;

#include <vector>
#include <filesystem>
#include <string>
#include <iostream>
#include <memory>
#include <map>

class ChromaToolFile : public std::enable_shared_from_this<ChromaToolFile> {
private:
	//
	std::shared_ptr<Application> owner = nullptr;
	//
	std::shared_ptr<TFSettingsWriter> settingsWriter;
	std::shared_ptr<TFLexer> lexer;
	std::shared_ptr<TFParser> parser;
	std::shared_ptr<TFInterpreter> interpreter;
	//
	bool build(std::shared_ptr<Toolbox> targetBox, std::filesystem::path fileDir, bool replaceExisting);
	//
	void report(std::string message);
	bool hadError = false;
	//
	void alert(std::string message);
protected:
public:
	//
	ChromaToolFile(std::shared_ptr<Application> owner);
	//
	bool loadFiles_default(std::shared_ptr<Toolbox> targetBox);
	bool writeFiles_defualt(
		std::vector<std::shared_ptr<Tool>> tools, bool overwrite,
		std::filesystem::path targetPath, bool useDefaultPath);
	bool writeToolFile(std::shared_ptr<Tool> tool, std::filesystem::path dir, bool overwrite);
	void entry(std::vector<std::filesystem::path> path, bool loadEntireDirectory);
	//
	void initializeConsole();
	std::shared_ptr<TFInterpreter> getInterpreter() { return interpreter; }
	//
	void ping(double time, std::string message);
	void print(std::string message);
	//
	void error(std::string message);
	void error(size_t line, std::string message);
	void error(std::shared_ptr<TFToken> line, std::string message);
	//
	void warning(std::string message);
	void warning(size_t line, std::string message);
	void warning(std::shared_ptr<TFToken> line, std::string message);
};

#endif