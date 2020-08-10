#include "priority_module.hpp"


class depth_priority : public priority_module
{
public:
	depth_priority(std::string* name): priority_module(name) {}
	void sort_data() {
		std::sort(all_data.begin(), all_data.end(), [](const ClassificationItem& item_1, const ClassificationItem& item_2) {
			return item_1.distance < item_2.distance;
			});
	}

	void issue_priority() {
		sort_data();

		int total = all_data.size();
		int number_of_highs = 0.8 * total;
		int number_of_mediums = (0.4 * total) - number_of_highs;

		for (size_t i = 0; i < all_data.size(); i++)
		{
			if (number_of_highs > 0)
			{
				all_data[i].prio = HIGH;
				number_of_highs--;
			}
			else if (number_of_mediums > 0)
			{
				all_data[i].prio = MEDIUM;
				number_of_mediums--;
			}
			else 
			{
				all_data[i].prio = LOW;
			}
		}



	}

};
