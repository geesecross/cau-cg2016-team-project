#pragma once
#include <map>
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture.h"

namespace Resource {
	enum MeshId {
		Plane,
		Arrow,
		Cube,
		Teapot,
		Particle
	};
	enum ShaderProgramId {
		Phong,
		Phong2,
		SimpleTexture,
		NormalMap,
		Parallax,
	};
	enum TextureId {
		TexturePng,
		TextureSpecular,
		TextureNormal,
		TextureHeight,
	};

	extern std::map<MeshId, Mesh *> meshes;
	extern std::map<ShaderProgramId, ShaderProgram *> shaderPrograms;
	extern std::map<TextureId, Texture *> textures;

	void init();
	void uninit();
}