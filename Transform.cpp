#include "Transform.h"
#include "MatrixFactory.h"

Transform & Transform::reset() {
	this->transformMatrix = Matrix4f::identity();
	return *this;
}

Transform & Transform::translate(const Vector3f & direction) {
	this->transformMatrix = MatrixFactory::translation(direction) * this->transformMatrix;
	return *this;
}

Transform & Transform::rotate(const Rotation & rotation) {
	this->transformMatrix = rotation.getRotationMatrix() * this->transformMatrix;
	return *this;
}

Transform & Transform::scale(const Vector3f & coefficient) {
	this->transformMatrix = MatrixFactory::scale(coefficient) * this->transformMatrix;
	return *this;
}

const Matrix4f Transform::getMatrix() const {
	return this->transformMatrix;
}

const Vector3f Transform::transformPoint(const Vector3f & point) {
	return (this->transformMatrix * Vector4f(point, 1)).xyz();
}

const Vector3f Transform::transformDirection(const Vector3f & direction) {
	return (this->transformMatrix * Vector4f(direction, 0)).xyz();
}
