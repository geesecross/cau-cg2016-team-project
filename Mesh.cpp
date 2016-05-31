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

			Face f;
			for (size_t i = 0; i < count; ++i) {
				is >> f.indices[0] >> f.indices[1] >> f.indices[2];
				mesh.faces.push_back(f);

				Vector3f faceNormal = mesh.calcFaceNormal(f, 0).normalized();
				mesh.faceNormals.push_back(faceNormal);
				mesh.vertexNormals[f.indices[0]] += faceNormal * mesh.calcCornerAngle(f, 0);
				mesh.vertexNormals[f.indices[1]] += faceNormal * mesh.calcCornerAngle(f, 1);
				mesh.vertexNormals[f.indices[2]] += faceNormal * mesh.calcCornerAngle(f, 2);
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

	return mesh;
}

const std::vector<Mesh::Vertex>& Mesh::getVertices() const {
	return this->vertices;
}

const std::vector<Vector2f>& Mesh::getTexCoords() const {
	return this->texCoords;
}

void Mesh::draw(const ShaderProgram & shaderProgram) const {

	//TODO: glBindTexture(GL_TEXTURE_2D, texture);
	GLint vertexPositionId = glGetAttribLocation(shaderProgram.getProgramId(), "in_vertexPosition");
	GLint vertexNormalId = glGetAttribLocation(shaderProgram.getProgramId(), "in_vertexNormal");


	if (0 <= vertexPositionId) {
		glEnableVertexAttribArray(vertexPositionId);
		glVertexAttribPointer(vertexPositionId, 3, GL_FLOAT, GL_FALSE, 0, this->vertices.data());
	}
	if (0 <= vertexNormalId) {
		glEnableVertexAttribArray(vertexNormalId);
		glVertexAttribPointer(vertexNormalId, 3, GL_FLOAT, GL_FALSE, 0, this->vertexNormals.data());
	}
	
	glDrawElements(GL_TRIANGLES, this->faces.size() * 3, GL_UNSIGNED_INT, this->faces.data());

	if (0 <= vertexPositionId) {
		glDisableVertexAttribArray(vertexPositionId);
	}
	if (0 <= vertexNormalId) {
		glDisableVertexAttribArray(vertexNormalId);
	}
}