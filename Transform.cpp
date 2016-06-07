#include "Transform.h"
#include "MatrixFactory.h"

Transform & Transform::reset() {
	this->matrix = Matrix4f::identity();
	this->invMatrix = Matrix4f::identity();
	return *this;
}

Transform & Transform::pushPre(const Transform & transform) {
	this->matrix *= transform.getMatrix();
	this->invMatrix = transform.getInvMatrix() * this->invMatrix;
	return *this;
}

Transform & Transform::pushPost(const Transform & transform) {
	this->matrix = transform.getMatrix() * this->matrix;
	this->invMatrix *= transform.getInvMatrix();
	return *this;
}

Transform & Transform::translatePost(const Vector3f & direction) {
	this->matrix = MatrixFactory::translation(direction) * this->matrix;
	this->invMatrix *= MatrixFactory::translation(Vector3f(-direction));
	return *this;
}

Transform & Transform::rotatePost(const Rotation & rotation) {
	this->matrix = rotation.getMatrix() * this->matrix;
	this->invMatrix *= rotation.getInvMatrix();
	return *this;
}

Transform & Transform::scalePost(const Vector3f & coefficient) {
	this->matrix = MatrixFactory::scale(coefficient) * this->matrix;
	this->invMatrix *= MatrixFactory::scale(Vector3f(1.0f / coefficient[0], 1.0f / coefficient[1], 1.0f / coefficient[2]));
	return *this;
}


Transform & Transform::translatePre(const Vector3f & direction) {
	this->matrix *= MatrixFactory::translation(direction);
	this->invMatrix = MatrixFactory::translation(Vector3f(-direction)) * this->invMatrix;
	return *this;
}

Transform & Transform::rotatePre(const Rotation & rotation) {
	this->matrix *= rotation.getMatrix();
	this->invMatrix = rotation.getInvMatrix() * this->invMatrix;
	return *this;
}

Transform & Transform::scalePre(const Vector3f & coefficient) {
	this->matrix *= MatrixFactory::scale(coefficient);
	this->invMatrix = MatrixFactory::scale(Vector3f(1.0f / coefficient[0], 1.0f / coefficient[1], 1.0f / coefficient[2])) * this->invMatrix;
	return *this;
}

const Matrix4f & Transform::getMatrix() const {
	return this->matrix;
}

const Matrix4f & Transform::getInvMatrix() const {
	return this->invMatrix;
}

const Vector3f Transform::transformPoint(const Vector3f & point) const {
	return (this->matrix * Vector4f(point, 1)).xyz();
}

const Vector3f Transform::invTransformPoint(const Vector3f & point) const {
	return (this->invMatrix * Vector4f(point, 1)).xyz();
}

const Vector3f Transform::transformDirection(const Vector3f & direction) const {
	return (this->matrix * Vector4f(direction, 0)).xyz();
}

const Vector3f Transform::invTransformDirection(const Vector3f & direction) const {
	return (this->invMatrix * Vector4f(direction, 0)).xyz();
}
