#include "../include/input/keys.h"
#include "../include/math/string.h"
#include "../include/cscript/CObject.h"

Keybind::Keybind()
{
	this->glfwKey = InputKey::unknown;
	this->modKey = InputModKey::none;
}
Keybind::Keybind(InputKey glfwKey, InputModKey modKey)
{
	this->glfwKey = glfwKey;
	this->modKey = modKey;
}
Keybind::Keybind(int glfwKey, int modKey)
{
	this->glfwKey = static_cast<InputKey>(glfwKey);
	this->modKey = static_cast<InputModKey>(modKey);
}
Keybind::Keybind(int keySig)
{
	int modKey = keySig % 10;
	int glfwKey = keySig - modKey;
	this->glfwKey = static_cast<InputKey>(glfwKey);
	this->modKey = static_cast<InputModKey>(modKey);
}

bool compareKeybind(Keybind left, Keybind right, bool allowNone)
{
	if ((left.glfwKey == InputKey::unknown || right.glfwKey == InputKey::unknown) && !allowNone) { return false; }
	if (left.glfwKey == right.glfwKey) {
		if (left.modKey == right.modKey) {
			return true;
		}
	}
	return false;
}
bool compareKeybind(Keybind left, InputKey rightKey, InputModKey rightModKey, bool allowNone)
{
	if ((left.glfwKey == InputKey::unknown || rightKey == InputKey::unknown) && !allowNone) { return false; }
	if (left.glfwKey == rightKey) {
		if (left.modKey == rightModKey) {
			return true;
		}
	}
	return false;
}
bool compareModKey(Keybind left, Keybind right, bool allowNone)
{
	if ((left.modKey == InputModKey::none || right.modKey == InputModKey::none) && !allowNone) { return false; }
	if (left.modKey == right.modKey) { return true; }
	return false;
}
bool compareModKey(Keybind left, InputModKey right, bool allowNone)
{
	if ((left.modKey == InputModKey::none || right == InputModKey::none) && !allowNone) { return false; }
	if (left.modKey == right) { return true; }
	return false;
}
bool compareModKey(InputModKey left, InputModKey right, bool allowNone)
{
	if ((left == InputModKey::none || right == InputModKey::none) && !allowNone) { return false; }
	if (left == right) { return true; }
	return false;
}
// Compare keys as components, where the right key is checked whether it is a component of the left.
// Use when comparing keys for inputs where two key states can simultaneously be true.
bool compareModKeyComponent(InputModKey left, InputModKey right, bool allowNone)
{
	if ((left == InputModKey::none || right == InputModKey::none) && !allowNone) { return false; }
	if (left == right) { return true; }
	switch (right) {
	case InputModKey::alt:
		switch (left) {
		case InputModKey::ctrlalt: case InputModKey::shiftalt: case InputModKey::shiftctrlalt: return true; break;
		}
		return false;
	case InputModKey::shift:
		switch (left) {
		case InputModKey::shiftalt: case InputModKey::shiftctrl: case InputModKey::shiftctrlalt: return true; break;
		}
		return false;
	case InputModKey::ctrl:
		switch (left) {
		case InputModKey::ctrlalt: case InputModKey::shiftctrl: case InputModKey::shiftctrlalt: return true; break;
		}
		return false;
	case InputModKey::ctrlalt:
	case InputModKey::shiftalt:
	case InputModKey::shiftctrlalt:
		// Only valid case is '==', handled at the top, or shiftctrlalt
		switch (left) {
		case InputModKey::shiftctrlalt: return true; break;
		}
	default:
		return false;
	}
	return false;
}
bool compareModKeyComponent(InputModKey left, Keybind right, bool allowNone)
{
	return compareModKeyComponent(left, right.modKey, allowNone);
}
bool compareModKeyComponent(Keybind left, Keybind right, bool allowNone)
{
	return compareModKeyComponent(left.modKey, right.modKey, allowNone);
}
bool compareModKeyComponent(Keybind left, InputModKey right, bool allowNone)
{
	return compareModKeyComponent(left.modKey, right, allowNone);
}

