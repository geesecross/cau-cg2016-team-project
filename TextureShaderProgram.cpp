#include "TextureShaderProgram.h"
#include "FileHelper.h"
//#include <GL/SOIL.h>
TextureShaderProgram::TextureShaderProgram(Recipe & recipe):
	ShaderProgram(recipe),
	texture(/*SOIL_load_OGL_texture
		(
			"shaders/texture.png",
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS
		)*/0)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);
}

TextureShaderProgram TextureShaderProgram::create()
{
	return TextureShaderProgram(Recipe()
		.addShader(Shader::compile(Shader::VertexShader, FileHelper::loadTextFile("shaders/Transform.glsl")))
		.addShader(Shader::compile(Shader::VertexShader, FileHelper::loadTextFile("shaders/Texture.vertex")))
		.addShader(Shader::compile(Shader::FragmentShader, FileHelper::loadTextFile("shaders/Texture.frag")))
		);
}

void TextureShaderProgram::initInput() const
{
	GLint objectId;
	if (0 <= (objectId = glGetUniformLocation(this->getProgramId(), "tex0"))) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture.getTextureId());

		glUniform1i(objectId, 0);
	}
}
