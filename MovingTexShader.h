#pragma once
#include "TextureShaderProgram.h"
#include "Model.h"
#include "Animation.h"

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

class MovingTextureAnimation : public Animation {
private:
	MovingTexShader & shader;
public:
	MovingTextureAnimation(MovingTexShader & shader);
	virtual bool stepFrame(const double timeElapsed, const double timeDelta);
};