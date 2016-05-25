#pragma once
#include <iostream>
#include <fstream>
#include <string>

namespace FileHelper {
	static std::string loadTextFile(const std::string & fileName) {
		std::ifstream is(fileName);
		if (!is.is_open()) {
			std::cerr << "** Failed to open text file: " << fileName << std::endl;
			return "";
		}

		std::string text, line;
		while (std::getline(is, line)) {
			text += line + "\n";
		}
		is.close();

		return text;
	}
}