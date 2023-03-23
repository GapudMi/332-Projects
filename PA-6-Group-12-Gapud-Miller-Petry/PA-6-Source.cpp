#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

struct cell {
    int value = -1;
    std::pair<int, int> location = std::make_pair(-1, -1); // row, col pair denotes location in matrix
    int previous[3]; // A cell can have up to 3 cells leading into it.
};

bool operator== (const cell& a, const cell& b) {
    return ((a.location.first == b.location.first) && (a.location.second == b.location.second));
}

void checkForExit(std::string input) {
    std::transform(input.begin(), input.end(), input.begin(),
                   ::tolower); // Convert input to lower case for case-nonsensitive input
    if (input == "exit") exit(0);
}

void printMatrix(cell** mat, int rows, int cols) {
    // in future this will also print arrows and shit idk
    std::cout << " ";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (mat[i][j] == cell{{}})  {
                continue;
            }
            else std::cout << mat[i][j].value << "     ";
        }
        std::cout << "\n\n";
    }
}

int main() {
    std::string divider = "     =============================================================================\n";
    while (true) {
        std::cout << divider;
        std::cout << "\tWelcome. Please enter your first sequence, then press enter.\n"
                  << "\tA sequence is a combination of letters and gaps (denoted by an underscore).\n"
                  << "\tAlternatively, type \"exit\" and press enter to quit at any time.\n" << divider;
        std::string sequenceOne;
        std::cin >> sequenceOne;
        checkForExit(sequenceOne);
        std::cout << "\tEnter your second sequence.\n";
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
        printMatrix(matrix, rowCount, colCount);
    }
}