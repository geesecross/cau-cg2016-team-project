#pragma once
#include <GL/glew.h>
#include <GL/glut.h>
#include <fstream>
#include <iostream>
#include <string>

class ShaderProgramBuilder {
private:
	GLuint vertexShaderId = 0, fragmentShaderId = 0;

	GLuint compileShaderFromFile(GLenum type, const std::string & fileName);
	ShaderProgramBuilder & setVertexShaderId(GLuint shaderId);
	ShaderProgramBuilder & setFragmentShaderId(GLuint shaderId);

public:
	virtual ~ShaderProgramBuilder();
	ShaderProgramBuilder & setVertexShaderFromFile(const std::string & fileName);
	ShaderProgramBuilder & setFragmentShaderFromFile(const std::string & fileName);

	GLuint buildProgram();
};
