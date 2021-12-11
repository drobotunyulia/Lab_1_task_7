#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

const size_t HASHSIZE = 128;


typedef struct
{
	string def_name;
	string def_value;
} table_element_t;

int load_file(string file_name, vector<string>& text);
unsigned long long int transform_to_dec(string number);
size_t get_hash_sum(string value);
size_t replace_text(string file_name);
size_t find_define_value(const vector<table_element_t> table_line,
	                     const string define_name_str,
	                     string &define_value_str);

int main(int argc, char* argv[])
{
	cout << "Laba 1 Task 7\nDrobotun Yulia M8o-210B-20\n" << endl << endl;
	cout << "Replace count = " << replace_text(argv[1]) << endl;
	return 0;
}

int load_file(string file_name, vector<string>& text)
{
	string str;
	ifstream file_in(file_name);
	if (!file_in.is_open())
		return -1;
	while (getline(file_in, str))
	{
		text.push_back(str);
	}
	file_in.close();
	return 0;
}

int save_file(string file_name, const vector<string>& text)
{
	ofstream file_out(file_name);
	if (!file_out.is_open())
		return -1;
	for (size_t i = 0; i < text.size(); i++)
	{
		file_out << text[i] << endl;
	}
	file_out.close();
	return 0;
}

unsigned long long int transform_to_dec(string number)
{
	unsigned long long int finished_number = 0;
	unsigned char symb;
	for (size_t i = 0; i < number.length(); i++)
	{
		symb = number[i];
		if (isdigit(symb))
		{
			symb = symb - '0';
		}
		if (isupper(symb))
		{
			symb = symb - 'A' + 10;
		}
		if (islower(symb))
		{
			symb = symb - 'a' + 36;
		}
		finished_number = finished_number * 62 + symb;
	}
	return finished_number;
}

size_t get_hash_sum(string value)
{
	size_t result = transform_to_dec(value) % HASHSIZE;
	return result;
}

size_t replace_text(string file_name)//#define в середине файла
{
	size_t replace_count = 0;
	string text_line, def_name_string, def_value_string, replace_value_string;
	table_element_t table_element;
	int index_space;
	size_t hash_sum, count_define_string = 0;
	size_t begin_name = 0;
	vector<string> def_name_list;
	vector<string> text;
	vector<vector<table_element_t>> hash_table(HASHSIZE);
	load_file(file_name, text);
	for (size_t i = 0; i < text.size(); i++)
	{
		text_line = text[i];
		if (text_line.find("#define", 0) == 0)
		{
			text_line.erase(text_line.begin(), text_line.begin() + 8);
			count_define_string++;
			index_space = text_line.find(" ", 0);
			def_name_string.resize(index_space);
			copy(text_line.begin(), text_line.begin() + index_space, def_name_string.begin());
			def_name_list.push_back(def_name_string);
			table_element.def_name = def_name_string;
			hash_sum = get_hash_sum(def_name_string);
			index_space = text_line.find(" ", 0);
			text_line.erase(text_line.begin(), text_line.begin() + index_space + 1);
			table_element.def_value = text_line;
			hash_table[hash_sum].push_back(table_element);
		}
		else
		{
			break;
		}
	}
	for (size_t i = count_define_string; i < text.size(); i++)
	{
		for (size_t j = 0; j < def_name_list.size(); j++)
		{
			find_define_value(hash_table[get_hash_sum(def_name_list[j])],
				              def_name_list[j],
				              replace_value_string);
			do
			{
				begin_name = text[i].find(def_name_list[j], begin_name + 1);
				if (begin_name != text[i].npos)
				{				
					text[i].replace(begin_name, def_name_list[j].length(), replace_value_string);
					replace_count++;
				}
			} while (begin_name != text[i].npos);
		}
	}
	save_file(file_name, text);
	return replace_count;
}


size_t find_define_value(const vector<table_element_t> table_line,
	                     const string define_name_str,
	                     string &define_value_str)
{
	for (size_t i = 0; i < table_line.size(); i++)
	{
		if (table_line[i].def_name == define_name_str)
		{
			define_value_str = table_line[i].def_value;
			return i;
		}
	}
	return -1;
}