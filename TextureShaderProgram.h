#pragma once
#include "SimpleIlluminationModelShaderProgram.h"
#include "Model.h"

class TextureShaderProgram : public SimpleIlluminationModelShaderProgram {
public:
	TextureShaderProgram(Recipe & recipe);

	static TextureShaderProgram create();

	virtual void onPreDraw(const Model & model) const;
	virtual void onPostDraw(const Model & model) const;
};