bool isKeybindModOnly(Keybind keybind, bool acceptNoneMod)
{
	if (keybind.glfwKey != InputKey::unknown && 
		keybind.glfwKey != InputKey::leftshift &&
		keybind.glfwKey != InputKey::leftctrl &&
		keybind.glfwKey != InputKey::leftalt &&
		keybind.glfwKey != InputKey::leftsuper &&
		keybind.glfwKey != InputKey::rightshift &&
		keybind.glfwKey != InputKey::rightctrl &&
		keybind.glfwKey != InputKey::rightalt &&
		keybind.glfwKey != InputKey::rightsuper) { return false; }
	if (!acceptNoneMod && keybind.modKey == InputModKey::none) { return false; }
	return true;
}
int keybindToInt(Keybind key)
{
	return static_cast<int>(key.glfwKey) + static_cast<int>(key.modKey);
}
Keybind stringToKeybind(std::string str)
{
	if (str == "" || str == " " || str == "NULL" || str == "NONE") {
		return Keybind();
	}
	std::string input = str;
	stringToLower(input);
	stringRemoveSpace(input);
	stringRemoveChar(input, '+');
	bool ctrl = false;
	bool shift = false;
	bool alt = false;
	if (input.find("ctrl") != std::string::npos) {
		ctrl = true;
		size_t pos = input.find("ctrl");
		input.erase(pos, 4);
	}
	if (input.find("shift") != std::string::npos) {
		shift = true;
		size_t pos = input.find("shift");
		input.erase(pos, 5);
	}
	if (input.find("alt") != std::string::npos) {
		alt = true;
		size_t pos = input.find("alt");
		input.erase(pos, 3);
	}
	InputModKey modKey = InputModKey::none;
	if (ctrl && shift && alt) { modKey = InputModKey::shiftctrlalt; }
	else if (shift && alt) { modKey = InputModKey::shiftalt; }
	else if (ctrl && shift) { modKey = InputModKey::shiftctrl; }
	else if (ctrl && alt) { modKey = InputModKey::ctrlalt; }
	else if (shift) { modKey = InputModKey::shift; }
	else if (ctrl) { modKey = InputModKey::ctrl; }
	else if (alt) { modKey = InputModKey::alt; }
	
	InputKey glfwKey = InputKey::unknown;
	if (stringInputKeyMap.count(input) == 1) {
		glfwKey = stringInputKeyMap.at(input);
	}

	return Keybind(glfwKey, modKey);
}
Keybind intToKeybind(int num)
{
	if (num < 8 && num >= 0) {
		return Keybind(InputKey::unknown, static_cast<InputModKey>(num));
	}
	int modKey = num % 10;
	int glfwKey = num - modKey;
	return Keybind(glfwKey, modKey);
}
Keybind objectToKeybind(std::shared_ptr<CObject> obj)
{
	if (obj.get()->objType.type == CLiteralTypes::_CNumber) {
		return intToKeybind((int)std::get<double>(obj.get()->obj));
	}
	else if (obj.get()->objType.type == CLiteralTypes::_CString) {
		return stringToKeybind(std::get<std::string>(obj.get()->obj));
	}
	return Keybind();
}

