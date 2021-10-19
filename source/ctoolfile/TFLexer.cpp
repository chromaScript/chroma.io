#include "../include/ctoolfile/TFLexer.h"
#include "../include/ctoolfile/ChromaToolFile.h"
#include "../include/ctoolfile/TFEnums.h"
#include "../include/ctoolfile/TFToken.h"

#include <vector>
#include <string>
#include <memory>
#include <cctype>

//
TFLexer::TFLexer()
{

}

void TFLexer::initialize(std::shared_ptr<ChromaToolFile> console)
{
	this->console = console;
}

//
std::vector<std::shared_ptr<TFToken>> TFLexer::scanTokens(std::string source)
{
	//
	tokens.clear();
	//
	size_t startSearch = 0;
	size_t currentIndex = 0;
	size_t line = 1;
	size_t advance = 0;
	size_t find = 0;
	bool decimalFound = false;
	//
	while (currentIndex < source.size())
	{
		startSearch = currentIndex;
		char c = source[startSearch];
		// Identifiers and Keywords
		if (islower(c) || isupper(c) || c == '_' || c == '[')
		{
			char cl = std::tolower(c);
			switch (cl)
			{
			case 'a': case 'b': case 'c': case 'd': case 'e': case 'i':
			case 'f': case 'g': case 'm': case 'o': case 'p': case 'r':
			case 's': case 't': case 'v': case 'w': case 'z': 
			case '_': case '[':
				switch (cl)
				{
				case 'a':
					if (std::tolower(source[startSearch + 1]) == 'l' &&
						std::tolower(source[startSearch + 2]) == 'p' &&
						std::tolower(source[startSearch + 3]) == 'h' &&
						std::tolower(source[startSearch + 4]) == 'a' &&
						(isspace(source[startSearch + 5]) || !isalnum(source[startSearch + 5])))
					{
						addToken(TFTokenType::ALPHA, line);
						currentIndex += 5;
						break;
					}
					if (std::tolower(source[startSearch + 1]) == 'n' &&
						std::tolower(source[startSearch + 2]) == 't' &&
						std::tolower(source[startSearch + 3]) == 'i' &&
						std::tolower(source[startSearch + 4]) == 'a' &&
						std::tolower(source[startSearch + 5]) == 'l' &&
						std::tolower(source[startSearch + 6]) == 'i' &&
						std::tolower(source[startSearch + 7]) == 'a' &&
						std::tolower(source[startSearch + 8]) == 's' &&
						std::tolower(source[startSearch + 9]) == 'i' &&
						std::tolower(source[startSearch + 10]) == 'n' &&
						std::tolower(source[startSearch + 11]) == 'g' &&
						(isspace(source[startSearch + 12]) || !isalnum(source[startSearch + 12])))
					{
						addToken(TFTokenType::ANTIALIASING, line);
						currentIndex += 12;
						break;
					}
					else { goto identifier; }
					break;
				case 'b':
					if (std::tolower(source[startSearch + 1]) == 'a' &&
						std::tolower(source[startSearch + 2]) == 's' &&
						std::tolower(source[startSearch + 3]) == 'i' &&
						std::tolower(source[startSearch + 4]) == 'c' &&
						(isspace(source[startSearch + 5]) || !isalnum(source[startSearch + 5])))
					{
						addToken(TFTokenType::BASIC, line);
						currentIndex += 5;
						break;
					}
					if (std::tolower(source[startSearch + 1]) == 'l' &&
						std::tolower(source[startSearch + 2]) == 'e' &&
						std::tolower(source[startSearch + 3]) == 'n' &&
						std::tolower(source[startSearch + 4]) == 'd' &&
						(isspace(source[startSearch + 5]) || !isalnum(source[startSearch + 5])))
					{
						addToken(TFTokenType::BLEND, line);
						currentIndex += 5;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'c':
					if (std::tolower(source[startSearch + 1]) == 'h' &&
						std::tolower(source[startSearch + 2]) == 'a' &&
						std::tolower(source[startSearch + 3]) == 'r' &&
						std::tolower(source[startSearch + 4]) == 'a' &&
						std::tolower(source[startSearch + 5]) == 'c' &&
						std::tolower(source[startSearch + 6]) == 't' &&
						std::tolower(source[startSearch + 7]) == 'e' &&
						std::tolower(source[startSearch + 8]) == 'r' &&
						(isspace(source[startSearch + 9]) || !isalnum(source[startSearch + 9])))
					{
						addToken(TFTokenType::CHARACTER, line);
						currentIndex += 9;
						break;
					}
					if (std::tolower(source[startSearch + 1]) == 'o' &&
						std::tolower(source[startSearch + 2]) == 'l' &&
						std::tolower(source[startSearch + 3]) == 'o' &&
						std::tolower(source[startSearch + 4]) == 'r' &&
						(isspace(source[startSearch + 5]) || !isalnum(source[startSearch + 5])))
					{
						addToken(TFTokenType::COLOR, line);
						currentIndex += 5;
						break;
					}
					if (std::tolower(source[startSearch + 1]) == 'o' &&
						std::tolower(source[startSearch + 2]) == 'n' &&
						std::tolower(source[startSearch + 3]) == 't' &&
						std::tolower(source[startSearch + 4]) == 'i' &&
						std::tolower(source[startSearch + 5]) == 'n' &&
						std::tolower(source[startSearch + 6]) == 'u' &&
						std::tolower(source[startSearch + 7]) == 'o' &&
						std::tolower(source[startSearch + 8]) == 'u' &&
						std::tolower(source[startSearch + 9]) == 's' &&
						(isspace(source[startSearch + 10]) || !isalnum(source[startSearch + 10])))
					{
						addToken(TFTokenType::CONTINUOUS, line);
						currentIndex += 10;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'd':
					if (std::tolower(source[startSearch + 1]) == 'i' &&
						std::tolower(source[startSearch + 2]) == 's' &&
						std::tolower(source[startSearch + 3]) == 't' &&
						std::tolower(source[startSearch + 4]) == 'a' &&
						std::tolower(source[startSearch + 5]) == 'n' &&
						std::tolower(source[startSearch + 6]) == 'c' &&
						std::tolower(source[startSearch + 7]) == 'e' &&
						(isspace(source[startSearch + 8]) || !isalnum(source[startSearch + 8])))
					{
						addToken(TFTokenType::DISTANCE, line);
						currentIndex += 8;
						break;
					}
					if (std::tolower(source[startSearch + 1]) == 'r' &&
						std::tolower(source[startSearch + 2]) == 'a' &&
						std::tolower(source[startSearch + 3]) == 'g' &&
						(isspace(source[startSearch + 4]) || !isalnum(source[startSearch + 4])))
					{
						addToken(TFTokenType::DRAG, line);
						currentIndex += 4;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'e':
					if (std::tolower(source[startSearch + 1]) == 'f' &&
						std::tolower(source[startSearch + 2]) == 'f' &&
						std::tolower(source[startSearch + 3]) == 'e' &&
						std::tolower(source[startSearch + 4]) == 'c' &&
						std::tolower(source[startSearch + 5]) == 't' &&
						std::tolower(source[startSearch + 6]) == 's' &&
						(isspace(source[startSearch + 7]) || !isalnum(source[startSearch + 7])))
					{
						addToken(TFTokenType::EFFECTS, line);
						currentIndex += 7;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'f':
					if (std::tolower(source[startSearch + 1]) == 'a' &&
						std::tolower(source[startSearch + 2]) == 'n' &&
						(isspace(source[startSearch + 3]) || !isalnum(source[startSearch + 3])))
					{
						addToken(TFTokenType::FAN, line);
						currentIndex += 3;
						break;
					}
					if (std::tolower(source[startSearch + 1]) == 'i' &&
						std::tolower(source[startSearch + 2]) == 'e' &&
						std::tolower(source[startSearch + 3]) == 'l' &&
						std::tolower(source[startSearch + 4]) == 'd' &&
						(isspace(source[startSearch + 5]) || !isalnum(source[startSearch + 5])))
					{
						addToken(TFTokenType::FIELD, line);
						currentIndex += 5;
						break;
					}
					if (std::tolower(source[startSearch + 1]) == 'i' &&
						std::tolower(source[startSearch + 2]) == 'l' &&
						std::tolower(source[startSearch + 3]) == 'l' &&
						(source[startSearch + 4] != '_' && (isspace(source[startSearch + 4]) || !isalnum(source[startSearch + 4]))))
					{
						addToken(TFTokenType::FILL, line);
						currentIndex += 4;
						break;
					}
					if (std::tolower(source[startSearch + 1]) == 'i' &&
						std::tolower(source[startSearch + 2]) == 'l' &&
						std::tolower(source[startSearch + 3]) == 't' &&
						std::tolower(source[startSearch + 4]) == 'e' &&
						std::tolower(source[startSearch + 5]) == 'r' &&
						(isspace(source[startSearch + 6]) || !isalnum(source[startSearch + 6])))
					{
						addToken(TFTokenType::FILTER, line);
						currentIndex += 6;
						break;
					}
					if (std::tolower(source[startSearch + 1]) == 'x' &&
						std::tolower(source[startSearch + 2]) == '_')
					{
						if (std::tolower(source[startSearch + 3]) == 'b' &&
							std::tolower(source[startSearch + 4]) == 'l' &&
							std::tolower(source[startSearch + 5]) == 'u' &&
							std::tolower(source[startSearch + 6]) == 'r' &&
							(isspace(source[startSearch + 7]) || !isalnum(source[startSearch + 7])))
						{
							addToken(TFTokenType::FX_BLUR, line);
							currentIndex += 7;
							break;
						}
						if (std::tolower(source[startSearch + 3]) == 'b' &&
							std::tolower(source[startSearch + 4]) == 'r' &&
							std::tolower(source[startSearch + 5]) == 'i' &&
							std::tolower(source[startSearch + 6]) == 'g' &&
							std::tolower(source[startSearch + 7]) == 'h' &&
							std::tolower(source[startSearch + 8]) == 't' &&
							std::tolower(source[startSearch + 9]) == 'c' &&
							std::tolower(source[startSearch + 10]) == 'o' &&
							std::tolower(source[startSearch + 11]) == 'n' &&
							std::tolower(source[startSearch + 12]) == 't' &&
							std::tolower(source[startSearch + 13]) == 'r' &&
							std::tolower(source[startSearch + 14]) == 'a' &&
							std::tolower(source[startSearch + 15]) == 's' &&
							std::tolower(source[startSearch + 16]) == 't' &&
							(isspace(source[startSearch + 17]) || !isalnum(source[startSearch + 17])))
						{
							addToken(TFTokenType::FX_BRIGHTCONTRAST, line);
							currentIndex += 17;
							break;
						}
						if (std::tolower(source[startSearch + 3]) == 'f' &&
							std::tolower(source[startSearch + 4]) == 'i' &&
							std::tolower(source[startSearch + 5]) == 'l' &&
							std::tolower(source[startSearch + 6]) == 'l' &&
							(isspace(source[startSearch + 7]) || !isalnum(source[startSearch + 7])))
						{
							addToken(TFTokenType::FX_FILL, line);
							currentIndex += 7;
							break;
						}
						if (std::tolower(source[startSearch + 3]) == 'g' &&
							std::tolower(source[startSearch + 4]) == 'r' &&
							std::tolower(source[startSearch + 5]) == 'a' &&
							std::tolower(source[startSearch + 6]) == 'd' &&
							std::tolower(source[startSearch + 7]) == 'i' &&
							std::tolower(source[startSearch + 8]) == 'e' &&
							std::tolower(source[startSearch + 9]) == 'n' &&
							std::tolower(source[startSearch + 10]) == 't' &&
							(isspace(source[startSearch + 11]) || !isalnum(source[startSearch + 11])))
						{
							addToken(TFTokenType::GRADIENT, line);
							currentIndex += 11;
							break;
						}
						if (std::tolower(source[startSearch + 3]) == 'h' &&
							std::tolower(source[startSearch + 4]) == 's' &&
							std::tolower(source[startSearch + 5]) == 'v' &&
							(isspace(source[startSearch + 6]) || !isalnum(source[startSearch + 6])))
						{
							addToken(TFTokenType::FX_HSV, line);
							currentIndex += 6;
							break;
						}
						if (std::tolower(source[startSearch + 3]) == 'i' &&
							std::tolower(source[startSearch + 4]) == 'n' &&
							std::tolower(source[startSearch + 5]) == 'v' &&
							std::tolower(source[startSearch + 6]) == 'e' &&
							std::tolower(source[startSearch + 7]) == 'r' &&
							std::tolower(source[startSearch + 8]) == 't' &&
							(isspace(source[startSearch + 9]) || !isalnum(source[startSearch + 9])))
						{
							addToken(TFTokenType::FX_INVERT, line);
							currentIndex += 9;
							break;
						}
						if (std::tolower(source[startSearch + 3]) == 'm' &&
							std::tolower(source[startSearch + 4]) == 'o' &&
							std::tolower(source[startSearch + 5]) == 'd' &&
							std::tolower(source[startSearch + 6]) == 'u' &&
							std::tolower(source[startSearch + 7]) == 'l' &&
							std::tolower(source[startSearch + 8]) == 'o' &&
							(isspace(source[startSearch + 9]) || !isalnum(source[startSearch + 9])))
						{
							addToken(TFTokenType::FX_MODULO, line);
							currentIndex += 9;
							break;
						}
						if (std::tolower(source[startSearch + 3]) == 'p' &&
							std::tolower(source[startSearch + 4]) == 'o' &&
							std::tolower(source[startSearch + 5]) == 's' &&
							std::tolower(source[startSearch + 6]) == 't' &&
							std::tolower(source[startSearch + 7]) == 'e' &&
							std::tolower(source[startSearch + 8]) == 'r' &&
							std::tolower(source[startSearch + 9]) == 'i' &&
							std::tolower(source[startSearch + 10]) == 'z' &&
							std::tolower(source[startSearch + 11]) == 'e' &&
							(isspace(source[startSearch + 12]) || !isalnum(source[startSearch + 12])))
						{
							addToken(TFTokenType::FX_POSTERIZE, line);
							currentIndex += 12;
							break;
						}
						if (std::tolower(source[startSearch + 3]) == 'p' &&
							std::tolower(source[startSearch + 4]) == 'o' &&
							std::tolower(source[startSearch + 5]) == 'w' &&
							std::tolower(source[startSearch + 6]) == 'e' &&
							std::tolower(source[startSearch + 7]) == 'r' &&
							(isspace(source[startSearch + 8]) || !isalnum(source[startSearch + 8])))
						{
							addToken(TFTokenType::FX_POWER, line);
							currentIndex += 8;
							break;
						}
						if (std::tolower(source[startSearch + 3]) == 't' &&
							std::tolower(source[startSearch + 4]) == 'h' &&
							std::tolower(source[startSearch + 5]) == 'r' &&
							std::tolower(source[startSearch + 6]) == 'e' &&
							std::tolower(source[startSearch + 7]) == 's' &&
							std::tolower(source[startSearch + 8]) == 'h' &&
							std::tolower(source[startSearch + 9]) == 'o' &&
							std::tolower(source[startSearch + 10]) == 'l' &&
							std::tolower(source[startSearch + 11]) == 'd' &&
							(isspace(source[startSearch + 12]) || !isalnum(source[startSearch + 12])))
						{
							addToken(TFTokenType::FX_THRESHOLD, line);
							currentIndex += 12;
							break;
						}
					}
					else { goto identifier; }
					break;
					//
				case 'g':
					if (std::tolower(source[startSearch + 1]) == 'r' &&
						std::tolower(source[startSearch + 2]) == 'a' &&
						std::tolower(source[startSearch + 3]) == 'd' &&
						std::tolower(source[startSearch + 4]) == 'i' &&
						std::tolower(source[startSearch + 5]) == 'e' &&
						std::tolower(source[startSearch + 6]) == 'n' &&
						std::tolower(source[startSearch + 7]) == 't' &&
						(source[startSearch + 8] != '_' && (isspace(source[startSearch + 8]) || !isalnum(source[startSearch + 8]))))
					{
						addToken(TFTokenType::GRADIENT, line);
						currentIndex += 8;
						break;
					}
					if (std::tolower(source[startSearch + 1]) == 'r' &&
						std::tolower(source[startSearch + 2]) == 'a' &&
						std::tolower(source[startSearch + 3]) == 'i' &&
						std::tolower(source[startSearch + 4]) == 'n' &&
						(isspace(source[startSearch + 5]) || !isalnum(source[startSearch + 5])))
					{
						addToken(TFTokenType::GRAIN, line);
						currentIndex += 5;
						break;
					}
					else { goto identifier; }
					break;
				case 'i':
					if (std::tolower(source[startSearch + 1]) == 'm' &&
						std::tolower(source[startSearch + 2]) == 'a' &&
						std::tolower(source[startSearch + 3]) == 'g' &&
						std::tolower(source[startSearch + 4]) == 'e' &&
						(isspace(source[startSearch + 5]) || !isalnum(source[startSearch + 5])))
					{
						addToken(TFTokenType::IMAGE, line);
						currentIndex += 5;
						break;
					}
					if (std::tolower(source[startSearch + 1]) == 'd' &&
						std::tolower(source[startSearch + 2]) == '=')
					{
						addToken(TFTokenType::ID, line);
						currentIndex += 3;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'm':
					if (std::tolower(source[startSearch + 1]) == 'e' &&
						std::tolower(source[startSearch + 2]) == 's' &&
						std::tolower(source[startSearch + 3]) == 'h' &&
						(isspace(source[startSearch + 4]) || !isalnum(source[startSearch + 4])))
					{
						addToken(TFTokenType::MESH, line);
						currentIndex += 4;
						break;
					}
					if (std::tolower(source[startSearch + 1]) == 'e' &&
						std::tolower(source[startSearch + 2]) == 't' &&
						std::tolower(source[startSearch + 3]) == 'a' &&
						std::tolower(source[startSearch + 4]) == 'd' &&
						std::tolower(source[startSearch + 5]) == 'a' &&
						std::tolower(source[startSearch + 6]) == 't' &&
						std::tolower(source[startSearch + 7]) == 'a' &&
						(isspace(source[startSearch + 8]) || !isalnum(source[startSearch + 8])))
					{
						addToken(TFTokenType::METADATA, line);
						currentIndex += 8;
						break;
					}
					if (std::tolower(source[startSearch + 1]) == 'i' &&
						std::tolower(source[startSearch + 2]) == 'x' &&
						std::tolower(source[startSearch + 3]) == 'i' &&
						std::tolower(source[startSearch + 4]) == 'n' &&
						std::tolower(source[startSearch + 5]) == 'g' &&
						(isspace(source[startSearch + 6]) || !isalnum(source[startSearch + 6])))
					{
						addToken(TFTokenType::MIXING, line);
						currentIndex += 6;
						break;
					}
					if (std::tolower(source[startSearch + 1]) == 'u' &&
						std::tolower(source[startSearch + 2]) == 'l' &&
						std::tolower(source[startSearch + 3]) == 't' &&
						std::tolower(source[startSearch + 4]) == 'i' &&
						std::tolower(source[startSearch + 5]) == 't' &&
						std::tolower(source[startSearch + 6]) == 'i' &&
						std::tolower(source[startSearch + 7]) == 'p' &&
						(isspace(source[startSearch + 8]) || !isalnum(source[startSearch + 8])))
					{
						addToken(TFTokenType::MULTITIP, line);
						currentIndex += 8;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'o':
					if (std::tolower(source[startSearch + 1]) == 'n' &&
						std::tolower(source[startSearch + 2]) == 'e' &&
						std::tolower(source[startSearch + 3]) == 'p' &&
						std::tolower(source[startSearch + 4]) == 'o' &&
						std::tolower(source[startSearch + 5]) == 'i' &&
						std::tolower(source[startSearch + 6]) == 'n' &&
						std::tolower(source[startSearch + 7]) == 't' &&
						(isspace(source[startSearch + 8]) || !isalnum(source[startSearch + 8])))
					{
						addToken(TFTokenType::ONEPOINT, line);
						currentIndex += 8;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'p':
					if (std::tolower(source[startSearch + 1]) == 'a' &&
						std::tolower(source[startSearch + 2]) == 'n' &&
						(isspace(source[startSearch + 3]) || !isalnum(source[startSearch + 3])))
					{
						addToken(TFTokenType::PAN, line);
						currentIndex += 3;
						break;
					}
					if (std::tolower(source[startSearch + 1]) == 'a' &&
						std::tolower(source[startSearch + 2]) == 't' &&
						std::tolower(source[startSearch + 3]) == 't' &&
						std::tolower(source[startSearch + 4]) == 'e' &&
						std::tolower(source[startSearch + 5]) == 'r' &&
						std::tolower(source[startSearch + 6]) == 'n' &&
						(isspace(source[startSearch + 7]) || !isalnum(source[startSearch + 7])))
					{
						addToken(TFTokenType::PATTERN, line);
						currentIndex += 7;
						break;
					}
					if (std::tolower(source[startSearch + 1]) == 'o' &&
						std::tolower(source[startSearch + 2]) == 'l' &&
						std::tolower(source[startSearch + 3]) == 'y' &&
						std::tolower(source[startSearch + 4]) == 'g' &&
						std::tolower(source[startSearch + 5]) == 'o' &&
						std::tolower(source[startSearch + 6]) == 'n' &&
						(isspace(source[startSearch + 7]) || !isalnum(source[startSearch + 7])))
					{
						addToken(TFTokenType::POLYGON, line);
						currentIndex += 7;
						break;
					}
					if (std::tolower(source[startSearch + 1]) == 'o' &&
						std::tolower(source[startSearch + 2]) == 'l' &&
						std::tolower(source[startSearch + 3]) == 'y' &&
						std::tolower(source[startSearch + 4]) == 'l' &&
						std::tolower(source[startSearch + 5]) == 'i' &&
						std::tolower(source[startSearch + 6]) == 'n' &&
						std::tolower(source[startSearch + 7]) == 'e' &&
						(isspace(source[startSearch + 8]) || !isalnum(source[startSearch + 8])))
					{
						addToken(TFTokenType::POLYLINE, line);
						currentIndex += 8;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'r':
					if (std::tolower(source[startSearch + 1]) == 'a' &&
						std::tolower(source[startSearch + 2]) == 'k' &&
						std::tolower(source[startSearch + 3]) == 'e' &&
						(isspace(source[startSearch + 4]) || !isalnum(source[startSearch + 4])))
					{
						addToken(TFTokenType::RAKE, line);
						currentIndex += 4;
						break;
					}
					if (std::tolower(source[startSearch + 1]) == 'e' &&
						std::tolower(source[startSearch + 2]) == 'f' &&
						std::tolower(source[startSearch + 3]) == 'e' &&
						std::tolower(source[startSearch + 4]) == 'r' &&
						std::tolower(source[startSearch + 5]) == 'e' &&
						std::tolower(source[startSearch + 6]) == 'n' &&
						std::tolower(source[startSearch + 7]) == 'c' &&
						std::tolower(source[startSearch + 8]) == 'e' &&
						(isspace(source[startSearch + 9]) || !isalnum(source[startSearch + 9])))
					{
						addToken(TFTokenType::REFERENCE, line);
						currentIndex += 9;
						break;
					}
					if (std::tolower(source[startSearch + 1]) == 'e' &&
						std::tolower(source[startSearch + 2]) == 'p' &&
						std::tolower(source[startSearch + 3]) == 'e' &&
						std::tolower(source[startSearch + 4]) == 'a' &&
						std::tolower(source[startSearch + 5]) == 't' &&
						(isspace(source[startSearch + 6]) || !isalnum(source[startSearch + 6])))
					{
						addToken(TFTokenType::REPEAT, line);
						currentIndex += 6;
						break;
					}
					if (std::tolower(source[startSearch + 1]) == 'o' &&
						std::tolower(source[startSearch + 2]) == 't' &&
						std::tolower(source[startSearch + 3]) == 'a' &&
						std::tolower(source[startSearch + 4]) == 't' &&
						std::tolower(source[startSearch + 5]) == 'e' &&
						(isspace(source[startSearch + 6]) || !isalnum(source[startSearch + 6])))
					{
						addToken(TFTokenType::ROTATE, line);
						currentIndex += 6;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 's':
					if (std::tolower(source[startSearch + 1]) == 'a' &&
						std::tolower(source[startSearch + 2]) == 'm' &&
						std::tolower(source[startSearch + 3]) == 'p' &&
						std::tolower(source[startSearch + 4]) == 'l' &&
						std::tolower(source[startSearch + 5]) == 'e' &&
						std::tolower(source[startSearch + 6]) == 'r' &&
						(isspace(source[startSearch + 7]) || !isalnum(source[startSearch + 7])))
					{
						addToken(TFTokenType::SAMPLER, line);
						currentIndex += 7;
						break;
					}
					if (std::tolower(source[startSearch + 1]) == 'c' &&
						std::tolower(source[startSearch + 2]) == 'a' &&
						std::tolower(source[startSearch + 3]) == 't' &&
						std::tolower(source[startSearch + 4]) == 't' &&
						std::tolower(source[startSearch + 5]) == 'e' &&
						std::tolower(source[startSearch + 6]) == 'r' &&
						(isspace(source[startSearch + 7]) || !isalnum(source[startSearch + 7])))
					{
						addToken(TFTokenType::SCATTER, line);
						currentIndex += 7;
						break;
					}
					if (std::tolower(source[startSearch + 1]) == 'i' &&
						std::tolower(source[startSearch + 2]) == 'd' &&
						std::tolower(source[startSearch + 3]) == '=')
					{
						addToken(TFTokenType::SID, line);
						currentIndex += 4;
						break;
					}
					if (std::tolower(source[startSearch + 1]) == 'h' &&
						std::tolower(source[startSearch + 2]) == 'a' &&
						std::tolower(source[startSearch + 3]) == 'd' &&
						std::tolower(source[startSearch + 4]) == 'e' &&
						std::tolower(source[startSearch + 5]) == 'r' &&
						(isspace(source[startSearch + 6]) || !isalnum(source[startSearch + 6])))
					{
						addToken(TFTokenType::SHADER, line);
						currentIndex += 6;
						break;
					}
					if (std::tolower(source[startSearch + 1]) == 'm' &&
						std::tolower(source[startSearch + 2]) == 'o' &&
						std::tolower(source[startSearch + 3]) == 'o' &&
						std::tolower(source[startSearch + 4]) == 't' &&
						std::tolower(source[startSearch + 5]) == 'h' &&
						std::tolower(source[startSearch + 6]) == 'i' &&
						std::tolower(source[startSearch + 7]) == 'n' &&
						std::tolower(source[startSearch + 8]) == 'g' &&
						(isspace(source[startSearch + 9]) || !isalnum(source[startSearch + 9])))
					{
						if (source[startSearch - 1] == ':') { goto identifier; }
						addToken(TFTokenType::SMOOTHING, line);
						currentIndex += 9;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 't':
					if (std::tolower(source[startSearch + 1]) == 'e' &&
						std::tolower(source[startSearch + 2]) == 'x' &&
						std::tolower(source[startSearch + 3]) == 't' &&
						std::tolower(source[startSearch + 4]) == 'u' &&
						std::tolower(source[startSearch + 5]) == 'r' &&
						std::tolower(source[startSearch + 6]) == 'e' &&
						(isspace(source[startSearch + 7]) || !isalnum(source[startSearch + 7])))
					{
						addToken(TFTokenType::TEXTURE, line);
						currentIndex += 7;
						break;
					}
					if (std::tolower(source[startSearch + 1]) == 'h' &&
						std::tolower(source[startSearch + 2]) == 'r' &&
						std::tolower(source[startSearch + 3]) == 'e' &&
						std::tolower(source[startSearch + 4]) == 'e' &&
						std::tolower(source[startSearch + 5]) == 'p' &&
						std::tolower(source[startSearch + 6]) == 'o' &&
						std::tolower(source[startSearch + 7]) == 'i' &&
						std::tolower(source[startSearch + 8]) == 'n' &&
						std::tolower(source[startSearch + 9]) == 't' &&
						(isspace(source[startSearch + 10]) || !isalnum(source[startSearch + 10])))
					{
						addToken(TFTokenType::THREEPOINT, line);
						currentIndex += 10;
						break;
					}
					if (std::tolower(source[startSearch + 1]) == 'w' &&
						std::tolower(source[startSearch + 2]) == 'o' &&
						std::tolower(source[startSearch + 3]) == 'p' &&
						std::tolower(source[startSearch + 4]) == 'o' &&
						std::tolower(source[startSearch + 5]) == 'i' &&
						std::tolower(source[startSearch + 6]) == 'n' &&
						std::tolower(source[startSearch + 7]) == 't' &&
						(isspace(source[startSearch + 8]) || !isalnum(source[startSearch + 8])))
					{
						addToken(TFTokenType::TWOPOINT, line);
						currentIndex += 8;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'v':
					if (std::tolower(source[startSearch + 1]) == 'e' &&
						std::tolower(source[startSearch + 2]) == 'c' &&
						std::tolower(source[startSearch + 3]) == 't' &&
						std::tolower(source[startSearch + 4]) == 'o' &&
						std::tolower(source[startSearch + 5]) == 'r' &&
						(isspace(source[startSearch + 6]) || !isalnum(source[startSearch + 6])))
					{
						addToken(TFTokenType::VECTOR, line);
						currentIndex += 6;
						break;
					}
					if (std::tolower(source[startSearch + 1]) == 'o' &&
						std::tolower(source[startSearch + 2]) == 'r' &&
						std::tolower(source[startSearch + 3]) == 't' &&
						std::tolower(source[startSearch + 4]) == 'e' &&
						std::tolower(source[startSearch + 5]) == 'x' &&
						(isspace(source[startSearch + 6]) || !isalnum(source[startSearch + 6])))
					{
						addToken(TFTokenType::VORTEX, line);
						currentIndex += 6;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'w':
					if (std::tolower(source[startSearch + 1]) == 'e' &&
						std::tolower(source[startSearch + 2]) == 't' &&
						std::tolower(source[startSearch + 3]) == 'e' &&
						std::tolower(source[startSearch + 4]) == 'd' &&
						std::tolower(source[startSearch + 5]) == 'g' &&
						std::tolower(source[startSearch + 6]) == 'e' &&
						std::tolower(source[startSearch + 7]) == 's' &&
						(isspace(source[startSearch + 8]) || !isalnum(source[startSearch + 8])))
					{
						addToken(TFTokenType::WETEDGES, line);
						currentIndex += 8;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'z':
					if (std::tolower(source[startSearch + 1]) == 'o' &&
						std::tolower(source[startSearch + 2]) == 'o' &&
						std::tolower(source[startSearch + 3]) == 'm' &&
						(isspace(source[startSearch + 4]) || !isalnum(source[startSearch + 4])))
					{
						addToken(TFTokenType::ZOOM, line);
						currentIndex += 4;
						break;
					}
					else { goto identifier; }
					break;
					//
				case '[':
					if (std::tolower(source[startSearch + 1]) == 'h' &&
						std::tolower(source[startSearch + 2]) == 'e' &&
						std::tolower(source[startSearch + 3]) == 'a' &&
						std::tolower(source[startSearch + 4]) == 'd' &&
						std::tolower(source[startSearch + 5]) == 'e' &&
						std::tolower(source[startSearch + 6]) == 'r' &&
						std::tolower(source[startSearch + 7]) == ']')
					{
						addToken(TFTokenType::HEADER, line);
						currentIndex += 8;
						break;
					}
					if (std::tolower(source[startSearch + 1]) == 'e' &&
						std::tolower(source[startSearch + 2]) == 'n' &&
						std::tolower(source[startSearch + 3]) == 'd' &&
						std::tolower(source[startSearch + 4]) == ']')
					{
						addToken(TFTokenType::ENDFILE, line);
						currentIndex += 5;
						break;
					}
					else { goto identifier; }
					break;
				case '_':
					goto identifier;
					break;
				default:
				identifier:
					advance = 0;
					while (startSearch + advance < source.size() &&
						(isalnum(source[startSearch + advance]) || source[startSearch + advance] == '_'))
					{
						advance++;
					}
					addToken(TFTokenType::STRING, source.substr(startSearch, advance), line);
					
					currentIndex += advance;
					break;
				}
				break;
			default:
				advance = 0;
				while (startSearch + advance < source.size() &&
					(isalnum(source[startSearch + advance]) || source[startSearch + advance] == '_'))
				{
					advance++;
				}
				addToken(TFTokenType::STRING, source.substr(startSearch, advance), line);
				currentIndex += advance;
				break;
			}
			continue;
		}
		// Whitespace, Strings, Numbers, Conditionals, Logic, Comments, and Operators
		switch (c)
		{
		case ' ': case '\r': case '\t': currentIndex++; break;
			//
		case '\n': line++; currentIndex++; break;
			//
		case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': case '-':
			decimalFound = false;
			advance = 0;
			while (
				startSearch + advance < source.size() && 
				(isdigit(source[startSearch + advance]) || 	source[startSearch + advance] == '.' || source[startSearch + advance] == '-'))
			{
				if (source[startSearch + advance] == '.')
				{
					if (decimalFound == false) { advance++; decimalFound = true; }
					else { break; }
				}
				else { advance++; }
			}
			addToken(TFTokenType::NUM, source.substr(startSearch, advance), line);
			currentIndex += advance;
			break;
			//
		case '{': addToken(TFTokenType::CURLY_LEFT, c, line); currentIndex++; break;
		case '}': addToken(TFTokenType::CURLY_RIGHT, c, line); currentIndex++; break;
		case ';': addToken(TFTokenType::SEMICOLON, c, line); currentIndex++; break;
		case '=': addToken(TFTokenType::ASSIGN, c, line); currentIndex++; break;
			//
		case ':':
			if (source[startSearch + 1] == ':') { addToken(TFTokenType::DOUBLE_COLON, line); currentIndex += 2; break; }
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
				if (find == std::string::npos) { console.get()->error(line, "[lexer:0101] Unclosed multi-line comment."); }
				currentIndex += (find - startSearch) + 2;
				line += countNewlines(source.substr(startSearch + 2, (find - 2) - startSearch));

				break;
			}
			//
		case '\"':
			find = source.find("\"", startSearch + 1);
			if (find == std::string::npos) { console.get()->error(line, "[lexer:0104] Unterminated string."); currentIndex = source.size(); break; }
			else
			{
				std::string val = source.substr(startSearch + 1, find - startSearch - 1);
				addToken(TFTokenType::STRING, val, line);
				line += countNewlines(val);
				currentIndex = find + 1;
				break;
			}
			//
		default:
			std::string err = "[lexer:0105] Unexcepted '";
			err.append({ c });
			err.append("' character.");

			console.get()->error(line, err);
		}
	}

	// Place end of input stream token
	tokens.emplace_back(std::make_shared<TFToken>(TFTokenType::END, int(line)));

	return tokens;
}

//
void TFLexer::addToken(TFTokenType type, size_t line)
{
	tokens.emplace_back(std::make_shared<TFToken>(type, int(line)));
}

//
void TFLexer::addToken(TFTokenType type, char lexeme, size_t line)
{
	std::string _lexeme = { lexeme };
	tokens.emplace_back(std::make_shared<TFToken>(type, _lexeme, int(line)));
}

//
void TFLexer::addToken(TFTokenType type, std::string lexeme, size_t line)
{
	tokens.emplace_back(std::make_shared<TFToken>(type, lexeme, int(line)));
}

//
int TFLexer::countNewlines(std::string val)
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