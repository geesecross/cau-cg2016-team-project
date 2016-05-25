#pragma once
#include <GL/glew.h>
#include <GL/glut.h>

#include <string>
#include <memory>
#include "Exception.h"

class Shader {
	class ShaderProgram;
public:
	enum ShaderType {
		VertexShader = GL_VERTEX_SHADER,
		FragmentShader = GL_FRAGMENT_SHADER,
	};
	class ShaderException : public Exception {
	public:
		ShaderException(std::string & message) : Exception(message) {};
	};

private:
	struct ShaderIdHolder {
		GLuint shaderId;
		ShaderIdHolder(GLuint shaderId);
		~ShaderIdHolder();
	};
	ShaderType shaderType;
	std::shared_ptr<ShaderIdHolder> shaderIdHolder;

	Shader(ShaderType shaderType, GLuint shaderId);
public:
	static Shader compile(ShaderType shaderType, const std::string & code);
	ShaderType getShaderType() const;
	GLuint getShaderId() const;

	virtual void initAttribute(const ShaderProgram & shaderProgram) const;
};
