#include "GameRule.h"
#include <ctime>
#include "Resource.h"
int GameRule::maxScramble = 20; // 스크램블 최대 횟수
const GLfloat Particle::gravity = 0.3f;
const Vector4f Particle::colors[10] = {
	{1.f, 1.f, 1.f, 1.f},
	{0.34f, 0.34f, 0.34f, 1.f},
	{1.f, 0.f, 0.f, 1.f},
	{0.f, 0.f, 1.f, 1.f},
	{0.f, 1.f, 0.f, 1.f},
	{1.f, 0.f, 1.f, 1.f},
	{0.f, 1.f, 1.f, 1.f},
	{1.f, 1.f, 0.f, 1.f},
	{1.f, 0.57f, 0.2f, 1.f},
	{0.5f, 0.29f, 0.1f, 1.f}
};

GameRule::GameRule(std::weak_ptr<RubiksCube> rubiksCube, std::weak_ptr<AnimationManager> animationManager, std::weak_ptr<Camera> camera)
	: rubiksCube(rubiksCube),
	animationManager(animationManager),
	camera(camera),
	debugMode(false),
	onFinishedTwistListener(this, &GameRule::onFinishedTwist),
	onCursorMovedListener(this, &GameRule::onCursorMoved),
	onCursorRotatedListener(this, &GameRule::onCursorRotated)
{
	this->rubiksCube.lock()->onFinishedTwist.addListener(this->onFinishedTwistListener);
	this->rubiksCube.lock()->getCursor()->onCursorMoved.addListener(this->onCursorMovedListener);
	this->rubiksCube.lock()->getCursor()->onCursorRotated.addListener(this->onCursorRotatedListener);
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

	if (this->scatterAnimation)
	{
		this->scatterAnimation->interrupt();
	}
	this->scatterAnimation = std::make_shared<ScatterAnimation>(this->rubiksCube);
	this->animationManager.lock()->push(this->scatterAnimation);
	this->gameStarted = false;
}

bool GameRule::isStarted() const {
	return this->gameStarted;
}

bool GameRule::toggleDebugMode()
{
	this->debugMode = !(this->debugMode);
	if(this->debugMode)
	{
		maxScramble = 1;
	} else {
		maxScramble = 20;
	}
	return this->debugMode;
}

