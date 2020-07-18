#pragma once
#include <vector>
#include <iostream>
#include "classification_result.hpp"

class priority_module {
protected:
	std::string name;
	std::vector<ClassificationItem> all_data ;
public:
	virtual std::string get_name() { return name; }
	virtual void sort_vector(std::string name, std::vector<PrioritisedClassificationResult>& result);
	virtual int compare(ClassificationItem& item_1, ClassificationItem& item_2) = 0;
	virtual std::vector<PrioritisedClassificationResult> run() = 0;
	
	void build_data(std::vector<ClassificationResult>& data);
	int partition(std::vector<ClassificationItem>& values, int left, int right);
	void quicksort(std::vector<ClassificationItem>& data, int left, int right);

	

};

