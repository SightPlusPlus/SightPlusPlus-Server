#include "ml_interface.hpp"
#include <iostream>

struct MLImplDepth : public ModelInterface {

	ClassificationResult do_work(cv::Mat color_matrix, cv::Mat depth_matrix) override {
		std::cout << "Doing some work with Depth frames\n";

		//auto depth = frames.get_depth_frame();
		//
		//float width = depth.get_width();
		//float height = depth.get_height();
		//
		//float distance = depth.get_distance(width / 2, height / 2);
		//
		//std::cout << "Did depth work, center of frame is " << distance << " meters away" << std::endl;
		
		return ClassificationResult("dummy_depth_model");
	}

};
