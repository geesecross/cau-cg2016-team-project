#include "Animation.h"

bool Animation::operator()(const double timeDelta) {
	if (!this->started) {
		this->started = true;
		this->onStart();
	}

	this->timeElapsed += timeDelta;

	if (this->stepFrame(this->timeElapsed, timeDelta)) {
		this->finished = true;
		this->onFinished();
	}

	return this->finished;
}

void Animation::onStart() {
	// empty default
}

void Animation::onFinished() {
	// empty default
}

bool Animation::isStarted() const {
	return this->started;
}

bool Animation::isFinished() const {
	return this->finished;
}

AnimationManager::Queue & AnimationManager::getFrontQueue() {
	return this->queue[this->queueIndex];
}

AnimationManager::Queue & AnimationManager::getBackQueue() {
	return this->queue[(this->queueIndex + 1) % 2];
}

void AnimationManager::swapQueue() {
	this->queueIndex++;
	this->queueIndex %= 2;
}

AnimationManager & AnimationManager::push(std::shared_ptr<Animation> animation) {
	this->getFrontQueue().push_back(animation);
	return *this;
}

void AnimationManager::step(const double timeDelta) {
	this->swapQueue();

	while (!this->getBackQueue().empty()) {
		std::shared_ptr<Animation> ani = this->getBackQueue().front();
		this->getBackQueue().pop_front();

		if (!(*ani)(timeDelta)) {
			this->getFrontQueue().push_back(ani);
		}
	}
}
