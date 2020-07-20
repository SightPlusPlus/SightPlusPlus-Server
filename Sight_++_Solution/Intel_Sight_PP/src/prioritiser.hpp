#pragma once
#include <vector>
#include <iostream>
#include "classification_result.hpp"
#include "priority_module.hpp"
#include "priority_module.hpp"

#include<map>

class Prioritiser
{
private:
	std::map<std::string, priority_module&> modules;
	std::string module_choice;
	priority_module* selected_module;
public:
	int load_module();
	void add_module(priority_module& p);
	void set_module(std::string name_module);
	std::vector<PrioritisedClassificationResult> prioritise(std::vector<ClassificationResult> results);
	
};