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

		textures[Textures::TexturePng] = new Texture(SOIL_load_OGL_texture(
			"resources/metalDif.jpg",
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
		));
		textures[Textures::TextureSpecular] = new Texture(SOIL_load_OGL_texture(
			"resources/fieldstone_SM.png",
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
			));
		textures[Textures::TextureNormal] = new Texture(SOIL_load_OGL_texture(
			"resources/metalNorm.jpg",
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
			));

		/*textures[TextureNormal] = new Texture(Texture::loadTextureFromFile("resources/metalNorm.jpg"));*/
		textures[Textures::TextureHeight] = new Texture(SOIL_load_OGL_texture(
			"resources/metalHeight.jpg",
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
		));
		textures[Textures::WaterNormal] = new Texture(SOIL_load_OGL_texture(
			"resources/water_normal.png",
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
		));
		textures[Textures::Cobble] = createTextureFromFile("brickwall.jpg");
		textures[Textures::CobbleNormal] = createTextureFromFile("brickwall_normal.jpg");
		/*
		textures[Textures::Skybox] = new Texture(SOIL_load_OGL_texture(
			"resources/skybox.jpg",
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
		));
		*/
		textures[Textures::Skybox] = new Texture(Texture::loadTextureFromFile("resources/skybox.jpg"));

		shaderPrograms[ShaderPrograms::Phong] = new SimpleIlluminationModelShaderProgram(
			SimpleIlluminationModelShaderProgram::createPhong()
			.setLightVector({ 100, 100, 100 })
			.enableLightVectorAsPosition(true)
			);
		shaderPrograms[ShaderPrograms::Phong2] = new SimpleIlluminationModelShaderProgram(
			SimpleIlluminationModelShaderProgram::createPhong()
			.setLightVector({ -1000, -1000, -1000 })
			.enableLightVectorAsPosition(true)
		);
		shaderPrograms[ShaderPrograms::SimpleTexture] = new TextureShaderProgram(
			static_cast<TextureShaderProgram &>(
				TextureShaderProgram::create()
				.setLightVector({ 100, 100, 100 })
				.enableLightVectorAsPosition(true)
				.setSpecularRatio(10)
			)
		);
		shaderPrograms[ShaderPrograms::NormalMap] = new NormalMappingProgram(
			static_cast<NormalMappingProgram&>(
			NormalMappingProgram::create()
			.setLightVector({ 100, 100, 100 })
			.enableLightVectorAsPosition(true)
				)
		);
		shaderPrograms[ShaderPrograms::Parallax] = new ParallaxOcclusionProgram(
			static_cast<ParallaxOcclusionProgram&>(
				ParallaxOcclusionProgram::create()
				.setLightVector({ 100, 100, 100 })
				.enableLightVectorAsPosition(true)
				)
		);
		shaderPrograms[ShaderPrograms::Water] = new MovingTexShader(
			static_cast<MovingTexShader&>(
				MovingTexShader::create()
				.setAmbientRatio(0.4f)
				.setDiffusionRatio(0.2f)
				.setSpecularRatio(0.8f)
				.setShiness(16)
				.setLightVector({ 100, 100, 100 })
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
		shaderPrograms[ShaderPrograms::Cobble] = new TextureShaderProgram(
			static_cast<TextureShaderProgram &>(
				TextureShaderProgram::create()
				.setAmbientRatio(0.4f)
				.setDiffusionRatio(0.3f)
				.setSpecularRatio(0.3f)
				.setShiness(16)
				.setLightVector({ 100, 100, 100 })
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