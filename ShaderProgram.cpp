#include "ShaderProgram.h"

#include <vector>

ShaderProgram::ShaderProgram(GLuint programId)
	: programIdHolder(new ShaderProgramIdHolder(programId))
{
}

ShaderProgram::ShaderProgram(const Recipe & recipe)
	: programIdHolder(new ShaderProgramIdHolder(recipe.link()))
{
}

ShaderProgram::~ShaderProgram() {
}

GLuint ShaderProgram::getProgramId() const {
	return this->programIdHolder->programId;
}

void ShaderProgram::initInput() const {
	// do nothing - this function is just for inherited classes
}

ShaderProgram::ShaderProgramIdHolder::ShaderProgramIdHolder(GLuint programId) {
	this->programId = programId;
}

ShaderProgram::ShaderProgramIdHolder::~ShaderProgramIdHolder() {
	glDeleteProgram(this->programId);
}

GLuint ShaderProgram::Recipe::link() const {
	GLuint programId = glCreateProgram();

	try {
		// attach
		for (const Shader & shader : this->shaders) {
			glAttachShader(programId, shader.getShaderId());
		}

		// link
		glLinkProgram(programId);

		// detach
		for (const Shader & shader : this->shaders) {
			glDetachShader(programId, shader.getShaderId());
		}

		// check the link status
		GLint glResult = GL_FALSE;
		glGetProgramiv(programId, GL_LINK_STATUS, &glResult);
		if (GL_TRUE != glResult) {
			int infoLogLen = 0;
			glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLen);

			std::vector<char> infoLog(infoLogLen + 1);
			glGetProgramInfoLog(programId, infoLogLen, nullptr, &infoLog[0]);
			std::string errorMessage = "Shader Link Error:\n";
			errorMessage += &infoLog[0];
			throw Exception(errorMessage);
		}
	}
	catch (Exception) {
		if (0 != programId) {
			glDeleteProgram(programId);
			programId = 0;
		}
		throw;
	}

	return programId;
}

ShaderProgram::Recipe & ShaderProgram::Recipe::addShader(const Shader & shader) {
	this->shaders.push_back(shader);
	return *this;
}

ShaderProgram::Recipe::Iterator ShaderProgram::Recipe::begin() {
	return this->shaders.begin();
}

ShaderProgram::Recipe::Iterator ShaderProgram::Recipe::end() {
	return this->shaders.end();
}

ShaderProgram::Recipe::ConstIterator ShaderProgram::Recipe::begin() const {
	return this->shaders.end();
}

ShaderProgram::Recipe::ConstIterator ShaderProgram::Recipe::end() const {
	return this->shaders.end();
}
