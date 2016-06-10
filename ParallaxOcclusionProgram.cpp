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
		.addShader(Shader::compile(Shader::FragmentShader, FileHelper::loadTextFile("shaders/Transform.glsl")))
		//.addShader(Shader::compile(Shader::FragmentShader, FileHelper::loadTextFile("shaders/SimpleIlluminationModel.glsl")))
		.addShader(Shader::compile(Shader::FragmentShader, FileHelper::loadTextFile("shaders/Parallax.frag")))
	);
}

void ParallaxOcclusionProgram::onPreDraw(const Model & model) const
{
	NormalMappingProgram::onPreDraw(model);

	GLint objectId;
	if (0 <= (objectId = glGetUniformLocation(getProgramId(), "parallaxScale"))) {
		glUniform1f(objectId, 0.1f);//0 ~ 0.5 is appropriate
	}

	this->initTexture(3, "in_texHeight", model.getHeightTexture());
}

void ParallaxOcclusionProgram::onPostDraw(const Model & model) const {
	this->uninitTexture(3, "in_texHeight");
	NormalMappingProgram::onPostDraw(model);
}

