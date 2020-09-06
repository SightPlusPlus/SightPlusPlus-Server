#pragma once

#include <opencv2/dnn.hpp>
#include "librealsense2/rs.hpp"
#include "../classification_result.hpp"

struct ModelInterface {

	// TODO Is void correct? Probably need to add output to concurrent list type thing in the MLController
	virtual ClassificationResult do_work(cv::Mat color_matrix, cv::Mat depth_matrix) = 0;
	virtual void set_resolution(size_t width, size_t height) = 0;

};
