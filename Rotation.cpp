#define _USE_MATH_DEFINES

#include "Rotation.h"
#include "MatrixFactory.h"
#include <cmath>

Rotation & Rotation::rotateByEuler(const Vector3f & euler) {
	this->matrix = MatrixFactory::rotation((float)(euler[2] * M_PI / 180), MatrixFactory::Z)
		* MatrixFactory::rotation((float)(euler[1] * M_PI / 180), MatrixFactory::Y)
		* MatrixFactory::rotation((float)(euler[0] * M_PI / 180), MatrixFactory::X)
		* this->matrix;
	this->invMatrix *= MatrixFactory::rotation((float)(-euler[0] * M_PI / 180), MatrixFactory::X)
		* MatrixFactory::rotation((float)(-euler[1] * M_PI / 180), MatrixFactory::Y)
		* MatrixFactory::rotation((float)(-euler[2] * M_PI / 180), MatrixFactory::Z);
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
