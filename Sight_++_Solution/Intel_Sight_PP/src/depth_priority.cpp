#include "priority_module.hpp"

class depth_priority: public priority_module
{
public:
	int compare(ClassificationItem& item_1, ClassificationItem& item_2)
	{
		return -1;
	}
	void sort_vector(std::vector<ClassificationResult>& data, std::vector<PrioritisedClassificationResult>& result)
	{
		//Todo: fill in method
		std::vector<PrioritisedClassificationResult> test;
	}
	std::vector<PrioritisedClassificationResult> run(std::vector<ClassificationResult>& results)
	{
		//Todo: fill in method
		std::vector<PrioritisedClassificationResult> test;
		return test;
	}
};

