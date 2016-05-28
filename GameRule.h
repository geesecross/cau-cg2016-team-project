#pragma once

#include "RubiksCube.h"

class GameRule {
	friend class PrintStrAnimation;
private:
	RubiksCube & cube;
	bool game_started;
	std::weak_ptr<AnimationManager> animationManager;
	CommandQueue commandQueue;
	bool isAllBlockAligned(Vector3f std_vector) const;
	void print(char * msg);
public:
	GameRule(RubiksCube & cube, std::weak_ptr<AnimationManager> animationManager);
	void reset();
	void scramble();
	bool judge();
	void win();
	bool isStart() const;
};

class PrintStrAnimation : public Animation
{
private:
	GameRule & gameRule;
	char* msg;
public:
	PrintStrAnimation(GameRule & gameRule, char* msg);
	void onStart() override;
	bool stepFrame(const double timeElpased, const double timeDelta) override;
	void onFinished() override;
};