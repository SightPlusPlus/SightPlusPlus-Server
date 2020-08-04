#include "priority_module.hpp"

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
			std::cout << "ItemDistance: " << item.distance << std::endl;
			if (item.distance < 1)
			{
				std::cout << "Setting priority to HIGH" << std::endl;
				item.priority = Priority::HIGH;
			}
			else if(item.distance < 2)
			{
				std::cout << "Setting priority to MEDIUM" << std::endl;
				item.priority = Priority::MEDIUM;
			}
			else
			{
				std::cout << "Setting priority to LOW" << std::endl;
				item.priority = Priority::LOW;
			}
		}
	}


};

