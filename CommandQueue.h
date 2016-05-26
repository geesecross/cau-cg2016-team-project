#pragma once
#include <deque>
#include <functional>

class Command {
private:
	std::function<void()> func;
public:
	Command(std::function<void()> & func) {
		this->func = func;
	}
public:
	void execute() const;
};

class CommandQueue {
private:
	std::deque<Command> queue;

public:
	CommandQueue & push(Command & command);
	CommandQueue & step();
	CommandQueue & executeAll();
};