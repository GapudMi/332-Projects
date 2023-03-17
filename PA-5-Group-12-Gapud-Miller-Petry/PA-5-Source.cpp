// PA-5-Source.cpp

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

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

void numberLine(std::vector<Task> set, int interval){
    for (int i = 0; i < set.at(set.size() - 1).endTime; i += interval) {
        // counter every multiple of interval
        std::cout << i;
        std::string num = std::to_string(i);
        for (int j = 0; j < interval - num.length(); j++) {
            std::cout << " ";
        }
    }
    std::cout << "\n";
}
void dottedLine(std::vector<Task> set, int interval, char marker, char normal) {
    for (int i = 0; i < set.at(set.size() - 1).endTime; i++) {
        if (i % interval == 0) {
            std::cout << marker;
        }
        else std::cout << normal;
    }
    std::cout << "\n";
}

void visualization(std::vector<Task> set) {
    std::cout << "\nStarting visualization\n";
    //whitespace alignment to account for the space where the task numbers are written
    std::string taskSpacer = "       ";
    std::cout << taskSpacer << ((set.size() < 10) ? " " : ((set.size() < 100) ? "  " : "   "));
    numberLine(set, 5);

    std::cout << taskSpacer << ((set.size() < 10) ? " " : ((set.size() < 100) ? "  " : "   "));
    dottedLine(set, 5, '|', '.');

    for (int i = 0; i < set.size(); i++) {
        //Add whitespace according to time before start time. 
        std::cout << "Task " << i + 1;
        if (set.size() >= 100) {
            std::cout << ((i+1 >= 100) ? "" : " ");
        }
        if (set.size() >= 10) {
            std::cout << ((i+1 >= 10) ? "" : " ");
        }
        std::cout << ':';
        for (int j = 0; j < set.at(i).startTime; j++) {
            std::cout << " ";
        }
        for (int j = set.at(i).startTime; j < set.at(i).endTime; j++) {
            std::cout << (char)254u;
        }
        std::cout << "\n";
    }

    std::cout << taskSpacer << ((set.size() < 10) ? " " : ((set.size() < 100) ? "  " : "   "));
    dottedLine(set, 5, '|', '\'');
    std::cout << taskSpacer << ((set.size() < 10) ? " " : ((set.size() < 100) ? "  " : "   "));
    numberLine(set, 5);
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
                  << "\tEnter all times as integers between 0 and 220 if you want the visualization to work.\n"
                  << "\tPress enter after each input.\n" << divider;
        for (int i = 0; i < numTasks; i++) {
           Task task;
            std::string start;
            std::string end;
            std::string pay;
            while (true) {
                std::cout << "\tTask " << i + 1 << std::endl;
                std::cout << "\tWhat is this task's payment?\n";
                    std::cin >> pay;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                    
                if (pay == "randomtasks") break;                                      // For debugging purposes, remove later
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

                std::cout << "\tWhat time does this task start?\n";
                    std::cin >> start;
                std::cout << "\tWhat time does this task end?\n";
                    std::cin >> end;
                task.pay = std::stoi(pay);
                task.endTime = std::stoi(end);
                task.startTime = std::stoi(start);
                if (task.endTime > task.startTime)
                    break;
                std::cout << "\tEnd time must be after start time.\n";
            }
            std::cout << divider;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                    
            if (pay == "randomtasks"){                                                  // For debugging purposes, remove later
                srand((unsigned)time(NULL));
                for (int i = 0; i < numTasks; i++) {
                    int randomStart = (rand() % (200-10));
                    task.startTime = randomStart;
                    int randomEnd = randomStart + (rand() % (200-randomStart));
                    task.endTime = randomEnd;
                    int randomPay = rand() % 50;
                    task.pay = randomPay;
                    tasks.push_back(task);
                }
                break;
            }
            tasks.push_back(task);
        }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
        // Sort tasks by their END time, least to greatest
        std::sort(tasks.begin(), tasks.end(), compareTasks);

        for (int i = 0; i < tasks.size(); i++) {
            tasks.at(i).print();
        }

        visualization(tasks);
    }
}
