#pragma once

#include "priority_module.hpp"
#include <spdlog/spdlog.h>
#include <string>
#include <map>
#include "../classification_result.hpp"

class priority_clock
{
protected:

	std::unordered_map<int, clock_t> cooldowns;
	bool check_cooldown(int id, int time_dif);
	bool check_contains(int id);
	bool update_timer(int id);
	bool insert_new_id(int id);
	bool remove_new_id(int id);
	bool clear_ids(int time_dif);

public:
	priority_clock();
	~priority_clock();

	
	

};
