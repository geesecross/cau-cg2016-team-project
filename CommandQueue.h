#pragma once
#include <deque>
#include <memory>
#include <functional>

class CommandQueue {
public:
	typedef std::function<bool()> Command;
private:
	typedef std::deque<Command> Queue;
	Queue queue;

public:
	bool empty() const;
	CommandQueue & push(Command && command);
	CommandQueue & execute();
};