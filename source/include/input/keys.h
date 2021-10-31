#ifndef KEYS_H
#define KEYS_H

class CObject;
#include "../math/math.h"
#include <string>
#include <map>
#include <memory>

enum class InputAction : int {
	press = 1,
	release = 0,
	repeat = 2,
	move = 3
};
// Documentation: Use these as flags for input flow gates
enum class InputHandlerFlag : int {
	reject = 86,
	release = 0,
	press = 1,
	continueInput = 3,
	allowPress = 4,
	allowPress_updateCursor = 5,
	release_continueInput = 6,
	allowMove_updateCursor = 7,
	noSignal = 8,
	allowMove = 9,
	wait = 12,
	previewLine = 13,
	releaseConnect = 14,
	previewCurves = 15,
	releaseCurve = 16
};
// Documentation: Use these to define mod-key combinations, these match the GLFW macros
enum class InputModKey : int {
	none = 0,
	shift = 1,
	ctrl = 2,
	alt = 4,
	shiftctrl = 3,
	shiftalt = 5,
	ctrlalt = 6,
	shiftctrlalt = 7,
};
inline std::map<std::string, InputModKey> stringInputModKeyMap = {
	{ "none", InputModKey::none },
	{ "shift", InputModKey::shift },
	{ "ctrl", InputModKey::ctrl },
	{ "alt", InputModKey::alt },
	//
	{ "shiftctrl", InputModKey::shiftctrl },
	{ "shift+ctrl", InputModKey::shiftctrl },
	//
	{ "shiftalt", InputModKey::shiftalt },
	{ "shift+alt", InputModKey::shiftalt },
	//
	{ "ctrlalt", InputModKey::ctrlalt },
	{ "ctrl+alt", InputModKey::ctrlalt },
	//
	{ "shiftctrlalt", InputModKey::shiftctrlalt },
	{ "shift+ctrl+alt", InputModKey::shiftctrlalt }
};
// #define for custom mouse buttons
enum class InputMouseButton : int {
	null = 86,
	hover = 26,
	left = 0,
	right = 1,
	middle = 2,
	left_dbl = 10, // LEFT + 10
	right_dbl = 11, // RIGHT + 10 
	middle_dbl = 12, // MIDDLE + 10
	scroll = 15,
	scroll_up = 16,
	scroll_down = 17
};

enum class InputKey : int {
	unknown = 0, 
	space = 320, apostrophe = 390, 
	comma = 440, minus = 450,
	period = 460, slash = 470,
	//
	n0 = 480, n1 = 490, n2 = 500, n3 = 510,
	n4 = 520, n5 = 530, n6 = 540, n7 = 550,
	n8 = 560, n9 = 570,
	//
	semicolon = 590, equal = 610,
	//
	a = 650, b = 660, c = 670,
	d = 680, e = 690, f = 700,
	g = 710, h = 720, i = 730,
	j = 740, k = 750, l = 760,
	m = 770, n = 780, o = 790,
	p = 800, q = 810, r = 820,
	s = 830, t = 840, u = 850,
	v = 860, w = 870, x = 880, 
	y = 890, z = 900,
	//
	leftbracket = 910, backslash = 920,
	rightbracket = 930, graveaccent = 960,
	//
	world1 = 1610, world2 = 1620,
	//
	escape = 2560, enter = 2570, tab = 2580,
	backspace = 2590, insert = 2600, deletekey = 2610,
	//
	arr_right = 2620, arr_left = 2630, arr_down = 2640, arr_up = 2650,
	//
	pageup = 2660, pagedown = 2670, home = 2680,
	end = 2690, capslock = 2800, scrolllock = 2810,
	numlock = 2820, printscreen = 2830, pause = 2840,
	//
	f1 = 2900, f2 = 2910, f3 = 2920, f4 = 2930,
	f5 = 2940, f6 = 2950, f7 = 2960, f8 = 2970,
	f9 = 2980, f10 = 2990, f11 = 3000, f12 = 3010,
	f13 = 3020, f14 = 3030, f15 = 3040, f16 = 3050,
	f17 = 3060, f18 = 3070, f19 = 3080, f20 = 3090,
	f21 = 3100, f22 = 3110, f23 = 3120, f24 = 3130, 
	f25 = 3140,
	//
	kp0 = 3200, kp1 = 3210, kp2 = 3220, kp3 = 3230,
	kp4 = 3240, kp5 = 3250, kp6 = 3260, kp7 = 3270,
	kp8 = 3280, kp9 = 3290,
	//
	kpdecimal = 3300, kpdivide = 3310, kpmultiply = 3320, kpsubtract = 3330,
	kpadd = 3340, kpenter = 3350, kpequal = 3360,
	//
	leftshift = 3400, leftctrl = 3410, leftalt = 3420, leftsuper = 3430,
	rightshift = 3440, rightctrl = 3450, rightalt = 3460, rightsuper = 3470,
	//
	menu = 3480,
};

