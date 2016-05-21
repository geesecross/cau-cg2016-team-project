#include "Mesh.hpp"

Mesh * Mesh::createFromDatFile(const std::string & filename) {
	Mesh * model = new Mesh();

	std::ifstream is(filename);
	std::string fieldName;

	while (!is.eof()) {
		is >> fieldName;

		if ("VERTEX" == fieldName) {
			std::string skip;
			size_t count;
			is >> skip >> count;

			Vertex v;
			for (size_t i = 0; i < count; ++i) {
				is >> v[0] >> v[1] >> v[2];
				model->vertices.push_back(v);
			}
		}
		else if ("FACE" == fieldName) {
			std::string skip;
			size_t count;
			is >> skip >> count;

			Face f;
			for (size_t i = 0; i < count; ++i) {
				is >> f.indices[0] >> f.indices[1] >> f.indices[2];
				model->faces.push_back(f);
			}
		}
	}
	is.close();

	return model;
}

void Mesh::draw(GLuint vertexAttribId) const {
	glEnableVertexAttribArray(vertexAttribId); {
		glVertexAttribPointer(vertexAttribId, 3, GL_FLOAT, GL_FALSE, 0, this->vertices.data());
		glDrawElements(GL_TRIANGLES, this->faces.size() * 3, GL_UNSIGNED_SHORT, this->faces.data());
	}
	glDisableVertexAttribArray(vertexAttribId);
}
