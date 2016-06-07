#define _USE_MATH_DEFINES

#include "Rotation.h"
#include "MatrixFactory.h"
#include <cmath>

Rotation & Rotation::rotateByEuler(const Vector3f & euler) {
	this->matrix = MatrixFactory::rotation(euler[2], MatrixFactory::Z)
		* MatrixFactory::rotation(euler[1], MatrixFactory::Y)
		* MatrixFactory::rotation(euler[0], MatrixFactory::X)
		* this->matrix;
	this->invMatrix *= MatrixFactory::rotation(-euler[0], MatrixFactory::X)
		* MatrixFactory::rotation(-euler[1], MatrixFactory::Y)
		* MatrixFactory::rotation(-euler[2], MatrixFactory::Z);
	return *this;
}

const Matrix4f & Rotation::getMatrix() const {
	return this->matrix;
}

const Matrix4f & Rotation::getInvMatrix() const {
	return this->invMatrix;
}

const Vector3f Rotation::transform(const Vector3f & vector) const {
	return (this->getMatrix() * Vector4f(vector, 1)).xyz();
}

const Vector3f Rotation::invTransform(const Vector3f & vector) const {
	return (this->getInvMatrix() * Vector4f(vector, 1)).xyz();
}
