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
	ApiController() = default;;
	void add_user(ApiUserInterface& user);
	void new_items(const std::vector<ClassificationItem>& results);
};