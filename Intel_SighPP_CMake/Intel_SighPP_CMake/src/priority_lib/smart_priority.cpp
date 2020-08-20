
#include "smart_priority.hpp"
#include <spdlog/spdlog.h>
#include <iostream>



void smart_priority::determine_prio(ClassificationItem& item) {
	// Basic no comparison priority maker
	

	msg_add_location(item);
	msg_add_name(item);
	msg_add_distance(item);

		int prio = run_emegency_rules(item);
		if (prio < 1)
		{
			prio = run_high_rules(item);
			if (prio < 1)
			{
				prio = run_medium_rules(item);
				if (prio < 1)
				{
					item.priority = Priority::LOW;
				}
			}
		}


	if (item.priority == Priority::UNDEFINED)
	{
		item.priority = Priority::LOW;
	}


}

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
bool is_middle(ClassificationItem& item) {
	if (item.location == Location::CENTRE || item.location == Location::IN_LEFT || item.location == Location::IN_RIGHT)
	{
		return true;
	}
	return false;
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
		if (y_bottom > above)
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
		if (y_bottom > above)
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
		if (y_bottom > above)
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
		if (y_bottom > above)
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
		if (y_bottom > above)
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
		insert = item.name;
	}
	else {
		insert = ", " + item.name;
	}


	item.msg += insert;
}
void smart_priority::msg_add_distance(ClassificationItem& item) {
	double value = std::ceil(item.distance * 100.0) / 100.0;
	std::string insert = "";
	if (value > 1) {
		insert = ", " + to_string_precise(value) + " meters";
	}
	else {
		insert = ", " + to_string_precise(value) + " meter";
	}
	item.msg += insert;
}
int smart_priority::run_emegency_rules(ClassificationItem& item) {

	double distance = item.distance;
	double speed = item.speed;
	int return_value = 0;
	if (distance < 0.5)
	{
		if (is_middle(item))
		{
			item.priority = Priority::URGENT;
			return -1;
		}
	}
	if (distance < 1)
	{
		if (speed > 0.4 && is_middle(item))
		{
			item.priority = Priority::URGENT;
			return -1;
		}
	}
	else if (distance < 2 && is_middle(item)) {
		if (speed > 0.8)
		{
			item.priority = Priority::URGENT;
			return -1;
		}
	}

	return 0;

}



int smart_priority::run_high_rules(ClassificationItem& item) {
	double distance = item.distance;
	double speed = item.speed;

	if (distance < 1.5)
	{
		item.priority = Priority::HIGH;
		return 1;
	}

	if (distance < 2)
	{
		if (speed > 0.4 && is_middle(item))
		{
			item.priority = Priority::HIGH;
			return 1;
		}
	}

	if (distance < 3 && is_middle(item))
	{
		if (speed > 0.2)
		{
			item.priority = Priority::HIGH;
			return 1;
		}
	}


	if (distance < 7)
	{
		if (speed > 0.9)
		{
			item.priority = Priority::HIGH;
			return 1;
		}
	}
	return 0;

}
int smart_priority::run_medium_rules(ClassificationItem& item) {
	double distance = item.distance;
	double speed = item.speed;

	if (item.distance < 3)
	{
		if (speed > -0.2)
		{
			item.priority = Priority::MEDIUM;
			return 1;
		}
	}
	else if (item.distance < 7)
	{
		if (speed > 0)
		{
			item.priority = Priority::MEDIUM;
			return 1;
		}
	}

	return 0;
}






