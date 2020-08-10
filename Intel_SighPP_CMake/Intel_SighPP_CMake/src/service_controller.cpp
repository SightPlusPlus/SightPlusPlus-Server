#include <iostream>
#include <utility>
#include "librealsense2/rs.hpp"
#include "ML_lib/ml_controller.hpp"
#include "priority_lib/prioritiser.hpp"
#include "classification_result.hpp"
#include "interface_controller/output_stream_controller.cpp"
#include "interface_controller/api_controller.hpp"
class ServiceController {

	rs2::pipeline pipe_;
	MLController ml_controller_;
	Prioritiser prioritiser_;
	ApiController api_controller_;
	OutputStreamController output_stream_controller_;
	rs2::video_stream_profile profile_;

	// For cropping
	cv::Rect crop;

	const size_t inWidth = 640;
	const size_t inHeight = 480;
	const float WHRatio = inWidth / (float)inHeight;
	const float inScaleFactor = 0.007843f;
	const float meanVal = 127.5;

public:

	ServiceController(
		rs2::pipeline& pipe, MLController& ml_controller,
		Prioritiser& prioritiser, ApiController& api_controller, OutputStreamController output_controller, const rs2::video_stream_profile& profile)
		: pipe_(pipe), ml_controller_(ml_controller), prioritiser_(prioritiser), api_controller_(api_controller), output_stream_controller_(std::move(output_controller)), profile_(profile)
	{}

	int main() try {
		rs2::align align_to(RS2_STREAM_COLOR);
		rs2::colorizer color_map;

		crop = create_rect(profile_);

		////TEST DATA: FOR PRIORITISER
		//std::vector<ClassificationResult> test_vector;
		//std::vector<ClassificationItem> test_items;
		//
		//ClassificationItem* item_1 = new ClassificationItem();
		//item_1->name = "cup";
		//item_1->distance = 1.5;
		//test_items.push_back(*item_1);
		//
		//ClassificationItem* item_2 = new ClassificationItem();
		//item_2->name = "plate";
		//item_2->distance = 0.9;
		//test_items.push_back(*item_2);
		//
		//ClassificationItem* item_3 = new ClassificationItem();
		//item_3->name = "couch";
		//item_3->distance = 2.5;
		//test_items.push_back(*item_3);
		//
		//ClassificationResult* clas_res = new ClassificationResult();
		//clas_res->model_name = "test_model";
		//clas_res->objects = test_items;
		//test_vector.push_back(*clas_res);
		//
		//////___________________________________________________


		while (output_stream_controller_.should_receieve_new_frames()) {
			std::cout << "Waiting for frames\n";
			auto data = pipe_.wait_for_frames();
			data = align_to.process(data);
			std::cout << "Got new frames\n";

			std::cout << "Get frames and crop" << std::endl;

			auto color_matrix = frame_to_mat(data.get_color_frame());
			auto depth_matrix = depth_frame_to_meters(data.get_depth_frame());
			color_matrix = color_matrix(crop);
			depth_matrix = depth_matrix(crop);

			std::cout << "Doing ML on frames\n";
			ml_controller_.new_frames(color_matrix, depth_matrix);

			std::cout << "Prioritising results" << std::endl;
			auto prioritised_results = prioritiser_.prioritise(ml_controller_.get_and_clear_results());

			//std::cout << test_vector[0].to_string() << std::endl;
			//auto prioritised_results = prioritiser_.prioritise(test_vector);

			std::cout << "Prioritised " << prioritised_results.size() << " results" << std::endl;
			if (prioritised_results.size() > 0)
			{
				std::cout << prioritised_results[0].to_string() << std::endl;
			}

			std::cout << "Sending items to API" << std::endl;
			api_controller_.new_items(prioritised_results);

			std::cout << "Streaming stream to output" << std::endl;
			// TODO Should apply_filter be here?
			output_stream_controller_.stream_to_windows(
				data.get_depth_frame().apply_filter(color_map), depth_matrix,
				data.get_color_frame(), color_matrix,
				prioritised_results);

			std::cout << "----------------------------" << std::endl;
		}

		return EXIT_SUCCESS;
	}
	catch (const rs2::error& e)
	{
		std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	cv::Rect create_rect(const rs2::video_stream_profile& profile) const
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

};





