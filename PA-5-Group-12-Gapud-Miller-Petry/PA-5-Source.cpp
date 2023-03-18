// PA-5-Source.cpp

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <limits>
#include <chrono>

struct Task {
    int pay;
    int startTime;
    int endTime;
    int id;

    void print() {
        std::cout << "Task " << id << ": Pay is " << pay << "\nstartTime is " << startTime << "\nendTime is " << endTime << std::endl;
    }

    bool operator==(const Task& b) const { return id == b.id; }
};

// Returns true if a ends before b
bool compareTasks(const Task& a, const Task& b) {
    return (a.endTime < b.endTime);
}

bool routesEqual(const std::vector<Task>& a, const std::vector<Task>& b) {
    if (a.size() != b.size())
        return false;
    for (int i=0; i<a.size(); i++) {
        if (a[i].id != b[i].id)
            return false;
    }
    return true;
}

// Brute force algorithms utilizing permutations
std::vector<std::vector<Task>> bruteForce(std::vector<Task> tasks) {
    int routesCompleted = 0;
    std::vector<std::vector<Task>> routeList;
    std::sort(tasks.begin(), tasks.end(), compareTasks);
    int maxValue = 0;
    std::vector<Task> bestRoute;
    std::vector<int> p;
    for (int i = 0; i < tasks.size(); i++)
        p.push_back(i);
    do {
        if (p[0] > routesCompleted) {
            routesCompleted++;
            p.erase(p.begin()+1);
        }

        std::vector<Task> route;
        route.clear();
        int bottom = -1;
        int value = 0;
        for (int i = 0; i < p.size(); i++) {
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
            routeList.clear();
            routeList.push_back(bestRoute);
            maxValue = value;
        }
        else if (value == maxValue) {
            if (!routesEqual) {
                std::cout << "something bad happened";
                routeList.push_back(bestRoute);
            }

        }
    } while (std::next_permutation(p.begin(), p.end()));
    return routeList;
}


