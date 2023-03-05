// PA-4-Source.cpp 

#include <iostream>
#include <vector>
#include <string>


int stairStepping(std::vector<int> steps, int destination) {
	return 0;
}

std::vector<int> setVectorTo(std::string input) {
	int numberLength = 0;
	std::vector<int> output;
	
	while(input.length()>0) {
		while (input[0] == ' ') {
			input = input.substr(1);
			if (input.length() == 0) break;
		}
		numberLength = (input.find(' ') != -1) ? input.find(' ') : input.length();
		output.push_back(std::stoi(input.substr(0, numberLength)));
		input = input.substr(numberLength);
	}

	return output;
}

// Makes sure user string is only integers and spaces. Also looks for "exit".
bool parseUserInput(std::string input) {
	if (input.find("exit")!=-1) {
		std::cout << "Exiting...";
		exit(0);
	}
	bool allNumbers = true;
	for (int i = 0; i < input.length(); i++) {
		if (!isdigit(input[i])) {
			// allow spaces
			if(input[i] != ' ') allNumbers = false;
		}
	}
	return allNumbers;
}

int main()
{
	std::vector<int> userSteps;
	std::string inputString;
	int desiredStep = 0;
	int pathCount = 0;

	std::cout << "Welcome to the staircase. ";
	while (true) {
		std::cout << "What step do you want to climb to? Please write one number and press enter, or\n"
			<< "type \"exit\" and press enter to quit at any time.\n";
		std::getline(std::cin, inputString);
		try {
			desiredStep = std::stoi(inputString);
			std::cout << "So we will be climbing to step number " << desiredStep << ".\n"
				<< "What step sizes do you want to use? \n"
				<< "Please write your step sizes separated by spaces then press enter.\n";
			std::getline(std::cin, inputString, '\n');
			if (parseUserInput(inputString)) {
				userSteps = setVectorTo(inputString);
				pathCount = stairStepping(userSteps, desiredStep);

			}
			else {
				std::cout << "Please type only numbers separated by spaces.\n";
				
			}

			}
		catch (std::invalid_argument e) {
			std::cout << "Please just write a number.\n";
		}
	}

	return 0;
}

