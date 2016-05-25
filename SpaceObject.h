#pragma once
#include "Matrix.h"

class SpaceObject {
private:
	Matrix4f transformMatrix;

public:
	const Vector3f getWorldPosition() const;

	const Matrix4f & getTransformMatrix() const;
	SpaceObject & setTransformMatrix(const Matrix4f & transformMatrix);
};
