#include "../include/cscript/CLexer.h"
#include "../include/cscript/ChromaScript.h"
#include "../include/cscript/CEnums.h"
#include "../include/cscript/CToken.h"

#include <vector>
#include <string>
#include <memory>
#include <cctype>

//
CLexer::CLexer()
{

}

void CLexer::initialize(std::shared_ptr<ChromaScript> console)
{
	this->console = console;
}

//
std::vector<std::shared_ptr<CToken>> CLexer::scanTokens(std::string source)
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
		if (islower(c))
		{
			switch (c)
			{
			case 'a': case 'b': case 'c': case 'd': case 'e':
			case 'f': case 'i': case 'n': case 'p': case 'r':
			case 's': case 't': case 'v': case 'w':
				switch (c)
				{
				case 'a':
					if (source[startSearch + 1] == 'r' &&
						source[startSearch + 2] == 'r' &&
						source[startSearch + 3] == 'a' &&
						source[startSearch + 4] == 'y' &&
						(isspace(source[startSearch + 5]) || !isalnum(source[startSearch + 5])))
					{
						addToken(CTokenType::ARRAY, line);
						currentIndex += 5;
						break;
					}
					else { goto identifier; }
					break;
				case 'b':
					if (source[startSearch + 1] == 'i' &&
						source[startSearch + 2] == 'n' &&
						source[startSearch + 3] == 'd' &&
						(isspace(source[startSearch + 4]) || !isalnum(source[startSearch + 4])))
					{
						addToken(CTokenType::BIND, line);
						currentIndex += 4;
						break;
					}
					if (source[startSearch + 1] == 'o' &&
						source[startSearch + 2] == 'o' &&
						source[startSearch + 3] == 'l' &&
						(isspace(source[startSearch + 4]) || !isalnum(source[startSearch + 4])))
					{
						addToken(CTokenType::BOOL, line);
						currentIndex += 4;
						break;
					}
					if (source[startSearch + 1] == 'r' &&
						source[startSearch + 2] == 'e' &&
						source[startSearch + 3] == 'a' &&
						source[startSearch + 4] == 'k' &&
						(isspace(source[startSearch + 5]) || !isalnum(source[startSearch + 5])))
					{
						addToken(CTokenType::BREAK, line);
						currentIndex += 5;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'c':
					if (source[startSearch + 1] == 'a' &&
						source[startSearch + 2] == 's' &&
						source[startSearch + 3] == 't' &&
						(isspace(source[startSearch + 4]) || !isalnum(source[startSearch + 4])))
					{
						addToken(CTokenType::CAST, line);
						currentIndex += 4;
						break;
					}
					if (source[startSearch + 1] == 'a' &&
						source[startSearch + 2] == 't' &&
						source[startSearch + 3] == 'c' &&
						source[startSearch + 4] == 'h' &&
						(isspace(source[startSearch + 5]) || !isalnum(source[startSearch + 5])))
					{
						addToken(CTokenType::CATCH, line);
						currentIndex += 5;
						break;
					}
					if (source[startSearch + 1] == 'l' &&
						source[startSearch + 2] == 'a' &&
						source[startSearch + 3] == 's' &&
						source[startSearch + 4] == 's' &&
						(isspace(source[startSearch + 5]) || !isalnum(source[startSearch + 5])))
					{
						addToken(CTokenType::CLASS, line);
						currentIndex += 5;
						break;
					}
					if (source[startSearch + 1] == 'o' &&
						source[startSearch + 2] == 'n' &&
						source[startSearch + 3] == 't' &&
						source[startSearch + 4] == 'i' &&
						source[startSearch + 5] == 'n' &&
						source[startSearch + 6] == 'u' &&
						source[startSearch + 7] == 'e' &&
						(isspace(source[startSearch + 8]) || !isalnum(source[startSearch + 8])))
					{
						addToken(CTokenType::CONTINUE, line);
						currentIndex += 8;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'd':
					if (source[startSearch + 1] == 'e' &&
						source[startSearch + 2] == 'l' &&
						source[startSearch + 3] == 'e' &&
						source[startSearch + 4] == 't' &&
						source[startSearch + 5] == 'e' &&
						(isspace(source[startSearch + 6]) || !isalnum(source[startSearch + 6])))
					{
						addToken(CTokenType::_DELETE, line);
						currentIndex += 6;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'e':
					if (source[startSearch + 1] == 'l' &&
						source[startSearch + 2] == 's' &&
						source[startSearch + 3] == 'e' &&
						(isspace(source[startSearch + 4]) || !isalnum(source[startSearch + 4])))
					{
						addToken(CTokenType::ELSE, line);
						currentIndex += 4;
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
						(isspace(source[startSearch + 5]) || !isalnum(source[startSearch + 5])))
					{
						addToken(CTokenType::BOOL, "false", line);
						currentIndex += 5;
						break;
					}
					if (source[startSearch + 1] == 'o' &&
						source[startSearch + 2] == 'r' &&
						(isspace(source[startSearch + 3]) || !isalnum(source[startSearch + 3])))
					{
						addToken(CTokenType::FOR, line);
						currentIndex += 3;
						break;
					}
					if (source[startSearch + 1] == 'o' &&
						source[startSearch + 2] == 'r' &&
						source[startSearch + 3] == 'e' &&
						source[startSearch + 4] == 'a' &&
						source[startSearch + 5] == 'c' &&
						source[startSearch + 6] == 'h' &&
						(isspace(source[startSearch + 7]) || !isalnum(source[startSearch + 7])))
					{
						addToken(CTokenType::FOREACH, line);
						currentIndex += 7;
						break;
					}
					if (source[startSearch + 1] == 'u' &&
						source[startSearch + 2] == 'n' &&
						source[startSearch + 3] == 'c' &&
						source[startSearch + 4] == 't' &&
						source[startSearch + 5] == 'i' &&
						source[startSearch + 6] == 'o' &&
						source[startSearch + 7] == 'n' &&
						(isspace(source[startSearch + 8]) || !isalnum(source[startSearch + 8])))
					{
						addToken(CTokenType::FUNCTION, line);
						currentIndex += 8;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'i':
					if (source[startSearch + 1] == 'f')
					{
						addToken(CTokenType::IF, line);
						currentIndex += 2;
						break;
					}
					if (source[startSearch + 1] == 'n' &&
						source[startSearch + 2] == 'h' &&
						source[startSearch + 3] == 'e' &&
						source[startSearch + 4] == 'r' &&
						source[startSearch + 5] == 'i' &&
						source[startSearch + 6] == 't' &&
						(isspace(source[startSearch + 7]) || !isalnum(source[startSearch + 7])))
					{
						addToken(CTokenType::INHERIT, line);
						currentIndex += 7;
						break;
					}
					else { goto identifier; }
					break;
				case 'n':
					if (source[startSearch + 1] == 'a' &&
						source[startSearch + 2] == 'm' &&
						source[startSearch + 3] == 'e' &&
						source[startSearch + 4] == 's' &&
						source[startSearch + 5] == 'p' &&
						source[startSearch + 6] == 'a' &&
						source[startSearch + 7] == 'c' &&
						source[startSearch + 8] == 'e' &&
						(isspace(source[startSearch + 9]) || !isalnum(source[startSearch + 9])))
					{
						addToken(CTokenType::NAMESPACE, line);
						currentIndex += 9;
						break;
					}
					if (source[startSearch + 1] == 'u' &&
						source[startSearch + 2] == 'm' &&
						(isspace(source[startSearch + 3]) || !isalnum(source[startSearch + 3])))
					{
						addToken(CTokenType::NUM, line);
						currentIndex += 3;
						break;
					}
					if (source[startSearch + 1] == 'i' &&
						source[startSearch + 2] == 'l' &&
						(isspace(source[startSearch + 3]) || !isalnum(source[startSearch + 3])))
					{
						addToken(CTokenType::NIL, line);
						currentIndex += 3;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'p':
					if (source[startSearch + 1] == 'r' &&
						source[startSearch + 2] == 'i' &&
						source[startSearch + 3] == 'n' &&
						source[startSearch + 4] == 't' &&
						(isspace(source[startSearch + 5]) || !isalnum(source[startSearch + 5])))
					{
						addToken(CTokenType::PRINT, line);
						currentIndex += 5;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'r':
					if (source[startSearch + 1] == 'e' &&
						source[startSearch + 2] == 't' &&
						source[startSearch + 3] == 'u' &&
						source[startSearch + 4] == 'r' &&
						source[startSearch + 5] == 'n' &&
						(isspace(source[startSearch + 6]) || !isalnum(source[startSearch + 6])))
					{
						addToken(CTokenType::RETURN, line);
						currentIndex += 6;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 's':
					if (source[startSearch + 1] == 't' &&
						source[startSearch + 2] == 'r' &&
						source[startSearch + 3] == 'i' &&
						source[startSearch + 4] == 'n' &&
						source[startSearch + 5] == 'g' &&
						(isspace(source[startSearch + 6]) || !isalnum(source[startSearch + 6])))
					{
						addToken(CTokenType::STRING, line);
						currentIndex += 6;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 't':
					if (source[startSearch + 1] == 'h' &&
						source[startSearch + 2] == 'i' &&
						source[startSearch + 3] == 's' &&
						(isspace(source[startSearch + 4]) || !isalnum(source[startSearch + 4])))
					{
						addToken(CTokenType::_THIS, line);
						currentIndex += 4;
						break;
					}
					if (source[startSearch + 1] == 'r' &&
						source[startSearch + 2] == 'u' &&
						source[startSearch + 3] == 'e' &&
						(isspace(source[startSearch + 4]) || !isalnum(source[startSearch + 4])))
					{
						addToken(CTokenType::BOOL, "true", line);
						currentIndex += 4;
						break;
					}
					if (source[startSearch + 1] == 'r' &&
						source[startSearch + 2] == 'y' &&
						(isspace(source[startSearch + 3]) || !isalnum(source[startSearch + 3])))
					{
						addToken(CTokenType::TRY, line);
						currentIndex += 3;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'v':
					if (source[startSearch + 1] == 'e' &&
						source[startSearch + 2] == 'c' &&
						(isdigit(source[startSearch + 3]) && (isspace(source[startSearch + 4]) || !isalnum(source[startSearch + 4]))))
					{
						switch (source[startSearch + 3])
						{
						case '2':
							addToken(CTokenType::VECTOR2, line);
							currentIndex += 4;
							break;
						case '3':
							addToken(CTokenType::VECTOR3, line);
							currentIndex += 4;
							break;
						case '4':
							addToken(CTokenType::VECTOR4, line);
							currentIndex += 4;
							break;
						default:
							goto identifier;
							break;
						}
						break;
					}
					if (source[startSearch + 1] == 'o' &&
						source[startSearch + 2] == 'i' &&
						source[startSearch + 3] == 'd' &&
						(isspace(source[startSearch + 4]) || !isalnum(source[startSearch + 4])))
					{
						addToken(CTokenType::_VOID, line);
						currentIndex += 4;
						break;
					}
					else { goto identifier; }
					break;
					//
				case 'w':
					if (source[startSearch + 1] == 'h' &&
						source[startSearch + 2] == 'i' &&
						source[startSearch + 3] == 'l' &&
						source[startSearch + 4] == 'e' &&
						(isspace(source[startSearch + 5]) || !isalnum(source[startSearch + 5])))
					{
						addToken(CTokenType::WHILE, line);
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
						(isalnum(source[startSearch + advance]) || source[startSearch + advance] == '_'))
					{
						advance++;
					}
					addToken(CTokenType::IDENTIFIER, source.substr(startSearch, advance), line);
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
				addToken(CTokenType::IDENTIFIER, source.substr(startSearch, advance), line);
				currentIndex += advance;
				break;
			}
			continue;
		}
		else if (isupper(c))
		{
			size_t advance = 0;
			while (startSearch + advance < source.size() &&
				(isalnum(source[startSearch + advance]) || source[startSearch + advance] == '_'))
			{
				advance++;
			}
			addToken(CTokenType::IDENTIFIER, source.substr(startSearch, advance), line);
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
		case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
			decimalFound = false;
			advance = 0;
			while (startSearch + advance < source.size() && (isdigit(source[startSearch + advance]) || source[startSearch + advance] == '.'))
			{
				if (source[startSearch + advance] == '.')
				{
					if (decimalFound == false) { advance++; decimalFound = true; }
					else { break; }
				}
				else { advance++; }
			}
			addToken(CTokenType::NUM, source.substr(startSearch, advance), line);
			currentIndex += advance;
			break;
			//
		case '(': addToken(CTokenType::PAREN_LEFT, c, line); currentIndex++; break;
		case ')': addToken(CTokenType::PAREN_RIGHT, c, line); currentIndex++; break;
		case '{': addToken(CTokenType::CURLY_LEFT, c, line); currentIndex++; break;
		case '}': addToken(CTokenType::CURLY_RIGHT, c, line); currentIndex++; break;
		case '[': addToken(CTokenType::SQUARE_LEFT, c, line); currentIndex++; break;
		case ']': addToken(CTokenType::SQUARE_RIGHT, c, line); currentIndex++; break;
		case '.': addToken(CTokenType::DOT, c, line); currentIndex++; break;
		case ',': addToken(CTokenType::COMMA, c, line); currentIndex++; break;
		case ';': addToken(CTokenType::SEMICOLON, c, line); currentIndex++; break;
			//
		case '+':
			if (source[startSearch + 1] == '+') { addToken(CTokenType::INCREMENT, line); currentIndex += 2; break; }
			else if (source[startSearch + 1] == '=') { addToken(CTokenType::ASSIGN_ADD, line); currentIndex += 2; break; }
			else { addToken(CTokenType::PLUS, c, line); currentIndex++; break; }
		case '-':
			if (source[startSearch + 1] == '-') { addToken(CTokenType::DECREMENT, line); currentIndex += 2; break; }
			else if (source[startSearch + 1] == '=') { addToken(CTokenType::ASSIGN_SUBTRACT, line); currentIndex += 2; break; }
			else if (isspace(source[startSearch + 1])) { addToken(CTokenType::MINUS, c, line); currentIndex++; break; }
			else { addToken(CTokenType::NEGATE, c, line); currentIndex++; break; }
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
			else if (source[startSearch + 1] == '=') { addToken(CTokenType::ASSIGN_DIVIDE, line); currentIndex += 2; break; }
			else { addToken(CTokenType::DIVIDE, line); currentIndex++; break; }
		case '*':
			if (source[startSearch + 1] == '*') { addToken(CTokenType::POWER, line); currentIndex += 2; break; }
			else if (source[startSearch + 1] == '=') { addToken(CTokenType::ASSIGN_MULTIPLY, line); currentIndex += 2; break; }
			else { addToken(CTokenType::MULTIPLY, c, line); currentIndex++; break; }
			//
		case '%': addToken(CTokenType::MODULO, line); currentIndex++;  break;
			//
		case '!':
			if (source[startSearch + 1] == '=') { addToken(CTokenType::NOT_EQUAL, line); currentIndex += 2; break; }
			else { addToken(CTokenType::FALSIFY, line); currentIndex++; break; }
		case '?': addToken(CTokenType::VALIDATE, line); currentIndex++; break; 
		case ':':
			if (source[startSearch + 1] == ':') { addToken(CTokenType::SCOPE, line); currentIndex += 2; break; }
			else { addToken(CTokenType::COLON, line); currentIndex++; break; }
			//
		case '>':
			if (source[startSearch + 1] == '=') { addToken(CTokenType::MORE_EQUAL, line); currentIndex += 2; break; }
			else { addToken(CTokenType::MORE, line); currentIndex++; break; }
		case '<':
			if (source[startSearch + 1] == '=') { addToken(CTokenType::LESS_EQUAL, line); currentIndex += 2; break; }
			else { addToken(CTokenType::LESS, line); currentIndex++; break; }
		case '=':
			if (source[startSearch + 1] == '=') { addToken(CTokenType::EQUAL, line); currentIndex += 2; break; }
			else { addToken(CTokenType::ASSIGN, line); currentIndex++; break; }
		case '|':
			if (source[startSearch + 1] == '|') { addToken(CTokenType::LOGIC_OR, line); currentIndex += 2; break; }
			else { console.get()->error(line, "[lexer:0102] Unexpected '|' character."); currentIndex++; break; }
		case '&':
			if (source[startSearch + 1] == '&') { addToken(CTokenType::LOGIC_AND, line); currentIndex += 2; break; }
			else { console.get()->error(line, "[lexer:0103] Unexpected '&' character."); currentIndex++; break; }
		case '~':
			if (source[startSearch + 1] == '=') { addToken(CTokenType::APPROXIMATE, line); currentIndex += 2; break; }
			else { console.get()->error(line, "Unexpected '~' character."); currentIndex++; break; }
			//
		case '\"':
			find = source.find("\"", startSearch + 1);
			if (find == std::string::npos) { console.get()->error(line, "[lexer:0104] Unterminated string."); currentIndex = source.size(); break; }
			else
			{
				std::string val = source.substr(startSearch + 1, find - startSearch - 1);
				addToken(CTokenType::STRING, val, line);
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
	tokens.emplace_back(std::make_shared<CToken>(CTokenType::END, int(line)));

	return tokens;
}

//
void CLexer::addToken(CTokenType type, size_t line)
{
	tokens.emplace_back(std::make_shared<CToken>(type, int(line)));
}

//
void CLexer::addToken(CTokenType type, char lexeme, size_t line)
{
	std::string _lexeme = { lexeme };
	tokens.emplace_back(std::make_shared<CToken>(type, _lexeme, int(line)));
}

//
void CLexer::addToken(CTokenType type, std::string lexeme, size_t line)
{
	tokens.emplace_back(std::make_shared<CToken>(type, lexeme, int(line)));
}

//
int CLexer::countNewlines(std::string val)
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