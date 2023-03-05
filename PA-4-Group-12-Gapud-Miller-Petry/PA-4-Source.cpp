// PA-4-Source.cpp 

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

int stairSteppingRecursive(std::vector<int> steps, int destination) {
	//std::cout << destination << " ";

	// I think this is correct, not the definition you gave us in class. 
	// If the destination - step value == 0, that always means a path was found 
	// This is because if some number of steps summed to the destination value, that sum - destination value necessarily equals zero.
	if (destination == 0) return 1;
	else if (destination < 0) return 0;
	else {
		int x = 0;
		for(int i = 0; i < steps.size(); i++){
			x += stairSteppingRecursive(steps, destination - steps.at(i));
		}
		return x;
	}
}

int stairSteppingDynamic(std::vector<int>steps, int destination) {
	// placeholder for later implementation.
	return -1;
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
		std::cout << "\n\tWhat step do you want to climb to?\n"
			<< "\tType your desired step number and press enter,\n"
			<< "\tor type \"exit\" and press enter to quit at any time.\n";
		std::getline(std::cin, inputString);
		std::transform(inputString.begin(), inputString.end(), inputString.begin(), ::tolower); // Convert input to lower case for case-nonsensitive input
		if (inputString == "exit") {
			return 0;
		} 
		try {
			desiredStep = std::stoi(inputString);
			std::cout << "\n\tSo we will be climbing to step number " << desiredStep << ".\n"
				<< "\tWhat step sizes do you want to use? \n"
				<< "\tPlease write your step sizes separated by spaces then press enter.\n";
			std::getline(std::cin, inputString, '\n');
			if (parseUserInput(inputString)) {
				userSteps = setVectorTo(inputString);
				std::sort(userSteps.begin(), userSteps.end());
				//debugging
				//std::cout << "inputString: " << inputString << "\nstep sizes: " << userSteps.at(0) << "\t" << userSteps.at(1) << "\n";
				
				pathCount = stairSteppingRecursive(userSteps, desiredStep);
				std::cout << "\tWe found " << pathCount << ((pathCount == 1) ? " path " : " paths ") << "using those step sizes to get to step " << desiredStep << ".\n";
			}
			else {
				std::cout << "\tPlease type only numbers separated by spaces.\n";
				
			}

			}
		catch (std::invalid_argument e) {
			std::cout << "Please just write a number or \"exit\".\n";
		}
	}

	return 0;
}

