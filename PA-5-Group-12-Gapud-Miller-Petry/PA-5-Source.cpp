// PA-4-Source.cpp

#include <iostream>
#include <string>
#include <algorithm>
#include <limits>
#include <vector>
#include <ctime>

struct Task {
    int pay;
    std::tm startTime;
    std::tm endTime;
};

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

std::tm parseTime(std::string str) {
    //std::cout << str.substr(0, str.find(':')) << std::endl;
    //std::cout << str.substr(str.find(':')+1) << std::endl;

    int hour = std::stoi(str.substr(0, str.find(':')));
    int minute = std::stoi(str.substr(str.find(':')+1));
    std::tm time{};
    time.tm_hour = hour;
    time.tm_min = minute;
    return time;
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
                  << "\tor type \"exit\" and press enter to quit at any time.\n";
        std::cout << divider;
        std::getline(std::cin, inputString);
        std::transform(inputString.begin(), inputString.end(), inputString.begin(),
                       ::tolower); // Convert input to lower case for case-nonsensitive input
        std::cout << divider;

        if (inputString == "exit") {
            return 0;
        }
        numTasks = std::stoi(inputString);
        if (numTasks <= 0) {
            std::cout << "\tPlease only enter numbers greater than zero.\n";
            continue;
        }
        std::cout << "\tSo there are " << numTasks << " tasks.\n"
            << "\tYou will now enter the pay and duration of each task.\n"
            << "\tPlease enter the salary, then the start time, then the start time, and finally the end time.\n"
            << "\tPress enter after each input.\n";
        std::cout << divider;
        for (int i = 0; i < numTasks; i++) {
            Task task;
            //int pay = 0;
            std::string start;
            std::string end;
            std::cout << "\tTask " << i+1 << std::endl;
            std::cout << "\tWhat is this task's payment?\n";
            std::cin >> task.pay;
            std::cout << "\tWhat time does this task start?\n";
            std::cin >> start;
            task.startTime = parseTime(start);
            std::cout << "\tWhat time does this task end?\n";
            std::cin >> end;
            task.endTime = parseTime(end);
            std::cout << divider;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            tasks.push_back(task);
        }
    }
}