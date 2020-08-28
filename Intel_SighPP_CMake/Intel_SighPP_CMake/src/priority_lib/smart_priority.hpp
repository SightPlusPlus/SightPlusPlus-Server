#pragma once

#include "priority_module.hpp"
#include "priority_clock.hpp"
#include <spdlog/spdlog.h>
#include <string>
#include <map>

class smart_priority : public priority_module
{
protected:
	int const size_w = 640;
	int const size_h = 480;
	int const cooldown = 5000;
	double const walking_speed = 1.4;
	int centre_line = 0;
	int out_left = 0;
	int out_right = 0;
	int in_left = 0;
	int in_right = 0;
	int above = 0;
	int bottom  = 0;
	int mid_w = 0;
	priority_clock pc;
public:

	smart_priority(std::string name = "default")
		: priority_module(&name) {
		determine_location_markers();
	}
	void determine_location_markers();
	//data: 
	void msg_add_location(ClassificationItem& item);
	void msg_add_name(ClassificationItem& item);
	void msg_add_distance(ClassificationItem& item);
	double time_until_colision(ClassificationItem& item);
	
	//Tracking check
	bool run_cooldown_tracker(ClassificationItem& item);
	bool check_cooldown_skip(ClassificationItem& item);
	bool exit_cooldown_high_rules(ClassificationItem& item);


	//Prio Current Level
	void determine_prio(ClassificationItem& item);
	bool run_emegency_rules(ClassificationItem& item);
	bool run_high_rules(ClassificationItem& item);
	bool run_medium_rules(ClassificationItem& item);

	void sort() {
		std::sort(all_data.begin(), all_data.end(), [](const ClassificationItem& item_1, const ClassificationItem& item_2) {
			return item_1.priority > item_2.priority;
			});
	}

	void assign_priority();


};


//
//class depth_priority : public priority_module
//{
//public:
//	depth_priority(std::string* name): priority_module(name) {}
//	void sort_data() {
//		std::sort(all_data.begin(), all_data.end(), [](const ClassificationItem& item_1, const ClassificationItem& item_2) {
//			return item_1.distance < item_2.distance;
//			});
//	}
//
//	void issue_priority() {
//		sort_data();
//
//		int total = all_data.size();
//		int number_of_highs = 0.8 * total;
//		int number_of_mediums = (0.4 * total) - number_of_highs;
//
//		for (size_t i = 0; i < all_data.size(); i++)
//		{
//			if (number_of_highs > 0)
//			{
//				all_data[i].prio = HIGH;
//				number_of_highs--;
//			}
//			else if (number_of_mediums > 0)
//			{
//				all_data[i].prio = MEDIUM;
//				number_of_mediums--;
//			}
//			else 
//			{
//				all_data[i].prio = LOW;
//			}
//		}
//
//
//
//	}
//
//};
