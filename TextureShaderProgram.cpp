#include "TextureShaderProgram.h"
#include "FileHelper.h"
#include "Resource.h"

TextureShaderProgram::TextureShaderProgram(Recipe & recipe):
	SimpleIlluminationModelShaderProgram(recipe)
{
}

TextureShaderProgram TextureShaderProgram::create() {
	return TextureShaderProgram(Recipe()
		.addShader(Shader::compile(Shader::VertexShader, FileHelper::loadTextFile("shaders/Transform.glsl")))
		.addShader(Shader::compile(Shader::VertexShader, FileHelper::loadTextFile("shaders/Texture.vert")))
		.addShader(Shader::compile(Shader::FragmentShader, FileHelper::loadTextFile("shaders/Texture.frag")))
		.addShader(Shader::compile(Shader::FragmentShader, FileHelper::loadTextFile("shaders/SimpleIlluminationModel.glsl")))
	);
}

void TextureShaderProgram::onPreDraw(const Model & model) const {
	SimpleIlluminationModelShaderProgram::onPreDraw(model);

	GLint objectId;
	if (nullptr != model.getDiffuseTexture() && nullptr != model.getMesh() && model.getMesh()->getVertices().size() == model.getMesh()->getTexCoords().size()) {
		GLint texCoordId;
		if (0 <= (objectId = glGetUniformLocation(this->getProgramId(), "in_tex0"))
			&& 0 <= (texCoordId = glGetAttribLocation(this->getProgramId(), "in_texCoord"))
		) {

			

			glEnable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0);
			if (0 <= (objectId = glGetUniformLocation(this->getProgramId(), "in_texSpecular")))
			{
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, model.getSpecularTexture()->getTextureId());
			}

			glBindTexture(GL_TEXTURE_2D, model.getDiffuseTexture()->getTextureId());
			glUniform1i(objectId, 0);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glEnableVertexAttribArray(texCoordId);
			glVertexAttribPointer(texCoordId, 2, GL_FLOAT, GL_FALSE, 0, model.getMesh()->getTexCoords().data());
		}
	}
}

void TextureShaderProgram::onPostDraw(const Model & model) const {
	GLint objectId;
	if (nullptr != model.getDiffuseTexture() && nullptr != model.getMesh() && model.getMesh()->getVertices().size() == model.getMesh()->getTexCoords().size()) {
		GLint texCoordId;
		if (0 <= (objectId = glGetUniformLocation(this->getProgramId(), "in_tex0"))
			&& 0 <= (texCoordId = glGetAttribLocation(this->getProgramId(), "in_texCoord"))
			) {
			glDisableVertexAttribArray(texCoordId);
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_2D);
		}
	}

	SimpleIlluminationModelShaderProgram::onPostDraw(model);
}
