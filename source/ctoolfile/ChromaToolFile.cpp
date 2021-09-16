#include "../include/ctoolfile/TFLexer.h"
#include "../include/ctoolfile/TFParser.h"
#include "../include/ctoolfile/TFInterpreter.h"
#include "../include/ctoolfile/ChromaToolFile.h"
#include "../include/ctoolfile/TFToken.h"
#include "../include/ctoolfile/TFSettingsWriter.h"
#include "../include/ctoolfile/TFEnums.h"

#include "../include/Application.h"
#include "../include/entities/UserInterface.h"
#include "../include/Toolbox.h"
#include "../include/Tool.h"

#include "../include/structs.h"

#include "glm.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <variant>

////////////////////////////////////////////////////////////////////////////////////////////////
//
// chromaToolFile Interpreter Constructor
//
////////////////////////////////////////////////////////////////////////////////////////////////

ChromaToolFile::ChromaToolFile(std::shared_ptr<Application> owner)
{
	for (size_t i = 0; i < TFTokenTypeNames->size(); i++)
	{
		TFTokenTypeMap.insert(std::pair<std::string, TFTokenType>(TFTokenTypeNames[i], (TFTokenType)i));
	}

	this->owner = owner;
	lexer = std::make_shared<TFLexer>();
	parser = std::make_shared<TFParser>();
	interpreter = std::make_shared<TFInterpreter>();

}

