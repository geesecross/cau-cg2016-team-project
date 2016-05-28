#include "Texture.h"

Texture::TextureIdHolder::TextureIdHolder(GLuint textureId) {
	this->textureId = textureId;
}

Texture::TextureIdHolder::~TextureIdHolder() {
	glDeleteTextures(1, &this->textureId);
}

Texture::Texture(GLuint textureId)
	: textureIdHolder(new TextureIdHolder(textureId))
{
}

GLuint Texture::getTextureId() const {
	return this->textureIdHolder->textureId;
}
