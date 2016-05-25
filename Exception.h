#pragma once
#include <string>

class Exception {
private:
	std::string message;
public:
	Exception(const std::string & message);
	virtual std::string what() const;
};
