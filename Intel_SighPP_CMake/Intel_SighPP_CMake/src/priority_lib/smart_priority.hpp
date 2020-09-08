// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2020 Sight++. All Rights Reserved.

#pragma once

#include "priority_module.hpp"
#include "priority_clock.hpp"
#include <spdlog/spdlog.h>
#include "../classification_result.hpp"
#include <string>
#include <map>

class smart_priority : public priority_module
{
protected:
	int const size_w = 640;
	int const size_h = 480;
	int const cooldown = 7000;
	int const cooldown_shortening = 1000;
	int const cooldown_emergency = 3000;
	double const walking_speed = 1.4;
	int centre_line = 0;
	int out_left = 0;
	int out_right = 0;
	int in_left = 0;
	int in_right = 0;
	int above = 0;
	int bottom = 0;
	int mid_w = 0;
	priority_clock pc;
	Priority min = Priority::UNDEFINED;

public:

	/// <summary>
	/// Object Creation. Not to be inherited furhter.
	/// Initialises location markers based on dividers, mostly in fifths of the screen.
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	smart_priority(std::string name = "default")
		: priority_module(&name) {
		determine_location_markers();
	}
	/// <summary>
	/// Determines location markers for objects within the screen. 
	/// </summary>
	void determine_location_markers();


	//________data and message control________: 

	/// <summary>
	/// Based on the rectangle box of recognised object, determin their locations.
	/// </summary>
	/// <param name="item"></param>
	void msg_add_location(ClassificationItem& item);

	/// <summary>
	/// Add name of object to message for interface.
	/// </summary>
	/// <param name="item"></param>
	void msg_add_name(ClassificationItem& item);

	/// <summary>
	/// Add distance of object to message for interface.
	/// </summary>
	/// <param name="item"></param>
	void msg_add_distance(ClassificationItem& item);

	/// <summary>
	/// Calculate time to impact with user. Note does not yet take location into account only distance.
	/// </summary>
	/// <param name="item"></param>
	double time_until_colision(ClassificationItem& item);

	//__________Tracking checking rules______________

	/// <summary>
	/// Tracks the main cooldown of each object. If the item is out of cooldown then it 
	/// is approved to run through the priority rules. If not it is currently 
	/// set to low prio.
	/// TODO: Items that are tracked but not currently seen are also set to low prio. This could be improved after report.
	/// </summary>
	/// <param name="item"></param>
	/// <returns></returns>
	bool run_cooldown_tracker(ClassificationItem& item);

	/// <summary>
	/// Method to run set of rules to see if item needs to be prioritised regardless of cooldown.
	/// </summary>
	/// <param name="item"></param>
	/// <returns></returns>
	bool check_cooldown_skip(ClassificationItem& item);

	/// <summary>
	/// TODO: Currently unused. 
	/// Method for rules that override cooldown but not emergency prio.
	/// </summary>
	/// <param name="item"></param>
	/// <returns></returns>
	bool exit_cooldown_high_rules(ClassificationItem& item);

	/// <summary>
	/// Method switch from object avoidence to object. 
	/// </summary>
	/// <returns></returns>
	bool move_up_prio();


	//____________Prio Current Level_____________

	/// <summary>
	/// Main method for determining the base prio for each item. untilises the methods declared above. 
	/// </summary>
	/// <param name="item"></param>
	void determine_prio(ClassificationItem& item);

	/// <summary>
	/// Check item against emergency rules. 
	/// </summary>
	/// <param name="item"></param>
	/// <returns>Return true if item activated emergency rule.</returns>
	bool run_emegency_rules(ClassificationItem& item);

	/// <summary>
	/// Check item against emergency rules. 
	/// </summary>
	/// <param name="item"></param>
	/// <returns>Return true if item activated High rule.</returns>
	bool run_high_rules(ClassificationItem& item);


	/// <summary>
	/// Check item against emergency rules. 
	/// </summary>
	/// <param name="item"></param>
	/// <returns>Return true if item activated Medium rule.</returns>
	bool run_medium_rules(ClassificationItem& item);

	/// <summary>
	/// Check if item is estimated to be obstructing the users direct path. 
	/// </summary>
	/// <param name="item"></param>
	/// <returns></returns>
	bool smart_priority::is_middle(ClassificationItem& item);

	void sort() {
		std::sort(all_data.begin(), all_data.end(), [](const ClassificationItem& item_1, const ClassificationItem& item_2) {
			return item_1.priority > item_2.priority;
			});
	}

	void assign_priority();


};

