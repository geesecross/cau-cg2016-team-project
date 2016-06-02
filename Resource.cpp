#include "Resource.h"
#include "SimpleIlluminationModelShaderProgram.h"
#include "TextureShaderProgram.h"
#include <GL/SOIL.h>

namespace Resource {
	std::map<MeshId, Mesh *> meshes;
	std::map<ShaderProgramId, ShaderProgram *> shaderPrograms;
	std::map<TextureId, Texture *> textures;

	void init() {
		meshes[Plane] = new Mesh(Mesh::createFromDatFile("resources/plane.dat"));
		meshes[Arrow] = new Mesh(Mesh::createFromDatFile("resources/arrow.dat"));
		meshes[Cube] = new Mesh(Mesh::createFromDatFile("resources/cube.dat"));
		meshes[Teapot] = new Mesh(Mesh::createFromDatFile("resources/Teapot.dat"));
		meshes[Particle] = new Mesh(Mesh::createFromDatFile("resources/particle.dat"));

		textures[TexturePng] = new Texture(SOIL_load_OGL_texture(
			"resources/Fieldstone_DM.png",
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS
		));
		textures[TextureSpecular] = new Texture(SOIL_load_OGL_texture(
			"resources/fieldstone_SM.png",
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS
			));
		textures[TextureNormal] = new Texture(SOIL_load_OGL_texture(
			"resources/fieldstone_NM.png",
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS
			));
		shaderPrograms[Phong] = new SimpleIlluminationModelShaderProgram(
			SimpleIlluminationModelShaderProgram::createPhong()
			.setLightVector({ 0, 0, 50 })
			.enableLightVectorAsPosition(true)
			);
		shaderPrograms[Phong2] = new SimpleIlluminationModelShaderProgram(
			SimpleIlluminationModelShaderProgram::createPhong()
			.setLightVector({ -1000, -1000, -1000 })
			.enableLightVectorAsPosition(true)
		);
		shaderPrograms[SimpleTexture] = new TextureShaderProgram(
			static_cast<TextureShaderProgram &>(
				TextureShaderProgram::create()
				.setLightVector({ 0, 1, 10 })
				.enableLightVectorAsPosition(true)
.setSpecularRatio(10)
			)
		);
	}

	void uninit() {
		for (const std::pair<MeshId, Mesh *> & pair : meshes) {
			delete pair.second;
		}
		for (const std::pair<ShaderProgramId, ShaderProgram *> & pair : shaderPrograms) {
			delete pair.second;
		}
	}
}