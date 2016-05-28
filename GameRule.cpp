#include "GameRule.h"
#include <ctime>
const int max_scramble = 20; // 스크램블 최대 횟수
GameRule::GameRule(RubiksCube & cube) : cube(cube) {
	this->game_started = false;
}

void GameRule::reset()
{
	this->game_started = false;
	std::cout << "game end" << std::endl;
	cube.reset();
}

void GameRule::scramble()
{
	cube.reset();
	srand((unsigned)time(NULL));
	const int indices[3] = { 0, 1, 2 };
	const Vector3f axis_vectors[3] = { Vector3f::xVector(), Vector3f::yVector(), Vector3f::zVector() };
	Rotation rot = Rotation();
	const int rot_degrees[3] = { 90, 180, 270 };
	int prev_axis_vec = 0;
	int prev_indices = 0;
	for(int i = 0; i < max_scramble; i++)
	{
		int cur_axis_vec = rand() % 3;
		int cur_indices = rand() % 3;
		// 만약 이전에 수행한 twist와 비슷한 조건이면 다른 조건을 뽑게 다시 실행
		if(prev_axis_vec == cur_axis_vec || prev_indices == cur_indices)
		{
			i--;
			continue;
		}
		cube.twist(indices[cur_indices], rot.rotateByEuler(axis_vectors[cur_axis_vec] * rot_degrees[rand() % 3]));
		prev_axis_vec = cur_axis_vec;
		prev_indices = cur_indices;
	}

	// game start
	std::cout << "game start" << std::endl;
	this->game_started = true;
}

bool GameRule::isAllBlockAligned(Vector3f std_vector) const
{
	size_t size = cube.getSize();
	Vector3f comparison_vector_f = cube.blocks[0][0][0].lock()->getTransform().transformDirection(std_vector);
	Vector<GLint, 3> comparison_vector_i = {
		(int)(comparison_vector_f[0] + 0.5f),
		(int)(comparison_vector_f[1] + 0.5f),
		(int)(comparison_vector_f[2] + 0.5f)
	};

	for (int x = 0; x < size; x++)
	{
		for (int y = 0; y < size; y++)
		{
			for (int z = 0; z < size; z++)
			{
				Vector3f vector_f = cube.blocks[x][y][z].lock()->getTransform().transformDirection(std_vector);
				Vector<GLint, 3> vector_i = {
					(int)(vector_f[0] + 0.5f),
					(int)(vector_f[1] + 0.5f),
					(int)(vector_f[2] + 0.5f)
				};
				if (vector_i != comparison_vector_i) return false;
			}
		}
	}
	return true;
}

bool GameRule::judge()
{
	if (this->game_started)
	{
		if(isAllBlockAligned(Vector3f::zVector()) 
			&& isAllBlockAligned(Vector3f::yVector()))
		{
			win();
			return true;
		}
		
	}
	return false;
}

void GameRule::win()
{
	std::cout << "win" << std::endl;
	this->game_started = false;
}

bool GameRule::isStart() const
{
	return this->game_started;
}
