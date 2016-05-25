#include "SimpleIlluminationModelShaderProgram.h"
#include "FileHelper.h"

SimpleIlluminationModelShaderProgram::SimpleIlluminationModelShaderProgram(Recipe & recipe)
	: ShaderProgram(recipe)
{
}

SimpleIlluminationModelShaderProgram SimpleIlluminationModelShaderProgram::createPhong() {
	return SimpleIlluminationModelShaderProgram(Recipe()
		.addShader(Shader::compile(Shader::VertexShader, FileHelper::loadTextFile("shaders/Transform.glsl")))
		.addShader(Shader::compile(Shader::VertexShader, FileHelper::loadTextFile("shaders/Phong.vert")))
		.addShader(Shader::compile(Shader::FragmentShader, FileHelper::loadTextFile("shaders/SimpleIlluminationModel.glsl")))
		.addShader(Shader::compile(Shader::FragmentShader, FileHelper::loadTextFile("shaders/Phong.frag")))
	);
}

SimpleIlluminationModelShaderProgram SimpleIlluminationModelShaderProgram::createGouraud() {
	return SimpleIlluminationModelShaderProgram(Recipe()
		.addShader(Shader::compile(Shader::VertexShader, FileHelper::loadTextFile("shaders/Transform.glsl")))
		.addShader(Shader::compile(Shader::VertexShader, FileHelper::loadTextFile("shaders/SimpleIlluminationModel.glsl")))
		.addShader(Shader::compile(Shader::VertexShader, FileHelper::loadTextFile("shaders/Gouraud.vert")))
		.addShader(Shader::compile(Shader::FragmentShader, FileHelper::loadTextFile("shaders/Gouraud.frag")))
	);
}

SimpleIlluminationModelShaderProgram & SimpleIlluminationModelShaderProgram::setAmbientRatio(GLfloat ratio) {
	this->ambientRatio = ratio;
	return *this;
}

GLfloat SimpleIlluminationModelShaderProgram::getAmbientRatio() const {
	return this->ambientRatio;
}

SimpleIlluminationModelShaderProgram & SimpleIlluminationModelShaderProgram::setDiffusionRatio(GLfloat ratio) {
	this->diffusionRatio = ratio;
	return *this;
}

GLfloat SimpleIlluminationModelShaderProgram::getDiffusionRatio() const {
	return this->diffusionRatio;
}

SimpleIlluminationModelShaderProgram & SimpleIlluminationModelShaderProgram::setSpecularRatio(GLfloat ratio) {
	this->specularRatio = ratio;
	return *this;
}

GLfloat SimpleIlluminationModelShaderProgram::getSpecularRatio() const {
	return this->specularRatio;
}

SimpleIlluminationModelShaderProgram & SimpleIlluminationModelShaderProgram::setShiness(GLfloat shiness) {
	this->shiness = shiness;
	return *this;
}

GLfloat SimpleIlluminationModelShaderProgram::getShiness() const {
	return this->shiness;
}

SimpleIlluminationModelShaderProgram & SimpleIlluminationModelShaderProgram::setLightVector(const Vector3f & vector) {
	this->lightVector = vector;
	return *this;
}

SimpleIlluminationModelShaderProgram & SimpleIlluminationModelShaderProgram::enableLightVectorAsPosition(bool enabled) {
	this->enabledLightVectorAsPosition = enabled;
	return *this;
}

void SimpleIlluminationModelShaderProgram::initInput() const {
	GLint objectId;
	if (0 <= (objectId = glGetUniformLocation(this->getProgramId(), "in_ambientRatio"))) {
		glUniform1f(objectId, this->ambientRatio);
	}
	if (0 <= (objectId = glGetUniformLocation(this->getProgramId(), "in_diffusionRatio"))) {
		glUniform1f(objectId, this->diffusionRatio);
	}
	if (0 <= (objectId = glGetUniformLocation(this->getProgramId(), "in_specularRatio"))) {
		glUniform1f(objectId, this->specularRatio);
	}
	if (0 <= (objectId = glGetUniformLocation(this->getProgramId(), "in_shiness"))) {
		glUniform1f(objectId, this->shiness);
	}

	if (0 <= (objectId = glGetUniformLocation(this->getProgramId(), "in_lightVector"))) {
		glUniform3fv(objectId, 1, this->lightVector.data());
	}
	if (0 <= (objectId = glGetUniformLocation(this->getProgramId(), "in_lightVectorAsPosition"))) {
		glUniform1i(objectId, this->enabledLightVectorAsPosition ? GL_TRUE : GL_FALSE);
	}
}
