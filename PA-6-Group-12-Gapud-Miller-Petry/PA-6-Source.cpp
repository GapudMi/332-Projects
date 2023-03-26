#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

struct cell {
    int value = -1;
    std::pair<int, int> location = std::make_pair(-1, -1); // row, col pair denotes location in matrix
    cell* previous[3]; // A cell can have up to 3 cells leading into it.
    int change = 0; // ignore this lol
    bool arrow[3] = { false, false, false }; // Boolean values corresponding to where arrows go in order (↓ ↘ →) but pointing other way (D M R)
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
    // in future this will also print arrows and shit idk
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
                // std::cout << mat[i][j].value << "     ";
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
    /*
    // DEBUGGING HELL
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (mat[i][j] == cell{ {} }) {
                continue;
            }
            else {
                // std::cout << mat[i][j].value << "     ";
                int number = mat[i][j].value;
                std::string strNumber = std::to_string(number);
                int spaceLength = 5;
                for (int i = 0; i < spaceLength - strNumber.length(); i++) {
                    std::cout << ' ';
                }
                std::cout << strNumber;
                if (mat[i][j].arrow[2]) {
                    std::cout << 'R';
                }
                else std::cout << 'X';
                if (mat[i][j].arrow[1]) {
                    std::cout << 'M';
                }
                else std::cout << 'X';
                if (mat[i][j].arrow[0]) {
                    std::cout << 'U';
                }
                else std::cout << 'X';
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n\n";
    */
}

void printWords(std::vector<cell> path, std::string across, std::string down) {
    std::vector<char> first;
    std::vector<char> second;
    int lastStep = path.size() - 1;
    int lastChar = down.size() - 1;
    int last = path[lastStep].location.first; //rows aka down
    first.push_back(down[lastChar]); 
    lastChar--;
    for (int i = lastStep - 1; i > 0; i--) {
        if (last != path[i].location.first && lastChar >= 0) { // changed
            last = path[i].location.first; 
            first.insert(first.begin(), down[lastChar]);
            lastChar--;
        }
        else {
            last = path[i].location.first;
            first.insert(first.begin(), '_');
        }
    }

    lastChar = across.size() - 1;
    last = path[lastStep].location.second; //cols aka across
    second.push_back(across[lastChar]);
    lastChar--;
    for (int i = lastStep - 1; i > 0; i--) {
        if (last != path[i].location.second && lastChar >=0) { // changed
            last = path[i].location.second;
            second.insert(second.begin(), across[lastChar]);
            lastChar--;
        }
        else {
            last = path[i].location.second;
            second.insert(second.begin(), '_');
        }
    }
    for (int i = 0; i < second.size(); i++) {
        std::cout << second[i];
    }
    std::cout << "\n";
    for (int i = 0; i < first.size(); i++) {
        std::cout << first[i];
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
        // create matrix cell[rows][cols]
        cell** matrix = new cell * [rows];

        // Populate matrix
        for (int row = 0; row < rows; row++) {
            matrix[row] = new cell[cols];
            matrix[row][0] = cell{ row * -1, std::make_pair(row, 0), NULL };
            matrix[row][0].arrow[0] = true;
        }
        for (int col = 0; col < cols; col++) {
            matrix[0][col] = cell{ col * -1, std::make_pair(0, col), NULL };
            if (col != 0) matrix[0][col].arrow[2] = true;
        }

        for (int row = 1; row < rows; row++) {
            for (int col = 1; col < cols; col++) {
                cell c = {};
                int bestscore;
                cell* diag = &matrix[row - 1][col - 1];
                cell* left = &matrix[row][col - 1];
                cell* up = &matrix[row - 1][col];
                int diagScore = (sequenceOne[col - 1] == sequenceTwo[row - 1]) ? diag->value + 5 : diag->value - 2;
                diag->change = (sequenceOne[col - 1] == sequenceTwo[row - 1]) ? 5 : -2;
                int leftScore = left->value - 1;
                left->change = -1;
                int upScore = up->value - 1;
                up->change = -1;
                if (diagScore == leftScore && leftScore == upScore) {
                    c.value = diagScore;
                    c.previous[0] = diag;   c.arrow[1] = true;
                    c.previous[1] = left;   c.arrow[2] = true;
                    c.previous[2] = up;     c.arrow[0] = true;
                }
                else {
                    // values
                    cell* a[3] = { diag, left, up };
                    std::sort(std::begin(a), std::end(a), cellGreater);
                    c.value = a[0]->value + a[0]->change;
                    if (a[0]->value + a[0]->change == a[1]->value + a[1]->change) {
                        c.previous[0] = a[0];
                        c.previous[1] = a[1];
                    }
                    else {
                        c.previous[0] = a[0];
                    }

                    // arrows
                    c.arrow[0] = (upScore == c.value);
                    c.arrow[1] = (diagScore == c.value);
                    c.arrow[2] = (leftScore == c.value);

                }
                diag->change = 0;
                up->change = 0;
                left->change = 0;


                c.location = std::make_pair(row, col);
                matrix[row][col] = c;
            }
        }
        printMatrix(matrix, rows, cols, sequenceOne, sequenceTwo);
        std::vector<std::vector<cell>> * allPaths = new std::vector<std::vector<cell>>;
        getPaths(matrix, rows-1, cols-1, allPaths);
        for (int i = 0; i < allPaths->size(); i++) {
            printWords(allPaths->at(i), sequenceOne, sequenceTwo);
        }
    }
}
