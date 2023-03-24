#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

struct cell {
    int value = -1;
    std::pair<int, int> location = std::make_pair(-1, -1); // row, col pair denotes location in matrix
    cell* previous[3]; // A cell can have up to 3 cells leading into it.
    bool arrow[3] = {false, false, false}; // Boolean values corresponding to where arrows go in order (↘ ↓ →) but pointing other way (M D R)
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
    for (int i = 0; i < rows; i++) {
        if (i != 0) {
            std::cout << "\n       ";
            for (int j = 1; j < cols; j++) {
            
                if (mat[i - 1][j - 1].arrow[1]) {
                    std::cout << "|   ";
                }
                else std::cout << "    ";
                if (mat[i - 1][j - 1].arrow[0]) {
                    std::cout << "\\   ";
                }
                else std::cout << "    ";
            }
        }
        std::cout << "\n";
        for (int j = 0; j < cols; j++) {
            if (mat[i][j] == cell{ {} }) {
                continue;
            }
            else {
                // std::cout << mat[i][j].value << "     ";
                int number = mat[i][j].value;
                std::string strNumber = std::to_string(number);
                int spaceLength = 4;
                
                if (mat[i][j].arrow[2]) {
                    std::cout << "  ->";
                } else { std::cout << "    "; }
                for (int i = 0; i < spaceLength - strNumber.length(); i++) {
                    std::cout << ' ';
                }
                std::cout << number;
            }
        }

    }

    std::cout << "\n\n";
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
        int cols = sequenceOne.length() + 1;
        int rows = sequenceTwo.length() + 1;
        // create matrix cell[rows][cols]
        cell** matrix = new cell * [rows];

        // Populate matrix
        for (int row = 0; row < rows; row++) {
            matrix[row] = new cell[cols];
            matrix[row][0] = cell{ row * -1, std::make_pair(row, 0), NULL };
        }
        for (int col = 0; col < cols; col++) {
            matrix[0][col] = cell{ col * -1, std::make_pair(0, col), NULL };
        }

        for (int row = 1; row < rows; row++) {
            for (int col = 1; col < cols; col++) {
                cell c = {};
                int ptr = 0;
                int bestScore = 0;
                if (sequenceOne[col - 1] == sequenceTwo[row - 1]) {
                    c.previous[0] = &matrix[row - 1][col - 1];
                    ptr++;
                    bestScore = matrix[row - 1][col - 1].value + 5;
                    c.arrow[0] = true; // ↘
                }
                cell* up = &matrix[row - 1][col];
                cell* left = &matrix[row][col - 1];
                if (left->value == up->value) {
                    if (matrix[row][col - 1].value >= bestScore) {
                        bestScore = left->value - 1;
                        c.previous[ptr] = left;
                        c.previous[ptr + 1] = up;
                        c.arrow[1] = true; // ↓
                        c.arrow[2] = true; // →
                    }
                }
                else {
                    cell* prev = (left->value > up->value) ? left : up;

                    if (prev->value >= bestScore) {
                        //c.value = prev->value - 1;
                        bestScore = prev->value - 1;
                        c.previous[ptr] = prev;
                        c.arrow[1] = !(left->value > up->value); // ↓ (up is >)
                        c.arrow[2] = (left->value > up->value); // → (left is >)
                    }

                }
                c.value = bestScore;
                c.location = std::make_pair(row, col);
                matrix[row][col] = c;
            }
        }
        printMatrix(matrix, rows, cols);
    }
}
