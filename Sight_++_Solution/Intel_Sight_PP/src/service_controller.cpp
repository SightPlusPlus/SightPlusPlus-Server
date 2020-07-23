#include <iostream>
#include <utility>
#include "librealsense2/rs.hpp"
#include "ml_controller.hpp"
#include "prioritiser.hpp"
#include "classification_result.hpp"
#include "output_stream_controller.cpp"

class ServiceController {

	rs2::pipeline pipe_;
	MLController ml_controller_;
	Prioritiser prioritiser_;
	OutputStreamController output_stream_controller_;

public:

	ServiceController(
		rs2::pipeline& pipe, MLController& ml_controller, 
		Prioritiser& prioritiser, OutputStreamController output_controller)
	: pipe_(pipe), ml_controller_(ml_controller), prioritiser_(prioritiser), output_stream_controller_(std::move(output_controller))
	{}

	int main() try {
		rs2::align align_to(RS2_STREAM_COLOR);
		rs2::colorizer color_map;

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
			
			std::cout << "Doing ML on frames\n";
			ml_controller_.new_frames(data);

			std::cout << "Prioritising results" << std::endl;
			auto prioritised_results = prioritiser_.prioritise(ml_controller_.get_and_clear_results());	

			//std::cout << test_vector[0].to_string() << std::endl;
			//auto prioritised_results = prioritiser_.prioritise(test_vector);

			std::cout << "Prioritised " << prioritised_results.size() << " results" << std::endl;
			if (prioritised_results.size() > 0)
			{
				std::cout << prioritised_results[0].to_string() << std::endl;
			}
      
			std::cout << "Streaming stream to output" << std::endl;
			// TODO Should apply_filter be here?
			output_stream_controller_.stream_to_windows(data.get_depth_frame().apply_filter(color_map), data.get_color_frame());
      
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
};