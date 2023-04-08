#include <iostream>
#include <limits>
#include <vector>
#include <map>
#include <algorithm>
#include <string>

const int NODE_LIMIT = 25;

enum color { white, gray, black };

// change color value from int to corresponding string
std::string getColorName(color c) {
    std::string a[3] = { "white", "gray", "black" };
    return a[c];
}

struct Node {
    std::string name;
    std::vector<Node*> edges;
    color col = white;
    Node* predecessor = nullptr;
    int firstTime = 0;
    int lastTime = std::numeric_limits<int>::infinity();
};

// operator overload for Node class
bool operator==(const Node& a, const Node& b)
{
    return a.name == b.name;
}

// check if the vector contains a given node
bool vectorContainsNode(std::vector<Node*> v, Node& a) {
    for (int i = 0; i < v.size(); i++) {
        if (v[i]->name == a.name)
            return true;
    }
    return false;
}

// print the adjacency list
void printAdjList(std::vector<Node>& list, int numNodes) {
    for (int i = 0; i < numNodes; i++) {
        std::cout << '\t' << list[i].name;
        for (Node* n : list[i].edges) {
            std::cout << " -> " << n->name;
        }
        std::cout << std::endl;
    }
}

// print the adjacency matrix
void printAdjMatrix(std::vector<Node>& nodes, int numNodes) {
    std::cout << "\t\t";
    for (int i = 0; i < numNodes; i++) {
        std::cout << nodes[i].name << "\t";
    }
    std::cout << std::endl;
    for (int i = 0; i < numNodes; i++) {
        std::cout << '\t' << nodes[i].name << "\t";
        for (int j = 0; j < numNodes; j++) {
            std::cout << vectorContainsNode(nodes[i].edges, nodes[j]) << "\t";
        }
        std::cout << std::endl;
    }
}

// print the current queue
void printQueue(std::vector<Node*> q) {
    std::cout << "\n\n\tQueue: ";
    for (int i = 0; i < q.size(); i++) {
        std::cout << q[i]->name << ' ';
    }
    std::cout << "\n";
}

// print node and its attributes
void printTable(std::vector<Node>& nodes) {
    int numNodes = static_cast<int>(nodes.size());
    int wraparound = 8;
    std::string div = "\n\t\t\t\t\t- - -\n";
    std::string bigdiv = "\n     =============================================================================\n";

    if (numNodes <= wraparound) {
        std::cout << bigdiv; // ======
        std::cout << "\n\t\t\t";
        for (int i = 0; i < numNodes; i++) {
            std::cout << nodes[i].name << "\t";
        }
        std::cout << "\n\tcolor\t\t";
        for (int i = 0; i < numNodes; i++) {
            std::cout << getColorName(nodes[i].col) << '\t';
        }
        std::cout << "\n\tpredecessor\t";
        for (int i = 0; i < numNodes; i++) {
            if (nodes[i].predecessor != nullptr) std::cout << nodes[i].predecessor->name << "\t";
            else std::cout << "none\t";
        }
        std::cout << "\n\tfirsttime\t";
        for (int i = 0; i < numNodes; i++) {
            std::cout << nodes[i].firstTime << "\t";
        }
        std::cout << "\n\tlasttime\t";
        for (int i = 0; i < numNodes; i++) {
            std::cout << nodes[i].lastTime << "\t";
        }
        std::cout << "\n";
    }
    // print formatting handling for when there are many (more than wraparound val) nodes
    else { 
        int i = 0;
        std::cout << bigdiv; // ======
        while (i < numNodes) {

            int start = i;
            i += wraparound;
            if (i > numNodes) i = numNodes;

            if (start > 0) std::cout << div; // - - -
            std::cout << "\n\t\t\t";
            for (int j = start; j < i; j++) {
                std::cout << nodes[j].name << "\t";
            }
            std::cout << "\n\tcolor\t\t";
            for (int j = start; j < i; j++) {
                std::cout << getColorName(nodes[j].col) << '\t';
            }
            std::cout << "\n\tpredecessor\t";
            for (int j = start; j < i; j++) {
                if (nodes[j].predecessor != nullptr) std::cout << nodes[j].predecessor->name << '\t';
                else std::cout << "none\t";
            }
            std::cout << "\n\tfirsttime\t";
            for (int j = start; j < i; j++) {
                std::cout << nodes[j].firstTime << "\t";
            }
            std::cout << "\n\tlasttime\t";
            for (int j = start; j < i; j++) {
                std::cout << nodes[j].lastTime << "\t";
            }
            std::cout << "\n";
        }
    }
    std::cout << "\n";
}