std::string keybindToCharacter(Keybind keybind)
{
	switch (keybind.glfwKey)
	{
	case InputKey::semicolon: 
		if (keybind.modKey == InputModKey::shift) { return ":"; }
		else if (keybind.modKey == InputModKey::none) { return ";"; }
		break;
	case InputKey::equal: 
		if (keybind.modKey == InputModKey::shift) { return "+"; }
		else if (keybind.modKey == InputModKey::none) { return "="; }
		break;
	case InputKey::space:
		if (keybind.modKey == InputModKey::shift) { return " "; }
		else if (keybind.modKey == InputModKey::none) { return " "; }
		break;
	case InputKey::apostrophe:
		if (keybind.modKey == InputModKey::shift) { return "\""; }
		else if (keybind.modKey == InputModKey::none) { return "'"; }
		break;
	case InputKey::comma:
		if (keybind.modKey == InputModKey::shift) { return "<"; }
		else if (keybind.modKey == InputModKey::none) { return ","; }
		break;
	case InputKey::minus:
		if (keybind.modKey == InputModKey::shift) { return "_"; }
		else if (keybind.modKey == InputModKey::none) { return "-"; }
		break;
	case InputKey::period:
		if (keybind.modKey == InputModKey::shift) { return ">"; }
		else if (keybind.modKey == InputModKey::none) { return "."; }
		break;
	case InputKey::slash:
		if (keybind.modKey == InputModKey::shift) { return "?"; }
		else if (keybind.modKey == InputModKey::none) { return "/"; }
		break;
	case InputKey::leftbracket:
		if (keybind.modKey == InputModKey::shift) { return "{"; }
		else if (keybind.modKey == InputModKey::none) { return "["; }
		break;
	case InputKey::backslash:
		if (keybind.modKey == InputModKey::shift) { return "|"; }
		else if (keybind.modKey == InputModKey::none) { return "\\"; }
		break;
	case InputKey::rightbracket:
		if (keybind.modKey == InputModKey::shift) { return "}"; }
		else if (keybind.modKey == InputModKey::none) { return "]"; }
		break;
	case InputKey::graveaccent: 
		if (keybind.modKey == InputModKey::shift) { return "~"; }
		else if (keybind.modKey == InputModKey::none) { return "`"; }
		break;
		// Numbers
	case InputKey::n0: 
		if (keybind.modKey == InputModKey::shift) { return ")"; }
		else if (keybind.modKey == InputModKey::none) { return "0"; }
		break;
	case InputKey::n1: 
		if (keybind.modKey == InputModKey::shift) { return "!"; }
		else if (keybind.modKey == InputModKey::none) { return "1"; }
		break;
	case InputKey::n2: 
		if (keybind.modKey == InputModKey::shift) { return "@"; }
		else if (keybind.modKey == InputModKey::none) { return "2"; }
		break;
	case InputKey::n3: 
		if (keybind.modKey == InputModKey::shift) { return "#"; }
		else if (keybind.modKey == InputModKey::none) { return "3"; }
		break;
	case InputKey::n4: 
		if (keybind.modKey == InputModKey::shift) { return "$"; }
		else if (keybind.modKey == InputModKey::none) { return "4"; }
		break;
	case InputKey::n5: 
		if (keybind.modKey == InputModKey::shift) { return "%"; }
		else if (keybind.modKey == InputModKey::none) { return "5"; }
		break;
	case InputKey::n6: 
		if (keybind.modKey == InputModKey::shift) { return "^"; }
		else if (keybind.modKey == InputModKey::none) { return "6"; }
		break;
	case InputKey::n7: 
		if (keybind.modKey == InputModKey::shift) { return "&"; }
		else if (keybind.modKey == InputModKey::none) { return "7"; }
		break;
	case InputKey::n8: 
		if (keybind.modKey == InputModKey::shift) { return "*"; }
		else if (keybind.modKey == InputModKey::none) { return "8"; }
		break;
	case InputKey::n9: 
		if (keybind.modKey == InputModKey::shift) { return "("; }
		else if (keybind.modKey == InputModKey::none) { return "9"; }
		break;
		// Letters
	case InputKey::a:
		if (keybind.modKey == InputModKey::shift) { return "A"; }
		else if (keybind.modKey == InputModKey::none) { return "a"; }
		break;
	case InputKey::b:
		if (keybind.modKey == InputModKey::shift) { return "B"; }
		else if (keybind.modKey == InputModKey::none) { return "b"; }
		break;
	case InputKey::c:
		if (keybind.modKey == InputModKey::shift) { return "C"; }
		else if (keybind.modKey == InputModKey::none) { return "c"; }
		break;
	case InputKey::d:
		if (keybind.modKey == InputModKey::shift) { return "D"; }
		else if (keybind.modKey == InputModKey::none) { return "d"; }
		break;
	case InputKey::e:
		if (keybind.modKey == InputModKey::shift) { return "E"; }
		else if (keybind.modKey == InputModKey::none) { return "e"; }
		break;

	case InputKey::f:
		if (keybind.modKey == InputModKey::shift) { return "F"; }
		else if (keybind.modKey == InputModKey::none) { return "f"; }
		break;
	case InputKey::g:
		if (keybind.modKey == InputModKey::shift) { return "G"; }
		else if (keybind.modKey == InputModKey::none) { return "g"; }
		break;
	case InputKey::h:
		if (keybind.modKey == InputModKey::shift) { return "H"; }
		else if (keybind.modKey == InputModKey::none) { return "h"; }
		break;
	case InputKey::i:
		if (keybind.modKey == InputModKey::shift) { return "I"; }
		else if (keybind.modKey == InputModKey::none) { return "i"; }
		break;
	case InputKey::j:
		if (keybind.modKey == InputModKey::shift) { return "J"; }
		else if (keybind.modKey == InputModKey::none) { return "j"; }
		break;

	case InputKey::k:
		if (keybind.modKey == InputModKey::shift) { return "K"; }
		else if (keybind.modKey == InputModKey::none) { return "k"; }
		break;
	case InputKey::l:
		if (keybind.modKey == InputModKey::shift) { return "L"; }
		else if (keybind.modKey == InputModKey::none) { return "l"; }
		break;
	case InputKey::m:
		if (keybind.modKey == InputModKey::shift) { return "M"; }
		else if (keybind.modKey == InputModKey::none) { return "m"; }
		break;
	case InputKey::n:
		if (keybind.modKey == InputModKey::shift) { return "N"; }
		else if (keybind.modKey == InputModKey::none) { return "n"; }
		break;
	case InputKey::o:
		if (keybind.modKey == InputModKey::shift) { return "O"; }
		else if (keybind.modKey == InputModKey::none) { return "o"; }
		break;

	case InputKey::p:
		if (keybind.modKey == InputModKey::shift) { return "P"; }
		else if (keybind.modKey == InputModKey::none) { return "p"; }
		break;
	case InputKey::q:
		if (keybind.modKey == InputModKey::shift) { return "Q"; }
		else if (keybind.modKey == InputModKey::none) { return "q"; }
		break;
	case InputKey::r:
		if (keybind.modKey == InputModKey::shift) { return "R"; }
		else if (keybind.modKey == InputModKey::none) { return "r"; }
		break;
	case InputKey::s:
		if (keybind.modKey == InputModKey::shift) { return "S"; }
		else if (keybind.modKey == InputModKey::none) { return "s"; }
		break;
	case InputKey::t:
		if (keybind.modKey == InputModKey::shift) { return "T"; }
		else if (keybind.modKey == InputModKey::none) { return "t"; }
		break;

	case InputKey::u:
		if (keybind.modKey == InputModKey::shift) { return "U"; }
		else if (keybind.modKey == InputModKey::none) { return "u"; }
		break;
	case InputKey::v:
		if (keybind.modKey == InputModKey::shift) { return "V"; }
		else if (keybind.modKey == InputModKey::none) { return "v"; }
		break;
	case InputKey::w:
		if (keybind.modKey == InputModKey::shift) { return "W"; }
		else if (keybind.modKey == InputModKey::none) { return "w"; }
		break;
	case InputKey::x:
		if (keybind.modKey == InputModKey::shift) { return "X"; }
		else if (keybind.modKey == InputModKey::none) { return "x"; }
		break;
	case InputKey::y:
		if (keybind.modKey == InputModKey::shift) { return "Y"; }
		else if (keybind.modKey == InputModKey::none) { return "y"; }
		break;

	case InputKey::z:
		if (keybind.modKey == InputModKey::shift) { return "Z"; }
		else if (keybind.modKey == InputModKey::none) { return "z"; }
		break;

	default: return "";
	}
	return "";
}

