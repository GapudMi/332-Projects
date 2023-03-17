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
    int id;
    void print() { std::cout << "Pay is "<<pay<< "\nstartTime is " << startTime << "\nendTime is " << endTime << std::endl; }
};

bool compareTasks(Task a, Task b) {
    if (a.endTime < b.endTime) return true;
    return false;
}

std::vector<Task> bruteForce(std::vector<Task> tasks) {
    std::sort(tasks.begin(), tasks.end(), compareTasks);
    int maxValue = 0;
    std::vector<Task> bestRoute;
    std::vector<int> p;
    for (int i = 0; i < tasks.size(); i++)
        p.push_back(i);
    do {
        std::vector<Task> route;
        int bottom = -1;
        int value = 0;
        for (int i=0; i<p.size(); i++) {
            if (p[i] < bottom)
                continue;
            if (route.size() == 0) {
                route.push_back(tasks[p[i]]);
                value += tasks[p[i]].pay;
                bottom = p[i];
                continue;
            }
            if (route.back().endTime > tasks[p[i]].startTime)
                continue;
            route.push_back(tasks[p[i]]);
            value += tasks[p[i]].pay;
            bottom = p[i];
        }
        if (value > maxValue) {
            bestRoute = route;
            maxValue = value;
        }

    } while (std::next_permutation(p.begin(),p.end()));
    return bestRoute;
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
            task.id = i;
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
        // temp function to output the brute force
        std::vector<Task> cringe = bruteForce(tasks);
        std::cout << "Optimal route according to the brute force algorithm:\n";
        for (int i = 0; i < cringe.size(); i++) {
            std::cout << "Task #" << cringe[i].id << std::endl;
        }

        // Sort tasks by their END time, least to greatest
        std::sort(tasks.begin(), tasks.end(), compareTasks);
        for (int i = 0; i < tasks.size(); i++) {
            tasks.at(i).print();
        }
    }
}
