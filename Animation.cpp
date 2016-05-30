#include "Animation.h"

bool Animation::operator()(const double timeDelta) {
	if (!this->started) {
		this->started = true;
		this->onStart();
	}

	this->timeElapsed += timeDelta;

	if (this->interrupted || this->stepFrame(this->timeElapsed, timeDelta)) {		// if interrupted, don't step
		if (!this->finished) {
			if (this->interrupted) {
				this->onInterrupted();
			}
			this->finished = true;
			this->onFinished();
		}
	}

	return this->finished;
}

void Animation::onStart() {
	// empty default
}

void Animation::onFinished() {
	// empty default
}

void Animation::onInterrupted() {
	// empty default
}

bool Animation::isStarted() const {
	return this->started;
}

bool Animation::isFinished() const {
	return this->finished;
}

bool Animation::isInterrupted() const {
	return this->interrupted;
}

void Animation::interrupt() {
	this->interrupted = true;
	(*this)(0);
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

void AnimationManager::step(const double timeDelta, const bool interrupted) {
	this->swapQueue();

	while (!this->getBackQueue().empty()) {
		std::shared_ptr<Animation> ani = this->getBackQueue().front();
		this->getBackQueue().pop_front();

		if (interrupted) {
			ani->interrupt();
		}
		else {
			if (!(*ani)(timeDelta)) {
				this->getFrontQueue().push_back(ani);
			}
		}
	}
}

void AnimationManager::clear() {
	this->step(0, true);
	this->getFrontQueue().clear();
	this->getBackQueue().clear();
}