inline std::map<std::string, InputKey> stringInputKeyMap = {
	{ "none", InputKey::unknown }, { "null", InputKey::unknown }, { "unknown", InputKey::unknown },
	{ "space", InputKey::space }, { "apostrophe", InputKey::apostrophe },
	{ "comma", InputKey::comma }, { "minus", InputKey::minus },
	{ "period", InputKey::period }, { "slash", InputKey::slash },
	//
	{ "0", InputKey::n0 }, { "1", InputKey::n1 }, { "2", InputKey::n2 }, { "3", InputKey::n3 },
	{ "4", InputKey::n4 }, { "5", InputKey::n5 }, { "6", InputKey::n6 }, { "7", InputKey::n7 },
	{ "8", InputKey::n8 }, { "9", InputKey::n9 },
	//
	{ "semicolon", InputKey::semicolon }, { "equal", InputKey::equal },
	//
	{ "a", InputKey::a }, { "b", InputKey::b }, { "c", InputKey::c },
	{ "d", InputKey::d }, { "e", InputKey::e }, { "f", InputKey::f },
	{ "g", InputKey::g }, { "h", InputKey::h }, { "i", InputKey::i },
	{ "j", InputKey::j }, { "k", InputKey::k }, { "l", InputKey::l },
	{ "m", InputKey::m }, { "n", InputKey::n }, { "o", InputKey::o },
	{ "p", InputKey::p }, { "q", InputKey::q }, { "r", InputKey::r },
	{ "s", InputKey::s }, { "t", InputKey::t }, { "u", InputKey::u },
	{ "v", InputKey::v }, { "w", InputKey::w }, { "x", InputKey::x },
	{ "y", InputKey::y }, { "z", InputKey::z },
	//
	{ "{", InputKey::leftbracket }, { "}", InputKey::backslash },
	{ "[", InputKey::rightbracket }, { "`", InputKey::graveaccent },
	//
	{ "none", InputKey::world1 }, { "null", InputKey::world2 },
	//
	{ "esc", InputKey::escape }, { "enter", InputKey::enter }, { "tab", InputKey::tab },
	{ "backspace", InputKey::backspace }, { "null", InputKey::insert }, { "unknown", InputKey::deletekey },
	//
	{ "right", InputKey::arr_right }, { "left", InputKey::arr_left }, { "down", InputKey::arr_down }, { "up", InputKey::arr_up },
	//
	{ "pageup", InputKey::pageup }, { "pagedown", InputKey::pagedown }, { "home", InputKey::home },
	{ "end", InputKey::end }, { "capslock", InputKey::capslock }, { "scrolllock", InputKey::scrolllock },
	{ "numlock", InputKey::numlock }, { "printscreen", InputKey::printscreen }, { "pause", InputKey::pause },
	//
	{ "f1", InputKey::f1 }, { "f2", InputKey::f2 }, { "f3", InputKey::f3 }, { "f4", InputKey::f4 },
	{ "f5", InputKey::f5 }, { "f6", InputKey::f6 }, { "f7", InputKey::f7 }, { "f8", InputKey::f8 },
	{ "f9", InputKey::f9 }, { "f10", InputKey::f10 }, { "f11", InputKey::f11 }, { "f12", InputKey::f12 },
	{ "f13", InputKey::f13 }, { "f14", InputKey::f14 }, { "f15", InputKey::f15 }, { "f16", InputKey::f16 },
	{ "f17", InputKey::f17 }, { "f18", InputKey::f18 }, { "f19", InputKey::f19 }, { "f20", InputKey::f20 },
	{ "f21", InputKey::f21 }, { "f22", InputKey::f22 }, { "f23", InputKey::f23 }, { "f24", InputKey::f24 },
	{ "f25", InputKey::f25 },
	//
	{ "kp0", InputKey::kp0 }, { "kp1", InputKey::kp1 }, { "kp2", InputKey::kp2 }, { "kp3", InputKey::kp3 },
	{ "kp4", InputKey::kp4 }, { "kp5", InputKey::kp5 }, { "kp6", InputKey::kp6 }, { "kp7", InputKey::kp7 },
	{ "kp8", InputKey::kp8 }, { "kp9", InputKey::kp9 },
	//
	{ ".", InputKey::kpdecimal }, { "/", InputKey::kpdivide }, { "*", InputKey::kpmultiply }, { "-", InputKey::kpsubtract },
	{ "+", InputKey::kpadd }, { "enter", InputKey::kpenter }, { "=", InputKey::kpequal },
	//
	{ "leftshift", InputKey::leftshift }, { "leftctrl", InputKey::leftctrl }, { "leftalt", InputKey::leftalt }, { "leftsuper", InputKey::leftsuper },
	{ "rightshift", InputKey::rightshift }, { "rightctrl", InputKey::rightctrl }, { "rightalt", InputKey::rightalt }, { "rightsuper", InputKey::rightsuper },
	{ "menu", InputKey::menu }
};

