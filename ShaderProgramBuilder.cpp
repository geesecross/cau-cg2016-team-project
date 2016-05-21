#include "ShaderProgramBuilder.hpp"
#include <vector>

GLuint ShaderProgramBuilder::compileShaderFromFile(GLenum type, const std::string & fileName) {
	std::cerr << "** Compiling Shader: " << fileName << std::endl;

	std::ifstream is(fileName);
	if (!is.is_open()) {
		std::cerr << "** Failed to open shader code file" << std::endl;
		return 0;
	}

	std::string shaderCode, line;
	while (std::getline(is, line)) {
		shaderCode += line + "\n";
	}
	is.close();

	GLuint shaderId = glCreateShader(type);
	const char * shaderCodePtr = shaderCode.c_str();
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

ShaderProgramBuilder &  ShaderProgramBuilder::setVertexShaderId(GLuint shaderId) {
	if (0 != this->vertexShaderId) {
		glDeleteShader(this->vertexShaderId);
	}
	this->vertexShaderId = shaderId;

	return *this;
}

ShaderProgramBuilder & ShaderProgramBuilder::setFragmentShaderId(GLuint shaderId) {
	if (0 != this->fragmentShaderId) {
		glDeleteShader(this->fragmentShaderId);
	}
	this->fragmentShaderId = shaderId;

	return *this;
}

ShaderProgramBuilder::~ShaderProgramBuilder() {
	this->setVertexShaderId(0);
	this->setFragmentShaderId(0);
}

ShaderProgramBuilder & ShaderProgramBuilder::setVertexShaderFromFile(const std::string & fileName) {
	this->setVertexShaderId(this->compileShaderFromFile(GL_VERTEX_SHADER, fileName));

	return *this;
}

ShaderProgramBuilder & ShaderProgramBuilder::setFragmentShaderFromFile(const std::string & fileName) {
	this->setFragmentShaderId(this->compileShaderFromFile(GL_FRAGMENT_SHADER, fileName));

	return *this;
}

GLuint ShaderProgramBuilder::buildProgram() {
	GLuint programId = glCreateProgram();

	try {
		if (0 != this->vertexShaderId) {
			glAttachShader(programId, this->vertexShaderId);
		}
		if (0 != this->fragmentShaderId) {
			glAttachShader(programId, this->fragmentShaderId);
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

	return programId;
}
