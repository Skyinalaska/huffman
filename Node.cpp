#include "Node.h"

node::node(std::map<char, int>::iterator it) : freq(it->second), symbol(it->first) {}

node::node(node* _left, node* _right)
{
	left = _left;
	right = _right;
	freq = _left->freq + _right->freq;
}