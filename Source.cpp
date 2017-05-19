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

std::map<char, int> frequency_symbol(std::string filename, int& count_of_symbol)
{
	std::ifstream f;
	filename = filename + ".txt";
	f.open(filename);
	std::map<char, int> frequency;
	char symbol;
	while (!f.eof())
	{
		f.get(symbol);
		if (!f.eof())
		{
			frequency[symbol]++;
			count_of_symbol++;
		}
		
	}
	//frequency[symbol]--;
	f.close();
	return frequency;
}

void table_code_helper(node *root, std::vector<bool>& code, std::map<char, std::vector<bool> >& table)
{
	if (root->left != NULL)
	{
		code.push_back(0);
		table_code_helper(root->left, code, table);
	}

	if (root->right != NULL)
	{
		code.push_back(1);
		table_code_helper(root->right, code, table);
	}

	if (root->symbol)
	{
		table[root->symbol] = code;
	}
	if (code.size() > 0)
	{
		code.pop_back();
	}
	
}

std::map<char, std::vector<bool> >table_code(node *root)
{
	std::vector<bool> code;
	std::map<char, std::vector<bool> >table;
	table_code_helper(root, code, table);
	return table;
}

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

void encoding(std::map<char, std::vector<bool> >table, std::map<char, int> table_of_frequency, std::string filename, int count_of_symbol)
{
	std::ofstream output_file;
	std::ifstream input_file;	
	std::string filename_cod = filename + "_coding.bin";
	filename = filename + ".txt";
	input_file.open(filename);
	output_file.open(filename_cod);
	
	output_file << count_of_symbol << " " << table_of_frequency.size(); //keeping count of symbol in coding file

	//output_file << table_of_frequency.size(); //keeping table of frequency in coding file
	for (auto it = table_of_frequency.begin(); it != table_of_frequency.end(); ++it)
	{
		output_file << it->first;
		output_file << it->second;
	}

	char tmp_char;
	int count_of_bit = 0;
	char byte = 0;
	while (!input_file.eof())
	{
		input_file.get(tmp_char);
		if (!input_file.eof())
		{
			for (int i = 0; i < table[tmp_char].size(); i++)
			{
				byte = byte | table[tmp_char][i] << (7 - count_of_bit);
				count_of_bit++;
				if (count_of_bit == 8)
				{
					count_of_bit = 0;
					output_file << byte;
					byte = 0;
				}
			}
		}
		else
		{
			output_file << byte;
		}
		
	}
	input_file.close();
	output_file.close();
}

void print_coding(std::map<char, std::vector<bool> >table, std::string filename)
{
	std::ifstream input_file;
	filename = filename + ".txt";
	input_file.open(filename);

	char tmp_char;
	while (!input_file.eof())
	{
		input_file.get(tmp_char);
		if (!input_file.eof())
		{
			std::vector < bool> x = table[tmp_char];
			for (int i = 0; i < x.size(); i++)
				std::cout << x[i];
			std::cout << ' ';
		}
	}
	std::cout << std::endl;
}


int main()
{
	std::string filename;
	std::cout << "Enter the filename:";
	std::cin >> filename;
	int count_of_symbol=0;
	std::map<char, int> table_of_frequency = frequency_symbol(filename, count_of_symbol);

	/*for (auto it = table_of_frequency.begin(); it != table_of_frequency.end(); ++it)
	{
		std::cout << it->first << " : " << it->second << std::endl;
	}*/

	if (table_of_frequency.size() > 1)
	{
		node *root = build_tree(table_of_frequency);
		std::map<char, std::vector<bool> >table = table_code(root);
		/*for (auto it = table.begin(); it != table.end(); ++it)
		{
			std::cout << it->first << " : ";//<< it->second << std::endl;///âûâîä íà ýêðàí
			std::vector < bool> x = it->second;
			for (int i = 0; i < x.size(); i++)
				std::cout << x[i];
			std::cout << std::endl;
		}*/
		encoding(table, table_of_frequency, filename, count_of_symbol);
	}
	else
	{
		std::ofstream output_file;
		std::ifstream input_file;
		std::string filename_cod = filename + "_coding.bin";
		filename = filename + ".txt";
		input_file.open(filename);
		output_file.open(filename_cod);
		output_file << count_of_symbol << " " << table_of_frequency.size() << table_of_frequency.begin()->first << table_of_frequency.begin()->second;
		input_file.close();
		output_file.close();
	}
//	print_coding(table, filename);
}
