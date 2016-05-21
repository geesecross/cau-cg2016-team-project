#pragma once
#include <GL/glew.h>
#include <GL/glut.h>
#include <string>
#include <list>

class ShaderProgramBuilder {
public:
	enum ShaderType {
		VertexShader = GL_VERTEX_SHADER,
		FragmentShader = GL_FRAGMENT_SHADER,
	};
private:
	std::list<GLuint> shaderIds;
	GLuint compileShader(ShaderType shaderType, const std::string & code);

public:
	virtual ~ShaderProgramBuilder();
	ShaderProgramBuilder & addShader(ShaderType shaderType, const std::string & fileName);

	GLuint buildProgram();
};
