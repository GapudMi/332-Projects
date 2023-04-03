#include <iostream>
#include <limits>
#include <vector>
#include <map>
#include <algorithm>
#include <string>

const int NODE_LIMIT = 25;

enum color { white, gray, black };

std::string getColorName(color c) {
    std::string a[3] = { "white", "gray", "black" };
    return a[c];
}

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

void printAdjList(Node*& list, int numNodes) {
    for (int i = 0; i < numNodes; i++) {
        std::cout << '\t' << list[i].name;
        for (Node* n : list[i].edges) {
            std::cout << " -> " << n->name;
        }
        std::cout << std::endl;
    }
}

void printAdjMatrix(Node*& nodes, int numNodes) {
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

void printQueue(std::vector<Node*> q) {
    std::cout << "\n\n\tQueue: ";
    for (int i = 0; i < q.size(); i++) {
        std::cout << q[i]->name << ' ';
    }
    std::cout << "\n";
}

void printTable(Node*& nodes, int numNodes) {
    std::cout << "\n     =============================================================================\n";
    std::cout << "\n\t\t\t";
    for (int i = 0; i < numNodes; i++) {
        std::cout << nodes[i].name << '\t';
    }
    std::cout << "\n\tcolor\t\t";
    for (int i = 0; i < numNodes; i++) {
        std::cout << getColorName(nodes[i].col) << '\t';
    }
    std::cout << "\n\tdistance\t";
    for (int i = 0; i < numNodes; i++) {
        std::cout << nodes[i].distance << '\t';
    }
    std::cout << "\n\tpredecessor\t";
    for (int i = 0; i < numNodes; i++) {
        if (nodes[i].predecessor != nullptr) std::cout << nodes[i].predecessor->name << '\t';
        else std::cout << "none\t";
    }

}

void breadthFirst(Node* nodes, std::string sourceName, int numNodes) {
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
    printTable(nodes, numNodes);
    printQueue(queue);
    for (int outer = 0; outer < numNodes; outer++) {
        if (nodes[outer].col == white) {
            nodes[outer].col = gray;
            nodes[outer].distance = 0;
            queue.insert(queue.begin(), &nodes[outer]);
            printTable(nodes, numNodes);
            printQueue(queue);
        }
        while (!queue.empty()) {
            s = queue.size() - 1;
            Node* head = queue[s];
            for (int i = 0; i < head->edges.size(); i++) {
                if (head->edges[i]->col == white) {
                    head->edges[i]->col = gray;
                    head->edges[i]->distance = head->distance + 1;
                    head->edges[i]->predecessor = head;
                    queue.push_back(head->edges[i]);
                }
            }
            head->col = black;
            queue.erase(queue.begin() + s);
            printTable(nodes, numNodes);
            printQueue(queue);
        }
    }
}

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
    catch (std::invalid_argument & e){
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
            if (numNodes < 0) {
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
        Node* nodes = new Node[numNodes];
        std::map<std::string, int> nodeNames;
        std::cout << divider;
        std::cout << "\tYour nodes and their names:\n";
        for (int i = 0; i < numNodes; i++) {
            // Give nodes predefined names
            Node node;
            std::string n= "N" + std::to_string(i + 1);
            node.name = n;
            nodes[i] = node;
            nodeNames[n] = i;
            std::cout << '\t' << node.name << "\n";
        }
        std::cout << divider;
        std::cout << "\tNow, you'll need to provide information about the nodes.\n";
        std::cout << divider << "\tEnter information about the edges.\n\tYou only need to specify each edge once.\n";
        for (int i = 0; i < numNodes; i++) {
            std::cout << "\tHow many edges does node N" << (i + 1) << " have?\n";
            std::string n;
            int numEdges;
            int size = nodes[i].edges.size();
            if (size == 0) {
                while (true) {
                    std::cin >> n;
                    numEdges = parseInt(n);
                    if (numEdges < 0) {
                        std::cout << "\tInvalid value, please try again.\n";
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                    else if (numEdges >= numNodes) {
                        std::cout << "\tYou cannot have more edges than nodes.\n";
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                    else
                        break;
                }
            }
            else {
                if (size == 1) std::cout << "\tNode " << nodes[i].name << " has " << size << " edge already. How many more do you want to add?\n";
                else std::cout << "\tNode " << nodes[i].name << " has " << size << " edges already. How many more do you want to add?\n";
                while (true) {
                    std::cin >> n;
                    numEdges = parseInt(n);
                    if (numEdges < 0) {
                        std::cout << "\tInvalid value, please try again.\n";
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                    else if (numEdges + size >= numNodes) {
                        std::cout << "\tEdges must be less than nodes.\n";
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                    else
                        break;
                }
            }

            for (int j = size; j < numEdges + size; j++) {
                std::cout << "\tEnter the name of the node that " << nodes[i].name << "'s edge number " << j + 1 << " connects to.\n";
                std::string name;
                while (1) {
                    std::cin >> name;
                    chexit(name);
                    std::transform(name.begin(), name.end(), name.begin(), ::toupper);
                    if (nodeNames.contains(name)) { // requires C++20
                        if (name == nodes[i].name) {
                            std::cout << "\tNodes can't connect to themselves, please try again.\n";
                            continue;
                        }
                        bool redundant = false; // i hate doing this it makes me feel so inelegant
                        for (int iter = 0; iter < nodes[i].edges.size(); iter++) {
                            if (nodes[i].edges[iter]->name == name) {
                                std::cout << "\tThis edge already exists, please try again.\n"; redundant = true;
                                break;
                            }
                        }
                        if (!redundant) {
                            nodes[i].edges.push_back(&nodes[nodeNames[name]]); // add named edge
                            nodes[std::stoi(name.substr(1)) - 1].edges.push_back(&nodes[nodeNames[nodes[i].name]]); // add that edge to the other end

                            std::cout << divider;
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
        std::cout << divider << "\tAdjacency list\n";
        printAdjList(nodes, numNodes);
        std::cout << divider << "\tAdjacency matrix\n";
        printAdjMatrix(nodes, numNodes);

        std::cout << divider << "\tWhat is the source node of your graph?\n";
        std::string name;
        Node source;
        while (true) {
            std::cin >> name;
            chexit(name);
            std::transform(name.begin(), name.end(), name.begin(), ::toupper);
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
