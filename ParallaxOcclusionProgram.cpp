#include "ParallaxOcclusionProgram.h"
#include "FileHelper.h"
ParallaxOcclusionProgram::ParallaxOcclusionProgram(Recipe & recipe) :NormalMappingProgram(recipe)
{

}

ParallaxOcclusionProgram ParallaxOcclusionProgram::create()
{
	return ParallaxOcclusionProgram(
		Recipe()
		.addShader(Shader::compile(Shader::VertexShader, FileHelper::loadTextFile("shaders/Transform.glsl")))
		.addShader(Shader::compile(Shader::VertexShader, FileHelper::loadTextFile("shaders/Parallax.vert")))
		.addShader(Shader::compile(Shader::FragmentShader, FileHelper::loadTextFile("shaders/Parallax.frag")))
		.addShader(Shader::compile(Shader::FragmentShader, FileHelper::loadTextFile("shaders/SimpleIlluminationModel.glsl")))
	);
}

void ParallaxOcclusionProgram::onPreDraw(const Model & model) const
{
	NormalMappingProgram::onPreDraw(model);
	//uniform float parallaxScale;
	GLint objectId;
	if (0 <= (objectId = glGetUniformLocation(getProgramId(), "parallaxScale"))) {
		glUniform1f(objectId, 0.1f);//0 ~ 0.5 is appropriate
		GLint heightId;
		if (0 <= (heightId = glGetUniformLocation(getProgramId(), "in_height"))) {
			glEnable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, model.getHeightTexture()->getTextureId());
			glUniform1i(heightId, 2);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		}

	}
}

void ParallaxOcclusionProgram::onPostDraw(const Model & model) const
{
	NormalMappingProgram::onPostDraw(model);
}

