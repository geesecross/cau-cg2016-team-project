#include "MovingTexShader.h"
#include "FileHelper.h"
#include "Resource.h"

MovingTexShader::MovingTexShader(Recipe& recipe) : NormalMappingProgram(recipe), texMoveValue(0.f){}

MovingTexShader MovingTexShader::create(){
	return MovingTexShader(Recipe()
		.addShader(Shader::compile(Shader::VertexShader, FileHelper::loadTextFile("shaders/Transform.glsl")))
		.addShader(Shader::compile(Shader::VertexShader, FileHelper::loadTextFile("shaders/MovingTexture.vert")))
		.addShader(Shader::compile(Shader::FragmentShader, FileHelper::loadTextFile("shaders/Transform.glsl")))
		.addShader(Shader::compile(Shader::FragmentShader, FileHelper::loadTextFile("shaders/SimpleIlluminationModel.glsl")))
		.addShader(Shader::compile(Shader::FragmentShader, FileHelper::loadTextFile("shaders/MovingTexture.frag")))
		);
}

void MovingTexShader::onPreDraw(const Model & model)const{
	NormalMappingProgram::onPreDraw(model);

	GLint objectId;
	if (0 <= (objectId = glGetUniformLocation(this->getProgramId(), "in_time"))) {
		glUniform2fv(objectId, 1, texMoveValue.data());
	}
}
void MovingTexShader::onPostDraw(const Model & model)const{
	NormalMappingProgram::onPostDraw(model);
}

MovingTextureAnimation::MovingTextureAnimation(MovingTexShader & shader) : shader(shader) {

}

bool MovingTextureAnimation::stepFrame(const double timeElapsed, const double timeDelta) {
	movement += (cosf((float)timeElapsed) + 0.60f) * 0.0005f;
	shader.setTexMoveValue({ movement * 0.5f, movement });
	//shader.setTexMoveValue({ timeElapsed * 0.1f, 0 });
	return false;
}