Keybind createKeybind(int modBit, int glfwKey)
{
	if (modBit <= static_cast<int>(InputKey::unknown) && (glfwKey <= 0)) { return Keybind(); }
	int mod = clampi(modBit, 0, 7);
	return Keybind(glfwKey * 10, mod);
}

TextInputCmd keybind_toInputCmd(Keybind keybind)
{
	switch (keybind.glfwKey)
	{
	case InputKey::escape: return TextInputCmd::escapeInput;
	case InputKey::enter: return TextInputCmd::commitInput;
	case InputKey::backspace: return TextInputCmd::backspaceChar;
	case InputKey::arr_left: 
		if (keybind.modKey == InputModKey::none) { return TextInputCmd::moveCursorLeft; }
		if (keybind.modKey == InputModKey::shift) { return TextInputCmd::selectMoveLeft; }
		if (keybind.modKey == InputModKey::ctrl) { return TextInputCmd::moveWordLeft; }
		if (keybind.modKey == InputModKey::shiftctrl) { return TextInputCmd::selectWordLeft; }
		break;
	case InputKey::arr_right: 
		if (keybind.modKey == InputModKey::none) { return TextInputCmd::moveCursorRight; }
		if (keybind.modKey == InputModKey::shift) { return TextInputCmd::selectMoveRight; }
		if (keybind.modKey == InputModKey::ctrl) { return TextInputCmd::moveWordRight; }
		if (keybind.modKey == InputModKey::shiftctrl) { return TextInputCmd::selectWordRight; }
		break;
	case InputKey::arr_up: return TextInputCmd::moveCursorUp;
	case InputKey::arr_down: return TextInputCmd::moveCursorRight;
	case InputKey::a: 
		if (keybind.modKey == InputModKey::ctrl) { return TextInputCmd::selectAll; } break;
	case InputKey::c: 
		if (keybind.modKey == InputModKey::ctrl) { return TextInputCmd::copyText; } break;
	case InputKey::v: 
		if (keybind.modKey == InputModKey::ctrl) { return TextInputCmd::pasteText; } break;
	case InputKey::x: 
		if (keybind.modKey == InputModKey::ctrl) { return TextInputCmd::cutText; } break;
	}
	return TextInputCmd::nullCmd;
}


