#include <vector>
#include <iostream>
#include "classification_result.hpp"

class Prioritiser
{

public:

	std::vector<PrioritisedClassificationResult> prioritise(std::vector<ClassificationResult> results)
	{
		std::cout << "Prioritising " << results.size() << " results\n";
		// TODO Prioritise results
		// TODO Transform from Classification Results to Prioritised Results
		return {};
	}
};