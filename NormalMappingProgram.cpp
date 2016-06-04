#include "NormalMappingProgram.h"
#include "FileHelper.h"
#include "Resource.h"
#include "Model.h"
NormalMappingProgram::NormalMappingProgram(Recipe & recipe) : SimpleIlluminationModelShaderProgram(recipe)
{

}

NormalMappingProgram NormalMappingProgram::create()
{
	return NormalMappingProgram(
		Recipe()
		.addShader(Shader::compile(Shader::VertexShader, FileHelper::loadTextFile("shaders/Transform.glsl")))
		.addShader(Shader::compile(Shader::VertexShader, FileHelper::loadTextFile("shaders/NormalMap.vert")))
		.addShader(Shader::compile(Shader::FragmentShader, FileHelper::loadTextFile("shaders/Transform.glsl")))
		.addShader(Shader::compile(Shader::FragmentShader, FileHelper::loadTextFile("shaders/SimpleIlluminationModel.glsl")))
		.addShader(Shader::compile(Shader::FragmentShader, FileHelper::loadTextFile("shaders/NormalMap.frag")))
		);
}

void NormalMappingProgram::onPreDraw(const Model & model) const
{
	SimpleIlluminationModelShaderProgram::onPreDraw(model);

	GLint objectId;
	if (nullptr != model.getDiffuseTexture() && nullptr != model.getMesh() && model.getMesh()->getVertices().size() == model.getMesh()->getTexCoords().size()) {
		GLint texCoordId;
		GLint texId, normalId;
		if (0 <= (texId = glGetUniformLocation(this->getProgramId(), "in_tex0"))
			&& 0 <= (texCoordId = glGetAttribLocation(this->getProgramId(), "in_texCoord"))
			&& 0 <= (normalId = glGetUniformLocation(this->getProgramId(), "in_normal"))
			) {
			glEnable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, model.getDiffuseTexture()->getTextureId());
			glUniform1i(texId, 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, model.getNormalTexture()->getTextureId());
			glUniform1i(normalId, 1);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glEnableVertexAttribArray(texCoordId);
			glVertexAttribPointer(texCoordId, 2, GL_FLOAT, GL_FALSE, 0, model.getMesh()->getTexCoords().data());

		}
	}
}

void NormalMappingProgram::onPostDraw(const Model & model) const
{
	GLint objectId, texCoordId;
	if (0 <= (objectId = glGetUniformLocation(this->getProgramId(), "in_tex0"))
		&& 0 <= (texCoordId = glGetAttribLocation(this->getProgramId(), "in_texCoord"))
		&& 0 <= (objectId = glGetAttribLocation(this->getProgramId(), "in_normalCoord"))
		) {
		glDisableVertexAttribArray(texCoordId);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}
	SimpleIlluminationModelShaderProgram::onPostDraw(model);
}

