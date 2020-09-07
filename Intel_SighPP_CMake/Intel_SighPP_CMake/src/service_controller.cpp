#include <iostream>
#include <utility>
#include "spdlog/spdlog.h"

#include "service_controller.hpp"

ServiceController::ServiceController(
		rs2::pipeline& pipe, MLController& ml_controller,
		Prioritiser& prioritiser, ApiController& api_controller, OutputStreamController output_controller, const rs2::video_stream_profile& profile)
		: pipe_(pipe), ml_controller_(ml_controller), prioritiser_(prioritiser), api_controller_(api_controller), output_stream_controller_(std::move(output_controller)), profile_(profile)
	{}

int ServiceController::main() try {
	rs2::align align_to(RS2_STREAM_COLOR);
	rs2::colorizer color_map;

	crop = create_rect(profile_);

	while (output_stream_controller_.should_receive_new_frames()) {
		SPDLOG_INFO("----------------------------");
		SPDLOG_INFO("Waiting for frames");
		auto data = pipe_.wait_for_frames();
		data = align_to.process(data);

		SPDLOG_INFO("Frame to opencv matrix and cropping");
		auto color_matrix = frame_to_mat(data.get_color_frame());
		auto depth_matrix = depth_frame_to_meters(data.get_depth_frame());
		color_matrix = color_matrix(crop);
		depth_matrix = depth_matrix(crop);

		SPDLOG_INFO("Doing ML on matrices");
		ml_controller_.new_frames(color_matrix, depth_matrix);
		

		auto prioritiser_instructions = api_controller_.get_prioritiser_instruction_updates();
		// TODO Implement this function or one that does something similar
		//prioritiser_.update_instructions(prioritiser_instructions);
		
		SPDLOG_INFO("Prioritising results");
		auto prioritised_results = prioritiser_.prioritise(ml_controller_.get_and_clear_results());
		SPDLOG_INFO("Prioritised {} results", prioritised_results.size());

		SPDLOG_INFO("Sending items to API");
		api_controller_.new_items(prioritised_results);

		SPDLOG_INFO("Sending frames to output stream");
		output_stream_controller_.stream_to_windows(
			data.get_depth_frame().apply_filter(color_map), depth_matrix,
			data.get_color_frame(), color_matrix,
			prioritised_results);
	}

	return EXIT_SUCCESS;
}
catch (const rs2::error& e)
{
	SPDLOG_ERROR("Realsense error, calling {} ({}): {}", e.get_failed_function(), e.get_failed_args(), e.what());
	return EXIT_FAILURE;
}
catch (const std::exception& e)
{
	SPDLOG_ERROR(e.what());
	return EXIT_FAILURE;
}

cv::Rect ServiceController::create_rect(const rs2::video_stream_profile& profile) const
{
	cv::Size crop_size;
	if (profile.width() / static_cast<float>(profile.height()) > WHRatio)
	{
		crop_size = cv::Size(static_cast<int>(profile.height() * WHRatio), profile.height());
	}
	else
	{
		crop_size = cv::Size(profile.width(), static_cast<int>(profile.width() / WHRatio));
	}

	return cv::Rect(cv::Point((profile.width() - crop_size.width) / 2, (profile.height() - crop_size.height) / 2), crop_size);
}
