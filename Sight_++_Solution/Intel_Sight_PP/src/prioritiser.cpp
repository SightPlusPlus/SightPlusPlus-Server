#include "prioritiser.hpp"


std::map<std::string, priority_module&> Prioritiser::get_module()
{
	return std::map<std::string, priority_module&>();
}

std::vector<PrioritisedClassificationResult> Prioritiser::prioritise(std::vector<ClassificationResult> results)
{
	std::cout << "Prioritising " << results.size() << " results\n";
	// TODO Prioritise results
	// TODO Transform from Classification Results to Prioritised Results
	return std::vector<PrioritisedClassificationResult>();
}
	