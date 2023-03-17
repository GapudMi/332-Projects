// PA-4-Source.cpp

#include <iostream>
#include <string>
#include <algorithm>
#include <limits>
#include <vector>
#include <ctime>
#include <optional>

struct Task {
    int pay;
    std::tm startTime;
    std::tm endTime;
};

// Convert string time into the C++ time struct
std::tm parseTime(std::string str) {
    std::tm time{};
    int hour;
    int minute;
    if (std::count(str.begin(), str.end(), ':') != 1)
        return (std::tm){-1};
    try {
        hour = std::stoi(str.substr(0, str.find(':')));
        minute = std::stoi(str.substr(str.find(':') + 1));
    }
    catch(std::exception) {
        return (std::tm){-1};
    }
    time.tm_hour = hour;
    time.tm_min = minute;
    return time;

}

// Returns true if gt > lt, otherwise returns false.
bool compareTime(std::tm gt, std::tm lt) {
    if (gt.tm_hour > lt.tm_hour)
        return true;
    else if (gt.tm_min > lt.tm_min)
        return true;
    else
        return false;
}

bool compareStartTime(Task lt, Task gt) {
    return compareTime(lt.startTime, gt.startTime);
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
            << "\tEnter all times in HH:MM format.\n"
            << "\tPress enter after each input.\n" << divider;
        for (int i = 0; i < numTasks; i++) {
            Task task;
            //int pay = 0;
            std::string start;
            std::string end;
            std::cout << "\tTask " << i+1 << std::endl;
            std::cout << "\tWhat is this task's payment?\n";
            std::cin >> task.pay;
            while (true) {
                std::cout << "\tWhat time does this task start?\n";
                while (true) {
                    std::cin >> start;
                    task.startTime = parseTime(start);
                    // If successfully parsed, we will continue to the next input.
                    // If parse failed, try again.
                    if (task.startTime.tm_sec != -1)
                        break;
                    std::cout << "\tInvalid format, please try again.\n";
                }
                std::cout << "\tWhat time does this task end?\n";
                while (true) {
                    std::cin >> end;
                    task.endTime = parseTime(end);
                    if (task.endTime.tm_sec != -1)
                        break;
                    std::cout << "\tInvalid format, please try again.\n";
                }
                if (compareTime(task.endTime, task.startTime))
                    break;
                std::cout << "\tEnd time must be after start time.\n";
            }
            std::cout << divider;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            tasks.push_back(task);
        }
        // Sort tasks by their start time
        std::sort(tasks.begin(), tasks.end(), compareStartTime);
    }
}