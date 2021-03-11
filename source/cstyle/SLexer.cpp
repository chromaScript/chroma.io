#include "../include/cstyle/SLexer.h"
#include "../include/cstyle/ChromaStyle.h"
#include "../include/cstyle/SEnums.h"
#include "../include/cstyle/SToken.h"

#include <vector>
#include <string>
#include <memory>
#include <cctype>

//
SLexer::SLexer()
{

}

void SLexer::initialize(std::shared_ptr<ChromaStyle> console)
{
	this->console = console;
}

//
std::vector<std::shared_ptr<SToken>> SLexer::scanTokens(std::string source)
{
	//
	tokens.clear();
	//
	size_t startSearch = 0;
	size_t currentIndex = 0;
	size_t line = 1;
	size_t advance = 0;
	size_t find = 0;
	int count = 0;
	bool decimalFound = false;
	//
	while (currentIndex < source.size())
	{
		startSearch = currentIndex;
		char c = source[startSearch];
		count++;
		if (count > 999999)
		{
			std::cout << c << line << " " << advance << " " << currentIndex << " " << std::endl;
		}
		// Identifiers and Keywords
		if (islower(c))
		{
			switch (c)
			{
			case 'a': case 'b': case 'c': case 'd': case 'f':
			case 'h': case 'i': case 'l': case 'm': case 'n':
			case 'o': case 'p': case 'r': case 's': case 't': 
			case 'u': case 'v': case 'w': case 'z':
				switch (c)
				{
				case 'a':
					if (source[startSearch + 1] == 'b' &&
						source[startSearch + 2] == 's' &&
						source[startSearch + 3] == 'o' &&
						source[startSearch + 4] == 'l' &&
						source[startSearch + 5] == 'u' &&
						source[startSearch + 6] == 't' &&
						source[startSearch + 7] == 'e' &&
						(!isalpha(source[startSearch + 8])))
					{
						addToken(STokenType::_ABSOLUTE, line);
						currentIndex += 8;
						break;
					}
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'c' &&
						source[startSearch + 3] == 'h' &&
						source[startSearch + 4] == 'o' &&
						source[startSearch + 5] == 'r' &&
						(!isalpha(source[startSearch + 6])))
					{
						addToken(STokenType::ANCHOR, line);
						currentIndex += 6;
						break;
					}
					if (source[startSearch + 1] == 'u' &&
						source[startSearch + 2] == 't' &&
						source[startSearch + 3] == 'o' &&
						(!isalpha(source[startSearch + 4])))
					{
						addToken(STokenType::AUTO, line);
						currentIndex += 4;
						break;
					}
					else { goto identifier; }
					break;
				case 'b':
					if (source[startSearch + 1] == 'a' &&
						source[startSearch + 2] == 'c' &&
						source[startSearch + 3] == 'k' &&
						source[startSearch + 4] == 'g' &&
						source[startSearch + 5] == 'r' &&
						source[startSearch + 6] == 'o' &&
						source[startSearch + 7] == 'u' &&
						source[startSearch + 8] == 'n' &&
						source[startSearch + 9] == 'd' &&
						source[startSearch + 10] == '-' &&
						(isalpha(source[startSearch + 11])))
					{
						if (source[startSearch + 11] == 't' &&
							source[startSearch + 12] == 'e' &&
							source[startSearch + 13] == 'x' &&
							source[startSearch + 14] == 't' &&
							source[startSearch + 15] == 'u' &&
							source[startSearch + 16] == 'r' &&
							source[startSearch + 17] == 'e' &&
							(!isalpha(source[startSearch + 18])))
						{
							addToken(STokenType::BACKGROUND_TEXTURE, line);
							currentIndex += 18;
							break;
						}
						if (source[startSearch + 11] == 'c' &&
							source[startSearch + 12] == 'o' &&
							source[startSearch + 13] == 'l' &&
							source[startSearch + 14] == 'o' &&
							source[startSearch + 15] == 'r')
						{
							if (source[startSearch + 16] != '-')
							{
								addToken(STokenType::BACKGROUND_COLOR, line);
								currentIndex += 16;
								break;
							}
							else
							{
								if (source[startSearch + 16] == '-' &&
									source[startSearch + 17] == 'a' &&
									source[startSearch + 18] == 'l' &&
									source[startSearch + 19] == 'p' &&
									source[startSearch + 20] == 'h' &&
									source[startSearch + 21] == 'a' &&
									(!isalpha(source[startSearch + 22])))
								{
									addToken(STokenType::BACKGROUND_COLOR_ALPHA, line);
									currentIndex += 22;
									break;
								}
								if (source[startSearch + 16] == '-' &&
									source[startSearch + 17] == 'b' &&
									source[startSearch + 18] == 'l' &&
									source[startSearch + 19] == 'u' &&
									source[startSearch + 20] == 'e' &&
									(!isalpha(source[startSearch + 21])))
								{
									addToken(STokenType::BACKGROUND_COLOR_BLUE, line);
									currentIndex += 21;
									break;
								}
								if (source[startSearch + 16] == '-' &&
									source[startSearch + 17] == 'r' &&
									source[startSearch + 18] == 'e' &&
									source[startSearch + 19] == 'd' &&
									(!isalpha(source[startSearch + 20])))
								{
									addToken(STokenType::BACKGROUND_COLOR_RED, line);
									currentIndex += 20;
									break;
								}
								if (source[startSearch + 16] == '-' &&
									source[startSearch + 17] == 'g' &&
									source[startSearch + 18] == 'r' &&
									source[startSearch + 19] == 'e' &&
									source[startSearch + 20] == 'e' &&
									source[startSearch + 21] == 'n' &&
									(!isalpha(source[startSearch + 22])))
								{
									addToken(STokenType::BACKGROUND_COLOR_GREEN, line);
									currentIndex += 22;
									break;
								}
								else { goto identifier; }
							}
						}
					}
					if (source[startSearch + 1] == 'o' &&
						source[startSearch + 2] == 'r' &&
						source[startSearch + 3] == 'd' &&
						source[startSearch + 4] == 'e' &&
						source[startSearch + 5] == 'r')
					{
						if (source[startSearch + 6] != '-')
						{
							addToken(STokenType::BORDER, line);
							currentIndex += 6;
							break;
						}
						else
						{
							if (source[startSearch + 6] == '-' &&
								source[startSearch + 7] == 'b' &&
								source[startSearch + 8] == 'o' &&
								source[startSearch + 9] == 't' &&
								source[startSearch + 10] == 't' &&
								source[startSearch + 11] == 'o' &&
								source[startSearch + 12] == 'm' &&
								(!isalpha(source[startSearch + 13])))
							{
								addToken(STokenType::BORDER_BOTTOM, line);
								currentIndex += 13;
								break;
							}
							if (source[startSearch + 6] == '-' &&
								source[startSearch + 7] == 'l' &&
								source[startSearch + 8] == 'e' &&
								source[startSearch + 9] == 'f' &&
								source[startSearch + 10] == 't' &&
								(!isalpha(source[startSearch + 11])))
							{
								addToken(STokenType::BORDER_LEFT, line);
								currentIndex += 11;
								break;
							}
							if (source[startSearch + 6] == '-' &&
								source[startSearch + 7] == 'r' &&
								source[startSearch + 8] == 'i' &&
								source[startSearch + 9] == 'g' &&
								source[startSearch + 10] == 'h' &&
								source[startSearch + 11] == 't' &&
								(!isalpha(source[startSearch + 12])))
							{
								addToken(STokenType::BORDER_RIGHT, line);
								currentIndex += 12;
								break;
							}
							if (source[startSearch + 6] == '-' &&
								source[startSearch + 7] == 't' &&
								source[startSearch + 8] == 'o' &&
								source[startSearch + 9] == 'p' &&
								(!isalpha(source[startSearch + 10])))
							{
								addToken(STokenType::BORDER_TOP, line);
								currentIndex += 10;
								break;
							}
							else { goto identifier; }
						}
					}
					if (source[startSearch + 1] == 'o' &&
						source[startSearch + 2] == 't' &&
						source[startSearch + 3] == 't' &&
						source[startSearch + 4] == 'o' &&
						source[startSearch + 5] == 'm' &&
						(!isalpha(source[startSearch + 6])))
					{
						addToken(STokenType::BOTTOM, line);
						currentIndex += 6;
						break;
					}
					if (source[startSearch + 1] == 'o' &&
						source[startSearch + 2] == 'x' &&
						source[startSearch + 3] == '-' &&
						source[startSearch + 4] == 's' &&
						source[startSearch + 5] == 'h' &&
						source[startSearch + 6] == 'a' &&
						source[startSearch + 7] == 'd' &&
						source[startSearch + 8] == 'o' &&
						source[startSearch + 9] == 'w' &&
						source[startSearch + 10] == '-' &&
						(isalpha(source[startSearch + 11])))
					{
						if (source[startSearch + 11] == 's' &&
							source[startSearch + 12] == 'i' &&
							source[startSearch + 13] == 'z' &&
							source[startSearch + 14] == 'e')
						{
							if (source[startSearch + 15] != '-')
							{
								addToken(STokenType::BOX_SHADOW_SIZE, line);
								currentIndex += 15;
								break;
							}
							else
							{
								if (source[startSearch + 15] == '-' &&
									source[startSearch + 16] == 'X' &&
									(!isalpha(source[startSearch + 17])))
								{
									addToken(STokenType::BOX_SHADOW_SIZE_X, line);
									currentIndex += 17;
									break;
								}
								if (source[startSearch + 15] == '-' &&
									source[startSearch + 16] == 'Y' &&
									(!isalpha(source[startSearch + 17])))
								{
									addToken(STokenType::BOX_SHADOW_SIZE_Y, line);
									currentIndex += 17;
									break;
								}
								else { goto identifier; }
							}
						}
						if (source[startSearch + 11] == 'c' &&
							source[startSearch + 12] == 'o' &&
							source[startSearch + 13] == 'l' &&
							source[startSearch + 14] == 'o' &&
							source[startSearch + 15] == 'r')
						{
							if (source[startSearch + 16] != '-')
							{
								addToken(STokenType::BOX_SHADOW_COLOR, line);
								currentIndex += 16;
								break;
							}
							else
							{
								if (source[startSearch + 16] == '-' &&
									source[startSearch + 17] == 'a' &&
									source[startSearch + 18] == 'l' &&
									source[startSearch + 19] == 'p' &&
									source[startSearch + 20] == 'h' &&
									source[startSearch + 21] == 'a' &&
									(!isalpha(source[startSearch + 22])))
								{
									addToken(STokenType::BOX_SHADOW_COLOR_ALPHA, line);
									currentIndex += 22;
									break;
								}
								if (source[startSearch + 16] == '-' &&
									source[startSearch + 17] == 'b' &&
									source[startSearch + 18] == 'l' &&
									source[startSearch + 19] == 'u' &&
									source[startSearch + 20] == 'e' &&
									(!isalpha(source[startSearch + 21])))
								{
									addToken(STokenType::BOX_SHADOW_COLOR_BLUE, line);
									currentIndex += 21;
									break;
								}
								if (source[startSearch + 16] == '-' &&
									source[startSearch + 17] == 'r' &&
									source[startSearch + 18] == 'e' &&
									source[startSearch + 19] == 'd' &&
									(!isalpha(source[startSearch + 20])))
								{
									addToken(STokenType::BOX_SHADOW_COLOR_RED, line);
									currentIndex += 20;
									break;
								}
								if (source[startSearch + 16] == '-' &&
									source[startSearch + 17] == 'g' &&
									source[startSearch + 18] == 'r' &&
									source[startSearch + 19] == 'e' &&
									source[startSearch + 20] == 'e' &&
									source[startSearch + 21] == 'n' &&
									(!isalpha(source[startSearch + 22])))
								{
									addToken(STokenType::BOX_SHADOW_COLOR_GREEN, line);
									currentIndex += 22;
									break;
								}
								else { goto identifier; }
							}
						}
					}
					if (source[startSearch + 1] == 'o' &&
						source[startSearch + 2] == 'u' &&
						source[startSearch + 3] == 'n' &&
						source[startSearch + 4] == 'd' &&
						source[startSearch + 5] == 's' &&
						(!isalpha(source[startSearch + 6])))
					{
						addToken(STokenType::BOUNDS, line);
						currentIndex += 6;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'c':
					if (source[startSearch + 1] == 'e' &&
						source[startSearch + 2] == 'n' &&
						source[startSearch + 3] == 't' &&
						source[startSearch + 4] == 'e' &&
						source[startSearch + 5] == 'r')
					{
						if (source[startSearch + 6] != '-')
						{
							addToken(STokenType::CENTER, line);
							currentIndex += 6;
							break;
						}
						else
						{
							if (source[startSearch + 6] == '-' &&
								source[startSearch + 7] == 'l' &&
								source[startSearch + 8] == 'e' &&
								source[startSearch + 9] == 'f' &&
								source[startSearch + 10] == 't' &&
								(!isalpha(source[startSearch + 11])))
							{
								addToken(STokenType::CENTER_LEFT, line);
								currentIndex += 11;
								break;
							}
							if (source[startSearch + 6] == '-' &&
								source[startSearch + 7] == 'r' &&
								source[startSearch + 8] == 'i' &&
								source[startSearch + 9] == 'g' &&
								source[startSearch + 10] == 'h' &&
								source[startSearch + 11] == 't' &&
								(!isalpha(source[startSearch + 12])))
							{
								addToken(STokenType::CENTER_RIGHT, line);
								currentIndex += 12;
								break;
							}
							else { goto identifier; }
						}
					}
					if (source[startSearch + 1] == 'l' &&
						source[startSearch + 2] == 'i' &&
						source[startSearch + 3] == 'p' &&
						source[startSearch + 4] == 'p' &&
						source[startSearch + 5] == 'i' &&
						source[startSearch + 6] == 'n' &&
						source[startSearch + 7] == 'g' &&
						(!isalpha(source[startSearch + 8])))
					{
						addToken(STokenType::CLIPPING, line);
						currentIndex += 8;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'd':
					if (source[startSearch + 1] == 'i' &&
						source[startSearch + 2] == 'm' &&
						source[startSearch + 3] == 'e' &&
						source[startSearch + 4] == 'n' &&
						source[startSearch + 5] == 's' &&
						source[startSearch + 6] == 'i' &&
						source[startSearch + 7] == 'o' &&
						source[startSearch + 8] == 'n' &&
						source[startSearch + 9] == 's' &&
						(!isalpha(source[startSearch + 10])))
					{
						addToken(STokenType::DIMENSIONS, line);
						currentIndex += 10;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'f':
					if (source[startSearch + 1] == 'a' &&
						source[startSearch + 2] == 'l' &&
						source[startSearch + 3] == 's' &&
						source[startSearch + 4] == 'e' &&
						(!isalpha(source[startSearch + 5])))
					{
						addToken(STokenType::BOOL, "false", line);
						currentIndex += 5;
						break;
					}
					if (source[startSearch + 1] == 'i' &&
						source[startSearch + 2] == 'l' &&
						source[startSearch + 3] == 'l' &&
						(!isalpha(source[startSearch + 4])) && source[startSearch + 4] != '-')
					{
						addToken(STokenType::FILL, line);
						currentIndex += 4;
						break;
					}
					if (source[startSearch + 1] == 'i' &&
						source[startSearch + 2] == 'l' &&
						source[startSearch + 3] == 'l' &&
						source[startSearch + 4] == '-' &&
						(isalpha(source[startSearch + 5])))
					{
						if (source[startSearch + 5] == 'c' &&
							source[startSearch + 6] == 'o' &&
							source[startSearch + 7] == 'l' &&
							source[startSearch + 8] == 'o' &&
							source[startSearch + 9] == 'r')
						{
							if (source[startSearch + 10] != '-')
							{
								addToken(STokenType::FILL_COLOR, line);
								currentIndex += 10;
								break;
							}
							else
							{
								if (source[startSearch + 10] == '-' &&
									source[startSearch + 11] == 'a' &&
									source[startSearch + 12] == 'l' &&
									source[startSearch + 13] == 'p' &&
									source[startSearch + 14] == 'h' &&
									source[startSearch + 15] == 'a' &&
									(!isalpha(source[startSearch + 16])))
								{
									addToken(STokenType::FILL_COLOR_ALPHA, line);
									currentIndex += 16;
									break;
								}
								if (source[startSearch + 10] == '-' &&
									source[startSearch + 11] == 'b' &&
									source[startSearch + 12] == 'l' &&
									source[startSearch + 13] == 'u' &&
									source[startSearch + 14] == 'e' &&
									(!isalpha(source[startSearch + 15])))
								{
									addToken(STokenType::FILL_COLOR_BLUE, line);
									currentIndex += 15;
									break;
								}
								if (source[startSearch + 10] == '-' &&
									source[startSearch + 11] == 'r' &&
									source[startSearch + 12] == 'e' &&
									source[startSearch + 13] == 'd' &&
									(!isalpha(source[startSearch + 14])))
								{
									addToken(STokenType::FILL_COLOR_RED, line);
									currentIndex += 14;
									break;
								}
								if (source[startSearch + 10] == '-' &&
									source[startSearch + 11] == 'g' &&
									source[startSearch + 12] == 'r' &&
									source[startSearch + 13] == 'e' &&
									source[startSearch + 14] == 'e' &&
									source[startSearch + 15] == 'n' &&
									(!isalpha(source[startSearch + 16])))
								{
									addToken(STokenType::FILL_COLOR_GREEN, line);
									currentIndex += 16;
									break;
								}
								else { goto identifier; }
							}
						}
					}
					if (source[startSearch + 1] == 'i' &&
						source[startSearch + 2] == 'x' &&
						source[startSearch + 3] == 'e' &&
						source[startSearch + 4] == 'd' &&
						(!isalpha(source[startSearch + 5])))
					{
						addToken(STokenType::FIXED, line);
						currentIndex += 5;
						break;
					}
					if (source[startSearch + 1] == 'l' &&
						source[startSearch + 2] == 'o' &&
						source[startSearch + 3] == 'a' &&
						source[startSearch + 4] == 't' &&
						(!isalpha(source[startSearch + 5])))
					{
						addToken(STokenType::FLOAT, line);
						currentIndex += 5;
						break;
					}
					if (source[startSearch + 1] == 'o' &&
						source[startSearch + 2] == 'c' &&
						source[startSearch + 3] == 'u' &&
						source[startSearch + 4] == 's' &&
						source[startSearch + 5] == '-' &&
						(isalpha(source[startSearch + 6])))
					{
						if (source[startSearch + 6] == 'c' &&
							source[startSearch + 7] == 'o' &&
							source[startSearch + 8] == 'l' &&
							source[startSearch + 9] == 'o' &&
							source[startSearch + 10] == 'r')
						{
							if (source[startSearch + 11] != '-')
							{
								addToken(STokenType::FOCUS_COLOR, line);
								currentIndex += 11;
								break;
							}
							else
							{
								if (source[startSearch + 11] == '-' &&
									source[startSearch + 12] == 'a' &&
									source[startSearch + 13] == 'l' &&
									source[startSearch + 14] == 'p' &&
									source[startSearch + 15] == 'h' &&
									source[startSearch + 16] == 'a' &&
									(!isalpha(source[startSearch + 16])))
								{
									addToken(STokenType::FOCUS_COLOR_ALPHA, line);
									currentIndex += 16;
									break;
								}
								if (source[startSearch + 11] == '-' &&
									source[startSearch + 12] == 'b' &&
									source[startSearch + 13] == 'l' &&
									source[startSearch + 14] == 'u' &&
									source[startSearch + 15] == 'e' &&
									(!isalpha(source[startSearch + 16])))
								{
									addToken(STokenType::FOCUS_COLOR_BLUE, line);
									currentIndex += 16;
									break;
								}
								if (source[startSearch + 11] == '-' &&
									source[startSearch + 12] == 'r' &&
									source[startSearch + 13] == 'e' &&
									source[startSearch + 14] == 'd' &&
									(!isalpha(source[startSearch + 15])))
								{
									addToken(STokenType::FOCUS_COLOR_RED, line);
									currentIndex += 15;
									break;
								}
								if (source[startSearch + 11] == '-' &&
									source[startSearch + 12] == 'g' &&
									source[startSearch + 13] == 'r' &&
									source[startSearch + 14] == 'e' &&
									source[startSearch + 15] == 'e' &&
									source[startSearch + 16] == 'n' &&
									(!isalpha(source[startSearch + 17])))
								{
									addToken(STokenType::FOCUS_COLOR_GREEN, line);
									currentIndex += 17;
									break;
								}
								else { goto identifier; }
							}
						}
					}
					if (source[startSearch + 1] == 'o' &&
						source[startSearch + 2] == 'n' &&
						source[startSearch + 3] == 't' &&
						source[startSearch + 4] == '-')
					{
							if (source[startSearch + 5] == 'p' &&
								source[startSearch + 6] == 'a' &&
								source[startSearch + 7] == 't' &&
								source[startSearch + 8] == 'h' &&
								(!isalpha(source[startSearch + 9])))
							{
								addToken(STokenType::FONT_PATH, line);
								currentIndex += 9;
								break;
							}
							if (source[startSearch + 5] == 's' &&
								source[startSearch + 6] == 'i' &&
								source[startSearch + 7] == 'z' &&
								source[startSearch + 8] == 'e' &&
								(!isalpha(source[startSearch + 9])))
							{
								addToken(STokenType::FONT_SIZE, line);
								currentIndex += 9;
								break;
							}
							else { goto identifier; }
					}
					else { goto identifier; }
					break;
					//
				case 'h':
					if (source[startSearch + 1] == 'e' &&
						source[startSearch + 2] == 'i' &&
						source[startSearch + 3] == 'g' &&
						source[startSearch + 4] == 'h' &&
						source[startSearch + 5] == 't' &&
						(!isalpha(source[startSearch + 6])))
					{
						addToken(STokenType::HEIGHT, line);
						currentIndex += 6;
						break;
					}
					if (source[startSearch + 1] == 'i' &&
						source[startSearch + 2] == 'd' &&
						source[startSearch + 3] == 'd' &&
						source[startSearch + 4] == 'e' &&
						source[startSearch + 5] == 'n' &&
						(!isalpha(source[startSearch + 6])))
					{
						addToken(STokenType::HIDDEN, line);
						currentIndex += 6;
						break;
					}
					if (source[startSearch + 1] == 'i' &&
						source[startSearch + 2] == 'd' &&
						source[startSearch + 3] == 'e' &&
						(!isalpha(source[startSearch + 4])))
					{
						addToken(STokenType::HIDE, line);
						currentIndex += 4;
						break;
					}
					if (source[startSearch + 1] == 'i' &&
						source[startSearch + 2] == 'g' &&
						source[startSearch + 3] == 'h' &&
						source[startSearch + 4] == 'l' &&
						source[startSearch + 5] == 'i' &&
						source[startSearch + 6] == 'g' &&
						source[startSearch + 7] == 'h' &&
						source[startSearch + 8] == 't' &&
						source[startSearch + 9] == '-' &&
						(isalpha(source[startSearch + 10])))
					{
						if (source[startSearch + 10] == 'c' &&
							source[startSearch + 11] == 'o' &&
							source[startSearch + 12] == 'l' &&
							source[startSearch + 13] == 'o' &&
							source[startSearch + 14] == 'r')
						{
							if (source[startSearch + 15] != '-')
							{
								addToken(STokenType::HIGHLIGHT_COLOR, line);
								currentIndex += 15;
								break;
							}
							else
							{
								if (source[startSearch + 15] == '-' &&
									source[startSearch + 16] == 'a' &&
									source[startSearch + 17] == 'l' &&
									source[startSearch + 18] == 'p' &&
									source[startSearch + 19] == 'h' &&
									source[startSearch + 20] == 'a' &&
									(!isalpha(source[startSearch + 21])))
								{
									addToken(STokenType::HIGHLIGHT_COLOR_ALPHA, line);
									currentIndex += 21;
									break;
								}
								if (source[startSearch + 15] == '-' &&
									source[startSearch + 16] == 'b' &&
									source[startSearch + 17] == 'l' &&
									source[startSearch + 18] == 'u' &&
									source[startSearch + 19] == 'e' &&
									(!isalpha(source[startSearch + 20])))
								{
									addToken(STokenType::HIGHLIGHT_COLOR_BLUE, line);
									currentIndex += 20;
									break;
								}
								if (source[startSearch + 15] == '-' &&
									source[startSearch + 16] == 'r' &&
									source[startSearch + 17] == 'e' &&
									source[startSearch + 18] == 'd' &&
									(!isalpha(source[startSearch + 19])))
								{
									addToken(STokenType::HIGHLIGHT_COLOR_RED, line);
									currentIndex += 19;
									break;
								}
								if (source[startSearch + 15] == '-' &&
									source[startSearch + 16] == 'g' &&
									source[startSearch + 17] == 'r' &&
									source[startSearch + 18] == 'e' &&
									source[startSearch + 19] == 'e' &&
									source[startSearch + 20] == 'n' &&
									(!isalpha(source[startSearch + 21])))
								{
									addToken(STokenType::HIGHLIGHT_COLOR_GREEN, line);
									currentIndex += 21;
									break;
								}
								else { goto identifier; }
							}
						}
					}
					else { goto identifier; }
					break;
					//
				case 'i':
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'h' &&
						source[startSearch + 3] == 'e' &&
						source[startSearch + 4] == 'r' &&
						source[startSearch + 5] == 'i' &&
						source[startSearch + 6] == 't' &&
						(!isalpha(source[startSearch + 7])))
					{
						addToken(STokenType::INHERIT, line);
						currentIndex += 7;
						break;
					}
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'n' &&
						source[startSearch + 3] == 'e' &&
						source[startSearch + 4] == 'r' &&
						source[startSearch + 5] == '-' &&
						source[startSearch + 6] == 'r' &&
						source[startSearch + 7] == 'a' &&
						source[startSearch + 8] == 'd' &&
						source[startSearch + 9] == 'i' &&
						source[startSearch + 10] == 'u' &&
						source[startSearch + 11] == 's' &&
						(!isalpha(source[startSearch + 12])))
					{
						addToken(STokenType::INNER_RADIUS, line);
						currentIndex += 12;
						break;
					}
					else { goto identifier; }
					break;
				case 'l':
					if (source[startSearch + 1] == 'e' &&
						source[startSearch + 2] == 'f' &&
						source[startSearch + 3] == 't' &&
						(!isalpha(source[startSearch + 4])))
					{
						addToken(STokenType::LEFT, line);
						currentIndex += 4;
						break;
					}
					if (source[startSearch + 1] == 'o' &&
						source[startSearch + 2] == 'w' &&
						source[startSearch + 3] == 'e' &&
						source[startSearch + 4] == 'r' &&
						source[startSearch + 5] == '-')
					{
						if (source[startSearch + 6] == 'c' &&
							source[startSearch + 7] == 'e' &&
							source[startSearch + 8] == 'n' &&
							source[startSearch + 9] == 't' &&
							source[startSearch + 10] == 'e' &&
							source[startSearch + 11] == 'r' &&
							(!isalpha(source[startSearch + 12])))
						{
							addToken(STokenType::LOWER_CENTER, line);
							currentIndex += 12;
							break;
						}
						if (source[startSearch + 6] == 'l' &&
							source[startSearch + 7] == 'e' &&
							source[startSearch + 8] == 'f' &&
							source[startSearch + 9] == 't' &&
							(!isalpha(source[startSearch + 10])))
						{
							addToken(STokenType::LOWER_LEFT, line);
							currentIndex += 10;
							break;
						}
						if (source[startSearch + 6] == 'r' &&
							source[startSearch + 7] == 'i' &&
							source[startSearch + 8] == 'g' &&
							source[startSearch + 9] == 'h' &&
							source[startSearch + 10] == 't' &&
							(!isalpha(source[startSearch + 11])))
						{
							addToken(STokenType::LOWER_RIGHT, line);
							currentIndex += 11;
							break;
						}
						else { goto identifier; }
					}
					else { goto identifier; }
					break;
				case 'm':
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'h' &&
						source[startSearch + 3] == 'e' &&
						source[startSearch + 4] == 'r' &&
						source[startSearch + 5] == 'i' &&
						source[startSearch + 6] == 't' &&
						(!isalpha(source[startSearch + 7])))
					{
						addToken(STokenType::INHERIT, line);
						currentIndex += 7;
						break;
					}
					if (source[startSearch + 1] == 'a' &&
						source[startSearch + 2] == 'r' &&
						source[startSearch + 3] == 'g' &&
						source[startSearch + 4] == 'i' &&
						source[startSearch + 5] == 'n')
					{
						if (source[startSearch + 6] != '-')
						{
							addToken(STokenType::MARGIN, line);
							currentIndex += 6;
							break;
						}
						else
						{
							if (source[startSearch + 6] == '-' &&
								source[startSearch + 7] == 'b' &&
								source[startSearch + 8] == 'o' &&
								source[startSearch + 9] == 't' &&
								source[startSearch + 10] == 't' &&
								source[startSearch + 11] == 'o' &&
								source[startSearch + 12] == 'm' &&
								(!isalpha(source[startSearch + 13])))
							{
								addToken(STokenType::MARGIN_BOTTOM, line);
								currentIndex += 13;
								break;
							}
							if (source[startSearch + 6] == '-' &&
								source[startSearch + 7] == 'l' &&
								source[startSearch + 8] == 'e' &&
								source[startSearch + 9] == 'f' &&
								source[startSearch + 10] == 't' &&
								(!isalpha(source[startSearch + 11])))
							{
								addToken(STokenType::MARGIN_LEFT, line);
								currentIndex += 11;
								break;
							}
							if (source[startSearch + 6] == '-' &&
								source[startSearch + 7] == 'r' &&
								source[startSearch + 8] == 'i' &&
								source[startSearch + 9] == 'g' &&
								source[startSearch + 10] == 'h' &&
								source[startSearch + 11] == 't' &&
								(!isalpha(source[startSearch + 12])))
							{
								addToken(STokenType::MARGIN_RIGHT, line);
								currentIndex += 12;
								break;
							}
							if (source[startSearch + 6] == '-' &&
								source[startSearch + 7] == 't' &&
								source[startSearch + 8] == 'o' &&
								source[startSearch + 9] == 'p' &&
								(!isalpha(source[startSearch + 10])))
							{
								addToken(STokenType::MARGIN_TOP, line);
								currentIndex += 10;
								break;
							}
							else { goto identifier; }
						}
					}
					if (source[startSearch + 1] == 'a' &&
						source[startSearch + 2] == 'x')
					{
						if (source[startSearch + 3] != '-')
						{
							addToken(STokenType::MAX, line);
							currentIndex += 3;
							break;
						}
						else
						{
							if (source[startSearch + 3] == '-' &&
								source[startSearch + 4] == 'h' &&
								source[startSearch + 5] == 'e' &&
								source[startSearch + 6] == 'i' &&
								source[startSearch + 7] == 'g' &&
								source[startSearch + 8] == 'h' &&
								source[startSearch + 9] == 't' &&
								(!isalpha(source[startSearch + 10])))
							{
								addToken(STokenType::MAX_HEIGHT, line);
								currentIndex += 10;
								break;
							}
							if (source[startSearch + 3] == '-' &&
								source[startSearch + 4] == 'w' &&
								source[startSearch + 5] == 'i' &&
								source[startSearch + 6] == 'd' &&
								source[startSearch + 7] == 't' &&
								source[startSearch + 8] == 'h' &&
								(!isalpha(source[startSearch + 9])))
							{
								addToken(STokenType::MAX_WIDTH, line);
								currentIndex += 9;
								break;
							}
							else { goto identifier; }
						}
					}
					if (source[startSearch + 1] == 'i' &&
						source[startSearch + 2] == 'n')
					{
						if (source[startSearch + 3] != '-')
						{
							addToken(STokenType::MIN, line);
							currentIndex += 3;
							break;
						}
						else
						{
							if (source[startSearch + 3] == '-' &&
								source[startSearch + 4] == 'h' &&
								source[startSearch + 5] == 'e' &&
								source[startSearch + 6] == 'i' &&
								source[startSearch + 7] == 'g' &&
								source[startSearch + 8] == 'h' &&
								source[startSearch + 9] == 't' &&
								(!isalpha(source[startSearch + 10])))
							{
								addToken(STokenType::MIN_HEIGHT, line);
								currentIndex += 10;
								break;
							}
							if (source[startSearch + 3] == '-' &&
								source[startSearch + 4] == 'w' &&
								source[startSearch + 5] == 'i' &&
								source[startSearch + 6] == 'd' &&
								source[startSearch + 7] == 't' &&
								source[startSearch + 8] == 'h' &&
								(!isalpha(source[startSearch + 9])))
							{
								addToken(STokenType::MIN_WIDTH, line);
								currentIndex += 9;
								break;
							}
							else { goto identifier; }
						}
					}
					else { goto identifier; }
					break;
				case 'n':
					if (source[startSearch + 1] == 'i' &&
						source[startSearch + 2] == 'l' &&
						(!isalpha(source[startSearch + 3])))
					{
						addToken(STokenType::NIL, line);
						currentIndex += 3;
						break;
					}
					if (source[startSearch + 1] == 'o' &&
						source[startSearch + 2] == 'r' &&
						source[startSearch + 3] == 'e' &&
						source[startSearch + 4] == 'p' &&
						source[startSearch + 5] == 'o' &&
						source[startSearch + 6] == 'r' &&
						source[startSearch + 7] == 't' &&
						(!isalpha(source[startSearch + 8])))
					{
						addToken(STokenType::NOREPORT, line);
						currentIndex += 8;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'o':
					if (source[startSearch + 1] == 'v' &&
						source[startSearch + 2] == 'e' &&
						source[startSearch + 3] == 'r' &&
						source[startSearch + 4] == 'f' &&
						source[startSearch + 5] == 'l' &&
						source[startSearch + 6] == 'o' &&
						source[startSearch + 7] == 'w')
					{
						if (source[startSearch + 8] != '-')
						{
							addToken(STokenType::$OVERFLOW, line);
							currentIndex += 8;
							break;
						}
						else
						{
							if (source[startSearch + 8] == '-' &&
								source[startSearch + 9] == 'X' &&
								(!isalpha(source[startSearch + 10])))
							{
								addToken(STokenType::OVERFLOW_X, line);
								currentIndex += 10;
								break;
							}
							if (source[startSearch + 8] == '-' &&
								source[startSearch + 9] == 'Y' &&
								(!isalpha(source[startSearch + 10])))
							{
								addToken(STokenType::OVERFLOW_Y, line);
								currentIndex += 10;
								break;
							}
							else { goto identifier; }
						}
					}
					if (source[startSearch + 1] == 'u' &&
						source[startSearch + 2] == 't' &&
						source[startSearch + 3] == 'e' &&
						source[startSearch + 4] == 'r' &&
						source[startSearch + 5] == '-' &&
						source[startSearch + 6] == 'r' &&
						source[startSearch + 7] == 'a' &&
						source[startSearch + 8] == 'd' &&
						source[startSearch + 9] == 'i' &&
						source[startSearch + 10] == 'u' &&
						source[startSearch + 11] == 's' &&
						(!isalpha(source[startSearch + 12])))
					{
						addToken(STokenType::OUTER_RADIUS, line);
						currentIndex += 12;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'p':
					if (source[startSearch + 1] == 'o' &&
						source[startSearch + 2] == 's' &&
						source[startSearch + 3] == 'i' &&
						source[startSearch + 4] == 't' &&
						source[startSearch + 5] == 'i' &&
						source[startSearch + 6] == 'o' &&
						source[startSearch + 7] == 'n' &&
						(!isalpha(source[startSearch + 8])))
					{
						addToken(STokenType::POSITION, line);
						currentIndex += 8;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'r':
					if (source[startSearch + 1] == 'e' &&
						source[startSearch + 2] == 'l' &&
						source[startSearch + 3] == 'a' &&
						source[startSearch + 4] == 't' &&
						source[startSearch + 5] == 'i' &&
						source[startSearch + 6] == 'v' &&
						source[startSearch + 7] == 'e' &&
						(!isalpha(source[startSearch + 8])))
					{
						addToken(STokenType::_RELATIVE, line);
						currentIndex += 8;
						break;
					}
					if (source[startSearch + 1] == 'i' &&
						source[startSearch + 2] == 'g' &&
						source[startSearch + 3] == 'h' &&
						source[startSearch + 4] == 't' &&
						(!isalpha(source[startSearch + 5])))
					{
						addToken(STokenType::RIGHT, line);
						currentIndex += 5;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 's':
					if (source[startSearch + 1] == 'c' &&
						source[startSearch + 2] == 'r' &&
						source[startSearch + 3] == 'o' &&
						source[startSearch + 4] == 'l' &&
						source[startSearch + 5] == 'l' &&
						(!isalpha(source[startSearch + 6])))
					{
						addToken(STokenType::SCROLL, line);
						currentIndex += 6;
						break;
					}
					if (source[startSearch + 1] == 'c' &&
						source[startSearch + 2] == 'r' &&
						source[startSearch + 3] == 'i' &&
						source[startSearch + 4] == 'p' &&
						source[startSearch + 5] == 't' &&
						(!isalpha(source[startSearch + 6])))
					{
						addToken(STokenType::SCRIPT_TAG, line);
						currentIndex += 6;
						break;
					}
					if (source[startSearch + 1] == 'h' &&
						source[startSearch + 2] == 'o' &&
						source[startSearch + 3] == 'w' &&
						(!isalpha(source[startSearch + 4])))
					{
						addToken(STokenType::SHOW, line);
						currentIndex += 4;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 't':
					if (source[startSearch + 1] == 'o' &&
						source[startSearch + 2] == 'p' &&
						(!isalpha(source[startSearch + 3])))
					{
						addToken(STokenType::TOP, line);
						currentIndex += 3;
						break;
					}
					if (source[startSearch + 1] == 'r' &&
						source[startSearch + 2] == 'u' &&
						source[startSearch + 3] == 'e' &&
						(!isalpha(source[startSearch + 4])))
					{
						addToken(STokenType::BOOL, "true", line);
						currentIndex += 4;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'u':
					if (source[startSearch + 1] == 'p' &&
						source[startSearch + 2] == 'p' &&
						source[startSearch + 3] == 'e' &&
						source[startSearch + 4] == 'r' &&
						source[startSearch + 5] == '-')
					{
						if (source[startSearch + 6] == 'c' &&
							source[startSearch + 7] == 'e' &&
							source[startSearch + 8] == 'n' &&
							source[startSearch + 9] == 't' &&
							source[startSearch + 10] == 'e' &&
							source[startSearch + 11] == 'r' &&
							(!isalpha(source[startSearch + 12])))
						{
							addToken(STokenType::UPPER_CENTER, line);
							currentIndex += 12;
							break;
						}
						if (source[startSearch + 6] == 'l' &&
							source[startSearch + 7] == 'e' &&
							source[startSearch + 8] == 'f' &&
							source[startSearch + 9] == 't' &&
							(!isalpha(source[startSearch + 10])))
						{
							addToken(STokenType::UPPER_LEFT, line);
							currentIndex += 10;
							break;
						}
						if (source[startSearch + 6] == 'r' &&
							source[startSearch + 7] == 'i' &&
							source[startSearch + 8] == 'g' &&
							source[startSearch + 9] == 'h' &&
							source[startSearch + 10] == 't' &&
							(!isalpha(source[startSearch + 11])))
						{
							addToken(STokenType::UPPER_RIGHT, line);
							currentIndex += 11;
							break;
						}
						else { goto identifier; }
					}
					else { goto identifier; }
					break;
					//
				case 'v':
					if (source[startSearch + 1] == 'e' &&
						source[startSearch + 2] == 'r' &&
						source[startSearch + 3] == 't' &&
						source[startSearch + 4] == 'e' &&
						source[startSearch + 5] == 'x' &&
						source[startSearch + 6] == '-' &&
						(isdigit(source[startSearch + 7])))
					{
						size_t i = 0;
						std::string str = "";
						while (isdigit(source[startSearch + 7 + i]))
						{
							str.push_back(source[startSearch + 7 + i]);
							i++;
						}
						unsigned char special = (unsigned char)std::stoi(str);
						if (source[startSearch + 7 + i] == '-' &&
							source[startSearch + 8 + i] == 'c' &&
							source[startSearch + 9 + i] == 'o' &&
							source[startSearch + 10 + i] == 'l' &&
							source[startSearch + 11 + i] == 'o' &&
							source[startSearch + 12 + i] == 'r')
						{
							if (source[startSearch + 13 + i] != '-')
							{
								addToken(STokenType::VERTEX_COLOR, "vertex-" + str + "-color", line, special);
								currentIndex += 13 + i;
								break;
							}
							else
							{
								if (source[startSearch + 13 + i] == '-' &&
									source[startSearch + 14 + i] == 'a' &&
									source[startSearch + 15 + i] == 'l' &&
									source[startSearch + 16 + i] == 'p' &&
									source[startSearch + 17 + i] == 'h' &&
									source[startSearch + 18 + i] == 'a' &&
									(!isalpha(source[startSearch + 19 + i])))
								{
									addToken(STokenType::VERTEX_COLOR_ALPHA, "vertex-" + str + "-color-alpha", line, special);
									currentIndex += 19 + i;
									break;
								}
								if (source[startSearch + 13 + i] == '-' &&
									source[startSearch + 14 + i] == 'b' &&
									source[startSearch + 15 + i] == 'l' &&
									source[startSearch + 16 + i] == 'u' &&
									source[startSearch + 17 + i] == 'e' &&
									(!isalpha(source[startSearch + 18 + i])))
								{
									addToken(STokenType::VERTEX_COLOR_BLUE, "vertex-" + str + "-color-red", line, special);
									currentIndex += 18 + i;
									break;
								}
								if (source[startSearch + 13 + i] == '-' &&
									source[startSearch + 14 + i] == 'r' &&
									source[startSearch + 15 + i] == 'e' &&
									source[startSearch + 16 + i] == 'd' &&
									(!isalpha(source[startSearch + 17 + i])))
								{
									addToken(STokenType::VERTEX_COLOR_RED, "vertex-" + str + "-color-blue", line, special);
									currentIndex += 17 + i;
									break;
								}
								if (source[startSearch + 13 + i] == '-' &&
									source[startSearch + 14 + i] == 'g' &&
									source[startSearch + 15 + i] == 'r' &&
									source[startSearch + 16 + i] == 'e' &&
									source[startSearch + 17 + i] == 'e' &&
									source[startSearch + 18 + i] == 'n' &&
									(!isalpha(source[startSearch + 19 + i])))
								{
									addToken(STokenType::VERTEX_COLOR_GREEN, "vertex-" + str + "-color-green", line, special);
									currentIndex += 19 + i;
									break;
								}
								else { goto identifier; }
							}
						}
					}
					if (source[startSearch + 1] == 'i' &&
						source[startSearch + 2] == 's' &&
						source[startSearch + 3] == 'i' &&
						source[startSearch + 4] == 'b' &&
						source[startSearch + 5] == 'i' &&
						source[startSearch + 6] == 'l' &&
						source[startSearch + 7] == 'i' &&
						source[startSearch + 8] == 't' &&
						source[startSearch + 9] == 'y' &&
						(!isalpha(source[startSearch + 10])))
					{
						addToken(STokenType::VISIBILITY, line);
						currentIndex += 10;
						break;
					}
					if (source[startSearch + 1] == 'i' &&
						source[startSearch + 2] == 's' &&
						source[startSearch + 3] == 'i' &&
						source[startSearch + 4] == 'b' &&
						source[startSearch + 5] == 'l' &&
						source[startSearch + 6] == 'e' &&
						(!isalpha(source[startSearch + 7])))
					{
						addToken(STokenType::VISIBILE, line);
						currentIndex += 7;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'w':
					if (source[startSearch + 1] == 'i' &&
						source[startSearch + 2] == 'd' &&
						source[startSearch + 3] == 't' &&
						source[startSearch + 4] == 'h' &&
						(!isalpha(source[startSearch + 5])))
					{
						addToken(STokenType::WIDTH, line);
						currentIndex += 5;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'z':
					if (source[startSearch + 1] == '-' &&
						source[startSearch + 2] == 'i' &&
						source[startSearch + 3] == 'n' &&
						source[startSearch + 4] == 'd' &&
						source[startSearch + 5] == 'e' &&
						source[startSearch + 6] == 'x' &&
						(!isalpha(source[startSearch + 7])))
					{
						addToken(STokenType::ZINDEX, line);
						currentIndex += 7;
						break;
					}
					else { goto identifier; }
					break;
					//
				default:
				identifier:
					advance = 0;
					while (startSearch + advance < source.size() &&
						(isalnum(source[startSearch + advance]) ||
							source[startSearch + advance] == '_' ||
							source[startSearch + advance] == '-'))
					{
						advance++;
					}
					addToken(STokenType::IDENTIFIER, source.substr(startSearch, advance), line);
					currentIndex += advance;
					break;
				}
				break;
			default:
				advance = 0;
				while (startSearch + advance < source.size() &&
					(isalnum(source[startSearch + advance]) ||
						source[startSearch + advance] == '_' ||
						source[startSearch + advance] == '-'))
				{
					advance++;
				}
				addToken(STokenType::IDENTIFIER, source.substr(startSearch, advance), line);
				currentIndex += advance;
				break;
			}
			continue;
		}
		else if (isupper(c) || (startSearch + advance <= source.size() && source[startSearch + advance] == '_'))
		{
			advance = 0;
			while (startSearch + advance < source.size() && 
				(isalnum(source[startSearch + advance]) || 
					source[startSearch + advance] == '_' ||
					source[startSearch + advance] == '-'))
			{
				advance++;
			}
			addToken(STokenType::IDENTIFIER, source.substr(startSearch, advance), line);
			currentIndex += advance;
			continue;
		}
		// Whitespace, Strings, Numbers, Conditionals, Logic, Comments, and Operators
		switch (c)
		{
		case ' ': case '\r': case '\t': currentIndex++; break;
			//
		case '\n': line++; currentIndex++; break;
			//
		case '-': case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
			advance = 0;
			if (c == '-') { advance++; }
			if (c == '-' && startSearch + 1 < source.size() && !(isdigit(source[startSearch + 1]) || source[startSearch + 1] == '.'))
			{
				console.get()->error(line, "[lexer:0101] Unexpected '-' character."); currentIndex++; break;
			}

			decimalFound = false;
			while (startSearch + advance < source.size() && (isdigit(source[startSearch + advance]) || source[startSearch + advance] == '.'))
			{
				if (source[startSearch + advance] == '.')
				{
					if (decimalFound == false) { advance++; decimalFound = true; }
					else { break; }
				}
				else { advance++; }
			}
			if (source[startSearch + advance] == '%') { advance++; }
			addToken(STokenType::NUM, source.substr(startSearch, advance), line);
			currentIndex += advance;
			break;
			//
		case '(': addToken(STokenType::PAREN_LEFT, c, line); currentIndex++; break;
		case ')': addToken(STokenType::PAREN_RIGHT, c, line); currentIndex++; break;
		case '{': addToken(STokenType::CURLY_LEFT, c, line); currentIndex++; break;
		case '}': addToken(STokenType::CURLY_RIGHT, c, line); currentIndex++; break;
		case '[': addToken(STokenType::SQUARE_LEFT, c, line); currentIndex++; break;
		case ']': addToken(STokenType::SQUARE_RIGHT, c, line); currentIndex++; break;
		case '<': addToken(STokenType::ANGLE_LEFT, c, line); currentIndex++; break;
		case '>': addToken(STokenType::ANGLE_RIGHT, c, line); currentIndex++; break;
		case '.': addToken(STokenType::DOT, c, line); currentIndex++; break;
		case '#': addToken(STokenType::POUND, c, line); currentIndex++; break;
		case '*': addToken(STokenType::WILDCARD, c, line); currentIndex++; break;
		case ',': addToken(STokenType::COMMA, c, line); currentIndex++; break;
		case ':':
			if (source[startSearch + 1] == ':') { addToken(STokenType::SCOPE, line); currentIndex += 2; break; }
			else { addToken(STokenType::COLON, line); currentIndex++; break; }
		case ';': addToken(STokenType::SEMICOLON, c, line); currentIndex++; break;
		case '&': addToken(STokenType::AND, c, line); currentIndex++; break;
			//
		case '/':
			if (source[startSearch + 1] == '/')
			{
				size_t find = source.find("\n", startSearch + 2);
				if (find == std::string::npos) { currentIndex = source.size(); break; }
				else { currentIndex = find + 1; line++; break; }
				break;
			}
			else if (source[startSearch + 1] == '*')
			{
				size_t find = source.find("*/", startSearch + 2);
				if (find == std::string::npos) { console.get()->error(line, "[lexer:0102] Unclosed multi-line comment, place closing */."); }
				currentIndex += (find - startSearch) + 2;
				line += countNewlines(source.substr(startSearch + 2, (find - 2) - startSearch));
				break;
			}
			else { console.get()->error(line, "Unexpected '/' character."); currentIndex++; break; }
		case '\"':
			find = source.find("\"", startSearch + 1);
			if (find == std::string::npos) { console.get()->error(line, "[lexer:0104] Unterminated string, place closing \"."); currentIndex = source.size(); break; }
			else
			{
				std::string val = source.substr(startSearch + 1, find - startSearch - 1);
				addToken(STokenType::STRING, val, line);
				line += countNewlines(val);
				currentIndex = find + 1;
				break;
			}
			//
		case '$':
			find = source.find("$", startSearch + 1);
			if (find == std::string::npos) { console.get()->error(line, "[lexer:0105] Unterminated script, place closing '$'."); currentIndex = source.size(); break; }
			else
			{
				std::string val = source.substr(startSearch + 1, find - startSearch - 1);
				addToken(STokenType::SCRIPT, val, line);
				line += countNewlines(val);
				currentIndex = find + 1;
				break;
			}
			//
		default:
			std::string err = "[lexer:0106] Unexcepted '";
			err.append({ c });
			err.append("' character.");
			currentIndex++;
			console.get()->error(line, err);
		}
	}

	// Place end of input stream token
	tokens.emplace_back(std::make_shared<SToken>(STokenType::END, int(line)));

	return tokens;
}

//
void SLexer::addToken(STokenType type, size_t line)
{
	tokens.emplace_back(std::make_shared<SToken>(type, int(line)));
}

//
void SLexer::addToken(STokenType type, char lexeme, size_t line)
{
	if (lexeme == ' ') { return; }
	std::string _lexeme = { lexeme };
	tokens.emplace_back(std::make_shared<SToken>(type, _lexeme, int(line)));
}
void SLexer::addToken(STokenType type, std::string lexeme, size_t line, unsigned char special)
{
	if (lexeme == "") { return; }
	std::string _lexeme = { lexeme };
	tokens.emplace_back(std::make_shared<SToken>(type, _lexeme, int(line), special));
}

//
void SLexer::addToken(STokenType type, std::string lexeme, size_t line)
{
	if (type == STokenType::IDENTIFIER)
	{
		lexeme.erase(std::remove(lexeme.begin(), lexeme.end(), '\n'), lexeme.end());
		lexeme.erase(remove_if(lexeme.begin(), lexeme.end(), isspace), lexeme.end());
		lexeme.erase(std::remove(lexeme.begin(), lexeme.end(), '\t'), lexeme.end());
	}
	if (lexeme == "" || lexeme == " ") { return; }
	tokens.emplace_back(std::make_shared<SToken>(type, lexeme, int(line)));
}

//
int SLexer::countNewlines(std::string val)
{
	size_t currentIndex = 0;
	int count = 0;
	while (val.find("\n", currentIndex) != std::string::npos)
	{
		size_t next = val.find("\n", currentIndex) + 1;
		count++;
		currentIndex = next;
	}
	return count;
}