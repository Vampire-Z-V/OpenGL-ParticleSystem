#include "ShaderProgram.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;


ShaderProgram::ShaderProgram(ShaderInfo* shaderInfo, ...)
{
	m_programID = glCreateProgram();

	// attach shaders
	ShaderInfo* info = shaderInfo;
	va_list args;
	va_start(args, shaderInfo);
	do
	{
		string extraCode;
		if (info->extraFilesNum != 0)
		{
			cout << "Attach extra files..." << endl;
			extraCode = getExtraFilesCode(info->extraFilesPath, info->extraFilesNum);
			cout << info->extraFilesNum << " files has been attached to ";
		}

		cout << info->shaderPath << endl << endl;
		attachShader(info->shaderPath, info->shaderType, extraCode);
	} while (info = va_arg(args, ShaderInfo*));
	va_end(args);

	// link program
	int success;
	char infoLog[512];
	glLinkProgram(m_programID);
	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_programID, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
	}
}

ShaderProgram::~ShaderProgram()
{
	glUseProgram(0);
	glDeleteProgram(m_programID);
}

void ShaderProgram::use()
{
	glUseProgram(m_programID);
}

GLuint ShaderProgram::getProgramID()
{
	return m_programID;
}

void ShaderProgram::setUniform(const string & name, bool value) const
{
	glUniform1i(glGetUniformLocation(m_programID, name.c_str()), (int)value);
}

void ShaderProgram::setUniform(const string & name, int value) const
{
	glUniform1i(glGetUniformLocation(m_programID, name.c_str()), value);
}
void ShaderProgram::setUniform(const std::string &name, unsigned int value) const
{
	glUniform1ui(glGetUniformLocation(m_programID, name.c_str()), value);
}

void ShaderProgram::setUniform(const string & name, float value) const
{
	glUniform1f(glGetUniformLocation(m_programID, name.c_str()), value);
}

void ShaderProgram::setUniform(const string & name, glm::vec3 value) const
{
	glUniform3fv(glGetUniformLocation(m_programID, name.c_str()), 1, glm::value_ptr(value));
}

void ShaderProgram::setUniform(const string & name, glm::mat4 value) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::attachShader(const GLchar* shaderPath, ShaderType shaderType, string &extraCode)
{
	string shaderCode = getShaderCode(shaderPath);

	GLenum type;
	switch (shaderType)
	{
	case COMPUTE:
		type = GL_COMPUTE_SHADER;
		break;
	case VERTEX:
		type = GL_VERTEX_SHADER;
		break;
	case GEOMETRY:
		type = GL_GEOMETRY_SHADER;
		break;
	case FRAGMENT:
		type = GL_FRAGMENT_SHADER;
		break;
	default:
		cout << "UNKNOWN_SHADETYPE" << endl;
		break;
	}

	string code = extraCode + shaderCode;
	//cout << code << endl;
	unsigned int shader = compileShader(type, code.c_str());
	glAttachShader(m_programID, shader);

	glDeleteShader(shader);
}

string ShaderProgram::getExtraFilesCode(const GLchar** extraFilesPath, GLuint extraFilesNum)
{
	string extraCode;

	for (int i = 0; i < extraFilesNum; i++)
	{
		ifstream extraFile;
		extraFile.exceptions(ifstream::failbit | ifstream::badbit);

		try
		{
			extraFile.open(extraFilesPath[i]);

			cout << '\t' << extraFilesPath[i] << endl;
			stringstream shaderStream;
			shaderStream << extraFile.rdbuf();

			extraFile.close();

			extraCode += shaderStream.str();
		}
		catch (ifstream::failure e)
		{
			cout << "ERROR::EXTRA_FILE::FILE_NOT_SUCCESFULLY_READ" << endl;
		}
	}

	return extraCode;
}

string ShaderProgram::getShaderCode(const GLchar * shaderPath)
{
	string shaderCode;
	ifstream shaderFile;

	shaderFile.exceptions(ifstream::failbit | ifstream::badbit);

	try
	{
		shaderFile.open(shaderPath);

		stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();

		shaderFile.close();

		shaderCode = shaderStream.str();
	}
	catch (ifstream::failure e)
	{
		cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
	}

	return shaderCode;
}

GLuint ShaderProgram::compileShader(GLenum type, const GLchar* shaderCode)
{
	unsigned int shader;
	int success;
	char infoLog[512];

	shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	}

	return shader;
}

