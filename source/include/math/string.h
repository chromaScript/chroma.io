#ifndef STRINGFUNC_H
#define STRINGFUNC_H

#include <glm.hpp>

#include <deque>
#include <vector>
#include <memory>
#include <filesystem>

#include <iostream>
#include <fstream>
#include <sstream>
#include <Ole2.h>

// Note: These structs should only contain generic data types, 
// including class headers here creates circular dependencies and usually won't compile

// Useful macros
#define _SIZE_T static_cast<size_t>

std::string ConvertWCSToMBS(const wchar_t* pstr, long wslen);
std::string ConvertBSTRToMBS(BSTR bstr);

std::string stringRemoveSpace(std::string& input);
bool compareChar(char in, char comparator);
std::string stringRemoveChar(std::string& input, char del);
double stringToNum(std::string string);
std::string stringToLower(std::string& input);
std::string stringToUpper(std::string& input);
std::string stringNumberOnly(std::string& input);
std::string stringNumberOnly_truncate(std::string& input, int decimals);

bool readFileToString(std::string& str, std::filesystem::path path);

std::string vec2ToString(glm::vec2 vector);
std::string vec3ToString(glm::vec3 vector);
std::string vec4ToString(glm::vec4 vector);
std::string floatVecToString(std::vector<float> vector);
std::string floatArrayToString(float* vector, int size);
std::string intVecToString(std::vector<int> vector);
std::string stringVecToString(std::vector<std::string> vector, bool addBrackets);
std::string vec3VecToString(std::vector<glm::vec3> vector);
std::string vec2VecToString(std::vector<glm::vec2> vector);

std::string sliceRegularString(std::string& str, std::string delimStart, std::string delimEnd);
std::string splitRegularString(std::string& str, std::string delim, bool removeDelim);

std::vector<std::string> stringVec_fromCommaSeparated(std::string input, bool removeWhitespace);

void printStr(std::string str);

#endif // !STRINGFUNC_H
