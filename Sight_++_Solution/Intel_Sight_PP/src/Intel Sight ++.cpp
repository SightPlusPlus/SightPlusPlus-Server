#include <iostream>
#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>

#include "ml_controller.hpp"
#include "ml_interface.hpp"
#include "ml_impl_depth.cpp"
#include "ml_impl_rgb.cpp"
#include "service_controller.cpp"
#include "output_interface_controller.cpp"
#include <iostream>
#include "caffe_impl.cpp"



int main(int argc, char** argv)
{
	std::cout << "Starting!\n";
	#define _SOLUTIONDIR = R"($(SolutionDir))"

	// TODO Allow for Reading a file or using stream from camera
	// TODO Read command line parameter to use recording or live stream
	// TODO Add protection against files not being found
	rs2::pipeline pipe;
	rs2::config cfg;




	/// <summary>
	/// This section is to capture the flags from the user. 
	/// Example flags are: 
	/// 1) realsense		: This is used run the system from the camera.
	/// 2) -rec hello.bag	: This is used to record the current input into a file with the name following the flag.
	/// 3) -play hello.bag  : This is used to play the file from the file following the flag.
	/// </summary>
	/// <param name="argc"></param>
	/// <param name="argv"></param>
	/// <returns></returns>
	if (argc > 1)
	{
		std::cout << "Flags found" << std::endl;
		for (size_t i = 1; i < argc; i++)
		{
			// Capture next arg
			std::string next_arg = argv[i];



			std::cout << next_arg << " next flag" << std::endl;
			if (next_arg.compare("realsense") == 0)
			{
				std::cout << "Streaming from camera" << std::endl;
				cfg.enable_all_streams();
				continue;
				
			}


			else if (next_arg.compare("-rec") == 0 && (i+1) < argc)
			{
				try
				{

					std::string file_ = argv[++i];
					std::string path_ = ".\\recordings\\" + file_;
					std::cout << "Recording To file: " << path_ << std::endl;
					cfg.enable_record_to_file(path_);
					continue;
				}
				catch (const std::exception&)
				{
					std::cout << "Error with recording" << std::endl;
				}
			}
			else if (next_arg.compare("-rec") == 0 && !((i + 1) < argc))
			{
				std::cout << "Missing flags for recording" << std::endl;
			}

			else if (next_arg.compare("-play") == 0 && (i + 1) < argc)
			{
				try
				{

					std::string file_ = argv[++i];
					std::string path_ = ".\\recordings\\" + file_;
					std::cout << "playing from file: " << path_ << std::endl;
					cfg.enable_device_from_file(path_);
					continue;
				}
				catch (const std::exception&)
				{
					std::cout << "Error with playing from file" << std::endl;
				}
			}
			else if (next_arg.compare("-play") == 0 && !((i + 1) < argc))
			{
				std::cout << "Missing flags for playing" << std::endl;
			}
		}

	}
	else
	{
		std::cout << "Streaming from test file" << std::endl;
		cfg.enable_device_from_file(".\\recordings\\outdoors.bag");
	}



	pipe.start(cfg);

	std::cout << "Got pipeline\n";

	MLController ml_controller;
	// TODO Read command line parameters for which models to use?
	MLImplDepth ml_depth;
	MLImplRGB ml_rgb;
	// TODO Add correct paths for testing
	// TODO Add command line parameter for files to use?
	CaffeModelImpl caffe("link to prototxt file", "link to caffemodel file", "link to class name text file");

	// Add more ML implementations here as needed
	ml_controller.add_depth_model(ml_depth);
	ml_controller.add_rgb_model(ml_rgb);
	ml_controller.add_rgb_model(caffe);

	std::cout << "Created MLController and added ML models\n";

	Prioritiser prioritiser;

	ServiceController service(pipe, ml_controller, prioritiser);
	service.main();
}