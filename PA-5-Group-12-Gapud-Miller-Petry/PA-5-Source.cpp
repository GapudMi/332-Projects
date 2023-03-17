// PA-5-Source.cpp

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <limits>

struct Task {
    int pay;
    int startTime;
    int endTime;
    void print() { std::cout << "Pay is "<<pay<< " startTime is " << startTime << " endTime is " << endTime << std::endl; }
};

bool compareTasks(Task a, Task b) {
    if (a.endTime < b.endTime) return true;
    return false;
}

int main() {
    int numTasks = 0;
    std::vector<Task> tasks;
    std::string inputString;
    std::string divider = "     =============================================================================\n";
    while (true) {
        std::cout << divider;
        std::cout << "\tHow many paid tasks are there?\n"
                  << "\tType your number and press enter,\n"
                  << "\tor type \"exit\" and press enter to quit at any time.\n" << divider;
        while (true) {
            try {
                std::getline(std::cin, inputString);
                std::transform(inputString.begin(), inputString.end(), inputString.begin(),
                               ::tolower); // Convert input to lower case for case-nonsensitive input
                if (inputString == "exit") {
                    return 0;
                }
                numTasks = std::stoi(inputString);
                break;
            }
            catch (std::invalid_argument e) {
                std::cout << "\tInvalid input, please try again.\n";
            }
        }
        if (numTasks <= 0) {
            std::cout << "\tPlease only enter numbers greater than zero.\n";
            continue;
        }
        std::cout << divider << "\tYou will now enter the pay and duration of each task.\n"
                  << "\tPlease enter the salary, then the start time, then the start time, and finally the end time.\n"
                  << "\tEnter all times as integers between 0 and 512.\n"
                  << "\tPress enter after each input.\n" << divider;
        for (int i = 0; i < numTasks; i++) {
           Task task;
            std::string start;
            std::string end;
            std::cout << "\tTask " << i + 1 << std::endl;
            std::cout << "\tWhat is this task's payment?\n";
            std::cin >> task.pay;
            while (true) {
                std::cout << "\tWhat time does this task start?\n";
                    std::cin >> start;
                
                std::cout << "\tWhat time does this task end?\n";
                    std::cin >> end;
                task.endTime = std::stoi(end);
                task.startTime = std::stoi(start);
                if (task.endTime > task.startTime)
                    break;
                std::cout << "\tEnd time must be after start time.\n";
            }
            std::cout << divider;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            tasks.push_back(task);
        }
        // Sort tasks by their END time, least to greatest
        std::sort(tasks.begin(), tasks.end(), compareTasks);

        for (int i = 0; i < tasks.size(); i++) {
            tasks.at(i).print();
        }
    }
}
