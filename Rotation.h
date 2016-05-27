#pragma once
#include "Matrix.h"

class Rotation {
private:
	Matrix4f matrix;

public:
	Rotation & rotateByEuler(const Vector3f & degree);
	const Matrix4f getRotationMatrix() const;

	const Vector3f transform(const Vector3f & vector) const;
};