struct Keybind
{
	InputKey glfwKey = InputKey::unknown;
	InputModKey modKey = InputModKey::none;
	Keybind();
	Keybind(InputKey glfwKey, InputModKey modKey);
	Keybind(int glfwKey, int modKey);
	Keybind(int keySig);
};

bool compareKeybind(Keybind left, Keybind right, bool allowNone);
bool compareKeybind(Keybind left, InputKey rightKey, InputModKey rightModKey, bool allowNone);
bool compareModKey(Keybind left, Keybind right, bool allowNone);
bool compareModKey(Keybind left, InputModKey right, bool allowNone);
bool compareModKey(InputModKey left, InputModKey right, bool allowNone);
bool compareModKeyComponent(Keybind left, Keybind right, bool allowNone);
bool compareModKeyComponent(Keybind left, InputModKey right, bool allowNone);
bool compareModKeyComponent(InputModKey left, InputModKey right, bool allowNone);
bool compareModKeyComponent(InputModKey left, Keybind right, bool allowNone);
bool isKeybindModOnly(Keybind keybind, bool acceptNoneMod);
int keybindToInt(Keybind key);
std::string keybindToString(Keybind keybind);
std::string keybindToCharacter(Keybind key);
Keybind stringToKeybind(std::string str);
Keybind intToKeybind(int num);
Keybind objectToKeybind(std::shared_ptr<CObject> obj);

enum class TextInputCmd
{
	nullCmd,
	escapeInput,
	commitInput,
	backspaceChar,
	deleteChar,
	moveCursorLeft,
	moveCursorRight,
	moveCursorUp,
	moveCursorDown,
	moveWordLeft,
	moveWordRight,
	selectMoveLeft,
	selectMoveRight,
	selectWordLeft,
	selectWordRight,
	selectAll,
	selectNone,
	copyText,
	cutText,
	pasteText
};

Keybind createKeybind(int modBit, int glfwKey);
TextInputCmd keybind_toInputCmd(Keybind keybind);

// Check Key Signatures Warning : Need to change all INTEGERS to InputKey as argument later. Tedius but good to do.
// To support changing to InputKey, must fill out all modifer key combinations in the InputKey emum (0 - 7)
bool isKeybind_escape(Keybind keybind);
bool isKeybind_delete(Keybind keybind);
bool isKeybind_space(Keybind keybind);
bool isKeybind_arrows(Keybind keybind);
bool isKeybind_f10f11f12(Keybind keybind);
bool isKeybind_enterTabBackInsert(Keybind keybind);
bool isKeybind_pagination(Keybind keybind);
bool isKeybind_modKey(Keybind keybind);
bool isKeybind_outOfBounds(Keybind keybind);
bool isKeybind_alphaOnly(Keybind keybind);
bool isKeybind_numericOnly(Keybind keybind);
bool isKeybind_punctuation(Keybind keybind);

#endif