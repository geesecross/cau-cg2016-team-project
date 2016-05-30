#pragma once

#include "RubiksCube.h"
#include "Camera.h"

class PrintStringAnimaiton;

class GameRule {
	friend class PrintStringAnimation;
private:
	Event<void()>::MemberFunctionListener<GameRule> onFinishedTwistListener;
	std::weak_ptr<RubiksCube> rubiksCube;
	std::weak_ptr<AnimationManager> animationManager;
	std::weak_ptr<Camera> camera;
	std::shared_ptr<PrintStringAnimation> messageAnimation;
	bool gameStarted;

	bool isAllBlockAligned(Vector3f std_vector) const;
	void print(const std::string & message);
	void onFinishedTwist();
public:
	GameRule(std::weak_ptr<RubiksCube> cube, std::weak_ptr<AnimationManager> animationManager, std::weak_ptr<Camera> camera);
	void reset();
	void scramble();
	bool judge();
	void win();
	bool isStarted() const;
};

class PrintStringAnimation : public Animation {
private:
	GameRule & gameRule;
	std::weak_ptr<Camera> camera;
	std::string message;
public:
	PrintStringAnimation(GameRule & gameRule, std::weak_ptr<Camera> camera, const std::string & message);
	bool stepFrame(const double timeElapsed, const double timeDelta) override;
};

