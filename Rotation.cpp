#define _USE_MATH_DEFINES

#include "Rotation.h"
#include "TransformFactory.h"
#include <cmath>

Rotation & Rotation::rotateByEuler(const Vector3f & euler) {
	this->matrix = TransformFactory::rotation((float)(euler[2] * M_PI / 180), TransformFactory::Z)
		* TransformFactory::rotation((float)(euler[1] * M_PI / 180), TransformFactory::Y)
		* TransformFactory::rotation((float)(euler[0] * M_PI / 180), TransformFactory::X)
		* this->matrix;
	return *this;
}

const Matrix4f Rotation::getRotationMatrix() const {
	return this->matrix;
}
