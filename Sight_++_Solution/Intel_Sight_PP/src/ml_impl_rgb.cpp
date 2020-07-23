#include "ml_interface.hpp"
#include <iostream>

struct MLImplRGB : public ModelInterface {
	
	ClassificationResult do_work(cv::Mat color_matrix, cv::Mat depth_matrix) override {
		std::cout << "Doing some work with RGB frames\n";
		return ClassificationResult("dummy_model");
	}

};
