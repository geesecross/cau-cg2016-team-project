#include "GameRule.h"
#include <ctime>
#include "Camera.h"
const int max_scramble = 20; // 스크램블 최대 횟수

extern std::shared_ptr<Camera> camera;
GameRule::GameRule(RubiksCube & cube, std::weak_ptr<AnimationManager> animationManager)
	: cube(cube),
	animationManager(animationManager),
	onFinishedTwistListener(this, &GameRule::onFinishedTwist)
{
	this->cube.onFinishedTwist.addListener(this->onFinishedTwistListener);
	this->gameStarted = false;
	print("press 'y' to start game");
}

void GameRule::reset()
{
	cube.reset();
	print("reset");
	this->gameStarted = false;
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
		// 만약 이전에 수행한 twist와 비슷한 조건이면 다른 조건을 뽑게 다시 실행
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

void GameRule::print(const std::string & message) {
	if (this->messageAnimation) {
		this->messageAnimation->interrupt();

	}
	this->messageAnimation = std::make_shared<PrintStringAnimation>(*this, message);
	this->animationManager.lock()->push(this->messageAnimation);
}

void GameRule::onFinishedTwist() {
	if (this->gameStarted) {
		if (this->judge()) {
			win();
		}
	}
	else {
		// game start
		print("game start");
		this->gameStarted = true;
	}
}

bool GameRule::judge() {
	if (this->gameStarted) {
		if(isAllBlockAligned(Vector3f::zVector()) && isAllBlockAligned(Vector3f::yVector())) {
			return true;
		}
		
	}
	return false;
}

void GameRule::win() {
	print("win");
	this->gameStarted = false;
}

bool GameRule::isStarted() const {
	return this->gameStarted;
}

PrintStringAnimation::PrintStringAnimation(GameRule & gameRule, const std::string & message) : gameRule(gameRule), message(message) {
}

bool PrintStringAnimation::stepFrame(const double timeElapsed, const double timeDelta) {
	glColor3f(1.f, 1.f, 1.f);
	
	const Vector3f & vrp = camera->getViewReferencePoint();
	const Vector3f & vpn = camera->getViewPlaneNormal();
	glRasterPos3f(vrp[0] - (vpn[0] * 2.f), vrp[1] - (vpn[1] * 2.f), vrp[2] - (vpn[2] * 2.f));

	for (size_t i = 0; i < this->message.size(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, this->message[i]);
	}

	return timeElapsed > 3;
}
