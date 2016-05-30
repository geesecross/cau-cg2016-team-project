#pragma once

#include "RubiksCube.h"
#include "Camera.h"
#define MAX_PARTICLES 500 // �ִ� Particle ����

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
	static const int maxScramble;

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
};

class PrintStringAnimation : public Animation {
private:
	std::weak_ptr<Camera> camera;
	std::string message;
public:
	PrintStringAnimation(std::weak_ptr<Camera> camera, const std::string & message);
	bool stepFrame(const double timeElapsed, const double timeDelta) override;
};

class Particle : public Actor
{
	friend class ParticleAnimation;
private:
	Vector4f color;
	static const GLfloat gravity;
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