void numberLine(std::vector<Task> set, int interval) {
    for (int i = 0; i < set.at(set.size() - 1).endTime + 4; i += interval) {
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
    for (int i = 0; i < set.at(set.size() - 1).endTime + 4; i++) {
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
    std::string taskSpacer = "      ";
    std::cout << taskSpacer << ((set.size() < 10) ? " " : ((set.size() < 100) ? "  " : "   "));
    numberLine(set, 5);

    std::cout << taskSpacer << ((set.size() < 10) ? " " : ((set.size() < 100) ? "  " : "   ")); dottedLine(set, 5, 'v', ' ');

    for (int i = 0; i < set.size(); i++) {
        //Add whitespace according to time before start time.
        std::cout << "Task " << set.at(i).id;
        if (set.size() >= 100) {
            std::cout << ((set.at(i).id >= 100) ? "" : " ");
        }
        if (set.size() >= 10) {
            std::cout << ((set.at(i).id >= 10) ? "" : " ");
        }
        std::cout << ':';
        for (int j = 0; j < set.at(i).startTime; j++) {
            if (j % 5 == 0) {
                std::cout << "|";
            }
            else std::cout << "-";
        }
        int barStartTime;
        for (int j = set.at(i).startTime; j < set.at(i).endTime; j++) {
            barStartTime = set.at(i).startTime;
            std::cout << (char)254u;
        }
        for (int j = set.at(i).endTime; j < set.at(set.size() - 1).endTime; j++) {
            if (j % 5 == 0) {
                std::cout << "|";
            }
            else std::cout << "-";
        }
        /* for debugging
        std::cout << "\n\t";
        std::cout << "[Start: " << set.at(i).startTime << ", End: " << set.at(i).endTime << ", Pay : " << set.at(i).pay << "]\n";
        */
        for (int j = (set.at(set.size() - 1).endTime) % 5; j < 5; j++) {
            if (j == 0) break;
            std::cout << '-';
        }
        std::cout << "| Pay: " << set.at(i).pay << "\n";
    }

    std::cout << taskSpacer << ((set.size() < 10) ? " " : ((set.size() < 100) ? "  " : "   ")); dottedLine(set, 5, '^', ' ');
    std::cout << taskSpacer << ((set.size() < 10) ? " " : ((set.size() < 100) ? "  " : "   "));
    numberLine(set, 5);
}


int main() {
    int numTasks = 10;
    std::vector<Task> tasks;
    std::string inputString;
    std::string divider = "     =============================================================================\n";
    while (true) {
        tasks.clear();
        std::cout << divider;
        std::cout << "\tHow many paid tasks are there?\n"
            << "\tType your number and press enter,\n"
            << "\tor type \"exit\" and press enter to quit at any time.\n" << divider;
        /*while (true) {
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
        }*/
        std::cout << divider << "\tYou will now enter the pay and duration of each task.\n"
            << "\tPlease enter the salary, then the start time, then the start time, and finally the end time.\n"
            << "\tEnter all times as integers between 0 and 220 if you want the visualization to work.\n"
            << "\tPress enter after each input.\n" << divider;
        for (int i = 0; i < numTasks; i++) {
            Task task;
            std::string start;
            std::string end;
            std::string pay;
            task.id = i + 1;
            std::cout << "\tTask " << i + 1 << std::endl;
            std::cout << "\tWhat is this task's payment?\n";
            //std::cin >> pay;
            pay = "randomtasks";
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (pay == "randomtasks") {                                                  // For debugging purposes, remove later
                srand((unsigned)time(NULL));
                int randmax = 200;
                int len = randmax/4;
                /*// remove comment to test different things
                std::cout << "\t Last possible endtime?\n";     //(total length of graph)
                std::cin >> randmax;
                std::cout << "\t Max length?\n";                //(shorter means more possible paths)
                std::cin >> len;
                */
                
                for (int i = 0; i < numTasks; i++) {
                    int randomStart = (rand() % (randmax - len));
                    task.startTime = randomStart;
                    int randomEnd = 1 + randomStart + (rand() % (len));
                    task.endTime = randomEnd;
                    int randomPay = rand() % 50;
                    task.pay = randomPay;
                    task.id = i + 1;
                    tasks.push_back(task);
                }
                break;
            }
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            std::cout << "\tWhat time does this task start?\n";
            while (true) {
                std::cin >> start;
                try {
                    task.startTime = std::stoi(start);
                    if (task.startTime < 0)
                        throw std::invalid_argument("");
                    break;
                }
                catch (std::invalid_argument) {
                    std::cout << "\tInvalid value, please try again.\n";
                }
            }
            std::cout << "\tWhat time does this task end?\n";
            while (true) {
                std::cin >> end;
                try {
                    task.endTime = std::stoi(end);
                    if (task.endTime < 0)
                        throw std::invalid_argument("");
                    break;
                }
                catch (std::invalid_argument) {
                    std::cout << "\tInvalid value, please try again.\n";
                }
            }
            task.pay = std::stoi(pay);

            /*if (task.endTime < task.startTime) {
                std::cout << "\tEnd time must be after start time.\n";
            }*/

            std::cout << divider;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            tasks.push_back(task);
        }
        // Sort tasks by their END time, least to greatest
        std::sort(tasks.begin(), tasks.end(), compareTasks);
        /*
        for (int i = 0; i < tasks.size(); i++) {
            tasks.at(i).print();
        }*/

        visualization(tasks);
        typedef std::chrono::high_resolution_clock Clock;
        auto bruteStart = Clock::now();
        auto cringe = bruteForce(tasks);
        auto bruteStop = Clock::now();
        std::cout << "\tTime elapsed in bruteforce algorithm: " << std::chrono::duration_cast<std::chrono::milliseconds>(bruteStop - bruteStart).count() << " milliseconds." << std::endl;
        int value = 0;

        std::cout << "\tOptimal route according to the brute force algorithm:\n";
        if (cringe.size() == 1) {
            for (int i = 0; i < cringe[0].size(); i++) {
                std::cout << "\t\tTask #" << cringe[0][i].id << ((i + 1 < cringe.size()) ? ", " : "\n");
                value += cringe[0][i].pay;
            }
        }
        else if (cringe.size() > 1) {
            for (int i = 0; i < cringe.size(); i++) {
                std::cout << "\t\tRoute " << i+1 << ":\n";
                for (int j = 0; i < cringe[i].size(); j++) {
                    std::cout << "Task #" << cringe[i][j].id << ((i + 1 < cringe[i].size()) ? ", " : "\n");
                    if (i==0)
                        value += cringe[i][j].pay;
                }
            }
        }
        std::cout << "\tTotal pay: " << value << std::endl;
        //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}
