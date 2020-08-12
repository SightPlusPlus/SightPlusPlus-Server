#include "priority_module.hpp"
#include <spdlog/spdlog.h>

class depth_priority: public priority_module
{
public:

	depth_priority(std::string* name) 
		: priority_module(name) {}
	

	

	void sort() {
		std::sort(all_data.begin(), all_data.end(), [](const ClassificationItem& item_1, const ClassificationItem& item_2) {
			return item_1.distance < item_2.distance;
			});
	}

	void assign_priority() override
	{
		for (auto && item : all_data)
		{
			SPDLOG_INFO("ItemDistance: {}", item.distance);
			if (item.distance < 1)
			{
				SPDLOG_INFO("Setting priority to HIGH");
				item.priority = Priority::HIGH;
			}
			else if(item.distance < 2)
			{
				SPDLOG_INFO("Setting priority to MEDIUM");
				item.priority = Priority::MEDIUM;
			}
			else
			{
				SPDLOG_INFO("Setting priority to LOW");
				item.priority = Priority::LOW;
			}
		}
	}


};

