#include "TextureShaderProgram.h"
#include "FileHelper.h"
#include "Resource.h"

TextureShaderProgram::TextureShaderProgram(Recipe & recipe):
	SimpleIlluminationModelShaderProgram(recipe)
{
}

void TextureShaderProgram::initTexture(size_t textureIndex, const std::string & samplerName, const Texture * texture) const {
	GLint objectId;
	bool samplerExists = false;

	if (nullptr != texture
		&& 0 <= (objectId = glGetUniformLocation(this->getProgramId(), samplerName.c_str()))
		) {
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0 + textureIndex);
		glBindTexture(GL_TEXTURE_2D, texture->getTextureId());
		glUniform1i(objectId, textureIndex);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		samplerExists = true;
	}
	if (0 <= (objectId = glGetUniformLocation(this->getProgramId(), (samplerName + "Exists").c_str()))) {
		glUniform1i(objectId, samplerExists ? GL_TRUE : GL_FALSE);
	}
}

void TextureShaderProgram::uninitTexture(const size_t textureIndex, const std::string & samplerName) const {
	GLint objectId;

	if (0 <= (objectId = glGetUniformLocation(this->getProgramId(), samplerName.c_str()))) {
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0 + textureIndex);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}
}

TextureShaderProgram TextureShaderProgram::create() {
	return TextureShaderProgram(Recipe()
		.addShader(Shader::compile(Shader::VertexShader, FileHelper::loadTextFile("shaders/Transform.glsl")))
		.addShader(Shader::compile(Shader::VertexShader, FileHelper::loadTextFile("shaders/Texture.vert")))
		.addShader(Shader::compile(Shader::FragmentShader, FileHelper::loadTextFile("shaders/SimpleIlluminationModel.glsl")))
		.addShader(Shader::compile(Shader::FragmentShader, FileHelper::loadTextFile("shaders/Texture.frag")))
	);
}

void TextureShaderProgram::onPreDraw(const Model & model) const {
	SimpleIlluminationModelShaderProgram::onPreDraw(model);

	GLint objectId;

	if (nullptr != model.getMesh()
		&& model.getMesh()->getVertices().size() == model.getMesh()->getTexCoords().size()
		&& 0 <= (objectId = glGetAttribLocation(this->getProgramId(), "in_texCoord"))
	) {
		glEnableVertexAttribArray(objectId);
		glVertexAttribPointer(objectId, 2, GL_FLOAT, GL_FALSE, 0, model.getMesh()->getTexCoords().data());
	}

	this->initTexture(0, "in_texDiffuse", model.getDiffuseTexture());
}

void TextureShaderProgram::onPostDraw(const Model & model) const {
	GLint objectId;

	if (nullptr != model.getMesh()
		&& model.getMesh()->getVertices().size() == model.getMesh()->getTexCoords().size()
		&& 0 <= (objectId = glGetAttribLocation(this->getProgramId(), "in_texCoord"))) {
		glDisableVertexAttribArray(objectId);
	}

	this->uninitTexture(0, "in_texDiffuse");

	SimpleIlluminationModelShaderProgram::onPostDraw(model);
}
