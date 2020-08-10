#pragma once
#include <fstream>

// Based on https://stackoverflow.com/questions/7868936/read-file-line-by-line-using-ifstream-in-c

static std::vector<std::string> read_class_name_file(std::string filepath)
{
	std::ifstream input(filepath);

	std::vector<std::string> classes;
	std::string line;
	
	while(std::getline(input, line))
	{
		classes.push_back(line);
	}

	return classes;
};