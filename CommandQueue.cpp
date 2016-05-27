#include "CommandQueue.h"

bool CommandQueue::empty() const {
	return this->queue.empty();
}

CommandQueue & CommandQueue::push(Command && command) {
	this->queue.push_back(command);
	return *this;
}

CommandQueue & CommandQueue::execute() {
	while (!this->queue.empty()) {
		if (!this->queue.front()()) {
			// not finished yet
			return *this;
		}
		this->queue.pop_front();
	}
	return *this;
}