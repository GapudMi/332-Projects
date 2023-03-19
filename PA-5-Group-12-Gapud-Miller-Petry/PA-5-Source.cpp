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
        std::cout << "Task " << id << ": Pay is " << pay << " startTime is " << startTime << " endTime is " << endTime << std::endl;
    }
};

// Returns true if a ends before b
bool compareTasks(const Task a, const Task b) {
    return (a.endTime < b.endTime);
}

bool operator== (const Task& a, const Task& b) {
    return a.id == b.id;
}

bool routesEqual(const std::vector<Task> a, const std::vector<Task> b) {
    if (a.size() != b.size())
        return false;
    for (int i = 0; i < a.size(); i++) {
        if (a[i].id != b[i].id)
            return false;
    }
    return true;
}

bool vectorContainsRoute(const std::vector<std::vector<Task>> a, const std::vector<Task> b) {
    for (std::vector<Task> r : a) {
        if (routesEqual(r, b))
            return true;
    }
    return false;
}

// Brute force algorithms utilizing permutations
std::pair<std::vector<std::vector<Task>>, std::vector<std::vector<Task>>> bruteForce(std::vector<Task> tasks) {
    int routesCompleted = 0;
    std::vector<std::vector<Task>> bestRouteList;
    std::vector<std::vector<Task>> maxRouteList;
    std::sort(tasks.begin(), tasks.end(), compareTasks);
    int maxValue = 0;
    std::vector<Task> bestRoute;
    std::vector<int> p;
    for (int i = 0; i < tasks.size(); i++)
        p.push_back(i);
    do {
        if (p[0] > routesCompleted) {
            routesCompleted++;
            p.erase(p.begin() + 1);
        }

        std::vector<Task> route;
        int bottom = -1;
        int value = 0;
        for (int i = 0; i < p.size(); i++) {
            if (route.size() == 0) {
                route.push_back(tasks[p[i]]);
                value += tasks[p[i]].pay;
                bottom = p[i];
                continue;
            }
            if (route.back().endTime > tasks[p[i]].startTime)
                continue;
            route.push_back(tasks[p[i]]);
            value += route.back().pay;
        }
        if (maxRouteList.size() == 0) {
            maxRouteList.push_back(route);
        }
        else {
            if (not vectorContainsRoute(maxRouteList, route)) {
                maxRouteList.push_back(route);
            }
        }
        /*else if (route.size() > maxRouteList[0].size()) {
            maxRouteList.clear();
            maxRouteList.push_back(route);
        }
        else if (route.size() == maxRouteList[0].size()) {


        }*/
        if (value > maxValue) {
            bestRoute = route;
            bestRouteList.clear();
            bestRouteList.push_back(bestRoute);
            maxValue = value;
        }
        else if (value == maxValue) {
            if (not vectorContainsRoute(bestRouteList, route)) {
                bestRouteList.push_back(route);
                if (bestRouteList.size() > 10) {
                    std::cout << "hell";
                }
            }

        }
    } while (std::next_permutation(p.begin(), p.end()));
    auto lkjhasdf = std::make_pair(bestRouteList, maxRouteList);
    return lkjhasdf;
}