void GameRule::onCursorMoved(const RubiksCube::Cursor & cursor, const Vector2f & movement) {
	auto ani = std::make_shared<CursorMovementFollowAnimation>(camera, cursor, movement);
	bool empty = this->cursorMoveQueue.isEmpty();

	this->cursorMoveQueue.push([=](bool interrupted) {
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
		this->cursorMoveQueue.execute();
	}
}

void GameRule::onCursorRotated(const RubiksCube::Cursor & cursor, const bool clockwise) {
	auto ani = std::make_shared<CursorRotationFollowAnimation>(camera, cursor, clockwise);
	bool empty = this->cursorMoveQueue.isEmpty();

	this->cursorMoveQueue.push([=](bool interrupted) {
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
		this->cursorMoveQueue.execute();
	}
}

void GameRule::step() {
	this->cursorMoveQueue.execute();
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
	this->createComponent<Model>()->bindMesh(Resource::meshes[Resource::Meshes::Particle])
		.bindShaderProgram(Resource::shaderPrograms[Resource::ShaderPrograms::Phong])
		.setColor(colors[(int)((float)rand() / RAND_MAX * 10)]);
	xSpeed = ((float)rand() / RAND_MAX * 0.1f - 0.2f);
	zSpeed = ((float)rand() / RAND_MAX * 0.1f - 0.2f);
	this->setTransform(Transform(this->getTransform()).scalePre(2.0f));
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
		particles[i].setTransform(
			Transform(particles[i].getTransform())
			.translatePost({
				vrp[0] - (vpn[0] * 2.f) - ((float)rand() / RAND_MAX * 5.f - 3.0f),
				vrp[1] - (vpn[1] * 2.f) - ((float)rand() / RAND_MAX * 5.f - 2.5f),
				vrp[2] - (vpn[2] * 2.f) - ((float)rand() / RAND_MAX * 5.f)
			})
		);
	}
}

bool ParticleAnimation::stepFrame(const double timeElapsed, const double timeDelta) {
	// particle들이 각자 x, z는 랜덤으로 y는 정해진 떨어지는 속도에 따라 움직임
	for (int i = 0; i < MAX_PARTICLES; i++)
	{	
		particles[i].xSpeed += ((float)rand() / RAND_MAX - 0.5f) * (float)timeDelta;
		particles[i].zSpeed += ((float)rand() / RAND_MAX - 0.5f) * (float)timeDelta;
		particles[i].setTransform(
			Transform(particles[i].getTransform())
			.translatePost({
				(float)timeDelta * particles[i].xSpeed,
				-((float)timeDelta * Particle::gravity),
				(float)timeDelta * particles[i].zSpeed
			})
		);
		camera.lock()->render(particles[i], true);
	}

	return timeElapsed > 5;
}

ScatterAnimation::ScatterAnimation(std::weak_ptr<RubiksCube> rubiksCube) : rubiksCube(rubiksCube)
{
}

bool ScatterAnimation::stepFrame(const double timeElapsed, const double timeDelta) {
	speed += (float)timeDelta * 3.f;
	float scale = ((float)sin(-timeElapsed * 5) * 0.15f + 1.0f);
	std::cout << scale << std::endl;
	if (timeElapsed < 3) {
		rubiksCube.lock()->setTransform(
			Transform(rubiksCube.lock()->getTransform())
			.scalePost(scale)
			.rotatePost(
				Rotation().rotateByEuler(Vector3f(speed))
			)
		);
	}
	else {
		for (std::shared_ptr<Actor> & block : *this->rubiksCube.lock()) {
			block->setTransform(
				Transform(block->getTransform())
				.translatePost(block->getTransform().transformPoint({ 0, 0, 0 }).normalized())
			);
		}
	}
	return timeElapsed > 5;
}

void ScatterAnimation::onFinished() {
	rubiksCube.lock()->setTransform(Transform());
	size_t i = 0;
	for (std::shared_ptr<Actor> & block : *this->rubiksCube.lock()) {
		block->setTransform(
			Transform(this->initialBlockTransforms[i++])
		);
	}
}

void ScatterAnimation::onStart() {
	for (std::shared_ptr<Actor> & block : *this->rubiksCube.lock()) {
		this->initialBlockTransforms.push_back(block->getTransform());
	}
	speed = 1.f;
}

GameRule::CursorMovementFollowAnimation::CursorMovementFollowAnimation(std::weak_ptr<Camera> & camera, const RubiksCube::Cursor & cursor, const Vector2f & movement) {
	this->camera = camera;
	this->startingTransform = cursor.getWorldTransform();
	this->movement = movement;
}

bool GameRule::CursorMovementFollowAnimation::stepFrame(const double timeElapsed, const double timeDelta) {
	static const float duration = 0.2f;
	float phase = (duration - (float)timeElapsed) / duration;
	if (phase < 0) {
		phase = 0;
	}

	this->camera.lock()->setTransform(
		Transform()
		.rotatePost(Rotation().rotateByEuler({ -15, 15, 0 }))
		.translatePost({ 2.5f, 2.5f, 10 })
		.translatePost({ -this->movement * phase * 2, 0 })
		.pushPost(this->startingTransform)
	);
	return phase <= 0;
}

GameRule::CursorRotationFollowAnimation::CursorRotationFollowAnimation(std::weak_ptr<Camera>& camera, const RubiksCube::Cursor & cursor, const bool clockwise) {
	this->camera = camera;
	this->startingTransform = cursor.getWorldTransform();
	this->clockwise = clockwise;
}

bool GameRule::CursorRotationFollowAnimation::stepFrame(const double timeElapsed, const double timeDelta) {
	static const float duration = 0.2f;
	float phase = (duration - (float)timeElapsed) / duration;
	if (phase < 0) {
		phase = 0;
	}

	this->camera.lock()->setTransform(
		Transform()
		.rotatePost(Rotation().rotateByEuler({ -15, 15, 0 }))
		.translatePost({ 2.5f, 2.5f, 10 })
		.rotatePost(Rotation().rotateByEuler(Vector3f::zVector() * phase * (this->clockwise ? 90.f : -90.f)))
		.pushPost(this->startingTransform)
	);
	return phase <= 0;
}
