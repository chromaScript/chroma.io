#include "../include/math/string.h"

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

////////////////////////////////////////////////////////////////
//
// WCHAR & BSTR CHAR // OLE2_H
//
////////////////////////////////////////////////////////////////

std::string ConvertWCSToMBS(const wchar_t* pstr, long wslen)
{
	int len = ::WideCharToMultiByte(CP_ACP, 0, pstr, wslen, NULL, 0, NULL, NULL);

	std::string dblstr(len, '\0');
	len = ::WideCharToMultiByte(CP_ACP, 0 /* no flags */,
		pstr, wslen /* not necessary NULL-terminated */,
		&dblstr[0], len,
		NULL, NULL /* no default char */);

	return dblstr;
}

std::string ConvertBSTRToMBS(BSTR bstr)
{
	int wslen = ::SysStringLen(bstr);
	return ConvertWCSToMBS((wchar_t*)bstr, wslen);
}

////////////////////////////////////////////////////////////////
//
// STRING CONVERSION
//
////////////////////////////////////////////////////////////////

std::string stringRemoveSpace(std::string& input)
{
	input.erase(remove_if(input.begin(), input.end(), isspace), input.end());
	return input;
}
bool compareChar(char in, char comparator)
{
	if (in == comparator) { return true; }
	else { return false; }
}
std::string stringRemoveChar(std::string& input, char del)
{
	input.erase(remove(input.begin(), input.end(), del), input.end());
	return input;
}

double stringToNum(std::string string)
{
	double outVal = 0.0;
	for (char c : string)
	{
		outVal += (double)c;
	}
	return outVal;
}

std::string stringToLower(std::string& input)
{
	for (int i = 0; i < input.size(); i++)
	{
		input[i] = std::tolower(input[i]);
	}
	return input;
}

std::string stringToUpper(std::string& input)
{
	for (int i = 0; i < input.size(); i++)
	{
		input[i] = std::toupper(input[i]);
	}
	return input;
}

std::string stringNumberOnly(std::string& input)
{
	for (int i = 0; i < input.size(); i++)
	{
		if (!isdigit(input[i]) && input[i] != '.' && input[i] != '-')
		{
			input[i] = ' ';
		}
	}
	input.erase(remove_if(input.begin(), input.end(), isspace), input.end());
	return input;
}

std::string stringNumberOnly_truncate(std::string& input, int decimals)
{
	input = stringNumberOnly(input);
	size_t decLoc = input.find_first_of('.');
	if (decLoc == std::string::npos) { return input; }
	switch (decimals)
	{
	case -1: return input; break;
	case 0: input = input.substr(0, decLoc); return input; break;
	default: input = input.substr(0, decLoc + 1 + decimals); return input; break;
	}
}

////////////////////////////////////////////////////////////////
//
// FILE HANDLING
//
////////////////////////////////////////////////////////////////

bool readFileToString(std::string& str, std::filesystem::path path)
{
	std::ifstream file;
	// check that the ifstream object can throw exceptions
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// Open the file
		file.open(path);
		std::stringstream fileStream;
		// Read file's buffer contents into the stream
		fileStream << file.rdbuf();
		// Convert the stream into a string
		str = fileStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		(void)e;
		std::cout << "ERROR::UTILITY::READFILETOSTRING::FAILED_TO_STREAM_FILE::" << path << std::endl;
		return false;
	}
	return true;
}

////////////////////////////////////////////////////////////////
//
// VECTOR TO STRING
//
////////////////////////////////////////////////////////////////

