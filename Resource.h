#pragma once
#include <map>
#include "Mesh.h"
#include "ShaderProgram.h"

namespace Resource {
	enum MeshId {
		Plane,
		Arrow,
		Cube,
	};
	enum ShaderProgramId {
		Phong,
	};

	extern std::map<MeshId, Mesh *> meshes;
	extern std::map<ShaderProgramId, ShaderProgram *> shaderPrograms;

	void init();
	void uninit();
}