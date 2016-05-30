#pragma once
#include "Actor.h"
#include "Block.h"
#include "Animation.h"
#include "CommandQueue.h"
#include <vector>

class RubiksCube : public Actor {
	friend class TwistAnimation;
public:
	class Cursor : public Actor {
	private:
		RubiksCube & cube;
		Vector3f position;
		Rotation rotation;
		void updateTransform();
	public:
		Cursor(RubiksCube & cube);
		Cursor & move(const Vector2f & vector);
		Vector3f getSelected();
		void twist(bool clockwise);
		void rotateAxis(bool clockwise);
		void reset();
	};
private:
	const size_t size;
	std::weak_ptr<AnimationManager> animationManager;
	std::weak_ptr<Cursor> cursor;
	CommandQueue commandQueue;

public:
	typedef std::vector<std::vector<std::vector<std::weak_ptr<Block>>>> BlockArray;
	BlockArray blocks;

	RubiksCube(const size_t size, std::weak_ptr<AnimationManager> animationManager);

	std::shared_ptr<Cursor> getCursor();

	void twist(const size_t index, const Rotation & rotation);
	size_t getSize() const;
	void reset();
};

class TwistAnimation : public Animation {
private:
	int count = 0;
	RubiksCube & cube;
	size_t index;
	Rotation rotation;
	std::vector<Transform> initialTransforms;

public:
	TwistAnimation(RubiksCube & cube, size_t index, const Rotation & rotation);
	virtual void onStart();
	virtual bool stepFrame(const double timeElpased, const double timeDelta);
	virtual void onFinished();
	virtual void onInterruted();

	void twist(const float angle);
};