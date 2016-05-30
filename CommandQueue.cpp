#include "CommandQueue.h"

bool CommandQueue::isEmpty() const {
	return this->queue.empty();
}

CommandQueue & CommandQueue::push(Command && command) {
	this->queue.push_back(command);
	return *this;
}

CommandQueue & CommandQueue::execute(const bool interrupted) {
	while (!this->queue.empty()) {
		Queue::value_type command = this->queue.front();
		this->queue.pop_front();
		if (!command(interrupted)) {
			// not finished yet
			this->queue.push_front(command);
			return *this;
		}
	}
	return *this;
}

CommandQueue & CommandQueue::clear() {
	this->execute(true);
	this->queue.clear();
	return *this;
}
