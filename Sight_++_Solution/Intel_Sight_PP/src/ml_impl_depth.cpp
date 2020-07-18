#include "ml_interface.hpp"
#include <iostream>

struct MLImplDepth : public ModelInterface<rs2::depth_frame> {

	ClassificationResult do_work(rs2::depth_frame frames) override {
		std::cout << "Doing some work with Depth frames\n";

		float width = frames.get_width();
		float height = frames.get_height();

		float distance = frames.get_distance(width / 2, height / 2);

		std::cout << "Did depth work, center of frame is " << distance << " meters away" << std::endl;
		
		return ClassificationResult();
	}

};
