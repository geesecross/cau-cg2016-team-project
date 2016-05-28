#pragma once
#include "ShaderProgram.h"
#include "Vector.h"
#include "Texture.h"
class TextureShaderProgram : public ShaderProgram {
private:
	Texture texture;

public:
	inline GLuint getTextureID()const { return texture.getTextureId(); }
	TextureShaderProgram(Recipe & recipe);
	static TextureShaderProgram create();
	virtual void initInput() const override;

};