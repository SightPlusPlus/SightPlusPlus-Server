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

	//std::string toStirng() {
	//	std::string s = "";
	//	try
	//	{
	//		s.append("Printing ClassificationItem: " + name + "\n");
	//		s.append("ClassificationItem distance: " + std::to_string(distance) + "\n");
	//	}
	//	catch (const std::exception&)
	//	{
	//		s.append("");
	//	}

	//	return s;
	//}
};

struct ClassificationResult {
	// TODO What results to store?
	
	std::string model_name;
	std::vector<ClassificationItem> objects;
	
};

struct PrioritisedClassificationResult
{
	// TODO What results to store in a prioritised results?

	std::string model_name;
	std::vector<ClassificationItem> objects;

	/*std::string toStirng() {
		std::string s = "";
		try
		{
			s.append("Printing PrioritisedClassificationResult: " + model_name + "\n");

			
			for (size_t i = 0; i < objects.size(); i++)
			{
				s.append("Object " + std::to_string(i) + ": " + objects[i].toStirng());
			}
		}
		catch (const std::exception&)
		{
			s.append("");
		}

		return s;
	}*/
};