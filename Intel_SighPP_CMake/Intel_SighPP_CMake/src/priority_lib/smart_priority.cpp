// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2020 Sight++. All Rights Reserved.

#include "smart_priority.hpp"
#include <spdlog/spdlog.h>
#include <iostream>

void smart_priority::determine_location_markers() {
	int fifth_w = ceil(size_w / 5);
	int fifth_h = ceil(size_h / 5);
	int eighth_h = ceil(size_h / 8);

	mid_w = ceil(size_w / 2);
	in_left = mid_w - fifth_w;
	in_right = mid_w + fifth_w;
	out_left = 0 + fifth_w;
	out_right = size_w - fifth_w;
	above = 0 + eighth_h;

}

bool smart_priority::is_middle(ClassificationItem& item) {
	if (item.location == Location::CENTRE || item.location == Location::IN_LEFT || item.location == Location::IN_RIGHT)
	{
		return true;
	}
	return false;
}

void smart_priority::determine_prio(ClassificationItem& item) {
	// Basic no comparison priority maker

	if (item.track_point < 5)
	{
		item.priority = Priority::LOW;
		return;
	}

	SPDLOG_INFO("/n/n Processing item {} : {}", item.name, item.id);
	if (run_cooldown_tracker(item))
	{
		SPDLOG_INFO("Item {}, runing this time", item.name, item.id);
		bool prio = run_emegency_rules(item);
		if (!prio)
		{
			prio = run_high_rules(item);
			if (!prio)
			{
				prio = run_medium_rules(item);
				if (!prio)
				{
					item.priority = Priority::LOW;
				}
			}
		}

	}
	msg_add_location(item);
	msg_add_name(item);
	msg_add_distance(item);
	if (item.priority == Priority::UNDEFINED)
	{
		item.priority = Priority::LOW;
	}
	else if (item.priority > min) {
		min = item.priority;
	}
}

bool smart_priority::run_cooldown_tracker(ClassificationItem& item)
{
	if (item.track_point != 5)
	{
		return false;
	}
	std::string id = pc.get_unique_id(item);
	SPDLOG_INFO("Tracking item : {}", id);
	if (pc.check_contains(id))
	{

		if (item.counter == 1)//check if new item reusing ID
		{
			SPDLOG_INFO("NEW Item {} : {}", item.name, item.id);
			pc.update_timer(id);
			return true;
		}
		else if (std::find(checklist.begin(), checklist.end(), item.name) != checklist.end() && pc.check_cooldown(id, cooldown - cooldown_shortening))
		{
			SPDLOG_INFO("Item OFF COOLDOWN PRIO LIST{} : {}", item.name, item.id);
			pc.update_timer(id);
			return true;
		}
		else if (pc.check_cooldown(id, cooldown)) // item is off cooldown
		{
			SPDLOG_INFO("Item OFF COOLDOWN NORMAL{} : {}", item.name, item.id);
			pc.update_timer(id);
			return true;
		}
		else if (check_cooldown_skip(item))
		{
			SPDLOG_INFO("Item OFF COOLDOWN SKIP{} : {}", item.name, item.id);
			pc.update_timer(id);
			return true;
		}
	}
	else if (item.counter < 2) // make sure its a new item with one fps safety net
	{
		pc.insert_new_id(id);
		return true;
	}

	return false;
}

bool smart_priority::check_cooldown_skip(ClassificationItem& item)
{
	if (run_emegency_rules(item))
	{
		if (pc.check_cooldown(pc.get_unique_id(item), cooldown_emergency))
		{
			SPDLOG_INFO("Item OFF COOLDOWN EMERGENCY SKIP{} : {}", item.name, item.id);
			return true;
		}
		else
		{
			SPDLOG_INFO("Item {} not off cooldown ", item.name);
			item.priority = Priority::UNDEFINED;
		}
	}

	//TODO: ANY OTHER REASONS FOR SKIPPIONG COOLDOWN?
	return false;
}

bool smart_priority::exit_cooldown_high_rules(ClassificationItem& item)
{
	double distance = item.distance;
	double speed = item.speed;

	if (time_until_colision(item) < 2 && is_middle(item))
	{
		return true;
	}


	if (distance < 1.5 && speed > walking_speed / 2 && is_middle(item))
	{
		return true;
	}

	if (distance < 10)
	{
		if (time_until_colision(item) < 5)
		{
			return true;
		}
	}
	return false;
}

