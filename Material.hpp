#pragma once
#include <string>
#include <GL/glew.h>
#include <GL/freeglut.h>

class Material {
private:
	Material();

public:
	static Material * createFromShaderFile(std::string vertexShaderFile, std::string fragShaderFile);
};
