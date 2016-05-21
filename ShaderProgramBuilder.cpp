#include "ShaderProgramBuilder.h"
#include <iostream>
#include <vector>

GLuint ShaderProgramBuilder::compileShader(ShaderType shaderType, const std::string & code) {
	GLuint shaderId = glCreateShader((GLenum) shaderType);
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
		std::cerr << "** Shader Compile Error:" << std::endl << &infoLog[0] << std::endl;

		glDeleteShader(shaderId);
		return 0;
	}

	return shaderId;
}

ShaderProgramBuilder::~ShaderProgramBuilder() {
	for (GLuint shaderId : this->shaderIds) {
		glDeleteShader(shaderId);
	}
}

ShaderProgramBuilder & ShaderProgramBuilder::addShader(ShaderType shaderType, const std::string & code) {
	GLuint id = this->compileShader(shaderType, code);
	if (0 != id) {
		this->shaderIds.push_back(id);
	}

	return *this;
}

GLuint ShaderProgramBuilder::buildProgram() {
	GLuint programId = glCreateProgram();

	try {
		for (GLuint shaderId : this->shaderIds) {
			glAttachShader(programId, shaderId);
		}

		// link
		glLinkProgram(programId);

		// check the link status
		GLint glResult = GL_FALSE;
		glGetProgramiv(programId, GL_LINK_STATUS, &glResult);
		if (GL_TRUE != glResult) {
			int infoLogLen = 0;
			glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLen);

			std::vector<char> infoLog(infoLogLen + 1);
			glGetProgramInfoLog(programId, infoLogLen, nullptr, &infoLog[0]);
			std::cerr << "** Shader Link Error:" << std::endl << &infoLog[0] << std::endl;
			throw programId;
		}
	}
	catch (GLuint programId) {
		if (0 != programId) {
			glDeleteProgram(programId);
			programId = 0;
		}
	}

	for (GLuint shaderId : this->shaderIds) {
		glDetachShader(programId, shaderId);
	}

	return programId;
}
