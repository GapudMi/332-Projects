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
		for (int i = 0; i < steps.size(); i++) {
			x += stairSteppingRecursive(steps, destination - steps.at(i));
		}
		return x;
	}
}

int stairSteppingDynamic(std::vector<int>steps, int destination) {
	// placeholder for later implementation.
	return -1;
}

// This takes a vector and understands it as each entry of the vector being a digit in a number of base "base" and increments it by 1 according to those rules.
std::vector<int> incrementBaseN(int base, std::vector<int> number, int index) {
	if (number.at(index) < base - 1) {
		number.at(index)++;
		return number;
	}
	else { // (number.at(index)+1 == base)
		for (int i = 0; i <= index; i++) {
			number.at(i) = 0;
		}
		return incrementBaseN(base, number, index + 1);
	}
}

// This is a brute force algorithm that finds every way that the three steps can sum to the destination,
void printRoutes(std::vector<int>steps, int destination, int pathCount) {
	std::vector<std::vector<int>> history;
	std::vector<int> routeHolder;
	int foundPaths = 0;
	int sum = 0;
	bool dupe = false;

	int base = steps.size();
	std::vector<int> incrementHolder;

	// The maximum path length = the destination / the smallest step size. It might be too long by one, but it should never be too small.
	for (int i = 0; i < (destination / steps.at(0)); i++) {
		incrementHolder.push_back(0);
		routeHolder.push_back(0);
	}

	while (foundPaths < pathCount) {
		sum = 0;

		// routeHolder needs to be re-emptied every loop to accomodate history checking without false negatives, i think
		for (int j = 0; j < routeHolder.size(); j++) {
			routeHolder.at(j) = 0;
		}
		// Create one whole naive path
		for (int i = 0; i < incrementHolder.size(); i++) {
			sum += steps.at(incrementHolder.at(i));
			routeHolder.at(i) = steps.at(incrementHolder.at(i));
			if (sum == destination) {

				// Check against every single other path previously generated. If it is not a duplicate path, let it through
				for (int j = 0; j < history.size(); j++) {
					for (int k = 0; k <= i; k++) {
						if (routeHolder.at(k) == history.at(j).at(k)) {
							dupe = true;
						}
						else {
							dupe = false; break;
						}
					}
					if (dupe) {
						break;
					}
				}

				// Only if it is not a duplicate value, it is allowed to be added to the path history
				if (!dupe) {
					history.push_back(routeHolder);
					foundPaths++;
					std::cout << "Way " << foundPaths << ": ";
					for (int j = 0; j <= i; j++) {
						std::cout << routeHolder.at(j) << ((j < i) ? "->" : "");
					}
					std::cout << std::endl;
				}
			}
		}

		// increasing number by 1.
		incrementHolder = incrementBaseN(base, incrementHolder, 0);
	}
	return;
}

std::vector<int> setVectorTo(std::string input) {
	int numberLength = 0;
	std::vector<int> output;

	while (input.length() > 0) {
		while (input[0] == ' ') {
			input = input.substr(1);
			if (input.length() == 0) break;
		}
		numberLength = (input.find(' ') != -1) ? input.find(' ') : input.length();
		if (numberLength == 0) break;
		output.push_back(std::stoi(input.substr(0, numberLength)));
		input = input.substr(numberLength);
	}

	return output;
}

// Makes sure user string is only integers and spaces. Also looks for "exit".
bool parseUserInput(std::string input) {
	if (input.find("exit") != -1) {
		std::cout << "Exiting...";
		exit(0);
	}
	bool allNumbers = true;
	for (int i = 0; i < input.length(); i++) {
		if (!isdigit(input[i])) {
			// allow spaces
			allNumbers = (input[i] == ' ');
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
				if (userSteps.size()==1) {
					std::cout << "\tPlease enter two or more step sizes.\n";
					continue;
				}
				std::sort(userSteps.begin(), userSteps.end());
				bool dupe = true;
				for (int i = 0; i < userSteps.size()-1; i++) {
					if (userSteps.at(i) == userSteps.at(i + 1)) {
						std::cout << "\tPlease do not enter duplicate values.\n";
						dupe = true;
						break;
					}
					else dupe = false;
				}

				if(!dupe){
					pathCount = stairSteppingRecursive(userSteps, desiredStep);
					std::cout << "\tWe found " << pathCount << ((pathCount == 1) ? " path " : " paths ") << "using those step sizes to get to step " << desiredStep << ".\n";
					printRoutes(userSteps, desiredStep, pathCount);
				}
			}
			else {
				std::cout << "\tPlease type only 2 or more numbers separated by spaces.\n";

			}

		}
		catch (std::invalid_argument e) {
			std::cout << "Please just write a number or \"exit\".\n";
		}
	}

	return 0;
}
