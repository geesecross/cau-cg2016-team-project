#include "MovingTexShader.h"
#include "FileHelper.h"
#include "Resource.h"

MovingTexShader::MovingTexShader(Recipe& recipe) :TextureShaderProgram(recipe), texMoveValue(0.f){}

MovingTexShader MovingTexShader::create(){
	return MovingTexShader(Recipe()
		.addShader(Shader::compile(Shader::VertexShader, FileHelper::loadTextFile("shaders/Transform.glsl")))
		.addShader(Shader::compile(Shader::VertexShader, FileHelper::loadTextFile("shaders/MovingTexture.vert")))
		.addShader(Shader::compile(Shader::FragmentShader, FileHelper::loadTextFile("shaders/MovingTexture.frag")))
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

MovingTextureAnimation::MovingTextureAnimation(MovingTexShader & shader) : shader(shader) {

}

bool MovingTextureAnimation::stepFrame(const double timeElapsed, const double timeDelta) {
	shader.setTexMoveValue(timeElapsed * 0.1f);
	return false;
}