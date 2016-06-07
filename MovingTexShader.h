#pragma once
#include "NormalMappingProgram.h"
#include "Model.h"
#include "Animation.h"

class MovingTexShader : public NormalMappingProgram {
private:
	Vector2f texMoveValue;
public:
	MovingTexShader(Recipe & recipe);

	static MovingTexShader create();

	virtual void onPreDraw(const Model & model) const;
	virtual void onPostDraw(const Model & model) const;
	const inline void setTexMoveValue(const Vector2f & offset){ texMoveValue = offset; }
};

class MovingTextureAnimation : public Animation {
private:
	MovingTexShader & shader;
	float movement = 0.0f;
public:
	MovingTextureAnimation(MovingTexShader & shader);
	virtual bool stepFrame(const double timeElapsed, const double timeDelta);
};