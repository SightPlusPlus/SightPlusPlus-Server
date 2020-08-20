#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include "./priority_lib/priority.hpp"
inline std::string append(std::string& s, const std::string key, const std::string value, const bool is_string_value, const bool is_last)
{
	s.append(" \"").append(key).append("\": ");
	if (is_string_value) s.append("\"").append(value).append("\"");
	else s.append(value);

	if (is_last) return s;
	return s.append(", ");
}



struct point {
	double x;
	double y;

	point(const int x, const int y) : x(x), y(y) {}

	std::string to_json()
	{
		std::string s = "{";
		append(s, "x", to_string_precise(x), true, false);
		append(s, "y", to_string_precise(y), true, true);
		s.append("}");
		return s;
	}
};

struct ClassificationItem
{
	std::string name;
	std::vector<std::string> data;
	std::string msg;
	std::string position;
	double distance = 0;
	double speed = 0;
	point bottom_left;
	point top_right;
	point top_left;
	point bottom_right;
	Priority priority = Priority::UNDEFINED;
	Location location = Location::CENTRE;
	Height height = Height::CENTRE;
	
	ClassificationItem(const std::string name) : name(name), bottom_left(point(0, 0)), top_right(point(0, 0)),  top_left(point(0, 0)), bottom_right(point(0, 0)) {}
	ClassificationItem(const std::string name, const double distance, const point bottom_left, const point top_right) : name(name), distance(distance), bottom_left(bottom_left), top_right(top_right), top_left(point(280, 150)), bottom_right(point(400, 300)), priority(Priority::UNDEFINED) {}



	std::string to_string() {
		std::string s = "";
		try
		{
			s.append("Printing ClassificationItem: " + name + "\n");
			s.append("ClassificationItem distance: " + std::to_string(distance) + "\n");
			s.append("Priority: " + std::to_string(static_cast<int>(priority)) + "\n");
		}
		catch (const std::exception&)
		{
			s.append("");
		}

		return s;
	}

	std::string to_json()
	{
		std::string s = "{";
		append(s, "name", name, true, false);
		append(s, "distance", to_string_precise(distance), false, false);
		append(s, "msg", msg, true, false);
		append(s, "priority", std::to_string(static_cast<int>(priority)),true,false);
		append(s, "bottom_left", bottom_left.to_json(), false, false);
		append(s, "top_right", top_right.to_json(), false, true);
		s.append("}");

		return s;
	}

};

struct ClassificationResult {
	// TODO What results to store?
public:
	std::string model_name;
	std::vector<ClassificationItem> objects;

	ClassificationResult(const std::string name) : model_name(name) {}

	std::string to_string() {
		std::string s = "";
		try
		{
			s.append("Printing ClassificationResult: " + model_name + "\n");


			for (size_t i = 0; i < objects.size(); i++)
			{
				s.append("Object " + std::to_string(i) + ": " + objects[i].to_string());
			}
		}
		catch (const std::exception&)
		{
			s.append("");
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
		try
		{
			s.append("Printing PrioritisedClassificationResult: " + model_name + "\n");


			for (size_t i = 0; i < objects.size(); i++)
			{
				s.append("Object " + std::to_string(i) + ": " + objects[i].to_string());
			}
		}
		catch (const std::exception&)
		{
			s.append("");
		}

		return s;
	}

};