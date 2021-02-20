#ifndef SHADER_H
#define SHADER_H

#include <glm.hpp>

#include <iostream>

class Shader
{
private:
	std::string name;
	void checkCompileErrors(unsigned int shader, std::string type);
	std::string loadShaderFile(const char* shaderPath);
protected:
public:

	unsigned int ID;
	Shader(const char* vertexPath, const char* fragmentPath, std::string n);

	void use();
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setVec2(const std::string &name, glm::vec2 &value) const;
	void setVec3(const std::string &name, glm::vec3 &value) const;
	void setVec4(const std::string &name, glm::vec4 &value) const;
	void setMat4(const std::string &name, const glm::mat4 &mat) const;
};
#endif