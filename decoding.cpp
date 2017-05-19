#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include <list>
#include "Node.h"


struct compare
{
	bool operator()(node* left, node *right) const
	{
		return left->freq < right->freq;
	}
};

node* build_tree(std::map<char, int> frequency)
{
	std::list<node*> freq_list;
	for (auto it = frequency.begin(); it != frequency.end(); ++it)
	{
		node *tmp = new node(it);
		freq_list.push_back(tmp);
	}

	while (freq_list.size() != 1)
	{
		freq_list.sort(compare());
		node* left = freq_list.front();
		freq_list.pop_front();
		node* right = freq_list.front();
		freq_list.pop_front();
		node* parent = new node(left, right);
		freq_list.push_back(parent);
	}
	return  freq_list.front();
}

void decoding(std::string filename)
{
	std::ofstream output_file;
	std::ifstream input_file;
	std::string filename_decod = filename + "decoding.txt";
	filename = filename + "_coding.bin";
	input_file.open(filename);
	output_file.open(filename_decod);

	int size_of_table, freq, count_of_symbol, count_of_read_symbol=0;
	input_file >> count_of_symbol;
	input_file >> size_of_table;
	char symbol;
	std::map<char, int> table_of_frequency;
	for (int i = 0; i < size_of_table; i++)
	{
		input_file.get(symbol);
		input_file >> freq;
		table_of_frequency.insert(std::pair<char, int>(symbol, freq));
	}

	if (table_of_frequency.size() == 1)
	{
		for (int i = 0; i < count_of_symbol; i++)
		{
			output_file << table_of_frequency.begin()->first;
		}
	}
	else
	{
		node *root = build_tree(table_of_frequency);
		node *tmp_node = root;
		int count_of_bit = 0;
		char byte;
		input_file.get(byte);
		while (!input_file.eof())
		{
			bool tmp_bool = byte & (1 << (7 - count_of_bit));

			if (tmp_bool)
				tmp_node = tmp_node->right;
			else
				tmp_node = tmp_node->left;

			if (tmp_node->left == NULL && tmp_node->right == NULL)
			{
				output_file << tmp_node->symbol;
				count_of_read_symbol++;
				if (count_of_read_symbol == count_of_symbol)
				{
					output_file.close();
					input_file.close();
					return;
				}
				tmp_node = root;
			}

			count_of_bit++;

			if (count_of_bit == 8)
			{
				count_of_bit = 0;
				input_file.get(byte);
			}
		}
	}
	output_file.close();
	input_file.close();
}

int main()
{
	std::string filename;
	std::cout << "Enter the filename:";
	std::cin >> filename;
	decoding(filename);
}