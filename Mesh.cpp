#define _USE_MATH_DEFINES
#include <cmath>
#include "Mesh.h"

Vector3f Mesh::calcFaceNormal(const Face & face, size_t cornerIndex) const {
	const Vector3f & v1 = this->vertices[face.indices[cornerIndex]];
	const Vector3f & v2 = this->vertices[face.indices[(cornerIndex + 1) % 3]];
	const Vector3f & v3 = this->vertices[face.indices[(cornerIndex + 2) % 3]];
	return Vector3f((v2 - v1).normalized()).cross((v3 - v1).normalized());
}

float Mesh::calcCornerAngle(const Face & face, size_t cornerIndex) const
{
	return std::asinf(this->calcFaceNormal(face, cornerIndex).length()) / (float)(M_PI * 2);
}

Mesh Mesh::createFromDatFile(const std::string & filename) {
	Mesh mesh;

	std::ifstream is(filename);
	std::string fieldName;

	while (!is.eof()) {
		fieldName = "";	// no more bug again
		is >> fieldName;

		if ("VERTEX" == fieldName) {
			std::string skip;
			size_t count;
			is >> skip >> count;

			Vertex v;
			for (size_t i = 0; i < count; ++i) {
				is >> v[0] >> v[1] >> v[2];
				mesh.vertices.push_back(v);
				mesh.vertexNormals.push_back(Vector3f());
			}
		}
		else if ("FACE" == fieldName) {
			std::string skip;
			size_t count;
			is >> skip >> count;

			Face face;
			for (size_t i = 0; i < count; ++i) {
				is >> face.indices[0] >> face.indices[1] >> face.indices[2];
				Vector3f faceNormal = mesh.calcFaceNormal(face, 0).normalized();
				face.normal = faceNormal;

				mesh.faces.push_back(face);
				mesh.vertexNormals[face.indices[0]] += faceNormal * mesh.calcCornerAngle(face, 0);
				mesh.vertexNormals[face.indices[1]] += faceNormal * mesh.calcCornerAngle(face, 1);
				mesh.vertexNormals[face.indices[2]] += faceNormal * mesh.calcCornerAngle(face, 2);
			}
		}
		else if ("TEXCOORD" == fieldName) {
			std::string skip;
			size_t count;
			is >> skip >> count;

			Vector2f v;
			for (size_t i = 0; i < count; ++i) {
				is >> v[0] >> v[1];
				mesh.texCoords.push_back(v);
			}
		}

	}
	is.close();

	for (Vector3f & normal : mesh.vertexNormals) {
		normal = normal.normalized();
	}
	
	if (mesh.texCoords.size() == mesh.vertices.size()) {
		for (Face & face : mesh.faces) {
			Vector3f edge1 = mesh.vertices[face.indices[1]] - mesh.vertices[face.indices[0]];
			Vector3f edge2 = mesh.vertices[face.indices[2]] - mesh.vertices[face.indices[0]];
			Vector2f deltaUV1 = mesh.texCoords[face.indices[1]] - mesh.texCoords[face.indices[0]];
			Vector2f deltaUV2 = mesh.texCoords[face.indices[2]] - mesh.texCoords[face.indices[0]];

			//float det = 1.0f / (deltaUV1[0] * deltaUV2[1] - deltaUV2[0] * deltaUV1[1]);

			face.tangent = Vector3f(
				deltaUV2[1] * edge1[0] - deltaUV1[1] * edge2[0],
				deltaUV2[1] * edge1[1] - deltaUV1[1] * edge2[1],
				deltaUV2[1] * edge1[2] - deltaUV1[1] * edge2[2]
			).normalized();
			face.bitangent = Vector3f(
				-deltaUV2[0] * edge1[0] + deltaUV1[0] * edge2[0],
				-deltaUV2[0] * edge1[1] + deltaUV1[0] * edge2[1],
				-deltaUV2[0] * edge1[2] + deltaUV1[0] * edge2[2]
			).normalized();
		}
	}

	return mesh;
}

const std::vector<Mesh::Vertex>& Mesh::getVertices() const {
	return this->vertices;
}

const std::vector<Vector2f>& Mesh::getTexCoords() const {
	return this->texCoords;
}

const std::vector<Vector3f>& Mesh::getVertexNormals() const {
	return this->vertexNormals;
}

void Mesh::draw(const ShaderProgram & shaderProgram, const Face & face) const {
	GLint objectId;
	if (0 <= (objectId = glGetUniformLocation(shaderProgram.getProgramId(), "in_surfaceNormal"))) {
		glUniform3fv(objectId, 1, face.normal.data());
	}
	if (0 <= (objectId = glGetUniformLocation(shaderProgram.getProgramId(), "in_tangentVector"))) {
		glUniform3fv(objectId, 1, face.tangent.data());
	}
	if (0 <= (objectId = glGetUniformLocation(shaderProgram.getProgramId(), "in_bitangentVector"))) {
		glUniform3fv(objectId, 1, face.bitangent.data());
	}
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, face.indices);
}

void Mesh::draw(const ShaderProgram & shaderProgram) const {
	GLint vertexPositionId = glGetAttribLocation(shaderProgram.getProgramId(), "in_vertexPosition");
	GLint vertexNormalId = glGetAttribLocation(shaderProgram.getProgramId(), "in_vertexNormal");
	GLint vertexTangentId = glGetAttribLocation(shaderProgram.getProgramId(), "in_vertexTangent");

	if (0 <= vertexPositionId) {
		glEnableVertexAttribArray(vertexPositionId);
		glVertexAttribPointer(vertexPositionId, 3, GL_FLOAT, GL_FALSE, 0, this->vertices.data());
	}
	if (0 <= vertexNormalId) {
		glEnableVertexAttribArray(vertexNormalId);
		glVertexAttribPointer(vertexNormalId, 3, GL_FLOAT, GL_FALSE, 0, this->vertexNormals.data());
	}

	for (const Face & face : this->faces) {
		this->draw(shaderProgram, face);
	}

	if (0 <= vertexPositionId) {
		glDisableVertexAttribArray(vertexPositionId);
	}
	if (0 <= vertexNormalId) {
		glDisableVertexAttribArray(vertexNormalId);
	}
}