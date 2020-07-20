#pragma once
#include <iostream>
#include <string>


struct point {
	int x;
	int y;
};

struct ClassificationItem
{
	char name[100];
	int distance;
	point topLeft;
	point topRight;
	point bottomRight;
};

struct ClassificationResult {
	// TODO What results to store?
	
	std::string model_name;
	std::vector<ClassificationItem> objects;
	
};

struct PrioritisedClassificationResult
{
	// TODO What results to store in a prioritised results?
};