#pragma once

#include <vector>

#include "api_user_interface.hpp"
#include "../classification_result.hpp"

class ApiController
{
	std::vector<ApiUserInterface*> api_users_;

public:
	ApiController() = default;;
	void add_user(ApiUserInterface& user);
	void new_items(const std::vector<PrioritisedClassificationResult>& results);
};