#pragma once

#include <GL/glew.h>
#include <GL/glut.h>

#include "Vector.h"
#include "Matrix.h"
#include "ShaderProgram.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Mesh {
public:
	typedef Vector3f Vertex;
	struct Face {
		GLuint indices[3];
		Vector3f normal;
		Vector3f tangent, bitangent;
	};

private:
	std::vector<Vertex> vertices;
	std::vector<Face> faces;
	std::vector<Vector3f> vertexNormals;
	std::vector<Vector2f> texCoords;

	Vector3f calcFaceNormal(const Face & face, size_t cornerIndex) const;
	float calcCornerAngle(const Face & face, size_t cornerIndex) const;

public:
	static Mesh createFromDatFile(const std::string & filename);

	const std::vector<Vertex> & getVertices() const;
	const std::vector<Vector2f> & getTexCoords() const;
	const std::vector<Vector3f> & getVertexNormals() const;
	void draw(const ShaderProgram & shaderProgram, const Face & face) const;
	void draw(const ShaderProgram & shaderProgram) const;
};