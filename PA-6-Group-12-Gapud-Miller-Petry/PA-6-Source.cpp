#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

struct cell {
    int value;
    std::pair<int, int> location; // row, col pair denotes location in matrix
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
        int colCount = sequenceOne.length();
        int rowCount = sequenceTwo.length();

        // create matrix cell[rowCount][colCount]
        cell** matrix = new cell*[rowCount];
        for(int row = 0; row < rowCount; row++) {
            matrix[row] = new cell[colCount];
            matrix[row][0] = cell{row*-1, std::make_pair(row, 0), NULL};
        }
        for (int col = 0; col < colCount; col++) {
            matrix[0][col] = cell{col*-1, std::make_pair(0, col), NULL};
        }
    }
}