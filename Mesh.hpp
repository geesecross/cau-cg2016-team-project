#pragma once

#include <GL/glew.h>
#include <GL/glut.h>

#include "Vector.hpp"
#include "Matrix.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Mesh {
public:
	typedef Vector3f Vertex;
	struct Face {
		GLushort indices[3];
	};

private:
	std::vector<Vertex> vertices;
	std::vector<Face> faces;

public:
	static Mesh * createFromDatFile(const std::string & filename);
	void draw(GLuint vertexAttribId) const;
};