std::string vec2ToString(glm::vec2 vector)
{
	return "{ " +
		std::to_string(vector.x) + ", " +
		std::to_string(vector.y) + " }";
}
std::string vec3ToString(glm::vec3 vector)
{
	return "{ " +
		std::to_string(vector.x) + ", " +
		std::to_string(vector.y) + ", " +
		std::to_string(vector.z) + " }";
}
std::string vec4ToString(glm::vec4 vector)
{
	return "{ " +
		std::to_string(vector.x) + ", " +
		std::to_string(vector.y) + ", " +
		std::to_string(vector.z) + ", " +
		std::to_string(vector.w) + " }";
}
std::string floatVecToString(std::vector<float> vector)
{
	std::string out = "{ ";
	size_t i = vector.size();
	for (size_t k = 0; k < i; k++)
	{
		out += " " + std::to_string(vector[k]) + ",";
	}
	out.pop_back();
	out += " }";
	return out;
}
std::string floatArrayToString(float* vector, int size)
{
	std::string out = "{ ";
	size_t i = size;
	for (size_t k = 0; k < i; k++)
	{
		out += " " + std::to_string(vector[k]) + ",";
	}
	out.pop_back();
	out += " }";
	return out;
}
std::string intVecToString(std::vector<int> vector)
{
	std::string out = "{ ";
	size_t i = vector.size();
	for (size_t k = 0; k < i; k++)
	{
		out += " " + std::to_string(vector[k]) + ",";
	}
	out.pop_back();
	out += " }";
	return out;
}
std::string stringVecToString(std::vector<std::string> vector, bool addBrackets)
{
	std::string out = "";

	if (addBrackets)
	{
		out += "{ ";
		size_t i = vector.size();
		for (size_t k = 0; k < i; k++)
		{
			out += vector[k] + ",";
		}
		out.pop_back();
		out += " }";
		return out;
	}
	else
	{
		size_t i = vector.size();
		for (size_t k = 0; k < i; k++)
		{
			out += vector[k];
			if (k != vector.size() - 1)
			{
				out += ", ";
			}
		}
		return out;
	}
}
std::string vec3VecToString(std::vector<glm::vec3> vector)
{
	std::string out = "{ ";
	size_t i = vector.size();
	for (size_t k = 0; k < i; k++)
	{
		out += std::to_string(k + 1) + "=[" + std::to_string(vector[k].x) + ", " + std::to_string(vector[k].y) + ", " + std::to_string(vector[k].z) + "] ";
	}
	out += " }";
	return out;
}
std::string vec2VecToString(std::vector<glm::vec2> vector)
{
	std::string out = "{ ";
	size_t i = vector.size();
	for (size_t k = 0; k < i; k++)
	{
		out += std::to_string(k + 1) + "=[" + std::to_string(vector[k].x) + ", " + std::to_string(vector[k].y) + "] ";
	}
	out += " }";
	return out;
}
std::string sliceRegularString(std::string& str, std::string delimStart, std::string delimEnd)
{
	size_t start;
	size_t delimStart_size = delimStart.size();
	//size_t end = 0;
	size_t slice;
	std::string out;
	while ((start = str.find(delimStart, 0)) != std::string::npos)
	{
		str = str.substr(start + delimStart_size, std::string::npos);
		slice = str.find_first_of(delimEnd);
		out = str.substr(0, slice);
		str = str.substr(slice + 1, std::string::npos);
	}
	return out;
}
std::string splitRegularString(std::string& str, std::string delim, bool removeDelim)
{
	size_t splitAt;
	std::string out;
	splitAt = str.find(delim, 0);
	out = str.substr(0, splitAt);
	if (removeDelim) {
		str = str.substr(splitAt + 1, std::string::npos);
	}
	else {
		str = str.substr(splitAt, std::string::npos);
	}
	return out;
}
std::vector<std::string> stringVec_fromCommaSeparated(std::string input, bool removeWhitespace)
{
	std::string in = input;
	std::vector<std::string> out;
	while (in.size() != 0)
	{
		if (in.find(',') != std::string::npos)
		{
			out.push_back(splitRegularString(in, ",", true));
		}
		else
		{
			out.push_back(in);
			in.clear();
		}
	}
	if (removeWhitespace)
	{
		for (std::string str : out)
		{
			stringRemoveSpace(str);
		}
	}
	return out;
}

void printStr(std::string str)
{
	bool allowPrint = true;
	if (allowPrint)
	{
		std::cout << str << std::endl;
	}
}