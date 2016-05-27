#include "Actor.h"

#include "TransformFactory.h"

Actor & Actor::getParent() {
	if (this->isRoot()) {
		return *this;
	}
	return *this->parent;
}

bool Actor::isRoot() const {
	return nullptr == this->parent;
}

std::shared_ptr<Actor> Actor::operator[](const size_t i) {
	if (i >= this->children.size()) {
		return nullptr;
	}

	return this->children[i];
}

Actor::Iterator Actor::begin() {
	return this->children.begin();
}

Actor::Iterator Actor::end() {
	return this->children.end();
}

Actor::ConstIterator Actor::begin() const {
	return this->children.begin();
}

Actor::ConstIterator Actor::end() const {
	return this->children.end();
}

Actor & Actor::moveOrigin(const Vector3f & delta) {
	this->transformMatrix = TransformFactory::translation(delta) * this->transformMatrix;
	return *this;
}

Actor & Actor::rotate(const Rotation & rotation) {
	this->transformMatrix = rotation.getRotationMatrix() * this->transformMatrix;
	return *this;
}

Actor & Actor::scale(const Vector3f & scale) {
	this->transformMatrix = TransformFactory::scale(scale) * this->transformMatrix;
	return *this;
}

Actor & Actor::resetTransform() {
	this->transformMatrix = Matrix4f::identity();
	return *this;
}

const Matrix4f Actor::getWorldMatrix() const {
	if (this->isRoot()) {
		return this->getTransformMatrix();
	}

	return this->parent->getWorldMatrix() * this->getTransformMatrix();
}

const Vector3f Actor::transformPointToWorld(const Vector3f & point) const {
	return (this->getWorldMatrix() * Vector4f(point, 1)).xyz();
}

const Vector3f Actor::transformDirectionToWorld(const Vector3f & direction) const {
	return (this->getWorldMatrix() * Vector4f(direction, 0)).xyz();
}

const Vector3f Actor::getWorldPosition() const {
	return this->transformPointToWorld(Vector3f::zeroVector());
}

const Matrix4f& Actor::getTransformMatrix() const {
	return this->transformMatrix;
}

Actor & Actor::setTransformMatrix(const Matrix4f & matrix) {
	this->transformMatrix = matrix;
	return *this;
}

const Vector3f Actor::transformPoint(const Vector3f & point) const {
	return (this->transformMatrix * Vector4f(point, 1)).xyz();
}

const Vector3f Actor::transformDirection(const Vector3f & direction) const {
	return (this->transformMatrix * Vector4f(direction, 0)).xyz();
}

Actor::Component::~Component() {
}

Actor & Actor::Component::getOwner() const {
	return *this->owner;
}
