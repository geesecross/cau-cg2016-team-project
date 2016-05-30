#pragma once
#include <deque>
#include <memory>
#include <functional>

class CommandQueue {
public:
	typedef std::function<bool(const bool)> Command;	//	bool func(const bool interrupted)
private:
	typedef std::deque<Command> Queue;
	Queue queue;

public:
	bool isEmpty() const;
	CommandQueue & push(Command && command);
	CommandQueue & execute(const bool interrupted = false);
	CommandQueue & clear();
};