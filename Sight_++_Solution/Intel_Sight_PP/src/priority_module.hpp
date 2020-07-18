#pragma once

#include <vector>
#include <iostream>
#include "classification_result.hpp"

class priority_module{
protected:
	std::string name;
public:
	virtual std::string get_name() { return name; }
	virtual int compare(ClassificationItem& item_1, ClassificationItem& item_2) = 0;
	virtual void sort_vector(std::vector<ClassificationResult>& data, std::vector<PrioritisedClassificationResult>& result) = 0;
	virtual std::vector<PrioritisedClassificationResult> run(std::vector<ClassificationResult>& results) = 0;

};

