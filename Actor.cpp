#include "Actor.h"

#include "MatrixFactory.h"

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

Actor & Actor::setTransform(const Transform & transform) {
	this->transform = transform;
	return *this;
}

Transform & Actor::getTransform() {
	return this->transform;
}

const Transform & Actor::getTransform() const {
	return this->transform;
}

const Matrix4f Actor::getWorldMatrix() const {
	if (this->isRoot()) {
		return this->transform.getMatrix();
	}

	return this->parent->getWorldMatrix() * this->transform.getMatrix();
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

Actor::Component::~Component() {
}

Actor & Actor::Component::getOwner() const {
	return *this->owner;
}
