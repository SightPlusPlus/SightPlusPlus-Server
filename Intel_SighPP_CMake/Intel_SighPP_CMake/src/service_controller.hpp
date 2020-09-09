// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2020 Sight++. All Rights Reserved.

#pragma once

#include "librealsense2/rs.hpp"

#include "interface_controller/api_controller.hpp"
#include "interface_controller/output_stream_controller.hpp"
#include "ml_lib/ml_controller.hpp"
#include "priority_lib/prioritiser.hpp"

class ServiceController {

	rs2::pipeline pipe_;
	MLController ml_controller_;
	Prioritiser prioritiser_;
	ApiController api_controller_;
	OutputStreamController output_stream_controller_;
	rs2::video_stream_profile profile_;

	cv::Rect crop;

	const size_t inWidth = 640;
	const size_t inHeight = 480;
	const float WHRatio = inWidth / (float)inHeight;
	const float inScaleFactor = 0.007843f;
	const float meanVal = 127.5;

public:

	ServiceController(
		rs2::pipeline& pipe, MLController& ml_controller,
		Prioritiser& prioritiser, ApiController& api_controller, 
		OutputStreamController output_controller, const rs2::video_stream_profile& profile);

	/// <summary>
	/// Main program loop. Receives and processes frames.
	/// </summary>
	/// <returns>Program exit code</returns>
	int main();

	/// <summary>
	/// Create a correctly sized cropped rectangle.
	/// Based on rs-dnn example.
	/// </summary>
	/// <param name="profile">Stream Profile containing width and height properties.</param>
	/// <returns></returns>
	cv::Rect create_rect(const rs2::video_stream_profile& profile) const;
};
