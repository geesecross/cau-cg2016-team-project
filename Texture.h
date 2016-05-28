#pragma once

#include <GL/glew.h>
#include <GL/glut.h>

#include <memory>

class Texture {
private:
	struct TextureIdHolder {
		GLuint textureId;
		TextureIdHolder(GLuint textureId);
		~TextureIdHolder();
	};
	std::shared_ptr<TextureIdHolder> textureIdHolder;

public:
	Texture(GLuint textureId);

	GLuint getTextureId() const;
};
