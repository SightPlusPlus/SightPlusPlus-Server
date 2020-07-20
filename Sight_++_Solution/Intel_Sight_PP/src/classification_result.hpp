#pragma once
#include <iostream>
#include <string>


struct point {
	double x;
	double y;
};

struct ClassificationItem
{
	std::string name;
	double distance;
	point topLeft;
	point topRight;
	point bottomRight;

	std::string to_string() {
		std::string s= "";
		s.append( "Printing ClassificationItem: " + name + "\n");
		s.append("ClassificationItem distance: " + std::to_string(distance) + "\n");
		return s;
	}
};

struct ClassificationResult {
	// TODO What results to store?
	
	std::string model_name;
	std::vector<ClassificationItem> objects;


	std::string to_string() {
		std::string s = "";
		s.append("Printing ClassificationResult: " + model_name + "\n");

		for (size_t i = 0; i < objects.size(); i++)
		{
			s.append("Object " + std::to_string(i) + ": " + objects[i].to_string());
		}
		return s;
	}
	
};

struct PrioritisedClassificationResult
{
	// TODO What results to store in a prioritised results?

	std::string model_name;
	std::vector<ClassificationItem> objects;

	std::string to_string() {
		std::string s = "";
		s.append("Printing PrioritisedClassificationResult: " + model_name + "\n");

		for (size_t i = 0; i < objects.size(); i++)
		{
			s.append("Object " + std::to_string(i) +": " + objects[i].to_string());
		}
		return s;
	}
};