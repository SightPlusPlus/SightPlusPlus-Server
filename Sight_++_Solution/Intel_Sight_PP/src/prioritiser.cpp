#include "prioritiser.hpp"

int Prioritiser::load_module()
{
	try
	{
		selected_module = &modules.at(module_choice);
		return 1;
	}
	catch (const std::exception&)
	{
		return -1;
	}
}

void Prioritiser::set_module(std::string name_module) {
	module_choice = name_module;
}
void Prioritiser::add_module(priority_module& p) {
	modules.insert({ p.get_name() ,p });
}


std::vector<PrioritisedClassificationResult> Prioritiser::prioritise(std::vector<ClassificationResult> results)
{

	std::vector<PrioritisedClassificationResult> vector_sorted;
	std::cout << "Prioritising " << results.size() << " results\n";
	auto output = selected_module->run(&results);
	vector_sorted.push_back(*(output));
	//TODO Create Vector of multiple items?
	return vector_sorted;
}
		