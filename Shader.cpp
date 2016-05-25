#include "Shader.h"
#include <vector>

Shader::Shader(ShaderType shaderType, GLuint shaderId)
	: shaderIdHolder(new ShaderIdHolder(shaderId))
{
	this->shaderType = shaderType;
}

Shader Shader::compile(ShaderType shaderType, const std::string & code) {
	GLuint shaderId = glCreateShader((GLenum)shaderType);
	const char * shaderCodePtr = code.c_str();
	glShaderSource(shaderId, 1, &shaderCodePtr, nullptr);
	glCompileShader(shaderId);

	// check the compile status
	GLint glResult = GL_FALSE;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &glResult);
	if (GL_TRUE != glResult) {
		int infoLogLen;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLen);

		std::vector<char> infoLog(infoLogLen + 1);
		glGetShaderInfoLog(shaderId, infoLogLen, nullptr, &infoLog[0]);
		std::string errorMessage = "Shader Compile Error:\n";
		errorMessage += &infoLog[0];
		glDeleteShader(shaderId);
		throw ShaderException(errorMessage);
	}

	return Shader(shaderType, shaderId);
}

Shader::ShaderType Shader::getShaderType() const {
	return this->shaderType;
}

GLuint Shader::getShaderId() const {
	return this->shaderIdHolder->shaderId;
	//return this->shaderId;
}

void Shader::initAttribute(const ShaderProgram & shaderProgram) const {
}

Shader::ShaderIdHolder::ShaderIdHolder(GLuint shaderId) {
	this->shaderId = shaderId;
}

Shader::ShaderIdHolder::~ShaderIdHolder() {
	glDeleteShader(this->shaderId);
}
