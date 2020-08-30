#pragma once
#include <fstream>
/// <summary>
/// Read class names from txt file
/// Based on https://stackoverflow.com/questions/7868936/read-file-line-by-line-using-ifstream-in-c
/// </summary>
/// <param name="filepath">path the txt file where the class names are stored</param>
/// <returns>a list of class names</returns>
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