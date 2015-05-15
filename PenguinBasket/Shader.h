#pragma once
#include <GL/glew.h>
#include <map>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <assert.h>


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
