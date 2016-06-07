#include "NormalMappingProgram.h"
#include "FileHelper.h"
#include "Resource.h"
NormalMappingProgram::NormalMappingProgram(Recipe & recipe) : TextureShaderProgram(recipe)
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

void NormalMappingProgram::onPreDraw(const Model & model) const {
	TextureShaderProgram::onPreDraw(model);
	this->initTexture(2, "in_texNormal", model.getNormalTexture());
}

void NormalMappingProgram::onPostDraw(const Model & model) const {
	this->uninitTexture(2, "in_texNormal");

	TextureShaderProgram::onPostDraw(model);
}

