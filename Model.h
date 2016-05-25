#pragma once
#include "Mesh.h"
#include "ShaderProgram.h"
#include "SpaceObject.h"

class Model: public SpaceObject {
private:
	static const ShaderProgram * getDefaultShaderProgram();

	const Mesh * mesh = nullptr;
	const ShaderProgram * shaderProgram = getDefaultShaderProgram();
	Vector4f color;

public:
	Model();

	const Mesh * getMesh() const;
	Model & bindMesh(const Mesh * mesh);

	const Vector4f & getColor() const;
	Model & setColor(const Vector4f & color);

	const ShaderProgram * getShaderProgram() const;
	Model & bindShaderProgram(const ShaderProgram * shaderProgram);

	void draw(const ShaderProgram & shaderProgram) const;
};