int dynamic(std::vector<Task> tasks) {
    std::vector<std::vector<Task>> allRoutes;
    std::vector<std::vector<Task>> allMaxRoutes;
    std::vector<int> maxChoose;
    std::vector<int> maxNoChoose;
    std::vector<int> trueMax;
    std::vector<Task> pre;
    std::vector<int> preIndex;
    Task noPrev; noPrev.endTime = 0; noPrev.startTime = 0; noPrev.pay = 0; noPrev.id = -1;

    // Initial condition
    int i = 0;
    pre.push_back(noPrev);
    maxChoose.push_back(tasks[i].pay);
    maxNoChoose.push_back(0);
    trueMax.push_back(tasks[i].pay);
    allRoutes.push_back({ tasks[i] });
    preIndex.push_back(-1);

    // General cases
    for (i = 1; i < tasks.size(); i++) {
        bool hasPrev = false;
        preIndex.push_back(-1);
        for (int j = i; j >= 0; j--) {
            if (tasks[i].startTime >= tasks[j].endTime) {
                pre.push_back(tasks[j]);
                preIndex[i] = j;
                hasPrev = true;
                break;
            }
        }
        if (!hasPrev) { // NO PREVIOUS POSSIBLE NODES
            pre.push_back(noPrev);
            maxChoose.push_back(tasks[i].pay);
            maxNoChoose.push_back(trueMax[i - 1]);
            if (maxChoose[i] > maxNoChoose[i]) { // there's like some space here for determining duplicate paths
                trueMax.push_back(maxChoose[i]);
                allRoutes.push_back({ tasks[i] }); // you can just push back this node as a route because there are no other routes, and you chose this node
            }
            else {
                trueMax.push_back(maxNoChoose[i]);
                allRoutes.push_back(allRoutes[i - 1]); // if you didn't choose this node, then the previous node's route was chosen
            }
        }
        else { // PREVIOUS NODES EXIST
            maxChoose.push_back(tasks[i].pay + trueMax[preIndex[i]]);
            maxNoChoose.push_back(trueMax[i - 1]);
            std::vector<Task> thisRoute;
            if (maxChoose[i] > maxNoChoose[i]) { // there's like some space here for determining duplicate paths
                trueMax.push_back(maxChoose[i]);
                thisRoute = allRoutes[preIndex[i]];
                thisRoute.push_back(tasks[i]);
                allRoutes.push_back(thisRoute);
            }
            else {
                trueMax.push_back(maxNoChoose[i]);
                allRoutes.push_back(allRoutes[i - 1]); // if you didn't choose this node, then the previous node's route was chosen
            }
        }
    }
    /* outputting unneeded
    // Find which routes have the max payout and print them
    int maxPayout = trueMax[trueMax.size() - 1];
    for (int outer = 0; outer < allRoutes.size(); outer++) {
        int sum = 0;
        if ((preIndex[outer] != -1) && (allRoutes[outer] != allRoutes[outer - 1])) {
            for (int inner = 0; inner < allRoutes[outer].size(); inner++) {
                sum += allRoutes[outer][inner].pay;
            }
        }
        // how to catch subsequent routes just containing the pre?
        if (sum == maxPayout) {
            std::cout << "\t\t";
            for (int j = 0; j < allRoutes[outer].size(); j++) {
                std::cout << "Task #" << allRoutes[outer][j].id << ((j == allRoutes[outer].size() - 1) ? "\n" : "->");
            }
        }

    }
    */
    return trueMax[trueMax.size() - 1];
}
/*
// recursively find every unique set of tasks
std::vector<std::vector<Task>> recursiveTasks(std::vector<Task> set) {
    std::sort(set.begin(), set.end(), compareTasks);
    auto routeList = taskListRecursive(set, nullptr);
    //will add checks to get rid of non-unique sets
    //compare every set to every other and remove them from routeList
    //routeList.erase(routeList.begin());
    return routeList;
}

// recursively finds *every* set of tasks
std::vector<std::vector<Task>> taskListRecursive(std::vector<Task> set, Task* currTask) {
    std::vector<std::vector<Task>> newSet;
    std::vector<Task> nextSet;

    //create vector of tasks that start after this one
    if (currTask) {
        for (int i = 0; i < set.size(); i++) {
            if (set[i].startTime >= currTask->endTime &&
                set[i].startTime != set[i].endTime) { //being cautious
                nextSet.push_back(set[i]);
            }
        }
    }
    else {
        nextSet = set;
    }

    //base case
    if (nextSet.size() == 0) {
        newSet.push_back(nextSet);
        return newSet;
    }
    //recursive case
    else {
        for (int i = 0; i < nextSet.size(); i++) {
            auto tempSet = taskListRecursive(nextSet, &nextSet[i]);

            for (int j = 0; j < tempSet.size(); j++) {
                tempSet[j].insert(tempSet[j].begin(), nextSet[i]); //inserts the current Task at the bigenning of the set
                newSet.push_back(tempSet[j]);
            }
        }
        return newSet;
    }
}
*/

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

void printMaxRoute(std::vector<Task> v, int payout) {
    std::cout << "\t";
    for (int i = 0; i < v.size(); i++) {
        std::cout << "Task_" << v[i].id << ((i + 1 < v.size()) ? "->" : ", ");
    }
    std::cout << "with a total earning of " << payout << ".\n";
}

void printRoute(std::vector<Task> v) {
    int value = 0;
    for (int i = 0; i < v.size(); i++) {
        std::cout << "Task_" << v[i].id << ((i + 1 < v.size()) ? "->" : ", ");
        value += v[i].pay;
    }
    std::cout << "with a total earning of " << value << ".\n";
}

int main() {
    int numTasks = 0;
    std::vector<Task> tasks;
    std::string inputString;
    std::string divider = "     =============================================================================\n";
    while (true) {
        tasks.clear();
        std::cout << divider;
        std::cout << "\tHow many paid tasks are there?\n"
            << "\tType your number and press enter,\n"
            << "\tor type \"exit\" and press enter to quit at any time.\n" << divider;
        while (true) {
            try {
                std::cin >> inputString;
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
            task.id = i + 1;
            std::cout << "\tTask " << i + 1 << std::endl;
            std::cout << "\tWhat is this task's payment?\n";
            std::cin >> pay;
            // debug
            //pay = "randomtasks";
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (pay == "randomtasks") {                                                  // For debugging purposes, remove later
                srand((unsigned)time(NULL));
                int randmax = 200;
                int len = randmax / 4;
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
                    int randomPay = rand() % 50 + 1;
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

        int value = 0;
        value = dynamic(tasks);
        auto dynStart = Clock::now();
        dynamic(tasks);
        auto dynStop = Clock::now();
        std::cout << "\n\tThe time elapsed in bruteforce algorithm is " << std::chrono::duration_cast<std::chrono::milliseconds>(bruteStop - bruteStart).count() << " milliseconds." << std::endl;
        std::cout << "\tThe elapsed in the non-recursive DP algorithm is " << std::chrono::duration_cast<std::chrono::milliseconds>(dynStop - dynStart).count() << " milliseconds." << std::endl;

        std::cout << std::endl;
        if (cringe.first.size() == 1)
            printMaxRoute(cringe.first[0], value);
        else {
            for (std::vector<Task> r : cringe.first) {
                printMaxRoute(r, value);
            }
        }
        
        
        if (cringe.second.size() == 1){
            std::cout << "\n\tThere is 1 set of tasks\n";
            std::cout << "\tOption 1: " ;
            printRoute(cringe.second[0]);
        }
        else {
            std::cout << "\n\tThere are " << cringe.second.size() << " different sets of tasks\n";
            int i = 1;
            for (std::vector<Task> r : cringe.second) {
                std::cout << "\tOption " << i << ": " ;
                printRoute(r);
                i++;
            }
        }
    }
}