std::string keybindToString(Keybind keybind)
{
	std::string modString = "";
	switch (keybind.modKey)
	{
	case InputModKey::none: modString = ""; break;
	case InputModKey::shift: modString = "SHIFT"; break;
	case InputModKey::ctrl: modString = "CTRL"; break;
	case InputModKey::alt: modString = "ALT"; break;
	case InputModKey::shiftctrl: modString = "SHIFT+CTRL"; break;
	case InputModKey::shiftalt: modString = "SHIFT+ALT"; break;
	case InputModKey::ctrlalt: modString = "CTRL+ALT"; break;
	case InputModKey::shiftctrlalt: modString = "SHIFT+CTRL+ALT"; break;
	}
	std::string keyString = "";
	switch (keybind.glfwKey)
	{
	case InputKey::space: keyString = "SPACE"; break;
	case InputKey::apostrophe: keyString = "'"; break; 
	case InputKey::comma: keyString = ","; break;
	case InputKey::minus: keyString = "-"; break; 
	case InputKey::period: keyString = "."; break; 
	case InputKey::slash: keyString = "/"; break; 
	case InputKey::n0: keyString = "0"; break;
	case InputKey::n1: keyString = "1"; break;
	case InputKey::n2: keyString = "2"; break;
	case InputKey::n3: keyString = "3"; break;
	case InputKey::n4: keyString = "4"; break;
	case InputKey::n5: keyString = "5"; break;
	case InputKey::n6: keyString = "6"; break;
	case InputKey::n7: keyString = "7"; break;
	case InputKey::n8: keyString = "8"; break;
	case InputKey::n9: keyString = "9"; break;
	case InputKey::semicolon: keyString = ";"; break; 
	case InputKey::equal: keyString = "="; break; 
	case InputKey::a: keyString = "A"; break;
	case InputKey::b: keyString = "B"; break;
	case InputKey::c: keyString = "C"; break;
	case InputKey::d: keyString = "D"; break;
	case InputKey::e: keyString = "E"; break;
	case InputKey::f: keyString = "F"; break;
	case InputKey::g: keyString = "G"; break;
	case InputKey::h: keyString = "H"; break;
	case InputKey::i: keyString = "I"; break;
	case InputKey::j: keyString = "J"; break;
	case InputKey::k: keyString = "K"; break;
	case InputKey::l: keyString = "L"; break;
	case InputKey::m: keyString = "M"; break;
	case InputKey::n: keyString = "N"; break;
	case InputKey::o: keyString = "O"; break;
	case InputKey::p: keyString = "P"; break;
	case InputKey::q: keyString = "Q"; break;
	case InputKey::r: keyString = "R"; break;
	case InputKey::s: keyString = "S"; break;
	case InputKey::t: keyString = "T"; break;
	case InputKey::u: keyString = "U"; break;
	case InputKey::v: keyString = "V"; break;
	case InputKey::w: keyString = "W"; break;
	case InputKey::x: keyString = "X"; break;
	case InputKey::y: keyString = "Y"; break;
	case InputKey::z: keyString = "Z"; break;
	case InputKey::leftbracket: keyString = "["; break; 
	case InputKey::backslash: keyString = "\\"; break; 
	case InputKey::rightbracket: keyString = "]"; break;
	case InputKey::graveaccent: keyString = "`"; break;
	case InputKey::insert: keyString = "INS"; break;
	case InputKey::deletekey: keyString = "DEL"; break;
	case InputKey::arr_right: keyString = "RIGHT"; break;
	case InputKey::arr_left: keyString = "LEFT"; break;
	case InputKey::arr_down: keyString = "DOWN"; break;
	case InputKey::arr_up: keyString = "UP"; break;
	case InputKey::pageup: keyString = "PAGE-UP"; break;
	case InputKey::pagedown: keyString = "PAGE-DOWN"; break;
	case InputKey::home: keyString = "HOME"; break;
	case InputKey::end: keyString = "END"; break;
	case InputKey::pause: keyString = "PAUSE"; break;
	case InputKey::f1: keyString = "F1"; break;
	case InputKey::f2: keyString = "F2"; break;
	case InputKey::f3: keyString = "F3"; break;
	case InputKey::f4: keyString = "F4"; break;
	case InputKey::f5: keyString = "F5"; break;
	case InputKey::f6: keyString = "F6"; break;
	case InputKey::f7: keyString = "F7"; break;
	case InputKey::f8: keyString = "F8"; break;
	case InputKey::f9: keyString = "F9"; break;
	case InputKey::f10: keyString = "F10"; break;
	case InputKey::f11: keyString = "F11"; break;
	case InputKey::f12: keyString = "F12"; break;
	case InputKey::f13: keyString = "F13"; break;
	case InputKey::f14: keyString = "F14"; break;
	case InputKey::f15: keyString = "F15"; break;
	case InputKey::f16: keyString = "F16"; break;
	case InputKey::f17: keyString = "F17"; break;
	case InputKey::f18: keyString = "F18"; break;
	case InputKey::f19: keyString = "F19"; break;
	case InputKey::f20: keyString = "F20"; break;
	case InputKey::f21: keyString = "F21"; break;
	case InputKey::f22: keyString = "F22"; break;
	case InputKey::f23: keyString = "F23"; break;
	case InputKey::f24: keyString = "F24"; break;
	case InputKey::f25: keyString = "F25"; break;
	case InputKey::menu: keyString = "F26"; break;
	}
	if (modString == "" && keyString != "")
	{
		return keyString;
	}
	else if (modString != "" && keyString != "")
	{
		return modString + "+" + keyString;
	}
	else if (modString != "" && keyString == "")
	{
		return modString;
	}
	return "NONE";
}

