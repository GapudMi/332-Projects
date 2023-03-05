#include <queue>
#include <iostream>
#include "Node.h"
struct Node* newNode(int value, int index, int size) {
	Node* n = new Node;
	n->value = value;
	n->index = index;
	n->size = size;
	for (int i = 0; i < size; i++) {
		n->child.push_back(NULL);
	}
	return n;
}

struct Node* insertValue(struct Node* root, int value, int index, int size, std::queue<Node*>& q)
{
	Node* node = newNode(value, index, size);
	if (root == NULL) root = node;
	else if (q.front()->child.at(index) == NULL) {
		q.front()->child.at(index) = node;
	}
	else {
		std::cout << "Something has gone very very wrong.\n";
	}
	q.push(node);
	return root;
}