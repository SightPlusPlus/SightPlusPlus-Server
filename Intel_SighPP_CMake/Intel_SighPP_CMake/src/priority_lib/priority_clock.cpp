#include "priority_clock.hpp"


bool priority_clock::check_cooldown(int id, int time_dif)
{
	auto time = clock();
	auto F = cooldowns.find(id);
	SPDLOG_INFO("Item {}, looking for time {}", id, time_dif);
	if (F != cooldowns.end())
	{
		auto old_time = F->second;
		SPDLOG_INFO("Item {}, time is  {}", id, (time - old_time));
		if ((time - old_time) > time_dif)
		{

			SPDLOG_INFO("Item {}, time is  {}", id, (time - old_time));
			return true;
		}
	}
	return false;
}

bool priority_clock::check_contains(int id)
{
	auto F = cooldowns.find(id);

	if (F != cooldowns.end())
	{
		return true;
	}
	return false;
}

bool priority_clock::update_timer(int id)
{
	auto F = cooldowns.find(id);

	if (F != cooldowns.end())
	{
		F->second = clock();
		SPDLOG_INFO("Item {},time updated to {}", id, F->second);
		return true;
	}
	return false;
}

bool priority_clock::insert_new_id(int id)
{
	SPDLOG_INFO("Item {}, inserting into timer map",id);
	auto F = cooldowns.find(id);
	if (F == cooldowns.end())

	{
		SPDLOG_INFO("Item {}, inserting into timer map sucessful", id);
		auto time = clock();
		cooldowns.insert(std::pair<int, clock_t>(id, time));
		return true;
	}
	return false;
}

bool priority_clock::remove_new_id(int id)
{
	if (cooldowns.erase(id) == 1)
	{
		return true;
	}
	return false;
}

bool priority_clock::clear_ids(int time_dif)
{
	auto time = clock();
	for (std::pair<int, clock_t> element : cooldowns)
	{
		time = clock();
		if ((time - element.second) > time_dif)
		{
			cooldowns.erase(element.first);
		}
	}
	return true;
}


priority_clock::priority_clock()
{

}

priority_clock::~priority_clock()
{
}