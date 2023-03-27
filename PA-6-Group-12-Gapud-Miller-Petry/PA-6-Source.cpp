#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <limits>

struct cell {
    int value = -1;
    std::pair<int, int> location = std::make_pair(-1, -1); // row, col pair denotes location in matrix
    cell* previous[3]; // A cell can have up to 3 cells leading into it.
    int change = 0; // ignore this lol
    bool arrow[3] = { false, false, false }; // Boolean values corresponding to where arrows go in order (down diagonal right) but pointing other way
};

bool operator== (const cell& a, const cell& b) {
    return ((a.location.first == b.location.first) && (a.location.second == b.location.second));
}

bool cellGreater(cell* &a, cell* &b) {
    return (a->value > b->value);
}

void checkForExit(std::string input) {
    std::transform(input.begin(), input.end(), input.begin(),
        ::tolower); // Convert input to lower case for case-nonsensitive input
    if (input == "exit") exit(0);
}


void printMatrix(cell** mat, int rows, int cols, std::string across, std::string down) {
    for (int i = 0; i < rows; i++) {
        if (i != 0) {
            std::cout << "\n        ";
            for (int j = 0; j < cols; j++) {
                if (j == 0) {
                    std::cout << "|   "; continue;
                }
                if (mat[i][j].arrow[1]) {
                    std::cout << "\\   ";
                }
                else {
                    std::cout << "    ";
                }
                if (mat[i][j].arrow[0]) {
                    std::cout << "|   ";
                }
                else {
                    std::cout << "    ";
                }
            }
        }
        else {
            std::cout << "                ";
            for (int j = 0; j < cols; j++) {
                std::cout << across[j] << "       ";
            }
        }
        std::cout << "\n";
        for (int j = 0; j < cols; j++) {
            if (j == 0) std::cout << ((i > 0) ? down[i - 1] : ' ');
            if (mat[i][j] == cell{ {} }) {
                continue;
            }
            else {
                int number = mat[i][j].value;
                std::string strNumber = std::to_string(number);
                int spaceLength = 4;

                if (mat[i][j].arrow[2]) {
                    std::cout << "  ->";
                }
                else { std::cout << "    "; }
                for (int i = 0; i < spaceLength - strNumber.length(); i++) {
                    std::cout << ' ';
                }
                std::cout << number;
            }
        }

    }

    std::cout << "\n\n";
}


void printWords(std::vector<cell> path, std::string across, std::string down) {
    std::reverse(path.begin(), path.end());
    std::vector<char> acr;
    std::vector<char> dow;
    int acLastChar = 0;
    int doLastChar = 0;
    std::pair<int, int> lastPair = path[0].location;
    std::pair<int, int> thisPair;
    for (int i = 1; i < path.size(); i++) {
        thisPair = path[i].location;
        if (lastPair.first == thisPair.first - 1 && doLastChar < down.size()) { // change in row
            dow.push_back(down[doLastChar]);
            doLastChar++;
        }
        else {
            dow.push_back('_');
        }
        if (lastPair.second == thisPair.second - 1 && acLastChar < across.size()) { // change in col
            acr.push_back(across[acLastChar]);
            acLastChar++;
        }
        else {
            acr.push_back('_');
        }
        lastPair = thisPair;
    }
    std::cout << "\tAlignment value: " << path[path.size()-1].value << "\n\t";
    for (char letter : dow) {
        std::cout << letter;
    }
    std::cout << "\n\t";
    for (char letter : acr) {
        std::cout << letter;
    }
    std::cout << "\n";
    std::cout << "\n";
}

std::vector<cell> getPaths(cell** matrix, int rows, int cols, std::vector<std::vector<cell>> * allpaths, std::vector<cell> path = {}) {
    int counter = 0;
    if (rows>-1&&cols>-1) {
        cell current = matrix[rows][cols];
        path.push_back(current);
        for (bool val : current.arrow) {
            if (val) counter++;
        }
        if (counter > 1) {
            if (current.arrow[0]) { // up
                counter--;
                if (counter == 0) {
                    return getPaths(matrix, rows - 1, cols, allpaths, path);
                }
                else {
                    getPaths(matrix, rows - 1, cols, allpaths, path);
                }
            }
            if (current.arrow[1]) { // diag
                counter--;
                if (counter == 0) {
                    return getPaths(matrix, rows - 1, cols - 1, allpaths, path);
                }
                else {
                    getPaths(matrix, rows - 1, cols - 1, allpaths, path);
                }
            }
            if (current.arrow[2]) { // left
                counter--;
                if (counter == 0) {
                    return getPaths(matrix, rows, cols - 1, allpaths, path);
                }
                else {
                    getPaths(matrix, rows, cols - 1, allpaths, path);
                }
            }
        }
        else {
            if (current.arrow[0]) { // up
                return getPaths(matrix, rows - 1, cols, allpaths, path);
            }
            if (current.arrow[1]) { // diag
                return getPaths(matrix, rows - 1, cols - 1, allpaths, path);
            }
            if (current.arrow[2]) { // left
                return getPaths(matrix, rows, cols - 1, allpaths, path);
            }
        }
    }
    if (rows == 0 && cols == 0) {
        allpaths->push_back(path);
    }
    return path;
}

