#include "Model.h"
#include "FileHelper.h"
#include "Shader.h"

const ShaderProgram * Model::getDefaultShaderProgram() {
	static ShaderProgram defaultShaderProgram(ShaderProgram::Recipe()
		.addShader(Shader::compile(Shader::VertexShader, FileHelper::loadTextFile("shaders/Transform.glsl")))
		.addShader(Shader::compile(Shader::VertexShader, FileHelper::loadTextFile("shaders/Null.vert")))
		.addShader(Shader::compile(Shader::FragmentShader, FileHelper::loadTextFile("shaders/FillColor.frag")))
	);
	return &defaultShaderProgram;
}

Model::Model() {
	this->color = { 1, 1, 1, 1 };
}

const Mesh * Model::getMesh() const {
	return this->mesh;
}

Model & Model::bindMesh(const Mesh * mesh) {
	this->mesh = mesh;
	return *this;
}

const Vector4f& Model::getColor() const {
	return this->color;
}

Model & Model::setColor(const Vector4f & color) {
	this->color = color;
	return *this;
}

const Texture * Model::getDiffuseTexture() const{
	return this->diffuseTexture;
}

Model & Model::bindDiffuseTexture(const Texture * texture) {
	this->diffuseTexture = texture;
	return *this;
}

const ShaderProgram * Model::getShaderProgram() const {
	return this->shaderProgram;
}

Model & Model::bindShaderProgram(const ShaderProgram * shaderProgram) {
	this->shaderProgram = nullptr == shaderProgram ? Model::getDefaultShaderProgram() : shaderProgram;
	return *this;
}

void Model::draw(const ShaderProgram & shaderProgram) const {
	if (nullptr == this->mesh) {
		return;
	}

	GLint objectId;
	if (0 <= (objectId = glGetUniformLocation(shaderProgram.getProgramId(), "in_color"))) {
		glUniform4fv(objectId, 1, this->color.data());
	}

	shaderProgram.onPreDraw(*this);
	this->mesh->draw(shaderProgram);
	shaderProgram.onPostDraw(*this);
}

const Texture * Model::getSpecularTexture() const
{
	return specularTexture;
}

Model& Model::bindSpecularTexture(const Texture * texture)
{
	specularTexture = texture;
	return *this;
}

const Texture * Model::getNormalTexture() const
{
	return normalTexture;
}

Model& Model::bindNormalTexture(const Texture * texture)
{
	normalTexture = texture;
	return *this;
}

Model& Model::bindHeightTexture(const Texture * texture)
{
	heightTexture = texture;
	return *this;
}

const Texture * Model::getHeightTexture() const
{
	return heightTexture;
}
