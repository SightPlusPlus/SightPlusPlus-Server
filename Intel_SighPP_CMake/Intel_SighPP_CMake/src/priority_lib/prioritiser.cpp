// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2020 Sight++. All Rights Reserved.

#include "prioritiser.hpp"

#include "spdlog/spdlog.h"

void Prioritiser::set_module(std::string name_module) {
	module_choice = name_module;
}

void Prioritiser::add_module(priority_module& p) {
	modules.insert({ p.get_name() ,p });
}

int Prioritiser::load_module()
{
	try
	{
		selected_module = &modules.at(module_choice);
		return 1;
	}
	catch (const std::exception&)
	{
		return -1;
	}
}

bool compare_items(ClassificationItem& item_1, ClassificationItem& item_2)
{
	bool result = (item_1.distance < item_2.distance);
	return result;
}

std::vector<PrioritisedClassificationResult> Prioritiser::prioritise(std::vector<ClassificationResult> results)
{

	std::vector<PrioritisedClassificationResult> vector_sorted;
	SPDLOG_INFO("Prioritising results from {} models", results.size());
	auto output = selected_module->run(&results);
	vector_sorted.push_back(*(output));
	//TODO Create Vector of multiple items?
	return vector_sorted;
}
