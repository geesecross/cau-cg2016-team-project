#include "GameRule.h"
#include <ctime>
#include "Resource.h"
const int GameRule::maxScramble = 1; // 스크램블 최대 횟수
const GLfloat Particle::gravity = 0.4f;

GameRule::GameRule(std::weak_ptr<RubiksCube> rubiksCube, std::weak_ptr<AnimationManager> animationManager, std::weak_ptr<Camera> camera)
	: rubiksCube(rubiksCube),
	animationManager(animationManager),
	camera(camera),
	onFinishedTwistListener(this, &GameRule::onFinishedTwist) {

	this->rubiksCube.lock()->onFinishedTwist.addListener(this->onFinishedTwistListener);
	this->gameStarted = false;
	print("press 'y' to start game");
}

void GameRule::resetGame() {
	rubiksCube.lock()->resetBlocksAndCursor();
	print("reset");
	this->gameStarted = false;
}

void GameRule::scramble() {
	rubiksCube.lock()->resetBlocksAndCursor();
	srand((unsigned)time(NULL));
	const int indices[3] = { 0, 1, 2 };
	const Vector3f axis_vectors[3] = { Vector3f::xVector(), Vector3f::yVector(), Vector3f::zVector() };
	Rotation rot = Rotation();
	const GLfloat rot_degrees[3] = { 90.f, 180.f, 270.f };
	int previous_axis_vector = 0;
	int previous_index = 0;
	int previous_rotation_degree = 0;
	for(int i = 0; i < maxScramble; i++)
	{
		int current_axis_vector = (int)((double)rand() / RAND_MAX * 3);
		int current_index = (int)((double)rand() / RAND_MAX * 3);
		int current_rotation_degree = (int)((double)rand() / RAND_MAX * 3);
		// 만약 이전에 수행한 twist와 비슷한 조건이면 다른 조건을 뽑게 다시 실행
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
	// game start
	print("game start");
	this->gameStarted = true;
}

bool GameRule::isAllBlockAligned(Vector3f std_vector) const {
	size_t size = rubiksCube.lock()->getSize();
	// 첫번째 블럭이 transform 한 대로, 기준 벡터를 transform 해서 다른 블럭들의 비교 대상으로 삼을 벡터를 만든다.
	Vector3f comparison_vector_f = rubiksCube.lock()->blocks[0][0][0].lock()->getTransform().transformDirection(std_vector);
	Vector<GLint, 3> comparison_vector_i = {
		(int)(comparison_vector_f[0] + 0.5f),
		(int)(comparison_vector_f[1] + 0.5f),
		(int)(comparison_vector_f[2] + 0.5f)
	};

	// 각각의 블럭에 대해 위와 같이 수행해서, 위에서 만든 비교 대상 벡터와 같은지 확인한다.
	// 모든 블럭이 같다면, 기준 벡터에 대해 모든 블럭이 같은 방향으로 transform 한 것.
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
	this->messageAnimation = std::make_shared<PrintStringAnimation>(camera, message);
	this->animationManager.lock()->push(this->messageAnimation);
}

void GameRule::onFinishedTwist() {
	if (this->gameStarted) {
		if (this->judge()) {
			win();
		}
	}
}

bool GameRule::judge() {
	if (this->gameStarted) {
		// z vector와 y vector를 기준으로 모든 블럭이 같은 방향을 보고 있는지 확인한다.
		if(isAllBlockAligned(Vector3f::zVector()) && isAllBlockAligned(Vector3f::yVector())) {
			return true;
		}
	}
	return false;
}

void GameRule::win() {
	print("win");
	if (this->particleAnimation) {
		this->particleAnimation->interrupt();
	}
	this->particleAnimation = std::make_shared<ParticleAnimation>(camera);
	this->animationManager.lock()->push(this->particleAnimation);
	this->gameStarted = false;
}

bool GameRule::isStarted() const {
	return this->gameStarted;
}

PrintStringAnimation::PrintStringAnimation(std::weak_ptr<Camera> camera, const std::string & message) 
	: camera(camera), message(message) {

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

Particle::Particle() {
	srand((unsigned)time(NULL));
	this->createComponent<Model>()->bindMesh(Resource::meshes[Resource::Particle])
		.bindShaderProgram(Resource::shaderPrograms[Resource::Phong])
		.setColor({
		1.f,
		1.f,
		1.f,
		1.f
	});

}

ParticleAnimation::ParticleAnimation(std::weak_ptr<Camera> camera) 
	: camera(camera) {
}

void ParticleAnimation::onStart() {
	// particle들의 초기 위치 설정
	const Vector3f & vrp = camera.lock()->getViewReferencePoint();
	const Vector3f & vpn = camera.lock()->getViewPlaneNormal();
	for(int i = 0; i < MAX_PARTICLES; i++)
	{
		particles[i].getTransform().translatePost({
			vrp[0] - (vpn[0] * 2.f) - ((float)rand() / RAND_MAX * 10.f - 5.f),
			vrp[1] - (vpn[1] * 2.f) - ((float)rand() / RAND_MAX * 10.f - 5.f),
			vrp[2] - (vpn[2] * 2.f) - ((float)rand() / RAND_MAX * 10.f - 5.f)
		});
	}
}

bool ParticleAnimation::stepFrame(const double timeElapsed, const double timeDelta) {
	// particle들이 각자 x, z는 랜덤으로 y는 정해진 떨어지는 속도에 따라 움직임
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		particles[i].getTransform().translatePost({
			(float)timeDelta * ((float)rand() / RAND_MAX - 0.5f),
			- ((float)timeDelta * Particle::gravity),
			(float)timeDelta * ((float)rand() / RAND_MAX - 0.5f)
		});
		camera.lock()->render(particles[i], true);
	}

	return timeElapsed > 5;
}