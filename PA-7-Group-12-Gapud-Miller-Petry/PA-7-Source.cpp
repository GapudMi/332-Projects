#include <iostream>
#include <limits>
#include <vector>
#include <map>
#include <algorithm>
#include <string>

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
        std::cout << list[i].name;
        for (Node* n : list[i].edges) {
            std::cout << " -> " << n->name;
        }
        std::cout << std::endl;
    }
}

void printAdjMatrix(Node*& nodes, int numNodes) {
    std::cout << "\t";
    for (int i = 0; i < numNodes; i++) {
        std::cout << nodes[i].name << "\t";
    }
    std::cout << std::endl;
    for (int i = 0; i < numNodes; i++) {
        std::cout << nodes[i].name << "\t";
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

    while (!queue.empty()) {
        s = queue.size() - 1;
        Node* head = queue[s];
        //std::cout << "\n\thead name: " << head->name;//debug
        for (int i = 0; i < head->edges.size(); i++) {
            //std::cout << "\nn->col: " << head->edges[i]->col;//debug
            if (head->edges[i]->col == white) {
                head->edges[i]->col = gray;
                //std::cout << "\nn->col: " << head->edges[i]->col;//debug
                head->edges[i]->distance = head->distance + 1;
                //std::cout << "\nn->distance: " << head->edges[i]->distance;//debug
                head->edges[i]->predecessor = head;
                //std::cout << "\nn->predecessor: " << head->edges[i]->predecessor->name;//debug
                queue.push_back(head->edges[i]);
            }
        }
        head->col = black;
        queue.erase(queue.begin() + s);
        //queue.push_back(newHead);
        printTable(nodes, numNodes);
        printQueue(queue);
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
        while (!(std::cin >> numNodes) || numNodes <= 0) {
            std::cout << "\tInvalid value, please try again.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        Node* nodes = new Node[numNodes];
        std::map<std::string, int> nodeNames;
        std::cout << divider;
        std::cout << "Your nodes and their names:\n";
        for (int i = 0; i < numNodes; i++) {
            Node node;
            std::string ARGH = "N" + std::to_string(i + 1);
            node.name = ARGH;
            nodes[i] = node;
            nodeNames[ARGH] = i;
            std::cout << node.name << "\n";
        }
        std::cout << divider;
        std::cout << "\tNow, you'll need to provide information about the nodes.\n";
        std::cout << divider << "\tEnter information about the edges.\n\tYou only need to specify each edge once.\n";
        for (int i = 0; i < numNodes; i++) {
            int numEdges;
            int size = nodes[i].edges.size();

            if (size == 0) {
                std::cout << "\tHow many edges does node " << nodes[i].name << " have?\n";
                while (!(std::cin >> numEdges) || numEdges >= numNodes) {
                    std::cout << "\tInvalid value, please try again.\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            }
            else {
                if (size == 1) std::cout << "\tNode " << nodes[i].name << " has " << size << " edge already. How many more do you want to add?\n";
                else std::cout << "\tNode " << nodes[i].name << " has " << size << " edges already. How many more do you want to add?\n";
                while (!(std::cin >> numEdges) || numEdges + size >= numNodes) {
                    std::cout << "\tInvalid value, please try again.\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            }
            
            for (int j = size; j < numEdges + size; j++) {
                std::cout << "\tEnter the name of the node that " << nodes[i].name << "'s edge number " << j + 1 << " connects to.\n";
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
                            nodes[i].edges.push_back(&nodes[nodeNames[name]]); // add named edge
                            nodes[std::stoi(name.substr(1)) - 1].edges.push_back(&nodes[nodeNames[nodes[i].name]]); // add that edge to the other end

                            std::cout << divider;
                            printAdjList(nodes, numNodes); //debugging? i actually like how this feels for ux
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
        printAdjList(nodes, numNodes);
        printAdjMatrix(nodes, numNodes);

        std::cout << divider << "\tWhat is the source node of your graph?\n";
        std::string name;
        Node source;
        while (true) {
            std::cin >> name;
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
