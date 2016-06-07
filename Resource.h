#pragma once
#include <map>
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture.h"

namespace Resource {
	namespace Meshes {
		enum Id {
			Plane,
			Arrow,
			Cube,
			Teapot,
			Particle,
			Skybox
		};

	}
	namespace ShaderPrograms {
		enum Id {
			Phong,
			Phong2,
			SimpleTexture,
			NormalMap,
			Parallax,
			Water,
			Skybox
		};
	}
	namespace Textures {
		enum Id {
			TexturePng,
			TextureSpecular,
			TextureNormal,
			TextureHeight,
			WaterNormal,
			Skybox,
		};
	}
	extern std::map<Meshes::Id, ::Mesh *> meshes;
	extern std::map<ShaderPrograms::Id, ::ShaderProgram *> shaderPrograms;
	extern std::map<Textures::Id, ::Texture *> textures;

	void init();
	void uninit();
}