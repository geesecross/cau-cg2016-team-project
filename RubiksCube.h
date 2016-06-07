#pragma once
#include "Actor.h"
#include "Block.h"
#include "Animation.h"
#include "CommandQueue.h"
#include "Event.h"
#include <vector>

class RubiksCube : public Actor {
	friend class TwistAnimation;
public:
	class Cursor : public Actor {
	public:
		using OnCursorMoved = Event<void(const Cursor &, const Vector2f &)>;
		using OnCursorRotated = Event<void(const Cursor &, const bool)>;
	private:
		RubiksCube & cube;
		Vector3f position;
		Rotation rotation;
		void updateTransform();
	public:
		OnCursorMoved onCursorMoved;
		OnCursorRotated onCursorRotated;
		Cursor(RubiksCube & cube);
		Cursor & move(const Vector2f & vector);
		Vector3f getSelected();
		void twist(bool clockwise);
		void rotateAxis(bool clockwise);
		void resetPositionAndRotation();
	};
private:
	const size_t size;
	std::weak_ptr<AnimationManager> animationManager;
	std::weak_ptr<Cursor> cursor;
	CommandQueue twistQueue;

public:
	typedef std::vector<std::vector<std::vector<std::weak_ptr<Block>>>> BlockArray;
	BlockArray blocks;
	Event<void()> onFinishedTwist;

	RubiksCube(const size_t size, std::weak_ptr<AnimationManager> animationManager);

	std::shared_ptr<Cursor> getCursor();

	void twist(const size_t index, const Rotation & rotation);
	size_t getSize() const;
	void resetBlocksAndCursor();
	bool isTwistFinished() const;
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