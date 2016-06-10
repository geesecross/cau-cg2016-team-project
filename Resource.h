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
		};
	}
	namespace Textures {
		enum Id {
			Metal,
			MetalNormal,
			MetalHeight,
			WaterNormal,
			Skybox,
			Cobble,
			CobbleNormal,
			CobbleHeight,
			CobbleSpecular,
			AlphaCircle,
		};
	}
	extern std::map<Meshes::Id, ::Mesh *> meshes;
	extern std::map<ShaderPrograms::Id, ::ShaderProgram *> shaderPrograms;
	extern std::map<Textures::Id, ::Texture *> textures;

	void init();
	void uninit();
}