void ChromaToolFile::initializeConsole()
{
	//global = std::make_shared<CEnvironment>(nullptr, shared_from_this(), "global");
	settingsWriter = std::make_shared<TFSettingsWriter>(shared_from_this());
	lexer.get()->initialize(shared_from_this());
	parser.get()->initialize(shared_from_this());
	interpreter.get()->initialize(shared_from_this());
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ChromaToolFile File Reading Entry Points
//
////////////////////////////////////////////////////////////////////////////////////////////////
//

// Entry point for loading tools during startup or when performing hard-resets of the currently loaded Tools
bool ChromaToolFile::loadFiles_default(std::shared_ptr<Toolbox> targetBox)
{
	std::vector<std::filesystem::path> filesList;
	std::filesystem::path root = std::filesystem::current_path() /= std::filesystem::path("assets/tools");

	// Iterate over the assets/tools folder location to find potential plugin locations
	if (std::filesystem::exists(root) && std::filesystem::is_directory(root))
	{
		for (auto const& entry : std::filesystem::recursive_directory_iterator(root))
		{
			if (std::filesystem::is_regular_file(entry) && entry.path().extension() == ".ctf")
			{
				filesList.push_back(entry.path());
				std::cout << "APPLICATION::TOOLBOX::TOOL-FILE::LOCATED=" << filesList.back() << std::endl;
			}
		}
	}
	if (filesList.size() == 0) { hadError = true; return hadError; }
	hadError = false;
	for (std::filesystem::path file : filesList)
	{
		build(targetBox, file, true);
	}
	return hadError;
}

// Entry Point for loading tools during normal execution
void ChromaToolFile::entry(std::vector<std::filesystem::path> path, bool loadEntireDirectory)
{
	hadError = false;
	if (hadError == true)
	{
		error("[console:0101] errror: Encountered error during compilation, aborting script."); return;
	}
	//compile(statements, _namespace);
	if (hadError == true)
	{
		error("[console:0102] errror: Encountered error during compilation, aborting script."); return;
	}
	//run(statements, _namespace);
	if (hadError == true)
	{
		error("[console:0103] errror: Encountered error during runtime."); return;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ChromaToolFile File Reading
//
////////////////////////////////////////////////////////////////////////////////////////////////
//

bool ChromaToolFile::build(std::shared_ptr<Toolbox> targetBox, std::filesystem::path fileDir, bool replaceExisting)
{
	std::string fileString = "";
	readFileToString(fileString, fileDir);
	// Tokenize the Tool File
	std::vector<std::shared_ptr<TFToken>> tokens = lexer.get()->scanTokens(fileString);
	// Parse the tokens into Tool Statements (Nested SettingsBlocks)
	std::vector<std::shared_ptr<TFStmt>> statements;
	statements = parser.get()->parseTokens(tokens);
	// Compile Tools
	interpreter.get()->interpret(statements, targetBox);
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ChromaToolFile File Writing
//
////////////////////////////////////////////////////////////////////////////////////////////////
//

// Default writing function for handling saving tools to files. This will write each tool to an individual
// file. The default behavior of chroma.io is to package all tools as individual files, and load them
// as individual files. Users wishing to distribute multi-tool packages should .zip them first, and end-users
// should unzip into the assets/tools/ folder.

bool ChromaToolFile::writeFiles_defualt(
	std::vector<std::shared_ptr<Tool>> tools, bool overwrite,
	std::filesystem::path targetPath, bool useDefaultPath)
{
	std::filesystem::path dir = (useDefaultPath) ? (std::filesystem::current_path()  /= "assets/tools") : targetPath;
	for (std::shared_ptr<Tool> tool : tools)
	{
		bool result = writeToolFile(tool, dir, overwrite);
	}
	return false;
}

bool ChromaToolFile::writeToolFile(std::shared_ptr<Tool> tool, std::filesystem::path dir, bool overwrite)
{
	std::string outPath = dir.string() + "\\";
	FILE* outFile;
	errno_t err;
	outPath = outPath + tool.get()->name + ".ctf";

	err = fopen_s(&outFile, outPath.c_str(), "wb");
	if (err != 0) { return false; }

	fprintf(outFile, "[header]\n\n");

	std::string meta = settingsWriter.get()->writeSetting_metaData(tool);
	fprintf(outFile, meta.c_str());

	for (auto const& toolSet : tool.get()->settings)
	{
		std::string setting = settingsWriter.get()->writeSettings_fromInterest(tool, toolSet.second.get()->type);
		fprintf(outFile, setting.c_str());
	}

	fprintf(outFile, "[end]");

	fclose(outFile);

	if (err == 0) { return true; }
	else { return false; }
}

//
void ChromaToolFile::ping(double time, std::string message)
{
	std::cout << "ChromaToolFile::CONSOLE::PING=" << time << "::MESSAGE=" << message << std::endl;
}
void ChromaToolFile::print(std::string message)
{
	std::cout << message << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ChromaToolFile Error Handling
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
void ChromaToolFile::error(std::string message)
{
	report("ctoolfile= " + message);
}
void ChromaToolFile::error(size_t line, std::string message)
{
	std::string out =
		"ctoolfile= [line:" + std::to_string(line) + "] error: " + message;
	report(out);
}
void ChromaToolFile::error(std::shared_ptr<TFToken> line, std::string message)
{
	if (line != nullptr)
	{
		std::string out =
			"cscript= [line:" + std::to_string(line.get()->line) + "] error: " + line.get()->typeString() + " - " + message;
		report(out);
	}
	else
	{
		std::string out =
			"cscript= [line: nil] error: nil - [console] Nullptr token passed to error handler.";
		report(out);
	}
}
// Error Reporter
void ChromaToolFile::report(std::string message)
{
	std::cout << message << std::endl;
	this->hadError = true;
}
// Warning Builder
void ChromaToolFile::warning(std::string message)
{
	alert("ctoolfile= " + message);
}
void ChromaToolFile::warning(size_t line, std::string message)
{
	std::string out =
		"ctoolfile= [line:" + std::to_string(line) + "] warning: " + message;
	alert(out);
}
void ChromaToolFile::warning(std::shared_ptr<TFToken> line, std::string message)
{
	//std::string out =
	//	"ctoolfile= [line:" + std::to_string(line.get()->line) + "] warning: " + line.get()->typeString() + " - " + message;
	//alert(out);
}
void ChromaToolFile::alert(std::string message)
{
	std::cout << message << std::endl;
}