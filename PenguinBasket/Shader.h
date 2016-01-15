#pragma once
#include <GL/glew.h>
#include <map>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <assert.h>
#include "glm/glm.hpp"



class Shader
{
public:
	Shader();
	Shader(const char* pathVertex, const char* pathGeometry, const char* pathFragment);
	bool LoadFromFile(const char *pathVertex, const char *pathGeometry, const char *pathFragment);
	void Bind();
	void Release();
	GLuint GetUniformLocation(std::string name);
	GLuint GetProgram() { return mProgram; };

	void SetUniform(GLuint loc, float value);
	void SetUniform(GLuint loc, float value1, float value2);
	void SetUniform(GLuint loc, float value1, float value2, float value3);
	void SetUniform(GLuint loc, glm::vec2 value);
	void SetUniform(GLuint loc, glm::vec3 value);
	void SetUniform(GLuint loc, glm::vec4 value);
	void SetUniform(GLuint loc, glm::mat4& value);

private:
	GLuint mProgram;
	GLuint mVertexShaderObj;
	GLuint mGeometryShaderObj;
	GLuint mFragmentShaderObj;

	std::string mVertexSource;
	std::string mGeometrySource;
	std::string mFragmentSource;

	std::map<std::string, GLuint> uniforms;
};
