#include <iostream>
#include <utility>
#include "librealsense2/rs.hpp"
#include "ml_controller.hpp"
#include "prioritiser.hpp"
#include "output_interface_controller.cpp"

class ServiceController {

	rs2::pipeline pipe_;
	MLController ml_controller_;
	Prioritiser prioritiser_;
	OutputInterfaceController output_controller_;

public:

	ServiceController(
		rs2::pipeline& pipe, MLController& ml_controller, 
		Prioritiser& prioritiser, OutputInterfaceController output_controller)
	: pipe_(pipe), ml_controller_(ml_controller), prioritiser_(prioritiser), output_controller_(std::move(output_controller))
	{}

	int main() try {
		rs2::align align_to(RS2_STREAM_COLOR);
		rs2::colorizer color_map;
		
		while (output_controller_.is_window_ready()) {
			std::cout << "Waiting for frames\n";
			auto data = pipe_.wait_for_frames();
			data = align_to.process(data);
			std::cout << "Got new frames\n";
			
			std::cout << "Doing ML on frames\n";
			ml_controller_.new_frames(data);
			std::cout << "Did ML on frames\n";

			// TODO Receive stuff from prioritiser class
			// TODO Send info from prioritiser to output interface class
			std::cout << "Prioritising results\n";
			auto prioritised_results = prioritiser_.prioritise(ml_controller_.get_and_clear_results());
			std::cout << "Prioritised results\n";

			std::cout << "Streaming stream to output" << std::endl;
			if (output_controller_.should_stream_depth())
			{
				// TODO Should apply_filter be here?
				output_controller_.depth_window(data.get_depth_frame().apply_filter(color_map));
			}
			if (output_controller_.should_stream_color())
			{
				output_controller_.color_window(data.get_color_frame());
			}
			std::cout << "Streamed stream to output" << std::endl;
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
