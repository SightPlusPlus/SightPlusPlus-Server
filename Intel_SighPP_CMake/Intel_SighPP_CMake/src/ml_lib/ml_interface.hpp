#pragma once

#include <opencv2/dnn.hpp>
#include "librealsense2/rs.hpp"
#include "../classification_result.hpp"

struct ModelInterface {

	size_t inWidth = 640;
	size_t inHeight = 480;	
	float WHRatio = inWidth / (float)inHeight;
	float inScaleFactor = 0.007843f;
	float meanVal = 127.5;
	float confidence_threshold = 0.8f;

	// TODO Is void correct? Probably need to add output to concurrent list type thing in the MLController
	virtual ClassificationResult do_work(cv::Mat color_matrix, cv::Mat depth_matrix) = 0;
	virtual void set_resolution(size_t width, size_t height) = 0;
	virtual void set_confidence_threshold(float threshold) = 0;

};
