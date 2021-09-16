#ifndef KEYS_H
#define KEYS_H

#include <string>

#define INPUT_KEY_UNKNOWN -1
#define INPUT_KEY_SPACE 320
#define INPUT_KEY_APOSTROPHE 390 /* ' */
#define INPUT_KEY_QUOTE 391 /* " */
#define INPUT_KEY_COMMA 440 /* , */
#define INPUT_KEY_ANGLELEFT 441 /* < */
#define INPUT_KEY_MINUS 450 /* - */
#define INPUT_KEY_UNDERSCORE 451 /* _ */
#define INPUT_KEY_PERIOD 460 /* . */
#define INPUT_KEY_ANGLERIGHT 461 /* > */
#define INPUT_KEY_SLASH 470 /* / */
#define INPUT_KEY_QUESTION 471 /* ? */
#define INPUT_KEY_0 480
#define INPUT_KEY_LEFTPAREN 481
#define INPUT_KEY_1 490
#define INPUT_KEY_EXCLAMATION 491
#define INPUT_KEY_2 500
#define INPUT_KEY_AT 501
#define INPUT_KEY_3 510
#define INPUT_KEY_POUND 511
#define INPUT_KEY_4 520
#define INPUT_KEY_DOLLAR 521
#define INPUT_KEY_5 530
#define INPUT_KEY_PERCENT 531
#define INPUT_KEY_6 540
#define INPUT_KEY_CARET 541
#define INPUT_KEY_7 550
#define INPUT_KEY_AMPERSAND 551
#define INPUT_KEY_8 560
#define INPUT_KEY_STAR 561
#define INPUT_KEY_9 570
#define INPUT_KEY_RIGHTPAREN 571
#define INPUT_KEY_SEMICOLON 590 /* ; */
#define INPUT_KEY_COLON 591 /* : */
#define INPUT_KEY_EQUAL 610 /* = */
#define INPUT_KEY_PLUS 611 /* + */
#define INPUT_KEY_A 650
#define INPUT_KEY_A_SHIFT 651
#define INPUT_KEY_A_CTRL 652
#define INPUT_KEY_B 660
#define INPUT_KEY_B_SHIFT 661
#define INPUT_KEY_C 670
#define INPUT_KEY_C_SHIFT 671
#define INPUT_KEY_C_CTRL 672
#define INPUT_KEY_D 680
#define INPUT_KEY_D_SHIFT 681
#define INPUT_KEY_E 690
#define INPUT_KEY_E_SHIFT 691
#define INPUT_KEY_F 700
#define INPUT_KEY_F_SHIFT 701
#define INPUT_KEY_G 710
#define INPUT_KEY_G_SHIFT 711
#define INPUT_KEY_H 720
#define INPUT_KEY_H_SHIFT 721
#define INPUT_KEY_I 730
#define INPUT_KEY_I_SHIFT 731
#define INPUT_KEY_J 740
#define INPUT_KEY_J_SHIFT 741
#define INPUT_KEY_K 750
#define INPUT_KEY_K_SHIFT 751
#define INPUT_KEY_L 760
#define INPUT_KEY_L_SHIFT 761
#define INPUT_KEY_M 770
#define INPUT_KEY_M_SHIFT 771
#define INPUT_KEY_N 780
#define INPUT_KEY_N_SHIFT 781
#define INPUT_KEY_O 790
#define INPUT_KEY_O_SHIFT 791
#define INPUT_KEY_P 800
#define INPUT_KEY_P_SHIFT 801
#define INPUT_KEY_Q 810
#define INPUT_KEY_Q_SHIFT 811
#define INPUT_KEY_R 820
#define INPUT_KEY_R_SHIFT 821
#define INPUT_KEY_S 830
#define INPUT_KEY_S_SHIFT 831
#define INPUT_KEY_S_CTRL 832
#define INPUT_KEY_T 840
#define INPUT_KEY_T_SHIFT 841
#define INPUT_KEY_U 850
#define INPUT_KEY_U_SHIFT 851
#define INPUT_KEY_V 860
#define INPUT_KEY_V_SHIFT 861
#define INPUT_KEY_V_CTRL 862
#define INPUT_KEY_W 870
#define INPUT_KEY_W_SHIFT 871
#define INPUT_KEY_X 880
#define INPUT_KEY_X_SHIFT 881
#define INPUT_KEY_X_CTRL 882
#define INPUT_KEY_Y 890
#define INPUT_KEY_Y_SHIFT 891
#define INPUT_KEY_Z 900
#define INPUT_KEY_Z_SHIFT 901
#define INPUT_KEY_LEFT_BRACKET 910 /* [ */
#define INPUT_KEY_LEFT_CURLYLEFT 911 /* { */
#define INPUT_KEY_BACKSLASH 920 /* \ */
#define INPUT_KEY_BAR 921 /* | */
#define INPUT_KEY_RIGHT_BRACKET 930 /* ] */
#define INPUT_KEY_RIGHT_CURLYRIGHT 931 /* } */
#define INPUT_KEY_GRAVE_ACCENT 960 /* ` */
#define INPUT_KEY_TILDE 961 /* ~ */
#define INPUT_KEY_WORLD_1 1610 /* non-US #1 */
#define INPUT_KEY_WORLD_2 1620 /* non-US #2 */
#define INPUT_KEY_ESCAPE 2560
#define INPUT_KEY_ENTER 2570
#define INPUT_KEY_TAB 2580
#define INPUT_KEY_BACKSPACE 2590
#define INPUT_KEY_INSERT 2600
#define INPUT_KEY_DELETE 2610
//
#define INPUT_KEY_RIGHT 2620
#define INPUT_KEY_RIGHT_SHIFT 2621
#define INPUT_KEY_RIGHT_CTRL 2622
#define INPUT_KEY_RIGHT_CTRLSHIFT 2623
//
#define INPUT_KEY_LEFT 2630
#define INPUT_KEY_LEFT_SHIFT 2631
#define INPUT_KEY_LEFT_CTRL 2632
#define INPUT_KEY_LEFT_CTRLSHIFT 2633
//
#define INPUT_KEY_DOWN 2640
#define INPUT_KEY_DOWN_SHIFT 2641
#define INPUT_KEY_DOWN_CTRL 2642
#define INPUT_KEY_DOWN_CTRLSHIFT 2643
//
#define INPUT_KEY_UP 2650
#define INPUT_KEY_UP_SHIFT 2651
#define INPUT_KEY_UP_CTRL 2652
#define INPUT_KEY_UP_CTRLSHIFT 2653
//
#define INPUT_KEY_PAGE_UP 2660
#define INPUT_KEY_PAGE_DOWN 2670
#define INPUT_KEY_HOME 2680
#define INPUT_KEY_END 2690
#define INPUT_KEY_CAPS_LOCK 2800
#define INPUT_KEY_SCROLL_LOCK 2810
#define INPUT_KEY_NUM_LOCK 2820
#define INPUT_KEY_PRINT_SCREEN 2830
#define INPUT_KEY_PAUSE 2840
#define INPUT_KEY_F1 2900
#define INPUT_KEY_F2 2910
#define INPUT_KEY_F3 2920
#define INPUT_KEY_F4 2930
#define INPUT_KEY_F5 2940
#define INPUT_KEY_F6 2950
#define INPUT_KEY_F7 2960
#define INPUT_KEY_F8 2970
#define INPUT_KEY_F9 2980
#define INPUT_KEY_F10 2990
#define INPUT_KEY_F11 3000
#define INPUT_KEY_F12 3010
#define INPUT_KEY_F13 3020
#define INPUT_KEY_F14 3030
#define INPUT_KEY_F15 3040
#define INPUT_KEY_F16 3050
#define INPUT_KEY_F17 3060
#define INPUT_KEY_F18 3070
#define INPUT_KEY_F19 3080
#define INPUT_KEY_F20 3090
#define INPUT_KEY_F21 3100
#define INPUT_KEY_F22 3110
#define INPUT_KEY_F23 3120
#define INPUT_KEY_F24 3130
#define INPUT_KEY_F25 3140
#define INPUT_KEY_KP_0 3200
#define INPUT_KEY_KP_1 3210
#define INPUT_KEY_KP_2 3220
#define INPUT_KEY_KP_3 3230
#define INPUT_KEY_KP_4 3240
#define INPUT_KEY_KP_5 3250
#define INPUT_KEY_KP_6 3260
#define INPUT_KEY_KP_7 3270
#define INPUT_KEY_KP_8 3280
#define INPUT_KEY_KP_9 3290
#define INPUT_KEY_KP_DECIMAL 3300
#define INPUT_KEY_KP_DIVIDE 3310
#define INPUT_KEY_KP_MULTIPLY 3320
#define INPUT_KEY_KP_SUBTRACT 3330
#define INPUT_KEY_KP_ADD 3340
#define INPUT_KEY_KP_ENTER 3350
#define INPUT_KEY_KP_EQUAL 3360
#define INPUT_KEY_MODLEFT_SHIFT 3400
#define INPUT_KEY_MODLEFT_CONTROL 3410
#define INPUT_KEY_MODLEFT_ALT 3420
#define INPUT_KEY_MODLEFT_SUPER 3430
#define INPUT_KEY_MODRIGHT_SHIFT 3440
#define INPUT_KEY_MODRIGHT_CONTROL 3450
#define INPUT_KEY_MODRIGHT_ALT 3460
#define INPUT_KEY_MODRIGHT_SUPER 3470
#define INPUT_KEY_MENU 3480
#define INPUT_KEY_LAST INPUT_KEY_MENU

