#ifndef INPUT_HPP
#define INPUT_HPP

#include <fcntl.h>
#include <iostream>


class Input {
private:
	char inputText[265];
public:
	static signed char stringMatch(const char* firstString, const char* secondString) {
		std::cout << "compare '" << firstString << "' and '" << secondString << std::endl;

		unsigned short index = 0;
		do {
			if(firstString[index] != secondString[index]) {
				std::cout << "'" << firstString[index] << "' != '" << secondString[index] << "'" << std::endl;
				return (firstString[index] > secondString[index]) ? 1 : -1;
			} else {
				std::cout << "'" << firstString[index] << "' == '" << secondString[index] << "'" << std::endl;
			}
		} while (firstString[index++] != '\0');

		std::cout << "Strings match" << std::endl;

		return 0;
	}

	void processInput(bool& continueRunning/*, char (&inputText)[265]*/) {
		std::cout << "getline: '";
		std::cin.getline(inputText, 265);
		std::cout << "'" << inputText << std::endl;

		if(inputText[0] != '\0') {
			if(stringMatch(inputText, "exit") == 0) {
				std::cout << "set run to false" << std::endl;
				continueRunning = false;
				return;
			} else {
				std::cout << "input was not 'exit'" << std::endl;
			}

			inputText[0] = '\0';
		}
	}
};

#endif