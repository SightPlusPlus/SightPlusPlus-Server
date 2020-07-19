#include "priority_module.hpp"

class depth_priority: public priority_module
{
public:
	int compare(ClassificationItem& item_1, ClassificationItem& item_2)
	{
		return -1;
	}
};

