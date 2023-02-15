#include <iostream>
#include <algorithm>

int recursiveFib(int n) {
    if (n == 0 || n == 1) {
        return n;
    } else {
        return recursiveFib(n - 1) + recursiveFib(n - 2);
    }
}

int main() {
    std::string input;
    while (true) {
        std::cout << "Enter an index, or input \"exit\" to quit.\n";
        std::cin >> input;
        std::transform(input.begin(), input.end(), input.begin(), ::tolower); // Convert input to lower case for case-nonsensitive input
        if (input == "exit") {
            return 0;
        }
        try {
            int n = std::stoi(input); // Attempt to convert input to int.
            if (n < 0) {
                std::cout << "Invalid option";
                continue;
            }
            std::cout << "The " << n << "th Fibonacci number is: " << recursiveFib(n) << std::endl;
        }
        catch (std::exception e) {
            std::cout << "Invalid option.\n";
        }

    }
}

