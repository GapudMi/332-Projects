#pragma once
#include <vector>
struct Node {
	int value;
	int index;
	int size;
	std::vector<Node*> child;
};
struct Node* newNode(int value, int index, int size);
struct Node* insertValue(struct Node* root, int value, int index, int size, std::queue<Node*>& q);