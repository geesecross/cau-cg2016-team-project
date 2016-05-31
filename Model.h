#pragma once
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Actor.h"

class Model: public Actor::Component {
private:
	static const ShaderProgram * getDefaultShaderProgram();

	const Mesh * mesh = nullptr;
	const Texture * texture = nullptr;
	const ShaderProgram * shaderProgram = getDefaultShaderProgram();
	Vector4f color;

public:
	Model();

	const Mesh * getMesh() const;
	Model & bindMesh(const Mesh * mesh);

	const Vector4f & getColor() const;
	Model & setColor(const Vector4f & color);

	const Texture * getTexture() const;
	Model & bindTexture(const Texture * texture);

	const ShaderProgram * getShaderProgram() const;
	Model & bindShaderProgram(const ShaderProgram * shaderProgram);

	void draw(const ShaderProgram & shaderProgram) const;
};