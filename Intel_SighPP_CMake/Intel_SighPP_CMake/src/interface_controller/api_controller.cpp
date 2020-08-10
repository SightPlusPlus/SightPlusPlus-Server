#pragma once

#include "api_controller.hpp"

#include <tbb/parallel_for_each.h>

void ApiController::add_user(ApiUserInterface& user)
{
	api_users_.push_back(&user);	
}

void ApiController::new_items(const std::vector<PrioritisedClassificationResult>& results)
{
	std::cout << "api_controller.new_items" << std::endl;
	for (auto&& result : results)
	{
		for (auto&& item : result.objects)
		{
			tbb::parallel_for_each(
				api_users_.begin(),
				api_users_.end(),
				[&](ApiUserInterface* api_user)
				{
					std::cout << "Sending results to output api users" << std::endl;
					api_user->new_item(item);
				}
			);
		}
	}

	std::cout << "api_controller done" << std::endl;	
}
