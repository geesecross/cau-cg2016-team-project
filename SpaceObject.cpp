#include "SpaceObject.h"

const Vector3f SpaceObject::getWorldPosition() const {
	Vector4f pos = this->transformMatrix * Vector4f({ 0, 0, 0, 1 });
	return Vector3f({ pos[0], pos[1], pos[2] });
}

const Matrix4f& SpaceObject::getTransformMatrix() const {
	return this->transformMatrix;
}

SpaceObject & SpaceObject::setTransformMatrix(const Matrix4f& transformMatrix) {
	this->transformMatrix = transformMatrix;
	return *this;
}
