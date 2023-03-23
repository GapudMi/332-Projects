#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

struct cell {
    int value;
    std::pair<int, int> location; // x, y pair denotes location in matrix
    int previous[3]; // A cell can have up to 3 cells leading into it.
};

void checkForExit(std::string input) {
    std::transform(input.begin(), input.end(), input.begin(),
                   ::tolower); // Convert input to lower case for case-nonsensitive input
    if (input == "exit") exit(0);
}

int main() {
    std::string divider = "     =============================================================================\n";
    std::cout << divider;
    while (true) {
        std::cout << "\tWelcome. Please enter your first sequence, then press enter.\n"
                  << "\tA sequence is a combination of letters and gaps (denoted by an underscore).\n"
                  << "\tAlternatively, type \"exit\" and press enter to quit at any time.\n" << divider;
        std::string sequenceOne;
        std::cin >> sequenceOne;
        checkForExit(sequenceOne);
        std::cout << "\tEnter your second sequence.";
        std::string sequenceTwo;
        std::cin >> sequenceTwo;
        checkForExit(sequenceTwo);

        // initialize 2D array :3
        int rowSize = sequenceOne.length();
        int colSize = sequenceTwo.length();
        cell **matrix;
        matrix = new cell*[rowSize];
        for (int i = 0; i < rowSize; i++) {
            matrix[i] = new cell[colSize];
        }
    }
}