#include "CommandQueue.h"

CommandQueue & CommandQueue::push(Command & command) {
	this->queue.push_back(command);
	return *this;
}

CommandQueue & CommandQueue::step() {
	this->queue.front().execute();
	this->queue.pop_front();
	return *this;
}

CommandQueue & CommandQueue::executeAll() {
	while (!this->queue.empty()) {
		this->step();
	}
	return *this;
}

void Command::execute() const {
	this->func();
}
