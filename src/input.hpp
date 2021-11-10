#ifndef INPUT_HPP
#define INPUT_HPP

#include <iostream>


class Input {
private:
public:
	static signed char stringMatch(const char* firstString, const char* secondString) {
		unsigned short index = 0;
		do {
			if(firstString[index] != secondString[index]) {
				return (firstString[index] > secondString[index]) ? 1 : -1;
			}
		} while (firstString[index++] != '\0');

		return 0;
	}

	int processInput(bool& run, char (&inputText)[265]) {
		std::cin.getline(inputText, 265);

		return 0;
	}
};

#endif