// Check Key Signatures Warning : Need to change all INTEGERS to InputKey as argument later. Tedius but good to do.
// To support changing to InputKey, must fill out all modifer key combinations in the InputKey emum (0 - 7)
bool isKeybind_escape(Keybind keybind)
{
	if (keybind.glfwKey == InputKey::escape) { return true; }
	return false;
}
bool isKeybind_delete(Keybind keybind)
{
	if (keybind.glfwKey == InputKey::deletekey) { return true; }
	return false;
}
bool isKeybind_space(Keybind keybind)
{
	if (keybind.glfwKey == InputKey::space) { return true; }
	return false;
}
bool isKeybind_arrows(Keybind keybind)
{
	// RIGHT / LEFT / DOWN / UP ARROWS
	if (keybind.glfwKey == InputKey::arr_left || 
		keybind.glfwKey == InputKey::arr_right || 
		keybind.glfwKey == InputKey::arr_up || 
		keybind.glfwKey == InputKey::arr_down) 
	{ 
		return true; 
	}
	return false;
}
bool isKeybind_f10f11f12(Keybind keybind)
{
	// F10 / F11 / F12
	if (keybind.glfwKey == InputKey::f10 ||
		keybind.glfwKey == InputKey::f11 ||
		keybind.glfwKey == InputKey::f12)
	{
		return true;
	}
	return false;
}
bool isKeybind_enterTabBackInsert(Keybind keybind)
{
	// ENTER / TAB / BACKSPACE / INSERT
	if (keybind.glfwKey == InputKey::enter ||
		keybind.glfwKey == InputKey::tab ||
		keybind.glfwKey == InputKey::backspace ||
		keybind.glfwKey == InputKey::insert)
	{
		return true;
	}
	return false;
}
bool isKeybind_pagination(Keybind keybind)
{
	// PAGE_UP / PAGE_DOWN / HOME / END / CAPS_LOCK / SCROLL_LOCK / NUM_LOCK / PRINT_SCREEN / PAUSE
	if (keybind.glfwKey == InputKey::pageup ||
		keybind.glfwKey == InputKey::pagedown ||
		keybind.glfwKey == InputKey::home ||
		keybind.glfwKey == InputKey::end ||
		keybind.glfwKey == InputKey::capslock ||
		keybind.glfwKey == InputKey::scrolllock ||
		keybind.glfwKey == InputKey::numlock ||
		keybind.glfwKey == InputKey::printscreen ||
		keybind.glfwKey == InputKey::pause)
	{
		return true;
	}
	return false;
}
bool isKeybind_modKey(Keybind keybind)
{
	// RIGHT & LEFT SHIFT / CTRL / ALT / SUPER / MENU
	if (keybind.glfwKey == InputKey::rightshift ||
		keybind.glfwKey == InputKey::rightctrl ||
		keybind.glfwKey == InputKey::rightalt ||
		keybind.glfwKey == InputKey::rightsuper ||
		keybind.glfwKey == InputKey::leftshift ||
		keybind.glfwKey == InputKey::leftctrl ||
		keybind.glfwKey == InputKey::leftalt ||
		keybind.glfwKey == InputKey::leftsuper ||
		keybind.glfwKey == InputKey::menu)
	{
		return true;
	}
	return false;
}
bool isKeybind_outOfBounds(Keybind keybind)
{
	// OUT OF BOUNDS SIGNATURE
	if (keybind.glfwKey == InputKey::unknown) { return true; }
	return false;
}
bool isKeybind_alphaOnly(Keybind keybind)
{
	int keySig = static_cast<int>(keybind.glfwKey);
	if (keySig >= 650 && keySig <= 900) { return true; }
	return false;
}
bool isKeybind_numericOnly(Keybind keybind)
{
	int keySig = static_cast<int>(keybind.glfwKey);
	if (keySig >= 480 && keySig <= 570) { return true; }
	return false;
}
bool isKeybind_punctuation(Keybind keybind)
{
	if (keybind.glfwKey == InputKey::semicolon ||
		keybind.glfwKey == InputKey::equal ||
		keybind.glfwKey == InputKey::apostrophe ||
		keybind.glfwKey == InputKey::comma ||
		keybind.glfwKey == InputKey::minus ||
		keybind.glfwKey == InputKey::period ||
		keybind.glfwKey == InputKey::slash ||
		keybind.glfwKey == InputKey::leftbracket ||
		keybind.glfwKey == InputKey::backslash ||
		keybind.glfwKey == InputKey::rightbracket)
	{
		return true;
	}
	return false;
}