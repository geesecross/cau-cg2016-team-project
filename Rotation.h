#pragma once
#include "Matrix.h"

class Rotation {
private:
	Matrix4f matrix;

public:
	Rotation & rotateByEuler(const Vector3f & euler);
	const Matrix4f getRotationMatrix() const;
};
