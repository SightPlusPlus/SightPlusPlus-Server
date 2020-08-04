#pragma once
#include <vector>
#include <algorithm>
#include <iostream>
#include "classification_result.hpp"

class priority_module {
protected:
	std::string name;
	std::vector<ClassificationItem> all_data;
public:
	priority_module(std::string* new_name) {
		name = *new_name;
	}
	priority_module() {
		name = "default";
	}
	virtual std::string get_name() { return name; }
	void build_data(std::vector<ClassificationResult>& data);
	virtual void sort() = 0;
	virtual void assign_priority() = 0;

	PrioritisedClassificationResult* run(std::vector<ClassificationResult>* result)
	{
		all_data.clear();
		PrioritisedClassificationResult* dept = new PrioritisedClassificationResult;
		priority_module::build_data(*result);
		dept->model_name = name;
		sort();
		assign_priority();
		dept->objects = all_data;
		return dept;
	}

};

	