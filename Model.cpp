#include "Model.h"

Model::Model() {
	this->color = Vector4f({ 0, 0, 0, 0 });
}

const Vector3f Model::getWorldPosition() const {
	Vector4f pos = this->transformMatrix * Vector4f({ 0, 0, 0, 1 });
	return Vector3f({ pos[0], pos[1], pos[2] });
}

const Mesh * Model::getMesh() const {
	return this->mesh;
}

Model & Model::setMesh(const Mesh * mesh) {
	this->mesh = mesh;
	return *this;
}

const Vector4f& Model::getColor() const {
	return this->color;
}

Model & Model::setColor(const Vector4f & color) {
	this->color = color;
	return *this;
}

const Matrix4f& Model::getTransformMatrix() const {
	return this->transformMatrix;
}

Model & Model::setTransformMatrix(const Matrix4f& transformMatrix) {
	this->transformMatrix = transformMatrix;
	return *this;
}
