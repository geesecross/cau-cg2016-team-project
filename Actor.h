#pragma once
#include "Matrix.h"
#include "Rotation.h"
#include <vector>
#include <list>
#include <memory>

class Actor {
public:
	class Component {
		friend class Actor;
	private:
		Actor * owner = nullptr;	// Actor will initialize this
	public:
		virtual ~Component();
		Actor & getOwner() const;
	};
	using TChildren = std::vector<std::shared_ptr<Actor>>;
	using Iterator = TChildren::iterator;
	using ConstIterator = TChildren::const_iterator;
private:
	Actor * parent = nullptr;
	TChildren children;
	std::list<std::shared_ptr<Component>> components;
	
	Matrix4f transformMatrix;

public:
	// parent operation
	Actor & getParent();
	bool isRoot() const;

	// child operation
	template<typename T, typename... TArgs>
	std::shared_ptr<T> createChild(TArgs... args);
	std::shared_ptr<Actor> operator [](const size_t i);
	Iterator begin();
	Iterator end();
	ConstIterator begin() const;
	ConstIterator end() const;

	// transform modifying operation
	Actor & moveOrigin(const Vector3f & delta);
	Actor & rotate(const Rotation & rotation);
	Actor & scale(const Vector3f & scale);
	Actor & resetTransform();

	// world transform operations
	const Matrix4f getWorldMatrix() const;
	const Vector3f transformPointToWorld(const Vector3f & point) const;
	const Vector3f transformDirectionToWorld(const Vector3f & direction) const;
	const Vector3f getWorldPosition() const;

	// local transform operations
	const Matrix4f & getTransformMatrix() const;
	Actor & setTransformMatrix(const Matrix4f & matrix);
	const Vector3f transformPoint(const Vector3f & point) const;
	const Vector3f transformDirection(const Vector3f & direction) const;

	// component operations
	template<typename TComponent, typename... TArgs>
	std::shared_ptr<TComponent> createComponent(TArgs... args);
	template<typename TComponent>
	std::shared_ptr<TComponent> getComponent();
	template<typename TComponent>
	std::vector<std::shared_ptr<TComponent>> getComponents();
};

template<typename T, typename ...TArgs>
inline std::shared_ptr<T> Actor::createChild(TArgs... args) {
	std::shared_ptr<T> child(new T(args...));
	child->parent = this;
	this->children.push_back(child);
	return child;
}

template<typename TComponent, typename... TArgs>
inline std::shared_ptr<TComponent> Actor::createComponent(TArgs... args) {
	std::shared_ptr<TComponent> c(new TComponent(args...));
	c.get()->owner = this;
	this->components.push_back(c);
	return c;
}

template<typename TComponent>
inline std::shared_ptr<TComponent> Actor::getComponent() {
	std::shared_ptr<TComponent> c;
	for (std::shared_ptr<Component> & component : this->components) {
		if (c = std::dynamic_pointer_cast<TComponent>(component)) {
			return c;
		}
	}

	return c;
}

template<typename TComponent>
inline std::vector<std::shared_ptr<TComponent>> Actor::getComponents() {
	std::vector<std::shared_ptr<TComponent>> vector;
	std::shared_ptr<TComponent> c;
	for (std::shared_ptr<Component> & component : this->components) {
		if (c = std::dynamic_pointer_cast<TComponent>(component)) {
			vector.push_back(c);
		}
	}
	return vector;
}
