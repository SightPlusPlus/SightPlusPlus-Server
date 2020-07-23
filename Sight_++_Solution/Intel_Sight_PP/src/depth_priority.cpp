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


};

