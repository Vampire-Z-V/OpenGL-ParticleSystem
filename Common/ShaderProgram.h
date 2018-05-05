#pragma once

#include <glad\glad.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <string>

enum ShaderType
{
	COMPUTE,
	VERTEX,
	GEOMETRY,
	FRAGMENT
};

struct ShaderInfo
{
	const GLchar* shaderPath;
	ShaderType shaderType;
	// should place in order
	const GLchar** extraFilesPath;
	GLuint extraFilesNum;

	ShaderInfo(const GLchar* _shaderPath, ShaderType _shaderType) 
		: shaderPath(_shaderPath), shaderType(_shaderType), extraFilesPath(nullptr), extraFilesNum(0) {}
	ShaderInfo(const GLchar* _shaderPath, ShaderType _shaderType, const GLchar** _extraFilesPath, GLuint _extraFilesNum)
		: shaderPath(_shaderPath), shaderType(_shaderType), extraFilesNum(_extraFilesNum)
	{
		extraFilesPath = new const GLchar*[_extraFilesNum];
		for (int i = 0; i < _extraFilesNum; i++)
		{
			extraFilesPath[i] = _extraFilesPath[i];
		}
	}

	~ShaderInfo()
	{
		delete[] extraFilesPath;
	}
};

class ShaderProgram
{
public:
	// use nullptr in the end
	ShaderProgram(ShaderInfo* shaderInfo, ...);
	~ShaderProgram();


	void use();
	GLuint getProgramID();
	void setUniform(const std::string &name, bool value) const;
	void setUniform(const std::string &name, int value) const;
	void setUniform(const std::string &name, unsigned int value) const;
	void setUniform(const std::string &name, float value) const;
	void setUniform(const std::string &name, glm::vec3 value) const;
	void setUniform(const std::string &name, glm::mat4 value) const;

protected:
	void attachShader(const GLchar* shaderPath, ShaderType shaderType, std::string &extraCode);
	std::string getExtraFilesCode(const GLchar** extraFilesPath, GLuint extraFilesNum);
	std::string getShaderCode(const GLchar* shaderPath);
	GLuint compileShader(GLenum type, const GLchar* shaderCode);

private:
	unsigned int m_programID;
};

