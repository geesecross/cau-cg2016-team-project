#include "GameRule.h"
#include <ctime>
const int max_scramble = 15;
GameRule::GameRule(RubiksCube & cube) : cube(cube) {
}

void GameRule::reset()
{
	cube.reset();
}

void GameRule::scramble()
{
	reset();
	srand((unsigned)time(NULL));
	const int indices[3] = { 0, 1, 2 };
	const Vector3f axis_vectors[3] = { Vector3f::xVector(), Vector3f::yVector(), Vector3f::zVector() };
	Rotation rot = Rotation();
	const int rot_degrees[3] = { 90, 180, 270 };

	for(int i = 0; i < max_scramble; i++)
	{
		cube.twist(indices[rand() % 3], rot.rotateByEuler(rot.transform(axis_vectors[rand()]) * rot_degrees[rand() % 3]));
	}
}

void GameRule::win()
{
}
