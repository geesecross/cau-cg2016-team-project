#pragma once
#include "Matrix.h"

class Rotation {
private:
	Matrix4f matrix, invMatrix;

public:
	Rotation & rotateByEuler(const Vector3f & degree);
	const Matrix4f & getMatrix() const;
	const Matrix4f & getInvMatrix() const;

	const Vector3f transform(const Vector3f & vector) const;
	const Vector3f invTransform(const Vector3f & vector) const;
};
