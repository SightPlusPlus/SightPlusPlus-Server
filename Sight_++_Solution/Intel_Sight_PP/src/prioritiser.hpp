#pragma once
#include <vector>
#include <iostream>
#include "classification_result.hpp"
#include "priority_module.hpp"
#include<map>

class Prioritiser
{
private:
	std::map<std::string, priority_module&> modules;
public:
	std::map<std::string, priority_module&> get_module();
	std::vector<PrioritisedClassificationResult> prioritise(std::vector<ClassificationResult> results);
};