bool smart_priority::move_up_prio()
{
	if (min == Priority::MEDIUM)
	{
		for each (auto item in all_data)
		{
			if (item.priority == Priority::MEDIUM)
			{
				item.priority = Priority::HIGH;
			}
		}
	}
}
void smart_priority::msg_add_location(ClassificationItem& item) {

	int x_left = item.top_left.x;
	int y_top = item.top_left.y;
	int x_right = item.bottom_right.x;
	int y_bottom = item.bottom_right.y;
	Location location_w = Location::CENTRE;
	Height location_h = Height::CENTRE;
	std::string result = "";
	if (x_right < out_left) // Out left side
	{
		if (y_bottom < above)
		{
			result += "Above, out left";
			location_w = Location::OUT_LEFT;
			location_h = Height::ABOVE;
		}
		else
		{
			result += "Out left";
			location_w = Location::OUT_LEFT;
		}


	}
	else if (x_left > out_right) //out right side
	{
		if (y_bottom < above)
		{
			result += "Above, out right";
			location_w = Location::OUT_RIGHT;
			location_h = Height::ABOVE;
		}
		else
		{
			result += "Out right";
			location_w = Location::OUT_RIGHT;
		}
	}
	else if (x_left < mid_w && x_right > mid_w) // centre
	{
		if (y_bottom < above)
		{
			result += "Above, centre";
			location_h = Height::ABOVE;
		}
		else
		{
			result += "Centre";
			location_h = Height::CENTRE;
		}

	}
	else if (x_right > out_left && x_right < mid_w) // in left
	{
		if (y_bottom < above)
		{
			result += "Above, in left";
			location_w = Location::IN_LEFT;
			location_h = Height::ABOVE;
		}
		else
		{
			result += "In left";
			location_w = Location::IN_LEFT;
		}
	}
	else if (x_left < out_right && x_left > mid_w) // in right
	{
		if (y_bottom < above)
		{
			result += "Above, in right";
			location_w = Location::IN_RIGHT;
			location_h = Height::ABOVE;
		}
		else
		{
			result += "In right";
			location_w = Location::IN_RIGHT;
		}
	}

	item.data.push_back(result);
	item.msg += result;
	item.location = location_w;
	item.height = location_h;

}
void smart_priority::msg_add_name(ClassificationItem& item) {
	std::string insert = "";
	if (item.msg.length() == 0)
	{
		insert = item.name + std::to_string(item.id);
	}
	else {
		insert = ", " + item.name + std::to_string(item.id);
	}


	item.msg += insert;
}
void smart_priority::msg_add_distance(ClassificationItem& item) {
	double value = std::ceil(item.distance * 100.0) / 100.0;
	std::string insert = "";
	insert = ", " + two_deci::to_string_precise(value);
	if (item.counter < 5) {
		if (value == 1) {
			insert = insert + " meter";
		}
		else {
			insert = insert + " meters";
		}
	}
	item.msg += insert;
}

double smart_priority::time_until_colision(ClassificationItem& item)
{
	if (item.speed > 0.1)
	{
		return item.distance / item.speed;
	}
	return 100000000;
}

bool smart_priority::run_emegency_rules(ClassificationItem& item) {

	double distance = item.distance;
	double speed = item.speed;
	int return_value = 0;
	if (distance < 0.4)
	{
		if (is_middle(item))
		{
			item.priority = Priority::URGENT;
			return true;
		}
	}
	/*if (time_until_colision(item) <= 1.5 && is_middle(item))
	{
		item.priority = Priority::URGENT;
		return true;
	}*/

	/*else if (distance < 3 && is_middle(item)) {
		if (time_until_colision(item) < 3)
		{
			item.priority = Priority::URGENT;
			return true;
		}
	}*/

	return false;

}

bool smart_priority::run_high_rules(ClassificationItem& item) {
	double distance = item.distance;
	double speed = item.speed;


	if (distance < 1)
	{
		item.priority = Priority::HIGH;
		return true;
	}

	else if (std::find(checklist.begin(), checklist.end(), item.name) != checklist.end())
	{
		item.priority = Priority::HIGH;
		return true;
	}
	else if (time_until_colision(item) <= 4 && is_middle(item))
	{
		item.priority = Priority::HIGH;
		return true;
	}

	else if (distance < 3 && is_middle(item))
	{
		item.priority = Priority::HIGH;
		return true;
	}
}
bool smart_priority::run_medium_rules(ClassificationItem& item) {
	double distance = item.distance;
	double speed = item.speed;

	if (item.distance < 5)
	{
		item.priority = Priority::MEDIUM;
		return true;
	}
	else if (item.distance < 10)
	{
		if (speed > 0.5)
		{
			item.priority = Priority::MEDIUM;
			return true;
		}
	}
	return false;
}

void smart_priority::assign_priority()
{

	for (auto&& item : all_data)
	{
		SPDLOG_INFO("Prioritising item {}", (item.id));
		determine_prio(item);
		SPDLOG_INFO("Item result:  {}", item.to_string());
	}

	pc.clear_ids(10000);
	sort();
}