int main() {
    std::string divider = "     =============================================================================\n";
    while (true) {
        std::cout << divider;
        std::cout << "\tWelcome. Please enter your first sequence, then press enter.\n"
            << "\tA sequence is a combination of letters and gaps (denoted by an underscore).\n"
            << "\tAlternatively, type \"exit\" and press enter to quit at any time.\n" << divider;
        std::string sequenceOne;
        std::getline(std::cin, sequenceOne);
        checkForExit(sequenceOne);
        std::cout << "\tEnter your second sequence.\n";
        std::string sequenceTwo;
        std::getline(std::cin, sequenceTwo);
        checkForExit(sequenceTwo);

        // initialize 2D array :3
        int cols = sequenceOne.length() + 1;
        int rows = sequenceTwo.length() + 1;
        cell** matrix = new cell * [rows];

        // Get payoff matrix
        std::string inputString;
        int match;
        int mismatch;
        int gap;
        std::cout << divider << "\tHow many points are gained for a match?\n"
            << "\tType your number and press enter,\n"
            << "\tor type \"exit\" and press enter to quit at any time.\n" << divider;
        while (true) {
            try {
                std::cin >> inputString;
                checkForExit(inputString);
                match = std::stoi(inputString);
                break;
            }
            catch (std::invalid_argument& e) {
                std::cout << "\tInvalid input, please try again.\n";
            }
        }
        std::cout << divider << "\tHow many points are LOST for a mismatch?\n"
            << "\tType your number and press enter,\n" << divider;
        while (true) {
            try {
                std::cin >> inputString;
                checkForExit(inputString);
                mismatch = std::stoi(inputString);
                break;
            }
            catch (std::invalid_argument& e) {
                std::cout << "\tInvalid input, please try again.\n";
            }
        }
        std::cout << divider << "\tHow many points are LOST for a gap?\n"
            << "\tType your number and press enter,\n" << divider;
        while (true) {
            try {
                std::cin >> inputString;
                checkForExit(inputString);
                gap = std::stoi(inputString);
                break;
            }
            catch (std::invalid_argument& e) {
                std::cout << "\tInvalid input, please try again.\n";
            }
        }
        // Populate matrix
        for (int row = 0; row < rows; row++) {
            matrix[row] = new cell[cols];
            matrix[row][0] = cell{ row * -1, std::make_pair(row, 0), NULL };
            matrix[row][0].arrow[0] = true;
            if (row != 0) {
                matrix[row][0].value = matrix[row-1][0].value - gap;
            }
            else {
                matrix[row][0].value = 0;
            }
        }
        for (int col = 0; col < cols; col++) {
            matrix[0][col] = cell{ col * -1, std::make_pair(0, col), NULL };
            if (col != 0) {
                matrix[0][col].arrow[2] = true;
                matrix[0][col].value = matrix[0][col-1].value - gap;
            }
            else {
                matrix[0][col].value = 0;
            }
        }
        // calculate all paths
        for (int row = 1; row < rows; row++) {
            for (int col = 1; col < cols; col++) {
                cell c = {};
                cell* diag = &matrix[row - 1][col - 1];
                cell* left = &matrix[row][col - 1];
                cell* up = &matrix[row - 1][col];
                int diagScore = (sequenceOne[col - 1] == sequenceTwo[row - 1]) ? diag->value + match : diag->value - mismatch;
                diag->change = (sequenceOne[col - 1] == sequenceTwo[row - 1]) ? match : -mismatch;
                int leftScore = left->value - gap;
                left->change = - gap;
                int upScore = up->value - gap;
                up->change = - gap;
                if (diagScore == leftScore && leftScore == upScore) {
                    c.value = diagScore;
                    c.previous[0] = diag;   c.arrow[1] = true;
                    c.previous[1] = left;   c.arrow[2] = true;
                    c.previous[2] = up;     c.arrow[0] = true;
                }
                else {
                    // THIS CODE IS TERRIBLE
                    if (diagScore > leftScore && diagScore > upScore) {
                        c.value = diagScore;
                        c.previous[0] = diag;   c.arrow[1] = true;
                    }
                    else if (upScore > diagScore && upScore > leftScore) {
                        c.value = upScore;
                        c.previous[0] = up;   c.arrow[0] = true;
                    }
                    else if (leftScore > upScore && leftScore > diagScore) {
                        c.value = leftScore;
                        c.previous[0] = left;   c.arrow[2] = true;
                    }
                    else if (leftScore == diagScore) {
                        c.value = diagScore;
                        c.previous[0] = diag;   c.arrow[1] = true;
                        c.previous[1] = left;   c.arrow[2] = true;
                    }
                    else if (leftScore == upScore) {
                        c.value = leftScore;
                        c.previous[0] = left;   c.arrow[2] = true;
                        c.previous[1] = up;   c.arrow[0] = true;
                    }
                    else if (diagScore == upScore) {
                        c.value = upScore;
                        c.previous[0] = diag;   c.arrow[1] = true;
                        c.previous[1] = up;   c.arrow[0] = true;
                    }
                }
                diag->change = 0;
                up->change = 0;
                left->change = 0;

                c.location = std::make_pair(row, col);
                matrix[row][col] = c;
            }
        }
        std::cout << divider << "\tAlignment chart:\n";
        printMatrix(matrix, rows, cols, sequenceOne, sequenceTwo);
        std::vector<std::vector<cell>>* allPaths = new std::vector<std::vector<cell>>;
        getPaths(matrix, rows - 1, cols - 1, allPaths);
        std::cout << divider << "Press enter to see all optimal alignments.";
        std::getchar();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << divider << "\tAll optimal combinations:\n";
        for (int i = 0; i < allPaths->size(); i++) {
            printWords(allPaths->at(i), sequenceOne, sequenceTwo);
        }
    }
}
