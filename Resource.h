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
			SimpleTexture,
			MetalNormal,
			MetalParallax,
			Water,
			Skybox,
			Cobble,
			Cushion,
			Wood,
		};
	}
	namespace Textures {
		enum Id {
			Metal,
			TextureSpecular,
			MetalNormal,
			MetalHeight,
			WaterNormal,
			Skybox,
			Cobble,
			CobbleNormal,
			CobbleHeight,
			CobbleSpecular,
			Cushion,
			CushionNormal,
			Wood,
			WoodNormal,
			WoodSpecular,
		};
	}
	extern std::map<Meshes::Id, ::Mesh *> meshes;
	extern std::map<ShaderPrograms::Id, ::ShaderProgram *> shaderPrograms;
	extern std::map<Textures::Id, ::Texture *> textures;

	void init();
	void uninit();
}