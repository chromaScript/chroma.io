#include "include/Shader.h"

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// Constructors
Shader::Shader(const char* vertexPath, const char* fragmentPath, std::string n)
{
	name = n;
	//1. Retrieve the vertex/fragment source code from filePath
	std::string vertString = loadShaderFile(vertexPath);
	std::string fragString = loadShaderFile(fragmentPath);
	const char* vShaderCode = vertString.c_str();
	const char* fShaderCode = fragString.c_str();
	// 2. Compile shaders
	unsigned int vertex, fragment;
	// Build Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");
	// Build Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");
	// Build Shader Program and copy the Fragment and Vertex Shaders into it
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");
	// Delete the component shaders
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

// Check for compile errors
void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- ---------------------------------------------- -- " << std::endl;
		}
		else
		{
			std::cout << "SUCCESS::SHADER::" << name << "::" << type << " : IS COMPILED" << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- ---------------------------------------------- -- " << std::endl;
		}
		else
		{
			std::cout << "SUCCESS::SHADER::" << name << "::" << type << " : IS LINKED" << std::endl;
		}
	}
}

// Load the Shader file and convert to a string, with error handling
std::string Shader::loadShaderFile(const char* shaderPath)
{
	std::string shaderCode;
	std::ifstream shaderFile;
	// check that the ifstream objects can throw exceptions
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open the files
		shaderFile.open(shaderPath);
		std::stringstream shaderStream;

		// read file's buffer contents into streams
		shaderStream << shaderFile.rdbuf();

		// close file handlers
		shaderFile.close();

		// convert stream into string
		shaderCode = shaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		(void)e;
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ::" << shaderPath << std::endl;
	}
	return shaderCode;
}

// Swap openGL Program
void Shader::use()
{
	glUseProgram(ID);
}

// Utility Functions for Shader Uniforms
void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setVec2(const std::string &name, glm::vec2 &value) const
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), value.x, value.y);
}
void Shader::setVec3(const std::string &name, glm::vec3 &value) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
}
void Shader::setVec4(const std::string &name, glm::vec4 &value) const
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z, value.w);
}
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

