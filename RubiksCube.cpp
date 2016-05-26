#include "RubiksCube.h"

RubiksCube::RubiksCube(const size_t size) {
	this->size = size;

	float center = (size - 1) / 2.0f;
	this->blocks.resize(size);
	for (size_t x = 0; x < size; ++x) {
		this->blocks[x].resize(size);
		for (size_t y = 0; y < size; ++y) {
			this->blocks[x][y].resize(size);
			for (size_t z = 0; z < size; ++z) {
				this->blocks[x][y][z] = this->createChild<Block>();
				this->blocks[x][y][z].lock()->
					scale(Vector3f(0.9f))
					.moveOrigin(Vector3f((float)x - center, (float)y - center, (float)z - center));
			}
		}
	}
}

void RubiksCube::twist(const size_t index, const Axis axis) {
	const size_t size = this->blocks.size();

	static Vector3f steps[] = {
		{ 1, 0, 0 },
		{ 0, 1, 0 },
		{ 0, 0, 1 },
	};
	static Vector3f selections[] = {
		{ 0, 1, 1 },
		{ 1, 0, 1 },
		{ 1, 1, 0 },
	};
	static size_t permutation[] = { 1, 2, 3, 4, 5, 6,};

	Vector3f step = steps[axis];
	Vector3f selection = selections[axis];

	for (size_t x = step[X] * index; x < size; x += step[X] == 0 ? 1 : size) {
		for (size_t y = step[Y] * index; y < size; y += step[Y] == 0 ? 1 : size) {
			for (size_t z = step[Z] * index; z < size; z += step[Z] == 0 ? 1 : size) {
				this->blocks[x][y][z].lock()->rotate(Rotation().rotateByEuler(step * 10));
			}
		}
	}
}
