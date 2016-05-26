#pragma once
#include "Actor.h"
#include "Block.h"
#include <vector>

class RubiksCube : public Actor {
public:
	enum Axis {
		X,
		Y,
		Z
	};
private:
	std::vector<std::vector<std::vector<std::weak_ptr<Block>>>> blocks;
	size_t size;

public:
	RubiksCube(const size_t size);

	void twist(const size_t index, const Axis axis);
};