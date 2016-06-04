#pragma once
#include "TextureShaderProgram.h"
#include "Model.h"

class MovingTexShader : public TextureShaderProgram {
public:
	MovingTexShader(Recipe & recipe);

	static MovingTexShader create();

	virtual void onPreDraw(const Model & model) const;
	virtual void onPostDraw(const Model & model) const;
	const inline void setTexMoveValue(const float& value){ texMoveValue = value; }
private:
	float texMoveValue;
};