#include "FooShader.h"
#include "FileHelper.h"

FooShader::FooShader()
	: ShaderProgram(
		Recipe().addShader(Shader::compile(Shader::VertexShader, FileHelper::loadTextFile("shaders/foo.vert")))
	)
{

}
