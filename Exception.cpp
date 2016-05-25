#include "Exception.h"

Exception::Exception(const std::string & message) {
	this->message = message;
}

std::string Exception::what() const {
	return this->message;
}
