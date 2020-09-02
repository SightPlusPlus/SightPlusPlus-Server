#pragma once

#include <opencv2/dnn.hpp>
#include "librealsense2/rs.hpp"
#include "../classification_result.hpp"

struct ModelInterface {
	virtual ~ModelInterface() = default;
	/// <summary>
 	/// Function to identify objects and return the resutls 
 	/// (obejct class names, relative location, distance and confidence)
 	/// </summary>
 	/// <param name="color_matrix">color matrix obtained from the camera</param>
 	/// <param name="depth_matrix">depth matrix obtained from the camera</param>
	virtual ClassificationResult do_work(cv::Mat color_matrix, cv::Mat depth_matrix) = 0;

};
