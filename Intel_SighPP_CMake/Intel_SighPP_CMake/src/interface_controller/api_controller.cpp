#pragma once

#include "api_controller.hpp"

#include <spdlog/spdlog.h>
#include <tbb/parallel_for_each.h>

void ApiController::add_user(ApiUserInterface& user)
{
	api_users_.push_back(&user);
}

void ApiController::new_items(const std::vector<PrioritisedClassificationResult>& results)
{
	SPDLOG_INFO("Sending items to API users");
	for (auto&& result : results)
	{
		for (auto&& item : result.objects)
		{
			tbb::parallel_for_each(
				api_users_.begin(),
				api_users_.end(),
				[&](ApiUserInterface* api_user)
				{
					api_user->new_item(item);
				}
			);
		}
	}
}
