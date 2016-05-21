#pragma once
#pragma once
#include "Mesh.hpp"

class Model {
private:
	const Mesh * mesh = nullptr;
	Vector4f color;
	Matrix4f transformMatrix;

public:
	Model();

	const Vector3f getWorldPosition() const;

	const Mesh * getMesh() const;
	Model & setMesh(const Mesh * mesh);

	const Vector4f & getColor() const;
	Model & setColor(const Vector4f & color);

	const Matrix4f & getTransformMatrix() const;
	Model & setTransformMatrix(const Matrix4f & transformMatrix);
};