#pragma once
#include <map>

class node
{
public:
	int freq;
	char symbol;
	node* left = NULL;
	node* right = NULL;
	node(std::map<char, int>::iterator it);
	node(node* _left = NULL, node* _right = NULL);
};
