#include <iostream>
#include <limits>
#include <vector>
#include <map>
#include <algorithm>

struct Node {
    std::string name;
    std::vector<Node> edges;
};

void printAdjList(Node * list, int numNodes) {
    for (int i = 0; i < numNodes; i++) {
        std::cout << list[i].name;
        for (Node n : list[i].edges) {
            std::cout << " -> " << n.name;
        }
        std::cout << std::endl;
    }
}

int main() {
    std::string divider = "     =============================================================================\n";
    while (true) {
        std::cout << divider;
        std::cout << "\tWelcome. How many nodes does this graph have?\n"
            << "\tEnter the number, then press enter.\n"
            << "\tAlternatively, type \"exit\" and press enter to quit at any time.\n"
            << divider;
        int numNodes;
        while(!(std::cin >> numNodes)){
            std::cout << "\tInvalid value, please try again.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        Node* nodes = new Node[numNodes];
        std::map<std::string, Node> nodeNames;
        std::cout << divider;
        std::cout << "\tNow, you'll need to provide information about the nodes.\n"
            << "\tType the name of each node, then press enter.\n"
            << divider;
        for (int i = 0; i < numNodes; i++) {
            Node node;
            std::string name;
            std::cout << "\tNode " << (i + 1) << ":\n";
            // TODO Disallow duplicate node names!
            std::cin >> name;
            std::transform(name.begin(), name.end(), name.begin(), ::toupper);
            node.name = name;
            nodes[i] = node;
            nodeNames[node.name] = node;
        }
        std::cout << divider << "\tNext, enter information about the edges.\n";
        for (int i = 0; i < numNodes; i++) {
            std::cout << "\tHow many edges does node " << nodes[i].name << " have?\n";
            int numEdges;
            while(!(std::cin >> numEdges)){
                std::cout << "\tInvalid value, please try again.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            for (int j = 0; j < numEdges; j++) {
                std::cout << "\tEnter the name of edge " << j + 1 << ".\n";
                std::string name;
                while (1) {
                    std::cin >> name;
                    std::transform(name.begin(), name.end(), name.begin(), ::toupper);
                    if (nodeNames.contains(name)) { // requires C++20
                        nodes[i].edges.push_back(nodeNames[name]);
                        break;
                    }
                    else {
                        std::cout << "\tThat node does not exist, please try again.\n";
                        continue;
                    }
                }
            }
        }
        printAdjList(nodes, numNodes);
    }
}