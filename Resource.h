#pragma once
#include <map>
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture.h"

namespace Resource {
	namespace Meshes {
		enum Id {
			Plane,
			Blockside,
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
			Sea,
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
			AlphaCircle,
		};
	}
	extern std::map<Meshes::Id, ::Mesh *> meshes;
	extern std::map<ShaderPrograms::Id, ::ShaderProgram *> shaderPrograms;
	extern std::map<Textures::Id, ::Texture *> textures;

	void init();
	void uninit();
}