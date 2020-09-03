#pragma once

#include <vector>
#include <ctime>

#include "api_user_interface.hpp"
#include "../classification_result.hpp"

class ApiController
{
	std::vector<ApiUserInterface*> api_users_;
	int duration = std::clock();
public:
	
	ApiController() = default;
	
	/// <summary>
	/// Add output API implementation to list of current users.
	/// </summary>
	/// <param name="user">User to add to current users.</param>
	void add_user(ApiUserInterface& user);
	void new_items(const std::vector<ClassificationItem>& results);

};