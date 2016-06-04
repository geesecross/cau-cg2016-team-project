#include "MovingTexShader.h"
#include "FileHelper.h"
#include "Resource.h"

MovingTexShader::MovingTexShader(Recipe& recipe) :TextureShaderProgram(recipe), texMoveValue(0.f){}

MovingTexShader MovingTexShader::create(){
	return MovingTexShader(Recipe()
		.addShader(Shader::compile(Shader::VertexShader, FileHelper::loadTextFile("shaders/Transform.glsl")))
		.addShader(Shader::compile(Shader::VertexShader, FileHelper::loadTextFile("shaders/Texture.vert")))
		.addShader(Shader::compile(Shader::FragmentShader, FileHelper::loadTextFile("shaders/Texture.frag")))
		.addShader(Shader::compile(Shader::FragmentShader, FileHelper::loadTextFile("shaders/SimpleIlluminationModel.glsl")))
		);
}

void MovingTexShader::onPreDraw(const Model & model)const{
	TextureShaderProgram::onPreDraw(model);

	//for moving textures
	static long previousTime = glutGet(GLUT_ELAPSED_TIME);

	GLint objectId;
	if (objectId = glGetUniformLocation(this->getProgramId(), "in_time")) {
		glUniform1f(objectId, texMoveValue);
	}
}
void MovingTexShader::onPostDraw(const Model & model)const{
	TextureShaderProgram::onPostDraw(model);
}