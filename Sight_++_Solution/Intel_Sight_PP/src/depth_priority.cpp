#include "priority_module.hpp"

class depth_priority: public priority_module
{
public:

	depth_priority(std::string* name) 
		: priority_module(name) {}

	int compare(ClassificationItem& item_1, ClassificationItem& item_2)
	{
		if (item_1.distance < item_2.distance)
		{
			return -1;
		}
		else if (item_1.distance > item_2.distance)
		{
			return 1;
		}
		else {
			return 0;
		}
	}
};

