#include "../include/clayout/LLexer.h"
#include "../include/clayout/ChromaLayout.h"
#include "../include/clayout/LEnums.h"
#include "../include/clayout/LToken.h"

#include <vector>
#include <string>
#include <memory>
#include <cctype>

//
LLexer::LLexer()
{

}

void LLexer::initialize(std::shared_ptr<ChromaLayout> console)
{
	this->console = console;
}

//
std::vector<std::shared_ptr<LToken>> LLexer::scanTokens(std::string source)
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
		if (islower(c) || isupper(c))
		{
			switch (c)
			{
			case 'B':
			case 'C': case 'c': case 'd': case 'G': case 'g':
			case 'H': case 'h': case 'I': case 'i': case 'L': case 'n':
			case 'o': case 'P': case 'p': case 'R': case 'r': case 's': case 'T': case 't':
			case 'V': case 'v':
				switch (c)
				{
				case 'B':
					if (source[startSearch + 1] == 'L' &&
						source[startSearch + 2] == 'O' &&
						source[startSearch + 3] == 'C' &&
						source[startSearch + 4] == 'K' &&
						(!isalpha(source[startSearch + 5])))
					{
						addToken(LTokenType::BLOCK, line);
						currentIndex += 5;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'C': case 'c':
					if (source[startSearch + 1] == 'l' &&
						source[startSearch + 2] == 'a' &&
						source[startSearch + 3] == 's' &&
						source[startSearch + 4] == 's' &&
						(!isalpha(source[startSearch + 5])))
					{
						addToken(LTokenType::CLASS, line);
						currentIndex += 5;
						break;
					}
					if (source[startSearch + 1] == 'O' &&
						source[startSearch + 2] == 'L' &&
						source[startSearch + 3] == 'O' &&
						source[startSearch + 4] == 'R' &&
						source[startSearch + 5] == '_' &&
						source[startSearch + 6] == 'B' &&
						source[startSearch + 7] == 'O' &&
						source[startSearch + 8] == 'X' &&
						(!isalpha(source[startSearch + 9])))
					{
						addToken(LTokenType::COLOR_BOX, line);
						currentIndex += 9;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'd':
					if (source[startSearch + 1] == 'e' &&
						source[startSearch + 2] == 'f' &&
						source[startSearch + 3] == 'a' &&
						source[startSearch + 4] == 'u' &&
						source[startSearch + 5] == 'l' &&
						source[startSearch + 6] == 't' &&
						source[startSearch + 7] == 'P' &&
						source[startSearch + 8] == 'o' &&
						source[startSearch + 9] == 's' &&
						source[startSearch + 10] == 'i' &&
						source[startSearch + 11] == 't' &&
						source[startSearch + 12] == 'i' &&
						source[startSearch + 13] == 'o' &&
						source[startSearch + 14] == 'n' &&
						(!isalpha(source[startSearch + 15])))
					{
						addToken(LTokenType::DEFAULT_POSITION, line);
						currentIndex += 15;
						break;
					}
					if (source[startSearch + 1] == 'r' &&
						source[startSearch + 2] == 'a' &&
						source[startSearch + 3] == 'g' &&
						source[startSearch + 4] == 'g' &&
						source[startSearch + 5] == 'a' &&
						source[startSearch + 6] == 'b' &&
						source[startSearch + 7] == 'l' &&
						source[startSearch + 8] == 'e' &&
						(!isalpha(source[startSearch + 9])))
					{
						addToken(LTokenType::DRAGGABLE, line);
						currentIndex += 9;
						break;
					}
					if (source[startSearch + 1] == 'r' &&
						source[startSearch + 2] == 'a' &&
						source[startSearch + 3] == 'g' &&
						source[startSearch + 4] == 'T' &&
						source[startSearch + 5] == 'y' &&
						source[startSearch + 6] == 'p' &&
						source[startSearch + 7] == 'e' &&
						(!isalpha(source[startSearch + 8])))
					{
						addToken(LTokenType::DRAG_TYPE, line);
						currentIndex += 8;
						break;
					}
					if (source[startSearch + 1] == 'r' &&
						source[startSearch + 2] == 'o' &&
						source[startSearch + 3] == 'p' &&
						source[startSearch + 4] == 'p' &&
						source[startSearch + 5] == 'a' &&
						source[startSearch + 6] == 'b' &&
						source[startSearch + 7] == 'l' &&
						source[startSearch + 8] == 'e' &&
						(!isalpha(source[startSearch + 9])))
					{
						addToken(LTokenType::DROPPABLE, line);
						currentIndex += 9;
						break;
					}
					if (source[startSearch + 1] == 'r' &&
						source[startSearch + 2] == 'o' &&
						source[startSearch + 3] == 'p' &&
						source[startSearch + 4] == 'T' &&
						source[startSearch + 5] == 'y' &&
						source[startSearch + 6] == 'p' &&
						source[startSearch + 7] == 'e' &&
						(!isalpha(source[startSearch + 8])))
					{
						addToken(LTokenType::DROP_TYPE, line);
						currentIndex += 8;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'G': case 'g':
					if (source[startSearch + 1] == 'R' &&
						source[startSearch + 2] == 'A' &&
						source[startSearch + 3] == 'D' &&
						source[startSearch + 4] == 'I' &&
						source[startSearch + 5] == 'E' &&
						source[startSearch + 6] == 'N' &&
						source[startSearch + 7] == 'T' &&
						source[startSearch + 8] == '_' &&
						source[startSearch + 9] == 'B' &&
						source[startSearch + 10] == 'O' &&
						source[startSearch + 11] == 'X' &&
						(!isalpha(source[startSearch + 12])))
					{
						addToken(LTokenType::GRADIENT_BOX, line);
						currentIndex += 12;
						break;
					}
					if (source[startSearch + 1] == 'r' &&
						source[startSearch + 2] == 'o' &&
						source[startSearch + 3] == 'u' &&
						source[startSearch + 4] == 'p' &&
						(!isalpha(source[startSearch + 5])))
					{
						addToken(LTokenType::GROUP, line);
						currentIndex += 5;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'H': case 'h':
					if (source[startSearch + 1] == '_' &&
						source[startSearch + 2] == 'B' &&
						source[startSearch + 3] == 'O' &&
						source[startSearch + 4] == 'X' &&
						(!isalpha(source[startSearch + 5])))
					{
						addToken(LTokenType::H_BOX, line);
						currentIndex += 5;
						break;
					}
					if (source[startSearch + 1] == 'U' &&
						source[startSearch + 2] == 'E' &&
						source[startSearch + 3] == '_' &&
						source[startSearch + 4] == 'R' &&
						source[startSearch + 5] == 'A' &&
						source[startSearch + 6] == 'D' &&
						source[startSearch + 7] == 'I' &&
						source[startSearch + 8] == 'U' &&
						source[startSearch + 9] == 'S' &&
						(!isalpha(source[startSearch + 10])))
					{
						addToken(LTokenType::HUE_RADIUS, line);
						currentIndex += 10;
						break;
					}
					if (source[startSearch + 1] == 'U' &&
						source[startSearch + 2] == 'E' &&
						source[startSearch + 3] == '_' &&
						source[startSearch + 4] == 'S' &&
						source[startSearch + 5] == 'T' &&
						source[startSearch + 6] == 'R' &&
						source[startSearch + 7] == 'I' &&
						source[startSearch + 8] == 'P' &&
						(!isalpha(source[startSearch + 9])))
					{
						addToken(LTokenType::HUE_STRIP, line);
						currentIndex += 9;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'I': case 'i':
					if (source[startSearch + 1] == 'd' &&
						(!isalpha(source[startSearch + 2])))
					{
						addToken(LTokenType::ID, line);
						currentIndex += 2;
						break;
					}
					if (source[startSearch + 1] == 'M' &&
						source[startSearch + 2] == 'A' &&
						source[startSearch + 3] == 'G' &&
						source[startSearch + 4] == 'E' &&
						(!isalpha(source[startSearch + 5])))
					{
						addToken(LTokenType::IMAGE, line);
						currentIndex += 5;
						break;
					}
					if (source[startSearch + 1] == 'm' &&
						source[startSearch + 2] == 'g' &&
						(!isalpha(source[startSearch + 3])))
					{
						addToken(LTokenType::IMG, line);
						currentIndex += 3;
						break;
					}
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'n' &&
						source[startSearch + 3] == 'e' &&
						source[startSearch + 4] == 'r' &&
						source[startSearch + 5] == 'C' &&
						source[startSearch + 6] == 'o' &&
						source[startSearch + 7] == 'n' &&
						source[startSearch + 8] == 't' &&
						source[startSearch + 9] == 'e' &&
						source[startSearch + 10] == 'n' &&
						source[startSearch + 11] == 't' &&
						(!isalpha(source[startSearch + 12])))
					{
						addToken(LTokenType::INNER_CONTENT, line);
						currentIndex += 12;
						break;
					}
					else { goto identifier; }
					break;
				case 'L':
					if (source[startSearch + 1] == 'I' &&
						source[startSearch + 2] == 'N' &&
						source[startSearch + 3] == 'E' &&
						(!isalpha(source[startSearch + 4])))
					{
						addToken(LTokenType::LINE, line);
						currentIndex += 4;
						break;
					}
					if (source[startSearch + 1] == 'I' &&
						source[startSearch + 2] == 'S' &&
						source[startSearch + 3] == 'T' &&
						(!isalpha(source[startSearch + 4]) && source[startSearch + 4] != '_'))
					{
						addToken(LTokenType::LIST, line);
						currentIndex += 4;
						break;
					}
					if (source[startSearch + 1] == 'I' &&
						source[startSearch + 2] == 'S' &&
						source[startSearch + 3] == 'T' &&
						source[startSearch + 4] == '_' &&
						source[startSearch + 5] == 'I' &&
						source[startSearch + 6] == 'T' &&
						source[startSearch + 7] == 'E' &&
						source[startSearch + 8] == 'M' &&
						(!isalpha(source[startSearch + 9])))
					{
						addToken(LTokenType::LIST_ITEM, line);
						currentIndex += 9;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'n':
					if (source[startSearch + 1] == 'a' &&
						source[startSearch + 2] == 'm' &&
						source[startSearch + 3] == 'e' &&
						(!isalpha(source[startSearch + 4])))
					{
						addToken(LTokenType::NAME, line);
						currentIndex += 4;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'o':
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'b' &&
						source[startSearch + 3] == 'l' &&
						source[startSearch + 4] == 'u' &&
						source[startSearch + 5] == 'r' &&
						(!isalpha(source[startSearch + 6])))
					{
						addToken(LTokenType::ON_BLUR, line);
						currentIndex += 6;
						break;
					}
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'c' &&
						source[startSearch + 3] == 'a' &&
						source[startSearch + 4] == 'n' &&
						source[startSearch + 5] == 'c' &&
						source[startSearch + 6] == 'e' &&
						source[startSearch + 7] == 'l' &&
						(!isalpha(source[startSearch + 8])))
					{
						addToken(LTokenType::ON_CANCEL, line);
						currentIndex += 8;
						break;
					}
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'c' &&
						source[startSearch + 3] == 'l' &&
						source[startSearch + 4] == 'i' &&
						source[startSearch + 5] == 'c' &&
						source[startSearch + 6] == 'k' &&
						(!isalpha(source[startSearch + 7])))
					{
						addToken(LTokenType::ON_CLICK, line);
						currentIndex += 7;
						break;
					}
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'c' &&
						source[startSearch + 3] == 'o' &&
						source[startSearch + 4] == 'p' &&
						source[startSearch + 5] == 'y' &&
						(!isalpha(source[startSearch + 6])))
					{
						addToken(LTokenType::ON_COPY, line);
						currentIndex += 6;
						break;
					}
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'c' &&
						source[startSearch + 3] == 'u' &&
						source[startSearch + 4] == 't' &&
						(!isalpha(source[startSearch + 5])))
					{
						addToken(LTokenType::ON_CUT, line);
						currentIndex += 5;
						break;
					}
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'd' &&
						source[startSearch + 3] == 'b' &&
						source[startSearch + 4] == 'l' &&
						source[startSearch + 5] == 'c' &&
						source[startSearch + 6] == 'l' &&
						source[startSearch + 7] == 'i' &&
						source[startSearch + 8] == 'c' &&
						source[startSearch + 9] == 'k' &&
						(!isalpha(source[startSearch + 10])))
					{
						addToken(LTokenType::ON_DBL_CLICK, line);
						currentIndex += 10;
						break;
					}
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'e' &&
						source[startSearch + 3] == 'n' &&
						source[startSearch + 4] == 't' &&
						source[startSearch + 5] == 'r' &&
						source[startSearch + 6] == 'y' &&
						(!isalpha(source[startSearch + 7])))
					{
						addToken(LTokenType::ON_ENTRY, line);
						currentIndex += 7;
						break;
					}
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'm' &&
						source[startSearch + 3] == 'i' &&
						source[startSearch + 4] == 'd' &&
						source[startSearch + 5] == 'd' &&
						source[startSearch + 6] == 'l' &&
						source[startSearch + 7] == 'e' &&
						source[startSearch + 8] == 'c' &&
						source[startSearch + 9] == 'l' &&
						source[startSearch + 10] == 'i' &&
						source[startSearch + 11] == 'c' &&
						source[startSearch + 12] == 'k' &&
						(!isalpha(source[startSearch + 13])))
					{
						addToken(LTokenType::ON_MIDDLECLICK, line);
						currentIndex += 13;
						break;
					}
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'd' &&
						source[startSearch + 3] == 'b' &&
						source[startSearch + 4] == 'l' &&
						source[startSearch + 5] == 'm' &&
						source[startSearch + 6] == 'i' &&
						source[startSearch + 7] == 'd' &&
						source[startSearch + 8] == 'd' &&
						source[startSearch + 9] == 'l' &&
						source[startSearch + 10] == 'e' &&
						source[startSearch + 11] == 'c' &&
						source[startSearch + 12] == 'l' &&
						source[startSearch + 13] == 'i' &&
						source[startSearch + 14] == 'c' &&
						source[startSearch + 15] == 'k' &&
						(!isalpha(source[startSearch + 16])))
					{
						addToken(LTokenType::ON_DBL_MIDDLECLICK, line);
						currentIndex += 16;
						break;
					}
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'd' &&
						source[startSearch + 3] == 'b' &&
						source[startSearch + 4] == 'l' &&
						source[startSearch + 5] == 'r' &&
						source[startSearch + 6] == 'i' &&
						source[startSearch + 7] == 'g' &&
						source[startSearch + 8] == 'h' &&
						source[startSearch + 9] == 't' &&
						source[startSearch + 10] == 'c' &&
						source[startSearch + 11] == 'l' &&
						source[startSearch + 12] == 'i' &&
						source[startSearch + 13] == 'c' &&
						source[startSearch + 14] == 'k' &&
						(!isalpha(source[startSearch + 15])))
					{
						addToken(LTokenType::ON_DBL_RIGHTCLICK, line);
						currentIndex += 15;
						break;
					}
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'd' &&
						source[startSearch + 3] == 'r' &&
						source[startSearch + 4] == 'a' &&
						source[startSearch + 5] == 'g' &&
						(!isalpha(source[startSearch + 6])))
					{
						addToken(LTokenType::ON_DRAG, line);
						currentIndex += 6;
						break;
					}
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'd' &&
						source[startSearch + 3] == 'r' &&
						source[startSearch + 4] == 'a' &&
						source[startSearch + 5] == 'g' &&
						source[startSearch + 6] == 'e' &&
						source[startSearch + 7] == 'n' &&
						source[startSearch + 8] == 'd' &&
						(!isalpha(source[startSearch + 9])))
					{
						addToken(LTokenType::ON_DRAG_END, line);
						currentIndex += 9;
						break;
					}
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'd' &&
						source[startSearch + 3] == 'r' &&
						source[startSearch + 4] == 'a' &&
						source[startSearch + 5] == 'g' &&
						source[startSearch + 6] == 'e' &&
						source[startSearch + 7] == 'n' &&
						source[startSearch + 8] == 't' &&
						source[startSearch + 9] == 'e' &&
						source[startSearch + 10] == 'r' &&
						(!isalpha(source[startSearch + 11])))
					{
						addToken(LTokenType::ON_DRAG_ENTER, line);
						currentIndex += 11;
						break;
					}
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'd' &&
						source[startSearch + 3] == 'r' &&
						source[startSearch + 4] == 'a' &&
						source[startSearch + 5] == 'g' &&
						source[startSearch + 6] == 'l' &&
						source[startSearch + 7] == 'e' &&
						source[startSearch + 8] == 'a' &&
						source[startSearch + 9] == 'v' &&
						source[startSearch + 10] == 'e' &&
						(!isalpha(source[startSearch + 11])))
					{
						addToken(LTokenType::ON_DRAG_LEAVE, line);
						currentIndex += 11;
						break;
					}
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'd' &&
						source[startSearch + 3] == 'r' &&
						source[startSearch + 4] == 'a' &&
						source[startSearch + 5] == 'g' &&
						source[startSearch + 6] == 'o' &&
						source[startSearch + 7] == 'v' &&
						source[startSearch + 8] == 'e' &&
						source[startSearch + 9] == 'r' &&
						(!isalpha(source[startSearch + 10])))
					{
						addToken(LTokenType::ON_DRAG_OVER, line);
						currentIndex += 10;
						break;
					}
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'd' &&
						source[startSearch + 3] == 'r' &&
						source[startSearch + 4] == 'a' &&
						source[startSearch + 5] == 'g' &&
						source[startSearch + 6] == 's' &&
						source[startSearch + 7] == 't' &&
						source[startSearch + 8] == 'a' &&
						source[startSearch + 9] == 'r' &&
						source[startSearch + 10] == 't' &&
						(!isalpha(source[startSearch + 11])))
					{
						addToken(LTokenType::ON_DRAG_START, line);
						currentIndex += 11;
						break;
					}
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'd' &&
						source[startSearch + 3] == 'r' &&
						source[startSearch + 4] == 'o' &&
						source[startSearch + 5] == 'p' &&
						(!isalpha(source[startSearch + 6])))
					{
						addToken(LTokenType::ON_DROP, line);
						currentIndex += 6;
						break;
					}
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'f' &&
						source[startSearch + 3] == 'o' &&
						source[startSearch + 4] == 'c' &&
						source[startSearch + 5] == 'u' &&
						source[startSearch + 6] == 's' &&
						(!isalpha(source[startSearch + 7])))
					{
						addToken(LTokenType::ON_FOCUS, line);
						currentIndex += 7;
						break;
					}
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'k' &&
						source[startSearch + 3] == 'e' &&
						source[startSearch + 4] == 'y' &&
						source[startSearch + 5] == 'd' &&
						source[startSearch + 6] == 'o' &&
						source[startSearch + 7] == 'w' &&
						source[startSearch + 8] == 'n' &&
						(!isalpha(source[startSearch + 9])))
					{
						addToken(LTokenType::ON_KEY_DOWN, line);
						currentIndex += 9;
						break;
					}
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'k' &&
						source[startSearch + 3] == 'e' &&
						source[startSearch + 4] == 'y' &&
						source[startSearch + 5] == 'p' &&
						source[startSearch + 6] == 'r' &&
						source[startSearch + 7] == 'e' &&
						source[startSearch + 8] == 's' &&
						source[startSearch + 9] == 's' &&
						(!isalpha(source[startSearch + 10])))
					{
						addToken(LTokenType::ON_KEY_PRESS, line);
						currentIndex += 10;
						break;
					}
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'k' &&
						source[startSearch + 3] == 'e' &&
						source[startSearch + 4] == 'y' &&
						source[startSearch + 5] == 'u' &&
						source[startSearch + 6] == 'p' &&
						(!isalpha(source[startSearch + 7])))
					{
						addToken(LTokenType::ON_KEY_UP, line);
						currentIndex += 7;
						break;
					}
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'l' &&
						source[startSearch + 3] == 'o' &&
						source[startSearch + 4] == 'a' &&
						source[startSearch + 5] == 'd' &&
						(!isalpha(source[startSearch + 8])))
					{
						addToken(LTokenType::ON_LOAD, line);
						currentIndex += 8;
						break;
					}
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'm' &&
						source[startSearch + 3] == 'i' &&
						source[startSearch + 4] == 'd' &&
						source[startSearch + 5] == 'd' &&
						source[startSearch + 6] == 'l' &&
						source[startSearch + 7] == 'e' &&
						source[startSearch + 8] == 'c' &&
						source[startSearch + 9] == 'l' &&
						source[startSearch + 10] == 'i' &&
						source[startSearch + 11] == 'c' &&
						source[startSearch + 12] == 'k' &&
						(!isalpha(source[startSearch + 13])))
					{
						addToken(LTokenType::ON_MIDDLECLICK, line);
						currentIndex += 13;
						break;
					}
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'm' &&
						source[startSearch + 3] == 'o' &&
						source[startSearch + 4] == 'u' &&
						source[startSearch + 5] == 's' &&
						source[startSearch + 6] == 'e' &&
						source[startSearch + 7] == 'e' &&
						source[startSearch + 8] == 'n' &&
						source[startSearch + 9] == 't' &&
						source[startSearch + 10] == 'e' &&
						source[startSearch + 11] == 'r' &&
						(!isalpha(source[startSearch + 12])))
					{
						addToken(LTokenType::ON_MOUSE_ENTER, line);
						currentIndex += 12;
						break;
					}
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'm' &&
						source[startSearch + 3] == 'o' &&
						source[startSearch + 4] == 'u' &&
						source[startSearch + 5] == 's' &&
						source[startSearch + 6] == 'e' &&
						source[startSearch + 7] == 'l' &&
						source[startSearch + 8] == 'e' &&
						source[startSearch + 9] == 'a' &&
						source[startSearch + 10] == 'v' &&
						source[startSearch + 11] == 'e' &&
						(!isalpha(source[startSearch + 12])))
					{
						addToken(LTokenType::ON_MOUSE_LEAVE, line);
						currentIndex += 12;
						break;
					}
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'm' &&
						source[startSearch + 3] == 'o' &&
						source[startSearch + 4] == 'u' &&
						source[startSearch + 5] == 's' &&
						source[startSearch + 6] == 'e' &&
						source[startSearch + 7] == 'o' &&
						source[startSearch + 8] == 'v' &&
						source[startSearch + 9] == 'e' &&
						source[startSearch + 10] == 'r' &&
						(!isalpha(source[startSearch + 11])))
					{
						addToken(LTokenType::ON_MOUSE_OVER, line);
						currentIndex += 11;
						break;
					}
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'm' &&
						source[startSearch + 3] == 'o' &&
						source[startSearch + 4] == 'u' &&
						source[startSearch + 5] == 's' &&
						source[startSearch + 6] == 'e' &&
						source[startSearch + 7] == 'w' &&
						source[startSearch + 8] == 'h' &&
						source[startSearch + 9] == 'e' &&
						source[startSearch + 10] == 'e' &&
						source[startSearch + 11] == 'l' &&
						source[startSearch + 12] == 'd' &&
						source[startSearch + 13] == 'o' &&
						source[startSearch + 14] == 'w' &&
						source[startSearch + 15] == 'n' &&
						(!isalpha(source[startSearch + 16])))
					{
						addToken(LTokenType::ON_MOUSE_WHEELDOWN, line);
						currentIndex += 16;
						break;
					}
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'm' &&
						source[startSearch + 3] == 'o' &&
						source[startSearch + 4] == 'u' &&
						source[startSearch + 5] == 's' &&
						source[startSearch + 6] == 'e' &&
						source[startSearch + 7] == 'w' &&
						source[startSearch + 8] == 'h' &&
						source[startSearch + 9] == 'e' &&
						source[startSearch + 10] == 'e' &&
						source[startSearch + 11] == 'l' &&
						source[startSearch + 12] == 'u' &&
						source[startSearch + 13] == 'p' &&
						(!isalpha(source[startSearch + 14])))
					{
						addToken(LTokenType::ON_MOUSE_WHEELUP, line);
						currentIndex += 14;
						break;
					}
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'p' &&
						source[startSearch + 3] == 'a' &&
						source[startSearch + 4] == 's' &&
						source[startSearch + 5] == 't' &&
						source[startSearch + 6] == 'e' &&
						(!isalpha(source[startSearch + 7])))
					{
						addToken(LTokenType::ON_PASTE, line);
						currentIndex += 7;
						break;
					}
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'r' &&
						source[startSearch + 3] == 'e' &&
						source[startSearch + 4] == 'l' &&
						source[startSearch + 5] == 'e' &&
						source[startSearch + 6] == 'a' &&
						source[startSearch + 7] == 's' &&
						source[startSearch + 8] == 'e' &&
						(!isalpha(source[startSearch + 9])))
					{
						addToken(LTokenType::ON_RELEASE, line);
						currentIndex += 9;
						break;
					}
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'r' &&
						source[startSearch + 3] == 'e' &&
						source[startSearch + 4] == 's' &&
						source[startSearch + 5] == 'i' &&
						source[startSearch + 6] == 'z' &&
						source[startSearch + 7] == 'e' &&
						(!isalpha(source[startSearch + 8])))
					{
						addToken(LTokenType::ON_RESIZE, line);
						currentIndex += 8;
						break;
					}
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'r' &&
						source[startSearch + 3] == 'i' &&
						source[startSearch + 4] == 'g' &&
						source[startSearch + 5] == 'h' &&
						source[startSearch + 6] == 't' &&
						source[startSearch + 7] == 'c' &&
						source[startSearch + 8] == 'l' &&
						source[startSearch + 9] == 'i' &&
						source[startSearch + 10] == 'c' &&
						source[startSearch + 11] == 'k' &&
						(!isalpha(source[startSearch + 12])))
					{
						addToken(LTokenType::ON_RIGGHTCLICK, line);
						currentIndex += 12;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'P': case 'p':
					if (source[startSearch + 1] == 'A' &&
						source[startSearch + 2] == 'N' &&
						source[startSearch + 3] == 'E' &&
						source[startSearch + 4] == 'L' &&
						(!isalpha(source[startSearch + 5])))
					{
						addToken(LTokenType::PANEL, line);
						currentIndex += 5;
						break;
					}
					if (source[startSearch + 1] == 'A' &&
						source[startSearch + 2] == 'R' &&
						source[startSearch + 3] == 'A' &&
						source[startSearch + 4] == 'G' &&
						source[startSearch + 5] == 'R' &&
						source[startSearch + 6] == 'A' &&
						source[startSearch + 7] == 'P' &&
						source[startSearch + 8] == 'H' &&
						(!isalpha(source[startSearch + 9])))
					{
						addToken(LTokenType::PARAGRAPH, line);
						currentIndex += 9;
						break;
					}
					if (source[startSearch + 1] == 'R' &&
						source[startSearch + 2] == 'O' &&
						source[startSearch + 3] == 'T' &&
						source[startSearch + 4] == 'O' &&
						(!isalpha(source[startSearch + 5])))
					{
						addToken(LTokenType::PROTO, line);
						currentIndex += 5;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'R': case 'r':
					if (source[startSearch + 1] == 'O' &&
						source[startSearch + 2] == 'O' &&
						source[startSearch + 3] == 'T' &&
						(!isalpha(source[startSearch + 4])))
					{
						addToken(LTokenType::ROOT, line);
						currentIndex += 4;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 's':
					/* Disabling SCRIPT tag until it is useful
					if (source[startSearch + 1] == 'c' &&
						source[startSearch + 2] == 'r' &&
						source[startSearch + 3] == 'i' &&
						source[startSearch + 4] == 'p' &&
						source[startSearch + 5] == 't' &&
						(!isalpha(source[startSearch + 6])))
					{
						addToken(LTokenType::SCRIPT, line);
						currentIndex += 6;
						break;
					}
					*/
					if (source[startSearch + 1] == 't' &&
						source[startSearch + 2] == 'y' &&
						source[startSearch + 3] == 'l' &&
						source[startSearch + 4] == 'e' &&
						(!isalpha(source[startSearch + 5])))
					{
						addToken(LTokenType::STYLE, line);
						currentIndex += 5;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'T': case 't':
					if (source[startSearch + 1] == 'E' &&
						source[startSearch + 2] == 'X' &&
						source[startSearch + 3] == 'T' &&
						(!isalpha(source[startSearch + 4])) && source[startSearch + 4] != '_')
					{
						addToken(LTokenType::TEXT, line);
						currentIndex += 4;
						break;
					}
					if (source[startSearch + 1] == 'E' &&
						source[startSearch + 2] == 'X' &&
						source[startSearch + 3] == 'T' &&
						source[startSearch + 4] == '_' &&
						source[startSearch + 5] == 'E' &&
						source[startSearch + 6] == 'N' &&
						source[startSearch + 7] == 'T' &&
						source[startSearch + 8] == 'R' &&
						source[startSearch + 9] == 'Y' &&
						(!isalpha(source[startSearch + 10])))
					{
						addToken(LTokenType::TEXT_ENTRY, line);
						currentIndex += 10;
						break;
					}
					if (source[startSearch + 1] == 'I' &&
						source[startSearch + 2] == 'L' &&
						source[startSearch + 3] == 'E' &&
						source[startSearch + 4] == '_' &&
						source[startSearch + 5] == 'B' &&
						source[startSearch + 6] == 'O' &&
						source[startSearch + 7] == 'X' &&
						(!isalpha(source[startSearch + 8])))
					{
						addToken(LTokenType::TILE_BOX, line);
						currentIndex += 8;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'V': case 'v':
					if (source[startSearch + 1] == 'a' &&
						source[startSearch + 2] == 'l' &&
						source[startSearch + 3] == 'u' &&
						source[startSearch + 4] == 'e' &&
						(!isalpha(source[startSearch + 5])))
					{
						addToken(LTokenType::VALUE, line);
						currentIndex += 5;
						break;
					}
					if (source[startSearch + 1] == '_' &&
						source[startSearch + 2] == 'B' &&
						source[startSearch + 3] == 'O' &&
						source[startSearch + 4] == 'X' &&
						(!isalpha(source[startSearch + 5])))
					{
						addToken(LTokenType::V_BOX, line);
						currentIndex += 5;
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
					addToken(LTokenType::IDENTIFIER, source.substr(startSearch, advance), line);
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
				addToken(LTokenType::IDENTIFIER, source.substr(startSearch, advance), line);
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
			addToken(LTokenType::IDENTIFIER, source.substr(startSearch, advance), line);
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
				console.get()->error(line, "Unexpected '-' character."); currentIndex++; break;
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
			addToken(LTokenType::NUM, source.substr(startSearch, advance), line);
			currentIndex += advance;
			break;
			//
		case '=': addToken(LTokenType::EQUAL, c, line); currentIndex++; break;
		case '<': addToken(LTokenType::ANGLE_LEFT, c, line); currentIndex++; break;
		case '>': addToken(LTokenType::ANGLE_RIGHT, c, line); currentIndex++; break;
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
				if (find == std::string::npos) { console.get()->error(line, "[lexer:0101] Unclosed multi-line comment, place closing */."); }
				currentIndex += (find - startSearch) + 2;
				line += countNewlines(source.substr(startSearch + 2, (find - 2) - startSearch));
				break;
			}
			else 
			{ 
				addToken(LTokenType::CLOSE, c, line); currentIndex++; break;
			}
		case '\"':
			find = source.find("\"", startSearch + 1);
			if (find == std::string::npos) { console.get()->error(line, "[lexer:0104] Unterminated string, place closing \"."); currentIndex = source.size(); break; }
			else
			{
				std::string val = source.substr(startSearch + 1, find - startSearch - 1);
				while (val.find("&quot;") != std::string::npos)
				{
					size_t pos = val.find("&quot;");
					val.erase(pos, 6);
					val.insert(pos, { '\"' });
				}
				addToken(LTokenType::STRING, val, line);
				line += countNewlines(val);
				currentIndex = find + 1;
				break;
			}
		default:
			std::string err = "[lexer:0105] Unexcepted '";
			err.append({ c });
			err.append("' character.");
			currentIndex++;
			console.get()->error(line, err);
		}
	}

	// Place end of input stream token
	tokens.emplace_back(std::make_shared<LToken>(LTokenType::END, int(line)));

	return tokens;
}

//
void LLexer::addToken(LTokenType type, size_t line)
{
	tokens.emplace_back(std::make_shared<LToken>(type, int(line)));
}

//
void LLexer::addToken(LTokenType type, char lexeme, size_t line)
{
	std::string _lexeme = { lexeme };
	tokens.emplace_back(std::make_shared<LToken>(type, _lexeme, int(line)));
}

//
void LLexer::addToken(LTokenType type, std::string lexeme, size_t line)
{
	tokens.emplace_back(std::make_shared<LToken>(type, lexeme, int(line)));
}

//
int LLexer::countNewlines(std::string val)
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