#include "GameRule.h"
#include <ctime>
#include "Camera.h"
const int max_scramble = 20; // ��ũ���� �ִ� Ƚ��

GameRule::GameRule(std::weak_ptr<RubiksCube> rubiksCube, std::weak_ptr<AnimationManager> animationManager, std::weak_ptr<Camera> camera)
	: rubiksCube(rubiksCube),
	animationManager(animationManager),
	camera(camera),
	onFinishedTwistListener(this, &GameRule::onFinishedTwist) {

	this->rubiksCube.lock()->onFinishedTwist.addListener(this->onFinishedTwistListener);
	this->gameStarted = false;
	print("press 'y' to start game");
}

void GameRule::reset() {
	rubiksCube.reset();
	print("reset");
	this->gameStarted = false;
}

void GameRule::scramble() {
	rubiksCube.reset();
	srand((unsigned)time(NULL));
	const int indices[3] = { 0, 1, 2 };
	const Vector3f axis_vectors[3] = { Vector3f::xVector(), Vector3f::yVector(), Vector3f::zVector() };
	Rotation rot = Rotation();
	const GLfloat rot_degrees[3] = { 90.f, 180.f, 270.f };
	int previous_axis_vector = 0;
	int previous_index = 0;
	int previous_rotation_degree = 0;
	for(int i = 0; i < max_scramble; i++)
	{
		int current_axis_vector = rand() % 3;
		int current_index = rand() % 3;
		int current_rotation_degree = rand() % 3;
		// ���� ������ ������ twist�� ����� �����̸� �ٸ� ������ �̰� �ٽ� ����
		if(previous_axis_vector == current_axis_vector 
			|| previous_index == current_index
			|| previous_rotation_degree == current_rotation_degree) {

			i--;
			continue;
		}
		rubiksCube.lock()->twist(indices[current_index], rot.rotateByEuler(axis_vectors[current_axis_vector] * rot_degrees[current_rotation_degree]));
		previous_axis_vector = current_axis_vector;
		previous_index = current_index;
		previous_rotation_degree = current_rotation_degree;
	}
}

bool GameRule::isAllBlockAligned(Vector3f std_vector) const {
	size_t size = rubiksCube.lock()->getSize();
	// ù��° ���� transform �� ���, ���� ���͸� transform �ؼ� �ٸ� ������ �� ������� ���� ���͸� �����.
	Vector3f comparison_vector_f = rubiksCube.lock()->blocks[0][0][0].lock()->getTransform().transformDirection(std_vector);
	Vector<GLint, 3> comparison_vector_i = {
		(int)(comparison_vector_f[0] + 0.5f),
		(int)(comparison_vector_f[1] + 0.5f),
		(int)(comparison_vector_f[2] + 0.5f)
	};

	// ������ ���� ���� ���� ���� �����ؼ�, ������ ���� �� ��� ���Ϳ� ������ Ȯ���Ѵ�.
	// ��� ���� ���ٸ�, ���� ���Ϳ� ���� ��� ���� ���� �������� transform �� ��.
	for (size_t x = 0; x < size; x++)
	{
		for (size_t y = 0; y < size; y++)
		{
			for (size_t z = 0; z < size; z++)
			{
				Vector3f vector_f = rubiksCube.lock()->blocks[x][y][z].lock()->getTransform().transformDirection(std_vector);
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
	this->messageAnimation = std::make_shared<PrintStringAnimation>(*this, camera, message);
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
		// z vector�� y vector�� �������� ��� ���� ���� ������ ���� �ִ��� Ȯ���Ѵ�.
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

PrintStringAnimation::PrintStringAnimation(GameRule & gameRule, std::weak_ptr<Camera> camera, const std::string & message) 
	: gameRule(gameRule), camera(camera), message(message) {

}

bool PrintStringAnimation::stepFrame(const double timeElapsed, const double timeDelta) {
	glColor3f(1.f, 1.f, 1.f);
	
	const Vector3f & vrp = camera.lock()->getViewReferencePoint();
	const Vector3f & vpn = camera.lock()->getViewPlaneNormal();
	glRasterPos3f(vrp[0] - (vpn[0] * 2.f), vrp[1] - (vpn[1] * 2.f), vrp[2] - (vpn[2] * 2.f));

	for (size_t i = 0; i < this->message.size(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, this->message[i]);
	}

	return timeElapsed > 3;
}
