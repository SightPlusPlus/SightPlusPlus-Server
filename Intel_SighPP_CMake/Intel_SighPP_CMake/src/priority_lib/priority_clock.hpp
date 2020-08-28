#pragma once

#include "priority_module.hpp"
#include <spdlog/spdlog.h>
#include <string>
#include <map>
#include "../classification_result.hpp"

class priority_clock
{
protected:
	/// <summary>
	/// Map for holding live ids of the current seen objects, with time of last sent to the user in the value;
	/// </summary>
	std::unordered_map<int, clock_t> cooldowns;



public:
	priority_clock();
	~priority_clock();
	/// <summary>
	/// Checks whether a given ID was last updated before a give time. 
	/// </summary>
	/// <param name="id"> id of the object </param>
	/// <param name="time_dif"> time in milliseconds for matching</param>
	/// <returns> True if  clock - value > time_dif else False</returns>
	bool check_cooldown(int id, int time_dif);
	bool check_contains(int id);
	bool update_timer(int id);
	bool insert_new_id(int id);
	bool remove_new_id(int id);
	bool clear_ids(int time_dif);




};
