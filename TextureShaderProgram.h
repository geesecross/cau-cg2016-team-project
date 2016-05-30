#pragma once
#include "ShaderProgram.h"
#include "Texture.h"

class TextureShaderProgram : public ShaderProgram {
private:
	std::shared_ptr<Texture> texture;

public:
	TextureShaderProgram(Recipe & recipe);
	static TextureShaderProgram create();
	GLuint getTextureId() const;

	virtual void initInput() const override;
};