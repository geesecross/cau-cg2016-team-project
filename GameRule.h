#pragma once

#include "RubiksCube.h"
#include "Camera.h"
#define MAX_PARTICLES 500 // 최대 Particle 개수

class PrintStringAnimaiton;
class ParticleAnimation;

class GameRule {
	friend class PrintStringAnimation;
private:
	Event<void()>::MemberFunctionListener<GameRule> onFinishedTwistListener;
	std::weak_ptr<RubiksCube> rubiksCube;
	std::weak_ptr<AnimationManager> animationManager;
	std::weak_ptr<Camera> camera;
	std::shared_ptr<PrintStringAnimation> messageAnimation;
	std::shared_ptr<ParticleAnimation> particleAnimation;
	bool gameStarted;
	static int maxScramble;
	bool debugMode;

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