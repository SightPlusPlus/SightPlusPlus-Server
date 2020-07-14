#include <iostream>
#include "librealsense2/rs.hpp"
#include "ml_controller.hpp"
#include "ml_interface.hpp"

class ServiceController {

	rs2::pipeline camera;
	MLController ml_controller;

public:

	ServiceController(rs2::pipeline& pipe, MLController ml_controller) : camera(pipe), ml_controller(ml_controller) {}

	int main() try {
		rs2::align align_to(rs2_stream::RS2_STREAM_COLOR);

		while (true) {
			auto data = camera.wait_for_frames();
			data = align_to.process(data);
			std::cout << "Got new frames\n";

			ml_controller.new_frames(data);

			// TODO Receive stuff from prioritiser class
			// TODO Send stream/frames to output interface class
			// TODO Send info from prioritiser to output interface class
		}

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