#pragma once

#include "RubiksCube.h"

class PrintStringAnimaiton;

class GameRule {
	friend class PrintStringAnimation;
private:
	Event<void()>::MemberFunctionListener<GameRule> onFinishedTwistListener;
	RubiksCube & cube;
	bool gameStarted;
	std::weak_ptr<AnimationManager> animationManager;
	std::shared_ptr<Animation> messageAnimation;

	bool isAllBlockAligned(Vector3f std_vector) const;
	void print(const std::string & message);
	void onFinishedTwist();
public:
	GameRule(RubiksCube & cube, std::weak_ptr<AnimationManager> animationManager);
	void reset();
	void scramble();
	bool judge();
	void win();
	bool isStarted() const;
};

class PrintStringAnimation : public Animation {
private:
	GameRule & gameRule;
	std::string message;
public:
	PrintStringAnimation(GameRule & gameRule, const std::string & message);
	bool stepFrame(const double timeElapsed, const double timeDelta) override;
};

