#include "Resource.h"
#include "SimpleIlluminationModelShaderProgram.h"
#include "TextureShaderProgram.h"
#include "NormalMappingProgram.h"
#include "ParallaxOcclusionProgram.h"
#include "MovingTexShader.h"
#include <GL/SOIL.h>

Texture * createTextureFromFile(const std::string & fileName) {
	std::string path = "resources/" + fileName;
	Texture * tex = new Texture(SOIL_load_OGL_texture(
		path.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
	));
	return tex;
}

Vector3f sun = { 25, 100, 25 };

namespace Resource {
	std::map<Meshes::Id, Mesh *> meshes;
	std::map<ShaderPrograms::Id, ShaderProgram *> shaderPrograms;
	std::map<Textures::Id, Texture *> textures;

	void init() {
		meshes[Meshes::Plane] = new Mesh(Mesh::createFromDatFile("resources/plane.dat"));
		meshes[Meshes::Arrow] = new Mesh(Mesh::createFromDatFile("resources/arrow.dat"));
		meshes[Meshes::Cube] = new Mesh(Mesh::createFromDatFile("resources/cube.dat"));
		meshes[Meshes::Teapot] = new Mesh(Mesh::createFromDatFile("resources/Teapot.dat"));
		meshes[Meshes::Particle] = new Mesh(Mesh::createFromDatFile("resources/particle.dat"));
		meshes[Meshes::Skybox] = new Mesh(Mesh::createFromDatFile("resources/skybox.dat"));

		textures[Textures::Metal] = createTextureFromFile("metalDif.jpg");
		textures[Textures::TextureSpecular] = createTextureFromFile("fieldstone_SM.png");
		textures[Textures::MetalNormal] = createTextureFromFile("metalNorm.jpg");
		textures[Textures::MetalHeight] = createTextureFromFile("metalHeight.jpg");
		textures[Textures::WaterNormal] = createTextureFromFile("water_normal.png");
		textures[Textures::Cobble] = createTextureFromFile("cobble.png");
		textures[Textures::CobbleNormal] = createTextureFromFile("cobble_normal.png");
		textures[Textures::CobbleHeight] = createTextureFromFile("cobble_height.png");
		textures[Textures::CobbleSpecular] = createTextureFromFile("cobble_specular.png");
		textures[Textures::Cushion] = createTextureFromFile("cushion.jpg");
		textures[Textures::CushionNormal] = createTextureFromFile("cushion_normal.png");
		textures[Textures::Wood] = createTextureFromFile("wood1-dif-1024p.tga");
		textures[Textures::WoodNormal] = createTextureFromFile("wood1-nor-1024p.tga");
		textures[Textures::WoodSpecular] = createTextureFromFile("wood1-spec-1024p.tga");
		textures[Textures::Skybox] = new Texture(Texture::loadTextureFromFile("resources/skybox.jpg"));

		shaderPrograms[ShaderPrograms::Phong] = new SimpleIlluminationModelShaderProgram(
			SimpleIlluminationModelShaderProgram::createPhong()
			.setLightVector(sun)
			.enableLightVectorAsPosition(true)
		);
		shaderPrograms[ShaderPrograms::SimpleTexture] = new TextureShaderProgram(
			static_cast<TextureShaderProgram &>(
				TextureShaderProgram::create()
				.setLightVector(sun)
				.enableLightVectorAsPosition(true)
				.setSpecularRatio(10)
			)
		);
		shaderPrograms[ShaderPrograms::MetalNormal] = new NormalMappingProgram(
			static_cast<NormalMappingProgram&>(
				NormalMappingProgram::create()
				.setAmbientRatio(0.3f)
				.setDiffusionRatio(0.7f)
				.setSpecularRatio(0.5f)
				.setLightVector(sun)
				.enableLightVectorAsPosition(true)
			)
		);
		shaderPrograms[ShaderPrograms::MetalParallax] = new ParallaxOcclusionProgram(
			static_cast<ParallaxOcclusionProgram&>(
				ParallaxOcclusionProgram::create()
				.setAmbientRatio(0.3f)
				.setDiffusionRatio(0.6f)
				.setSpecularRatio(0.5f)
				.setLightVector(sun)
				.enableLightVectorAsPosition(true)
			)
		);
		shaderPrograms[ShaderPrograms::Water] = new MovingTexShader(
			static_cast<MovingTexShader&>(
				MovingTexShader::create()
				.setAmbientRatio(0.2f)
				.setDiffusionRatio(0.4f)
				.setSpecularRatio(0.8f)
				.setShiness(16)
				.setLightVector(sun)
				.enableLightVectorAsPosition(true)
			)
		);
		shaderPrograms[ShaderPrograms::Skybox] = new TextureShaderProgram(
			static_cast<TextureShaderProgram &>(
				TextureShaderProgram::create()
				.setAmbientRatio(1.0f)
				.setDiffusionRatio(0.0f)
				.setSpecularRatio(0.0f)
			)
		);
		shaderPrograms[ShaderPrograms::Cobble] = new ParallaxOcclusionProgram(
			static_cast<ParallaxOcclusionProgram &>(
				ParallaxOcclusionProgram::create()
				.setAmbientRatio(0.4f)
				.setDiffusionRatio(0.6f)
				.setSpecularRatio(0.5f)
				.setShiness(32)
				.setLightVector(sun)
				.enableLightVectorAsPosition(true)
			)
		);
		shaderPrograms[ShaderPrograms::Cushion] = new NormalMappingProgram(
			static_cast<NormalMappingProgram &>(
				NormalMappingProgram::create()
				.setAmbientRatio(0.6f)
				.setDiffusionRatio(0.6f)
				.setSpecularRatio(0.2f)
				.setShiness(2)
				.setLightVector(sun)
				.enableLightVectorAsPosition(true)
			)
		);
		shaderPrograms[ShaderPrograms::Wood] = new NormalMappingProgram(
			static_cast<NormalMappingProgram &>(
				NormalMappingProgram::create()
				.setAmbientRatio(0.4f)
				.setDiffusionRatio(0.9f)
				.setSpecularRatio(0.2f)
				.setShiness(2)
				.setLightVector(sun)
				.enableLightVectorAsPosition(true)
				)
		);
	}

	void uninit() {
		for (const std::pair<Meshes::Id, Mesh *> & pair : meshes) {
			delete pair.second;
		}
		for (const std::pair<ShaderPrograms::Id, ShaderProgram *> & pair : shaderPrograms) {
			delete pair.second;
		}
	}
}