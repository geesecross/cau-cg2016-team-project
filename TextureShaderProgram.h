#pragma once
#include "SimpleIlluminationModelShaderProgram.h"
#include "Model.h"

class TextureShaderProgram : public SimpleIlluminationModelShaderProgram {
protected:
	TextureShaderProgram(Recipe & recipe);
	void initTexture(const size_t textureIndex, const std::string & samplerName, const Texture * texture) const;
	void uninitTexture(const size_t textureIndex, const std::string & samplerName) const;

public:
	static TextureShaderProgram create();

	virtual void onPreDraw(const Model & model) const;
	virtual void onPostDraw(const Model & model) const;
};