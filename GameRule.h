#pragma once

#include "RubiksCube.h"

class GameRule {
private:
	RubiksCube & cube;
public:
	GameRule(RubiksCube & cube);
	void reset();
	void scramble();
	void win();
};
