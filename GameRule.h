#pragma once

#include "RubiksCube.h"

class GameRule {
private:
	RubiksCube & cube;
	bool game_started;
	bool isAllBlockAligned(Vector3f std_vector);
public:
	GameRule(RubiksCube & cube);
	void reset();
	void scramble();
	bool judge();
	void win();
	bool isStart() const;
};
