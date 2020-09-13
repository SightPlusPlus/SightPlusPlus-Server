// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2020 Sight++. All Rights Reserved.

#include <vector>
#include <iostream>
#include "priority_module.hpp"

void priority_module::add_to_checklist(std::string& input)
{
	if (std::find(checklist.begin(), checklist.end(), input) != checklist.end())
	{
		checklist.push_back(input);
	}
	else
	{
		SPDLOG_INFO("_____________Items already in the list____________");
	}
}

void priority_module::remove_from_checklist(std::string& input)
{
	try
	{
		std::remove(checklist.begin(), checklist.end(), input);
	}
	catch (const std::exception&)
	{
		SPDLOG_INFO("_____________Error in removing from checklist____________");
	}
}

void priority_module::clear_checklist()
{
	checklist.clear();
}

void priority_module::build_data(std::vector<ClassificationResult>& data) {

	SPDLOG_INFO("Building data into single vector");
	for (size_t i = 0; i < data.size(); i++)
	{
		all_data.insert(all_data.end(), data[i].objects.begin(), data[i].objects.end());
	}
};
