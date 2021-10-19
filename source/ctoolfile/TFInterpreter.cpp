#include "../include/ctoolfile/TFInterpreter.h"
#include "../include/ctoolfile/ChromaToolFile.h"
#include "../include/ctoolfile/TFForward.h"
#include "../include/ctoolfile/TFExpr.h"
#include "../include/ctoolfile/TFStmt.h"
#include "../include/ctoolfile/TFEnums.h"
#include "../include/ctoolfile/TFToken.h"
#include "../include/cscript/CObject.h"
#include "../include/ctoolfile/TFVisitors.h"
#include "../include/tool/Toolbox.h"
#include "../include/tool/ToolSettings.h"
#include "../include/tool/Tool.h"
#include "../include/input/keys.h"
#include <glm.hpp>

#include <string>
#include <vector>
#include <exception>

//
TFInterpreter::TFInterpreter()
{

}
void TFInterpreter::initialize(std::shared_ptr<ChromaToolFile> console)
{
	this->console = console;
}
//
void TFInterpreter::interpret(std::vector<std::shared_ptr<TFStmt>> toolStatements, std::shared_ptr<Toolbox> targetBox)
{
	toolsCount = 0;
	for (std::shared_ptr<TFStmt> statement : toolStatements)
	{
		if (statement != nullptr)
		{
			buildTool(statement, targetBox);
		}
	}
	if (toolsCount == 0) 
	{ 
		console.get()->error("[interpreter:0001] Failed to build any tools."); 
	}
}
//
void TFInterpreter::buildTool(std::shared_ptr<TFStmt> stmt, std::shared_ptr<Toolbox> targetBox)
{
	currentToolbox = targetBox;
	stmt.get()->accept(*this);
	currentToolbox.reset();
	currentTool.reset();
	isMetaSearch = foundMeta = false;
	currentInterestMask = TSetType::null;
	currentSettingID = -1;
	currentSubSettingID = -1;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Visit Statements
//
////////////////////////////////////////////////////////////////////////////////////////////////

void TFInterpreter::visit(std::shared_ptr<TFStmt_Tool> stmt)
{
	// First Construct the target tool by locating the metaData block
	isMetaSearch = true;
	for (std::shared_ptr<TFStmt> settingsBlock : stmt.get()->statements)
	{
		currentSettingsMap.clear();
		settingsBlock.get()->accept(*this);
		if (isMetaSearch == false)
		{
			foundMeta = false;
			settingsBlock.get()->accept(*this);
			std::string toolName = std::get<std::string>(currentSettingsMap.at(std::pair<int, int>(3, -1)).get()->obj);

			int toolID = (int)std::get<double>(currentSettingsMap.at(std::pair<int, int>(0, -1)).get()->obj);
			std::string inID = std::get<std::string>(currentSettingsMap.at(std::pair<int, int>(30, -1)).get()->obj);
			std::string outID = std::get<std::string>(currentSettingsMap.at(std::pair<int, int>(31, -1)).get()->obj);
			std::string controlScheme = std::get<std::string>(currentSettingsMap.at(std::pair<int, int>(32, -1)).get()->obj);

			std::string cursorUp = std::get<std::string>(currentSettingsMap.at(std::pair<int, int>(24, -1)).get()->obj);
			std::string cursorDown = std::get<std::string>(currentSettingsMap.at(std::pair<int, int>(25, -1)).get()->obj);

			int keySig = 0; int modBit = 0; int glfwKey = 0;
			Keybind keybind = Keybind();
			if (currentSettingsMap.at(std::pair<int, int>(21, -1)).get()->objType.type == CLiteralTypes::_CNumber) {
				keySig = (int)std::get<double>(currentSettingsMap.at(std::pair<int, int>(21, -1)).get()->obj);
				modBit = keySig % 10;
				glfwKey = int((keySig - modBit) / 10);
				keybind = Keybind(glfwKey, modBit);
			}
			else if (currentSettingsMap.at(std::pair<int, int>(21, -1)).get()->objType.type == CLiteralTypes::_CString) {
				keybind = stringToKeybind(std::get<std::string>(currentSettingsMap.at(std::pair<int, int>(21, -1)).get()->obj));
			}
			bool result = currentToolbox.get()->createCustomTool(cursorUp, cursorDown, toolID, toolName, inID, controlScheme, outID, keybind);
			if (!result)
			{
				console.get()->error(
					"[interpreter:0102] Tool=" + toolName + std::string(" - Tool File invalid I/O combination. Aborting tool creation."));
				return;
			}
			currentTool = currentToolbox.get()->getTool_lastCreated();
			foundMeta = true;
			// Write the remaining tool data from metaData
			for (auto const& entry : currentSettingsMap)
			{
				if (entry.first.first == 3 ||
					entry.first.first == 0 ||
					entry.first.first == 30 ||
					entry.first.first == 31 ||
					entry.first.first == 32 ||
					entry.first.first == 24 ||
					entry.first.first == 25 ||
					entry.first.first == 21) 
				{
					continue;
				}
				else
				{
					currentTool.get()->putProperty(nullptr, entry.first.first, entry.first.second, entry.second, false, false, false);
				}
			}
			break;
		}
	}
	if (isMetaSearch == true)
	{
		console.get()->error(
			"[interpreter:0101] Line=" + stmt.get()->line.get()->line + std::string(" - Tool File does not contain metaData. Aborting tool creation."));
		return;
	}
	for (std::shared_ptr<TFStmt> settingsBlock : stmt.get()->statements)
	{
		currentSettingsMap.clear();
		settingsBlock.get()->accept(*this);
		for (auto const& entry : currentSettingsMap)
		{
			if (entry.first.first == -1 && entry.first.second == -1)
			{
				currentTool.get()->putInterest(nullptr, currentInterestMask, entry.second, false);
			}
			else
			{
				currentTool.get()->putProperty(nullptr, entry.first.first, entry.first.second, entry.second, false, false, false);
			}
		}
	}
	toolsCount++;
}
void TFInterpreter::visit(std::shared_ptr<TFStmt_SettingsBlock> stmt)
{
	// While searching for metaData, do not write
	if (isMetaSearch)
	{
		if (stmt.get()->type == TFTokenType::METADATA) { isMetaSearch = false; return; }
		else { return; }
	}
	// Set the current interest mask
	currentInterestMask = convertTFTokenType_toTSetType(stmt.get()->type);
	// Discard this visit if it is metadata
	if (currentInterestMask == TSetType::metadata && foundMeta) { return; }
	// Otherwise, create all settings
	for (std::shared_ptr<TFStmt> setting : stmt.get()->settings)
	{
		setting.get()->accept(*this);
	}
}
void TFInterpreter::visit(std::shared_ptr<TFStmt_Setting> stmt)
{
	std::shared_ptr<CObject> name = stmt.get()->name.get()->acceptInterpreter(*this);
	std::shared_ptr<CObject> value = stmt.get()->value.get()->acceptInterpreter(*this);
	currentSettingsMap.insert(std::pair<std::pair<int, int>, std::shared_ptr<CObject>>(std::pair<int, int>(currentSettingID, currentSubSettingID), value));
	currentSettingID = currentSubSettingID = -1;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Visit Expressions
//
////////////////////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<CObject> TFInterpreter::visit(std::shared_ptr<TFExpr_Name> expr)
{
	currentSettingID = expr.get()->settingsID;
	currentSubSettingID = expr.get()->settingSubID;
	return std::make_shared<CObject>(expr.get()->settingsName);
}
std::shared_ptr<CObject> TFInterpreter::visit(std::shared_ptr<TFExpr_Value> expr)
{
	return expr.get()->value;
}


TSetType TFInterpreter::convertTFTokenType_toTSetType(TFTokenType tokenType)
{
	switch (tokenType)
	{
	case TFTokenType::METADATA:
		return TSetType::metadata; break;
	case TFTokenType::PAN:
		return TSetType::pan; break;
	case TFTokenType::ZOOM:
		return TSetType::zoom; break;
	case TFTokenType::ROTATE:
		return TSetType::rotate; break;
	case TFTokenType::SAMPLER:
		return TSetType::sampler; break;
	case TFTokenType::CONTINUOUS:
		return TSetType::continuous; break;
	case TFTokenType::DRAG:
		return TSetType::drag; break;
	case TFTokenType::ONEPOINT:
		return TSetType::onepoint; break;
	case TFTokenType::TWOPOINT:
		return TSetType::twopoint; break;
	case TFTokenType::THREEPOINT:
		return TSetType::threepoint; break;
	case TFTokenType::BLEND:
		return TSetType::blend; break;
	case TFTokenType::BASIC:
		return TSetType::basic; break;
	case TFTokenType::IMAGE:
		return TSetType::image; break;
	case TFTokenType::CHARACTER:
		return TSetType::character; break;
	case TFTokenType::SCATTER:
		return TSetType::scatter; break;
	case TFTokenType::COLOR:
		return TSetType::color; break;
	case TFTokenType::ALPHA:
		return TSetType::alpha; break;
	case TFTokenType::DISTANCE:
		return TSetType::distance; break;
	case TFTokenType::MIXING:
		return TSetType::mixing; break;
	case TFTokenType::MULTITIP:
		return TSetType::multiTip; break;
	case TFTokenType::TEXTURE:
		return TSetType::texture; break;
	case TFTokenType::GRAIN:
		return TSetType::gradient; break;
	case TFTokenType::REPEAT:
		return TSetType::repeat; break;
	case TFTokenType::REFERENCE:
		return TSetType::reference; break;
	case TFTokenType::SMOOTHING:
		return TSetType::smoothing; break;
	case TFTokenType::ANTIALIASING:
		return TSetType::antiAliasing; break;
	case TFTokenType::VECTOR:
		return TSetType::vector; break;
	case TFTokenType::POLYGON:
		return TSetType::polygon; break;
	case TFTokenType::POLYLINE:
		return TSetType::polyline; break;
	case TFTokenType::MESH:
		return TSetType::mesh; break;
	case TFTokenType::VORTEX:
		return TSetType::vortex; break;
	case TFTokenType::FAN:
		return TSetType::fan; break;
	case TFTokenType::RAKE:
		return TSetType::rake; break;
	case TFTokenType::GRADIENT:
		return TSetType::gradient; break;
	case TFTokenType::FILL:
		return TSetType::fill; break;
	case TFTokenType::PATTERN:
		return TSetType::pattern; break;
	case TFTokenType::FIELD:
		return TSetType::field; break;
	case TFTokenType::SHADER:
		return TSetType::shader; break;
	case TFTokenType::FILTER:
		return TSetType::filter; break;
	case TFTokenType::FX_FILL:
		return TSetType::fx_fill; break;
	case TFTokenType::FX_GRADIENT:
		return TSetType::gradient; break;
	case TFTokenType::FX_POSTERIZE:
		return TSetType::fx_posterize; break;
	case TFTokenType::FX_INVERT:
		return TSetType::fx_invert; break;
	case TFTokenType::FX_THRESHOLD:
		return TSetType::fx_threshold; break;
	case TFTokenType::FX_BRIGHTCONTRAST:
		return TSetType::fx_brightnessContrast; break;
	case TFTokenType::FX_HSV:
		return TSetType::fx_HSV; break;
	case TFTokenType::EFFECTS:
		return TSetType::effects; break;
	}
	return TSetType::null;
}