#include "Resource.h"
#include "SimpleIlluminationModelShaderProgram.h"

namespace Resource {
	std::map<MeshId, Mesh *> meshes;
	std::map<ShaderProgramId, ShaderProgram *> shaderPrograms;

	void init() {
		shaderPrograms[Phong] = new SimpleIlluminationModelShaderProgram(
			SimpleIlluminationModelShaderProgram::createPhong()
			.setLightVector({ 100, 100, 100 })
			.enableLightVectorAsPosition(true)
		);
		meshes[Plane] = new Mesh(Mesh::createFromDatFile("models/plane.dat"));
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