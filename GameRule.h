#pragma once

#include "RubiksCube.h"
#include "Camera.h"

#define MAX_PARTICLES 100 // 최대 Particle 개수

class PrintStringAnimaiton;
class ParticleAnimation;
class ScatterAnimation;

class GameRule {
	friend class PrintStringAnimation;
private:
	class CursorMovementFollowAnimation : public Animation {
	private:
		std::weak_ptr<Camera> camera;
		Transform startingTransform;
		Vector2f movement;
	public:
		CursorMovementFollowAnimation(std::weak_ptr<Camera> & camera, const RubiksCube::Cursor & cursor, const Vector2f & movement);
		virtual bool stepFrame(const double timeElapsed, const double timeDelta) override;
	};
	class CursorRotationFollowAnimation : public Animation {
	private:
		std::weak_ptr<Camera> camera;
		Transform startingTransform;
		bool clockwise;
	public:
		CursorRotationFollowAnimation(std::weak_ptr<Camera> & camera, const RubiksCube::Cursor & cursor, const bool clockwise);
		virtual bool stepFrame(const double timeElapsed, const double timeDelta) override;
	};

	Event<void()>::MemberFunctionListener<GameRule> onFinishedTwistListener;
	RubiksCube::Cursor::OnCursorMoved::MemberFunctionListener<GameRule> onCursorMovedListener;
	RubiksCube::Cursor::OnCursorRotated::MemberFunctionListener<GameRule> onCursorRotatedListener;
	std::weak_ptr<RubiksCube> rubiksCube;
	std::weak_ptr<AnimationManager> animationManager;
	std::weak_ptr<Camera> camera;
	std::shared_ptr<PrintStringAnimation> messageAnimation;
	std::shared_ptr<ParticleAnimation> particleAnimation;
	std::shared_ptr<ScatterAnimation> scatterAnimation;
	bool gameStarted, blockedInput = false;
	static int maxScramble;
	bool debugMode;
	CommandQueue cursorMoveQueue;

	bool isAllBlockAligned(Vector3f std_vector) const;
	void print(const std::string & message);
	void onFinishedTwist();
public:
	GameRule(std::weak_ptr<RubiksCube> cube, std::weak_ptr<AnimationManager> animationManager, std::weak_ptr<Camera> camera);
	void resetGame();
	void scramble();
	bool judge();
	void win();
	bool isStarted() const;
	bool toggleDebugMode();
	void onCursorMoved(const RubiksCube::Cursor & cursor, const Vector2f & movement);
	void onCursorRotated(const RubiksCube::Cursor & cursor, const bool clockwise);
	void step();
	void move(const Vector2f & vector);
	void twist(bool clockwise);
	void rotateAxis(bool clockwise);
	void setBlockedInput(bool blocked);
};

class PrintStringAnimation : public Animation {
private:
	std::weak_ptr<Camera> camera;
	std::string message;
public:
	PrintStringAnimation(std::weak_ptr<Camera> camera, const std::string & message);
	bool stepFrame(const double timeElapsed, const double timeDelta) override;
};

enum ParticleColor
{
	WHITE, GRAY, RED, BLUE, GREEN, MAGENTA, CYAN, YELLOW, ORANGE, BROWN
};

class Particle : public Actor
{
	friend class ParticleAnimation;
private:
	static const Vector4f colors[10];
	static const GLfloat gravity;
	GLfloat xSpeed;
	GLfloat zSpeed;
public:
	Particle();
};

class ParticleAnimation : public Animation
{
private:
	Particle particles[MAX_PARTICLES];
	std::weak_ptr<Camera> camera;
public:
	ParticleAnimation(std::weak_ptr<Camera> camera);
	void onStart() override;
	bool stepFrame(const double timeElapsed, const double timeDelta) override;
};

class ScatterAnimation : public Animation
{
	std::weak_ptr<RubiksCube> rubiksCube;
	GLfloat speed;
	std::vector<Transform> initialBlockTransforms;
	GameRule & gameRule;
public:
	ScatterAnimation(GameRule & gameRule, std::weak_ptr<RubiksCube> rubiksCube);
	bool stepFrame(const double timeElapsed, const double timeDelta) override;
	void onFinished() override;
	void onStart() override;
};