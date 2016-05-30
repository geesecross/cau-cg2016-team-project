#include "CommandQueue.h"

bool CommandQueue::empty() const {
	return this->queue.empty();
}

CommandQueue & CommandQueue::push(Command && command) {
	this->queue.push_back(command);
	return *this;
}

CommandQueue & CommandQueue::execute(const bool interrupted) {
	while (!this->queue.empty()) {
		if (!this->queue.front()(interrupted)) {
			// not finished yet
			return *this;
		}
		if (!this->queue.empty()) {
			this->queue.pop_front();
		}
	}
	return *this;
}

CommandQueue & CommandQueue::clear() {
	this->execute(true);
	this->queue.clear();
	return *this;
}
