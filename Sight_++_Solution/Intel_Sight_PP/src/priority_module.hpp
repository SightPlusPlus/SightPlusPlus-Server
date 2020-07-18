#pragma once
#include <vector>
#include <iostream>
#include "classification_result.hpp"

class priority_module {
protected:
	std::string name;
	std::vector<ClassificationItem> all_data ;
public:
	priority_module(std::string* new_name) {
		name = *new_name;
	}
	virtual std::string get_name() { return name; }
	virtual PrioritisedClassificationResult* run(std::vector<ClassificationResult>* result);
	virtual int compare(ClassificationItem& item_1, ClassificationItem& item_2) = 0;
	
	void build_data(std::vector<ClassificationResult>& data);
	int partition(std::vector<ClassificationItem>& values, int left, int right);
	void quicksort(std::vector<ClassificationItem>& data, int left, int right);

};

