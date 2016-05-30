#include "GameRule.h"
#include <ctime>
#include "Camera.h"
const int max_scramble = 20; // ��ũ���� �ִ� Ƚ��

extern std::shared_ptr<Camera> camera;
GameRule::GameRule(RubiksCube & cube, std::weak_ptr<AnimationManager> animationManager) : cube(cube), animationManager(animationManager){
	this->game_started = false;
	print("press 'y' to start game");
}

void GameRule::reset()
{
	cube.reset();
	print("reset");
	this->game_started = false;
}

void GameRule::scramble()
{
	cube.reset();
	srand((unsigned)time(NULL));
	const int indices[3] = { 0, 1, 2 };
	const Vector3f axis_vectors[3] = { Vector3f::xVector(), Vector3f::yVector(), Vector3f::zVector() };
	Rotation rot = Rotation();
	const GLfloat rot_degrees[3] = { 90.f, 180.f, 270.f };
	int prev_axis_vec = 0;
	int prev_indices = 0;
	int prev_rot_degree = 0;
	for(int i = 0; i < max_scramble; i++)
	{
		int cur_axis_vec = rand() % 3;
		int cur_indices = rand() % 3;
		int cur_rot_degree = rand() % 3;
		// ���� ������ ������ twist�� ����� �����̸� �ٸ� ������ �̰� �ٽ� ����
		if(prev_axis_vec == cur_axis_vec 
			|| prev_indices == cur_indices
			|| prev_rot_degree == cur_rot_degree)
		{
			i--;
			continue;
		}
		cube.twist(indices[cur_indices], rot.rotateByEuler(axis_vectors[cur_axis_vec] * rot_degrees[cur_rot_degree]));
		prev_axis_vec = cur_axis_vec;
		prev_indices = cur_indices;
		prev_rot_degree = cur_rot_degree;
	}

	// game start
	print("game start");
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

	for (size_t x = 0; x < size; x++)
	{
		for (size_t y = 0; y < size; y++)
		{
			for (size_t z = 0; z < size; z++)
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

void GameRule::print(char* msg)
{
	auto ani = std::make_shared<PrintStrAnimation>(*this, msg);
	bool empty = this->commandQueue.empty();

	this->commandQueue.push([=](bool interrupted) {
		if (!ani->isStarted()) {
			this->animationManager.lock()->push(ani);
		}
		if (interrupted) {
			ani->interrupt();
		}
		return ani->isFinished();
	});

	if (empty) {
		// first entry
		this->commandQueue.execute();
	}
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
	print("win");
	this->game_started = false;
}

bool GameRule::isStart() const
{
	return this->game_started;
}

PrintStrAnimation::PrintStrAnimation(GameRule & gameRule, char* msg) : gameRule(gameRule), msg(msg)
{
}

void PrintStrAnimation::onStart()
{

}

bool PrintStrAnimation::stepFrame(const double timeElpased, const double timeDelta)
{
	glColor3f(1.f, 1.f, 1.f);
	const Vector3f & vrp = camera->getViewReferencePoint();
	const Vector3f & vpn = camera->getViewPlaneNormal();
	glRasterPos3f(vrp[0]-(vpn[0]*2.f), vrp[1]-(vpn[1]*2.f), vrp[2]-(vpn[2]*2.f));
	int len = strlen(this->msg);
	for(int i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, this->msg[i]);
	}
	return timeElpased > 3;
}

void PrintStrAnimation::onFinished()
{
	gameRule.commandQueue.execute();
}
