#pragma once
#include "Rotation.h"

class Transform {
private:
	Matrix4f transformMatrix;

public:
	Transform & reset();
	Transform & translate(const Vector3f & direction);
	Transform & rotate(const Rotation & rotation);
	Transform & scale(const Vector3f & coefficient);
	const Matrix4f getMatrix() const;

	const Vector3f transformPoint(const Vector3f & point);
	const Vector3f transformDirection(const Vector3f & direction);
};