inline std::string keySig_toChar(int sig)
{
	switch (sig)
	{
	// Symbols
	case INPUT_KEY_TILDE: return "~";
	case INPUT_KEY_LEFT_CURLYLEFT: return "{";
	case INPUT_KEY_BAR: return "|";
	case INPUT_KEY_RIGHT_CURLYRIGHT: return "}";
	case INPUT_KEY_COLON: return ":";
	case INPUT_KEY_PLUS: return "+";
	case INPUT_KEY_QUOTE: return "\"";
	case INPUT_KEY_ANGLELEFT: return "<";
	case INPUT_KEY_UNDERSCORE: return "_";
	case INPUT_KEY_ANGLERIGHT: return ">";
	case INPUT_KEY_QUESTION: return "?";
	case INPUT_KEY_LEFTPAREN: return "(";
	case INPUT_KEY_RIGHTPAREN: return ")";
	case INPUT_KEY_EXCLAMATION: return "!";
	case INPUT_KEY_AT: return "@";
	case INPUT_KEY_POUND: return "#";
	case INPUT_KEY_DOLLAR: return "$";
	case INPUT_KEY_PERCENT: return "%";
	case INPUT_KEY_CARET: return "^";
	case INPUT_KEY_AMPERSAND: return "&";
	case INPUT_KEY_STAR: return "*";
	case INPUT_KEY_SEMICOLON: return ";";
	case INPUT_KEY_EQUAL: return "=";
	case INPUT_KEY_SPACE: return " ";
	case (INPUT_KEY_SPACE + 1): return " ";
	case INPUT_KEY_APOSTROPHE: return "'";
	case INPUT_KEY_COMMA: return ",";
	case INPUT_KEY_MINUS: return "-";
	case INPUT_KEY_PERIOD: return ".";
	case INPUT_KEY_SLASH: return "/";
	case INPUT_KEY_LEFT_BRACKET: return "[";
	case INPUT_KEY_BACKSLASH: return "\"";
	case INPUT_KEY_RIGHT_BRACKET: return "]";
	case INPUT_KEY_GRAVE_ACCENT: return "`";
	// Numbers
	case INPUT_KEY_0: return "0";
	case INPUT_KEY_1: return "1";
	case INPUT_KEY_2: return "2";
	case INPUT_KEY_3: return "3";
	case INPUT_KEY_4: return "4";
	case INPUT_KEY_5: return "5";
	case INPUT_KEY_6: return "6";
	case INPUT_KEY_7: return "7";
	case INPUT_KEY_8: return "8";
	case INPUT_KEY_9: return "9";
	// Letters
	case INPUT_KEY_A: return "a";
	case INPUT_KEY_A_SHIFT: return "A";
	case INPUT_KEY_B: return "b";
	case INPUT_KEY_B_SHIFT: return "B";
	case INPUT_KEY_C: return "c";
	case INPUT_KEY_C_SHIFT: return "C";
	case INPUT_KEY_D: return "d";
	case INPUT_KEY_D_SHIFT: return "D";
	case INPUT_KEY_E: return "e";
	case INPUT_KEY_E_SHIFT: return "E";

	case INPUT_KEY_F: return "f";
	case INPUT_KEY_F_SHIFT: return "F";
	case INPUT_KEY_G: return "g";
	case INPUT_KEY_G_SHIFT: return "G";
	case INPUT_KEY_H: return "h";
	case INPUT_KEY_H_SHIFT: return "H";
	case INPUT_KEY_I: return "i";
	case INPUT_KEY_I_SHIFT: return "I";
	case INPUT_KEY_J: return "j";
	case INPUT_KEY_J_SHIFT: return "J";

	case INPUT_KEY_K: return "k";
	case INPUT_KEY_K_SHIFT: return "K";
	case INPUT_KEY_L: return "l";
	case INPUT_KEY_L_SHIFT: return "L";
	case INPUT_KEY_M: return "m";
	case INPUT_KEY_M_SHIFT: return "M";
	case INPUT_KEY_N: return "n";
	case INPUT_KEY_N_SHIFT: return "N";
	case INPUT_KEY_O: return "o";
	case INPUT_KEY_O_SHIFT: return "O";

	case INPUT_KEY_P: return "p";
	case INPUT_KEY_P_SHIFT: return "P";
	case INPUT_KEY_Q: return "q";
	case INPUT_KEY_Q_SHIFT: return "Q";
	case INPUT_KEY_R: return "r";
	case INPUT_KEY_R_SHIFT: return "R";
	case INPUT_KEY_S: return "s";
	case INPUT_KEY_S_SHIFT: return "S";
	case INPUT_KEY_T: return "t";
	case INPUT_KEY_T_SHIFT: return "T";

	case INPUT_KEY_U: return "u";
	case INPUT_KEY_U_SHIFT: return "U";
	case INPUT_KEY_V: return "v";
	case INPUT_KEY_V_SHIFT: return "V";
	case INPUT_KEY_W: return "w";
	case INPUT_KEY_W_SHIFT: return "W";
	case INPUT_KEY_X: return "x";
	case INPUT_KEY_X_SHIFT: return "X";
	case INPUT_KEY_Y: return "y";
	case INPUT_KEY_Y_SHIFT: return "Y";

	case INPUT_KEY_Z: return "z";
	case INPUT_KEY_Z_SHIFT: return "Z";

	default: return "";
	}
	return "";
}

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

