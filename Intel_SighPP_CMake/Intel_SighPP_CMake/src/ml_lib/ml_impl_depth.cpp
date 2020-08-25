#include "ml_interface.hpp"
#include <iostream>

/// <summary>
/// This struct creates a model that should run some tasks (no tasks defiend so far) on depth frames obtainde from the camara
/// Used for testing and demonstration only, not used in the actual system
/// </summary>
struct MLImplDepth : public ModelInterface {

	ClassificationResult do_work(cv::Mat color_matrix, cv::Mat depth_matrix) override {
		std::cout << "Doing some work with Depth frames\n";
		return ClassificationResult("dummy_depth_model");
	}

};
