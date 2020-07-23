#include <iostream>
#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>

#include "ml_controller.hpp"
#include "ml_interface.hpp"
#include "ml_impl_depth.cpp"
#include "ml_impl_rgb.cpp"
#include "service_controller.cpp"
#include "output_stream_controller.cpp"
#include <iostream>
#include "caffe_impl.cpp"
#include "depth_priority.cpp"
#include "size_priority.cpp"
#include "classification_result.hpp"



int main(int argc, char** argv)
{
	std::cout << "Starting!\n";
	#define _SOLUTIONDIR = R"($(SolutionDir))"

	// TODO Allow for Reading a file or using stream from camera
	// TODO Read command line parameter to use recording or live stream
	// TODO Add protection against files not being found
	rs2::pipeline pipe;
	rs2::config cfg;

	auto stream_depth = false;
	auto stream_color = false;



	/// <summary>
	/// This section is to capture the flags from the user. 
	/// Example flags are: 
	/// 1) realsense		: This is used run the system from the camera.
	/// 2) -rec hello.bag	: This is used to record the current input into a file with the name following the flag.
	/// 3) -play hello.bag  : This is used to play the file from the file following the flag.
	/// 4) -depth			: This is used to show the depth stream in a window
	/// 5) -color			: This is used to show the color stream in a window
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
				// TODO enable_all_stream() enables the streams with 640x480
				cfg.enable_all_streams();				
				//cfg.enable_stream(RS2_STREAM_COLOR, 1280, 720, RS2_FORMAT_RGB8);
				//cfg.enable_stream(RS2_STREAM_DEPTH, 1280, 720, RS2_FORMAT_Z16, 30);
				
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

			else if (next_arg.compare("-depth") == 0)
			{
				std::cout << "Streaming Depth Output" << std::endl;
				stream_depth = true;
			}

			else if (next_arg.compare("-color") == 0)
			{
				std::cout << "Streaming Color Output" << std::endl;
				stream_color = true;
			}
		}

	}
	else
	{
		std::cout << "Streaming from test file" << std::endl;
		cfg.enable_device_from_file(".\\recordings\\outdoors.bag");
	}



	auto config = pipe.start(cfg);

	for (auto && stream_profile : pipe.get_active_profile().get_streams())
	{
		auto profile = stream_profile.as<rs2::video_stream_profile>();
		std::cout << profile.stream_name() << ": " << profile.width() << "x" << profile.height() << std::endl;
	}
	
	std::cout << "Got pipeline\n";

	MLController ml_controller;
	// TODO Read command line parameters for which models to use?
	
	MLImplDepth ml_depth;
	MLImplRGB ml_rgb;

	
	// TODO Add correct paths for testing
	// TODO Add command line parameter for files to use?
	
	auto profile = config.get_stream(RS2_STREAM_COLOR).as<rs2::video_stream_profile>();
	CaffeModelImpl caffe("./models/no_bn.prototxt", "./models/no_bn.caffemodel", "./models/no_bn-classnames.txt", profile);
	
	// Add more ML implementations here as needed
	ml_controller.add_model(ml_depth);
	ml_controller.add_model(ml_rgb);
	ml_controller.add_model(caffe);

	std::cout << "Created MLController and added ML models\n";

	std::string name_prio_depth = "depth_prio";
	depth_priority* prio_depth = new depth_priority(&name_prio_depth);

	std::cout << prio_depth->get_name() << std::endl;

	//std::string name_prio_size = "size";
	//priority_module* prio_depth = new size_priority(&name_prio_size);


	Prioritiser* prioritiser = new Prioritiser;
	//add modules
	prioritiser->add_module(*prio_depth);
	// Todo: load prio model from flag
	prioritiser->set_module(name_prio_depth);
	prioritiser->load_module();

	OutputStreamController output_stream_controller(stream_depth, stream_color);
  
	ServiceController service(pipe, ml_controller, *prioritiser, output_stream_controller);
	service.main();
}