inline int createKeySig(int modBit, int glfwKey)
{
	if (modBit <= INPUT_KEY_UNKNOWN && (glfwKey <= 0)) { return INPUT_KEY_UNKNOWN; }
	int mod = clampi(modBit, 0, 8);
	return (glfwKey * 10) + mod;
}

inline TextInputCmd keySig_toInputCmd(int sig)
{
	switch (sig)
	{
	case INPUT_KEY_ESCAPE: return TextInputCmd::escapeInput;
	case INPUT_KEY_ENTER: return TextInputCmd::commitInput;
	case INPUT_KEY_BACKSPACE: return TextInputCmd::backspaceChar;
	case INPUT_KEY_LEFT: return TextInputCmd::moveCursorLeft;
	case INPUT_KEY_LEFT_SHIFT: return TextInputCmd::selectMoveLeft;
	case INPUT_KEY_LEFT_CTRL: return TextInputCmd::moveWordLeft;
	case INPUT_KEY_LEFT_CTRLSHIFT: return TextInputCmd::selectWordLeft;
	case INPUT_KEY_RIGHT: return TextInputCmd::moveCursorRight;
	case INPUT_KEY_RIGHT_SHIFT: return TextInputCmd::selectMoveRight;
	case INPUT_KEY_RIGHT_CTRL: return TextInputCmd::moveWordRight;
	case INPUT_KEY_RIGHT_CTRLSHIFT: return TextInputCmd::selectWordRight;
	case INPUT_KEY_UP: return TextInputCmd::moveCursorUp;
	case INPUT_KEY_DOWN: return TextInputCmd::moveCursorDown;
	case INPUT_KEY_A_CTRL: return TextInputCmd::selectAll;
	case INPUT_KEY_C_CTRL: return TextInputCmd::copyText;
	case INPUT_KEY_V_CTRL: return TextInputCmd::pasteText;
	case INPUT_KEY_X_CTRL: return TextInputCmd::cutText;
	}
	return TextInputCmd::nullCmd;
}

