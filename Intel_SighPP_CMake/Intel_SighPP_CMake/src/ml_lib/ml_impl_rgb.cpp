#include "ml_interface.hpp"
#include <iostream>

/// <summary>
/// This struct creates a modelthat should run some tasks (no tasks defiend so far) on RGB frames obtainde from the camara
/// Used for testing and demonstration only, not used in the actual system
/// </summary>
struct MLImplRGB : public ModelInterface {
	
	ClassificationResult do_work(cv::Mat color_matrix, cv::Mat depth_matrix) override {
		std::cout << "Doing some work with RGB frames\n";
		return ClassificationResult("dummy_model");
	}

};
