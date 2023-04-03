#include <iostream>
#include <limits>
#include <vector>
#include <map>
#include <algorithm>
#include <string>

enum color { white, gray, black };

struct Node {
    std::string name;
    std::vector<Node*> edges;
    color col = white;
    double distance = std::numeric_limits<double>::infinity();
    Node* predecessor = nullptr;
};

bool operator==(const Node& a, const Node& b)
{
    return a.name == b.name;
}

bool vectorContainsNode(std::vector<Node*> v, Node a) {
    for (int i = 0; i < v.size(); i++) {
        if (v[i]->name == a.name)
            return true;
    }
    return false;
}

void printAdjList(Node * list, int numNodes) {
    for (int i = 0; i < numNodes; i++) {
        std::cout << list[i].name;
        for (Node* n : list[i].edges) {
            std::cout << " -> " << n->name;
        }
        std::cout << std::endl;
    }
}

void printAdjMatrix(Node * nodes, int numNodes) {
    std::cout << "  ";
    for (int i = 0; i < numNodes; i++) {
        std::cout << nodes[i].name << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < numNodes; i++) {
        std::cout << nodes[i].name << " ";
        for (int j = 0; j < numNodes; j++) {
            std::cout << vectorContainsNode(nodes[i].edges, nodes[j]) << " ";
        }
        std::cout << std::endl;
    }
}

void breadthFirst(Node * nodes, std::string sourceName, int numNodes) {
    std::vector<Node*> queue;
    int s;
    for (int i = 0; i < numNodes; i++) {
        if (nodes[i].name == sourceName) {
            s = i;
            break;
        }
    }
    nodes[s].col = gray;
    nodes[s].distance = 0;
    queue.push_back(&nodes[s]);
    while (!queue.empty()) {
        Node* head = queue[0];
        for (Node* n : head->edges) {
            if (n->col == white) {
                n->col = gray;
                n->distance = head->distance+1;
                n->predecessor = head;
                queue.push_back(n);
            }
        }
        head->col = black;
        Node* newHead = queue.back();
        queue.clear(); // This empties the queue of all nodes, not just the head
        queue.push_back(newHead); 
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
        std::cout << "Your nodes and their names:\n";
        for (int i = 0; i < numNodes; i++) {
            Node node;
            node.name = "N" + std::to_string(i+1);
            nodes[i] = node;
            nodeNames[node.name] = node;
            std::cout << node.name << "\n";
        }
        std::cout << divider;
        std::cout << "\tNow, you'll need to provide information about the nodes.\n";
            // << "\tType the name of each node, then press enter.\n"
            // << divider;
        std::cout << divider << "\tEnter information about the edges.\n\tYou only need to specify each edge once.\n";
        for (int i = 0; i < numNodes; i++) {
            std::cout << "\tHow many edges does node " << nodes[i].name << " have?\n";
            int numEdges;
            while (!(std::cin >> numEdges) || numEdges >= numNodes || numEdges < nodes[i].edges.size()) {
                std::cout << "\tInvalid value, please try again.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            for (int j = 0; j < numEdges - nodes[i].edges.size(); j++) {
                std::cout << "\tEnter the name of edge " << j + 1 << ".\n";
                std::string name;
                while (1) {
                    std::cin >> name;
                    std::transform(name.begin(), name.end(), name.begin(), ::toupper);
                    if (nodeNames.contains(name)) { // requires C++20
                        if (name == nodes[i].name) {
                            std::cout << "\tNodes can't connect to themselves, please try again.\n";
                            continue;
                        }
                        bool redundant = false; // i hate doing this it makes me feel so inelegant
                        for (int iter = 0; iter < nodes[i].edges.size(); iter++) {
                            //std::cout << "\nEdge list name " << nodes[i].edges[iter]->name << " user inputted name: " << name; Debug
                            if (nodes[i].edges[iter]->name == name) {
                                std::cout << "\tThis edge already exists, please try again.\n"; redundant = true;
                                break;
                            }
                        }
                        if (!redundant) {
                            nodes[i].edges.push_back(&nodeNames[name]); // add named edge
                            nodes[std::stoi(name.substr(1))-1].edges.push_back(&nodeNames[nodes[i].name]); // add that edge to the other end
                            printAdjList(nodes, numNodes); //debugging
                            break;
                        }
                        else {
                            continue;
                        }
                    }
                    else {
                        std::cout << "\tThat node does not exist, please try again.\n";
                        continue;
                    }
                }
            }
        }
        printAdjList(nodes, numNodes);
        printAdjMatrix(nodes, numNodes);

        std::cout << divider << "\tWhat is the source node of your graph?\n";
        std::string name;
        Node source;
        while (true) {
            std::cin >> name;
            std::transform(name.begin(), name.end(), name.begin(), ::toupper); //lol order
            if (!nodeNames.contains(name)) {
                std::cout << "\tThat node does not exist, please try again.\n";
            }
            else {
                break;
            }
        }
        breadthFirst(nodes, name, numNodes);
    }
}