inline std::string keySigToString(int modBit, int glfwKey, int keySig)
{
	if ((glfwKey * 10) + modBit != keySig) { return "BAD KEY GET!"; }
	std::string modString = "";
	switch (modBit)
	{
	case INPUT_MOD_NONE: modString = ""; break;
	case INPUT_MOD_SHIFT: modString = "SHIFT"; break;
	case INPUT_MOD_CTRL: modString = "CTRL"; break;
	case INPUT_MOD_ALT: modString = "ALT"; break;
	case INPUT_MOD_SHIFTCTRL: modString = "SHIFT+CTRL"; break;
	case INPUT_MOD_SHIFTALT: modString = "SHIFT+ALT"; break;
	case INPUT_MOD_CTRLALT: modString = "CTRL+ALT"; break;
	case INPUT_MOD_SHIFTCTRLALT: modString = "SHIFT+CTRL+ALT"; break;
	}
	std::string keyString = "";
	switch (glfwKey * 10)
	{
	case INPUT_KEY_SPACE: keyString = "SPACE"; break;
	case INPUT_KEY_APOSTROPHE: keyString = "'"; break; /* ' */
	case INPUT_KEY_COMMA: keyString = ","; break; /* , */
	case INPUT_KEY_MINUS: keyString = "-"; break; /* - */
	case INPUT_KEY_PERIOD: keyString = "."; break; /* . */
	case INPUT_KEY_SLASH: keyString = "/"; break; /* / */
	case INPUT_KEY_0: keyString = "0"; break;
	case INPUT_KEY_1: keyString = "1"; break;
	case INPUT_KEY_2: keyString = "2"; break;
	case INPUT_KEY_3: keyString = "3"; break;
	case INPUT_KEY_4: keyString = "4"; break;
	case INPUT_KEY_5: keyString = "5"; break;
	case INPUT_KEY_6: keyString = "6"; break;
	case INPUT_KEY_7: keyString = "7"; break;
	case INPUT_KEY_8: keyString = "8"; break;
	case INPUT_KEY_9: keyString = "9"; break;
	case INPUT_KEY_SEMICOLON: keyString = ";"; break; /* ; */
	case INPUT_KEY_EQUAL: keyString = "="; break; /* = */
	case INPUT_KEY_A: keyString = "A"; break;
	case INPUT_KEY_B: keyString = "B"; break;
	case INPUT_KEY_C: keyString = "C"; break;
	case INPUT_KEY_D: keyString = "D"; break;
	case INPUT_KEY_E: keyString = "E"; break;
	case INPUT_KEY_F: keyString = "F"; break;
	case INPUT_KEY_G: keyString = "G"; break;
	case INPUT_KEY_H: keyString = "H"; break;
	case INPUT_KEY_I: keyString = "I"; break;
	case INPUT_KEY_J: keyString = "J"; break;
	case INPUT_KEY_K: keyString = "K"; break;
	case INPUT_KEY_L: keyString = "L"; break;
	case INPUT_KEY_M: keyString = "M"; break;
	case INPUT_KEY_N: keyString = "N"; break;
	case INPUT_KEY_O: keyString = "O"; break;
	case INPUT_KEY_P: keyString = "P"; break;
	case INPUT_KEY_Q: keyString = "Q"; break;
	case INPUT_KEY_R: keyString = "R"; break;
	case INPUT_KEY_S: keyString = "S"; break;
	case INPUT_KEY_T: keyString = "T"; break;
	case INPUT_KEY_U: keyString = "U"; break;
	case INPUT_KEY_V: keyString = "V"; break;
	case INPUT_KEY_W: keyString = "W"; break;
	case INPUT_KEY_X: keyString = "X"; break;
	case INPUT_KEY_Y: keyString = "Y"; break;
	case INPUT_KEY_Z: keyString = "Z"; break;
	case INPUT_KEY_LEFT_BRACKET: keyString = "["; break; /* [ */
	case INPUT_KEY_BACKSLASH: keyString = "\\"; break; /* \ */
	case INPUT_KEY_RIGHT_BRACKET: keyString = "]"; break; /* ] */
	case INPUT_KEY_GRAVE_ACCENT: keyString = "`"; break; /* ` */
	case INPUT_KEY_INSERT: keyString = "INS"; break;
	case INPUT_KEY_DELETE: keyString = "DEL"; break;
	case INPUT_KEY_RIGHT: keyString = "RIGHT"; break;
	case INPUT_KEY_LEFT: keyString = "LEFT"; break;
	case INPUT_KEY_DOWN: keyString = "DOWN"; break;
	case INPUT_KEY_UP: keyString = "UP"; break;
	case INPUT_KEY_PAGE_UP: keyString = "PAGE-UP"; break;
	case INPUT_KEY_PAGE_DOWN: keyString = "PAGE-DOWN"; break;
	case INPUT_KEY_HOME: keyString = "HOME"; break;
	case INPUT_KEY_END: keyString = "END"; break;
	case INPUT_KEY_PAUSE: keyString = "PAUSE"; break;
	case INPUT_KEY_F1: keyString = "F1"; break;
	case INPUT_KEY_F2: keyString = "F2"; break;
	case INPUT_KEY_F3: keyString = "F3"; break;
	case INPUT_KEY_F4: keyString = "F4"; break;
	case INPUT_KEY_F5: keyString = "F5"; break;
	case INPUT_KEY_F6: keyString = "F6"; break;
	case INPUT_KEY_F7: keyString = "F7"; break;
	case INPUT_KEY_F8: keyString = "F8"; break;
	case INPUT_KEY_F9: keyString = "F9"; break;
	case INPUT_KEY_F10: keyString = "F10"; break;
	case INPUT_KEY_F11: keyString = "F11"; break;
	case INPUT_KEY_F12: keyString = "F12"; break;
	case INPUT_KEY_F13: keyString = "F13"; break;
	case INPUT_KEY_F14: keyString = "F14"; break;
	case INPUT_KEY_F15: keyString = "F15"; break;
	case INPUT_KEY_F16: keyString = "F16"; break;
	case INPUT_KEY_F17: keyString = "F17"; break;
	case INPUT_KEY_F18: keyString = "F18"; break;
	case INPUT_KEY_F19: keyString = "F19"; break;
	case INPUT_KEY_F20: keyString = "F20"; break;
	case INPUT_KEY_F21: keyString = "F21"; break;
	case INPUT_KEY_F22: keyString = "F22"; break;
	case INPUT_KEY_F23: keyString = "F23"; break;
	case INPUT_KEY_F24: keyString = "F24"; break;
	case INPUT_KEY_F25: keyString = "F25"; break;
	case INPUT_KEY_MENU: keyString = "F26"; break;
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
	return "";
}

inline std::string keySigToString(int sig)
{
	if (sig <= 0)
	{
		return "NONE";
	}
	else if (sig <= 8)
	{
		int modBit = sig;
		int glfwKey = 0;
		return keySigToString(modBit, glfwKey, sig);
	}
	else
	{
		int modBit = sig % 10;
		int glfwKey = int((sig - modBit) / 10);
		return keySigToString(modBit, glfwKey, sig);
	}
}
// Check Key Signatures
inline bool isKeySig_escape(int sig)
{
	if (sig >= 2560 && sig <= 2567) { return true; }
	return false;
}
inline bool isKeySig_delete(int sig)
{
	if (sig >= 2610 && sig <= 2617) { return true; }
	return false;
}
inline bool isKeySig_space(int sig)
{
	if (sig >= 320 && sig <= 327) { return true; }
	return false;
}
inline bool isKeySig_arrows(int sig)
{
	// RIGHT / LEFT / DOWN / UP ARROWS
	if (sig >= 2620 && sig <= 2657) { return true; }
	return false;
}
inline bool isKeySig_f10f11f12(int sig)
{
	// F10 / F11 / F12
	if (sig >= 2990 && sig <= 3017) { return true; }
	return false;
}
inline bool isKeySig_enterTabBackInsert(int sig)
{
	// ENTER / TAB / BACKSPACE / INSERT
	if (sig >= 2570 && sig <= 2607) { return true; }
	return false;
}
inline bool isKeySig_pagination(int sig)
{
	// PAGE_UP / PAGE_DOWN / HOME / END / CAPS_LOCK / SCROLL_LOCK / NUM_LOCK / PRINT_SCREEN / PAUSE
	if (sig >= 2660 && sig <= 2847) { return true; }
	return false;
}
inline bool isKeySig_modKey(int sig)
{
	// RIGHT & LEFT SHIFT / CTRL / ALT / SUPER / MENU
	if (sig >= 3400 && sig <= 3487) { return true; }
	return false;
}
inline bool isKeySig_outOfBounds(int sig)
{
	// OUT OF BOUNDS SIGNATURE
	if (sig >= 3487 || sig < 320) { return true; }
	return false;
}
inline bool isKeySig_alphaOnly(int sig)
{
	if (sig >= 650 && sig <= 907) { return true; }
	return false;
}
inline bool isKeySig_numericOnly(int sig)
{
	if (sig >= 480 && sig <= 577) { return true; }
	return false;
}
inline bool isKeySig_punctuation(int sig)
{
	if (sig >= 590 && sig <= 617) { return true; }
	if (sig >= 390 && sig <= 477) { return true; }
	if (sig >= 910 && sig <= 937) { return true; }
	return false;
}
#endif