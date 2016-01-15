﻿#include "Shader.h"


Shader::Shader()
{

}

Shader::Shader(const char* pathVertex, const char* pathGeometry, const char* pathFragment)
{
	LoadFromFile(pathVertex, pathGeometry, pathFragment);
}

bool Shader::LoadFromFile(const char *pathVertex, const char *pathGeometry, const char *pathFragment)
{
	mVertexShaderObj = glCreateShader(GL_VERTEX_SHADER);
	if (pathGeometry)
		mGeometryShaderObj = glCreateShader(GL_GEOMETRY_SHADER);
	mFragmentShaderObj = glCreateShader(GL_FRAGMENT_SHADER);

	mProgram = glCreateProgram();

	std::ifstream fileVer;
	std::ifstream fileGeo;
	std::ifstream fileFrag;

	fileVer.open(pathVertex);
	if (fileVer.is_open())
	{
		std::string buffer;

		while (fileVer.good())
		{
			std::getline(fileVer, buffer);
			mVertexSource.append(buffer + "\n");

		}

		fileVer.close();
	}
	else
	{
		std::cout << "Cannot open shader file: " << pathVertex << std::endl;
		return false;
	}

	if (pathGeometry){
		fileGeo.open(pathGeometry);
		if (fileGeo.is_open())
		{
			std::string buffer;

			while (fileGeo.good())
			{
				std::getline(fileGeo, buffer);
				mGeometrySource.append(buffer + "\n");

			}

			fileGeo.close();
		}
		else
		{
			std::cout << "Cannot open shader file: " << pathGeometry << std::endl;
			return false;
		}
	}

	fileFrag.open(pathFragment);
	if (fileFrag.is_open())
	{
		std::string buffer;

		while (fileFrag.good())
		{
			getline(fileFrag, buffer);
			mFragmentSource.append(buffer + "\n");
		}

		fileFrag.close();
	}
	else
	{
		std::cout << "Cannot open shader file: " << pathFragment << std::endl;
		return false;
	}

	const char *vP = mVertexSource.c_str();
	const char *vG = pathGeometry ? mGeometrySource.c_str() : NULL;
	const char *vF = mFragmentSource.c_str();

	glShaderSource(mVertexShaderObj, 1, &vP, NULL);
	if (pathGeometry)
		glShaderSource(mGeometryShaderObj, 1, &vG, NULL);
	glShaderSource(mFragmentShaderObj, 1, &vF, NULL);

	//assert(mVertexShaderObj != 4);

	glCompileShader(mVertexShaderObj);

	GLint Result;
	GLint InfoLogLength;

	glGetShaderiv(mVertexShaderObj, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(mVertexShaderObj, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> VertexShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(mVertexShaderObj, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

	if (pathGeometry){
		glCompileShader(mGeometryShaderObj);
		glGetShaderiv(mFragmentShaderObj, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(mFragmentShaderObj, GL_INFO_LOG_LENGTH, &InfoLogLength);
		VertexShaderErrorMessage = std::vector<char>(InfoLogLength);
		glGetShaderInfoLog(mFragmentShaderObj, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);
	}
	glCompileShader(mFragmentShaderObj);
	glGetShaderiv(mFragmentShaderObj, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(mFragmentShaderObj, GL_INFO_LOG_LENGTH, &InfoLogLength);
	VertexShaderErrorMessage = std::vector<char>(InfoLogLength);
	glGetShaderInfoLog(mFragmentShaderObj, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

	glAttachShader(mProgram, mVertexShaderObj);
	if (pathGeometry)
		glAttachShader(mProgram, mGeometryShaderObj);
	glAttachShader(mProgram, mFragmentShaderObj);

	glLinkProgram(mProgram);

	glDeleteShader(mVertexShaderObj);
	if (pathGeometry)
		glDeleteShader(mGeometryShaderObj);
	glDeleteShader(mFragmentShaderObj);

	GLint numActiveAttribs = 0;
	GLint numActiveUniforms = 0;
	glGetProgramiv(mProgram, GL_ACTIVE_ATTRIBUTES, &numActiveAttribs);
	glGetProgramiv(mProgram, GL_ACTIVE_UNIFORMS, &numActiveUniforms);

	std::vector<GLchar> nameData(256);
	for (int unif = 0; unif < numActiveUniforms; ++unif)
	{
		GLint arraySize = 0;
		GLenum type = 0;
		GLsizei actualLength = 0;
		glGetActiveUniform(mProgram, unif, nameData.size(), &actualLength, &arraySize, &type, &nameData[0]);
		std::string name((char*) &nameData[0], actualLength);
		uniforms.insert(std::pair<std::string, GLuint>(name, glGetUniformLocation(mProgram, name.c_str())));
	}

	return true;
}

void Shader::Bind()
{
	glUseProgram(mProgram);
}

void Shader::Release()
{
	glUseProgram(0);
}

GLuint Shader::GetUniformLocation(std::string name)
{
	std::map<std::string, GLuint>::iterator it = uniforms.find(name);
	return it == uniforms.end() ? -1 : it->second;
}

void Shader::SetUniform(GLuint loc, float value)
{
	glUniform1f(loc, value);
}

void Shader::SetUniform(GLuint loc, float value1, float value2)
{
	glUniform2f(loc, value1, value2);
}

void Shader::SetUniform(GLuint loc, float value1, float value2, float value3)
{
	glUniform3f(loc, value1, value2, value3);
}

void Shader::SetUniform(GLuint loc, glm::vec2 value)
{
	glUniform2f(loc, value.x, value.y);
}

void Shader::SetUniform(GLuint loc, glm::vec3 value)
{
	glUniform3f(loc, value.x, value.y, value.z);
}

void Shader::SetUniform(GLuint loc, glm::vec4 value)
{
	glUniform4f(loc, value.x, value.y, value.z, value.w);
}

void Shader::SetUniform(GLuint loc, glm::mat4& value)
{
	glUniformMatrix4fv(loc, 1, false, &value[0][0]);
}


