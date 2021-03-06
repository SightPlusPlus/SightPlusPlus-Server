// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2020 Sight++. All Rights Reserved.

#pragma once
#include <vector>
#include <algorithm>
#include <iostream>
#include "spdlog/spdlog.h"
#include "../classification_result.hpp"



class priority_module {
protected:
public:

	std::vector<std::string> checklist;
	std::string name;
	std::vector<ClassificationItem> all_data;
	priority_module(std::string* new_name) {
		name = *new_name;
	}
	priority_module() {
		name = "default";
	}
	virtual ~priority_module() {}
	virtual void assign_priority() = 0;
	
	void add_to_checklist(std::string& input);
	void remove_from_checklist(std::string& input);
	void clear_checklist();


	std::string get_name() { return name; }
	void build_data(std::vector<ClassificationResult>& data);
	

	std::vector<ClassificationItem>* run(std::vector<ClassificationResult>* result)
	{
		SPDLOG_INFO("Prioritising run function starting");
		try
		{
			if (!all_data.empty())
			{
				all_data.clear();
			}
		}
		catch (const std::exception&)
		{
			SPDLOG_INFO("Failed to clear data with");
		}
		SPDLOG_INFO("Data Cleared in run priortiser");
		priority_module::build_data(*result);
		SPDLOG_INFO("Prioriter data ready for prioriting");
		assign_priority();
		return &all_data;
	}

};

	