void dfsVisit(Node* n, int &time) {
    n->col = gray;
    n->firstTime = time;
    time++;
    for (Node* &v : n->edges) {
        if (v->col == white) {
            v->predecessor = n;
            dfsVisit(v, time);
        }
    }
    n->col = black;
    n->lastTime = time;
    time++;
}

void depthFirst(std::vector<Node>& nodes) {
    int time = 0;
    for (Node &v : nodes) {
        if (v.col == white) {
            dfsVisit(&v, time);
        }
        printTable(nodes);
    }
}

// parse integer input
int parseInt(std::string i) {
    int out;
    try {
        // If valid integer, return it
        out = std::stoi(i);
        if (out < 0)
            return -1;
        else
            return out;
    }
    catch (std::invalid_argument& e) {
        // If output is "exit, then exit
        std::transform(i.begin(), i.end(), i.begin(),
                       ::tolower); // Convert input to lower case for case-nonsensitive input
        if (i == "exit") exit(0);
        else
            return -1; // -1 represents an error, since we can not accept negative values anyway
    }
    catch (const std::exception& unknown) {
        return -1;
    }
}

// check for "exit" input
void chexit(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   ::tolower); // Convert input to lower case for case-nonsensitive input
    if (s == "exit") exit(0);
}

int main() {
    std::string divider = "     =============================================================================\n";
    while (true) {
        std::cout << divider;
        std::cout << "\tWelcome. How many nodes does this graph have?\n"
                  << "\tEnter the number, then press enter.\n"
                  << "\tAlternatively, type \"exit\" and press enter to quit at any time.\n"
                  << divider;
        std::string num;
        int numNodes;
        while (true) {
            // Check if the input is "exit", or an invalid integer.
            std::cin >> num;
            numNodes = parseInt(num);
            if (numNodes <= 0) {
                std::cout << "\tInvalid value, please try again.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            else if (numNodes > NODE_LIMIT) {
                std::cout << "\tFor performance reasons, charts larger than " << NODE_LIMIT << " nodes are disabled.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            else
                break;
        }
        std::vector<Node> nodes;
        std::map<std::string, int> nodeNames;
        std::cout << divider;
        std::cout << "\tYour nodes and their names:\n";
        for (int i = 0; i < numNodes; i++) {
            // Give nodes predefined names
            // N1, N2, N3... etc.
            Node node;
            std::string n = "N" + std::to_string(i + 1);
            node.name = n;
            nodes.push_back(node);
            nodeNames[n] = i;
            std::cout << '\t' << node.name << "\n";
        }
        std::cout << divider;
        std::cout << "\tNow, you'll need to provide information about the nodes.\n";
        std::cout << divider << "\tEnter information about the edges.\n\tYou only need to specify each edge once.\n";
        for (int i = 0; i < numNodes; i++) {
            std::string n;
            int numEdges = 0;
            int size = static_cast<int>((nodes[i].edges.size()));
            std::cout << "\tHow many edges does node N" << (i + 1) << " have?\n";
            while (true) {
                std::cin >> n;
                numEdges = parseInt(n);
                if (numEdges < 0) {
                    std::cout << "\tInvalid value, please try again.\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                else if (numEdges + size >= numNodes) {
                    std::cout << "\tYou cannot have more edges than nodes.\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                else
                    break;
            }
            // connect nodes
            for (int j = size; j < numEdges + size; j++) {
                std::cout << "\tEnter the name of the node that " << nodes[i].name << "'s edge number " << j + 1 << " connects to.\n";
                std::string name;
                while (true) {
                    std::cin >> name;
                    chexit(name);
                    std::transform(name.begin(), name.end(), name.begin(), ::toupper);
                    if (nodeNames.contains(name)) { // requires C++20
                        if (name == nodes[i].name) {
                            std::cout << "\tNodes can't connect to themselves, please try again.\n";
                            continue;
                        }
                        bool redundant = false;
                        for (int iter = 0; iter < nodes[i].edges.size(); iter++) {
                            if (nodes[i].edges[iter]->name == name) {
                                std::cout << "\tThis edge already exists, please try again.\n"; redundant = true;
                                break;
                            }
                        }
                        if (!redundant) {
                            nodes[i].edges.push_back(&nodes[nodeNames[name]]); // add named edge

                            std::cout << divider << "\tAdjacency list\n";
                            printAdjList(nodes, numNodes);
                            std::cout << divider;
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
        // output resulting adjacency list and matrix
        std::cout << divider << "\tAdjacency list\n";
        printAdjList(nodes, numNodes);
        std::cout << divider << "\tAdjacency matrix\n";
        printAdjMatrix(nodes, numNodes);

        depthFirst(nodes);
    }
}
