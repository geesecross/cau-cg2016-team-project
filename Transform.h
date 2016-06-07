#pragma once
#include "Rotation.h"

class Transform {
private:
	Matrix4f matrix, invMatrix;

public:
	Transform & reset();

	Transform & pushPre(const Transform & transform);
	Transform & pushPost(const Transform & transform);

	Transform & translatePost(const Vector3f & direction);
	Transform & rotatePost(const Rotation & rotation);
	Transform & scalePost(const Vector3f & coefficient);

	Transform & translatePre(const Vector3f & direction);
	Transform & rotatePre(const Rotation & rotation);
	Transform & scalePre(const Vector3f & coefficient);

	const Matrix4f & getMatrix() const;
	const Matrix4f & getInvMatrix() const;

	const Vector3f transformPoint(const Vector3f & point) const;
	const Vector3f invTransformPoint(const Vector3f & point) const;
	const Vector3f transformDirection(const Vector3f & direction) const;
	const Vector3f invTransformDirection(const Vector3f & direction) const;
};
