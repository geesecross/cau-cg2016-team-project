#include "Resource.h"
#include "SimpleIlluminationModelShaderProgram.h"

namespace Resource {
	std::map<MeshId, Mesh *> meshes;
	std::map<ShaderProgramId, ShaderProgram *> shaderPrograms;

	void init() {
		shaderPrograms[Phong] = new SimpleIlluminationModelShaderProgram(
			SimpleIlluminationModelShaderProgram::createPhong()
			.setLightVector({ 1000, 1000, 1000 })
			.enableLightVectorAsPosition(true)
		);
		shaderPrograms[Phong2] = new SimpleIlluminationModelShaderProgram(
			SimpleIlluminationModelShaderProgram::createPhong()
			.setLightVector({ -1000, -1000, -1000 })
			.enableLightVectorAsPosition(true)
		);
		meshes[Plane] = new Mesh(Mesh::createFromDatFile("models/plane.dat"));
		meshes[Arrow] = new Mesh(Mesh::createFromDatFile("models/arrow.dat"));
		meshes[Cube] = new Mesh(Mesh::createFromDatFile("models/cube.dat"));
		meshes[Teapot] = new Mesh(Mesh::createFromDatFile("models/Teapot.dat"));
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