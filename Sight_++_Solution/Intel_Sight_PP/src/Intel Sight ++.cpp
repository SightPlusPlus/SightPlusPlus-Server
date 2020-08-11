#include <iostream>
#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>

#include "spdlog/spdlog.h"

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
#include "api_controller.hpp"
#include "api_impl_websocket.cpp"
#include "setup_helper.hpp"


int main(int argc, char** argv)
{
	#define _SOLUTIONDIR = R"($(SolutionDir))"

	// TODO Allow for Reading a file or using stream from camera
	// TODO Read command line parameter to use recording or live stream
	// TODO Add protection against files not being found
	rs2::pipeline pipe;
	rs2::config cfg;
	
	auto stream_depth = false;
	auto stream_color = false;

	setup_logging();
	
	SPDLOG_INFO("Starting");

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
		
		SPDLOG_INFO("Flags found");
		for (size_t i = 1; i < argc; i++)
		{
			// Capture next arg
			std::string next_arg = argv[i];


			SPDLOG_INFO("Next flag is {}", next_arg);
			if (next_arg.compare("realsense") == 0)
			{
				SPDLOG_INFO("Streaming from camera");
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
					SPDLOG_INFO("Saving stream to file: {}", path_);
					cfg.enable_record_to_file(path_);
					continue;
				}
				catch (const std::exception& exception)
				{
					SPDLOG_CRITICAL("Error with recording: {}", exception.what());
				}
			}
			else if (next_arg.compare("-rec") == 0 && !((i + 1) < argc))
			{
				SPDLOG_ERROR("Missing required flag for recording, record to what file?");
			}

			else if (next_arg.compare("-play") == 0 && (i + 1) < argc)
			{
				try
				{

					std::string file_ = argv[++i];
					std::string path_ = ".\\recordings\\" + file_;
					SPDLOG_INFO("Playing from file:  {}", path_);
					cfg.enable_device_from_file(path_);
					continue;
				}
				catch (const std::exception& exception)
				{
					SPDLOG_CRITICAL("Error with playing from file: {}", exception.what());
				}
			}
			else if (next_arg.compare("-play") == 0 && !((i + 1) < argc))
			{
				SPDLOG_ERROR("Missing required flag for recording, play from what file?");
			}

			else if (next_arg.compare("-depth") == 0)
			{
				SPDLOG_INFO("Streaming depth output to window");
				stream_depth = true;
			}

			else if (next_arg.compare("-color") == 0)
			{
				SPDLOG_INFO("Streaming color output to window");
				stream_color = true;
			}
		}

	}
	else
	{
		SPDLOG_INFO("Using default recording");
		cfg.enable_device_from_file(".\\recordings\\outdoors.bag");
	}



	auto config = pipe.start(cfg);

	SPDLOG_INFO("Streaming profiles:");
	for (auto && stream_profile : pipe.get_active_profile().get_streams())
	{
		auto profile = stream_profile.as<rs2::video_stream_profile>();
		SPDLOG_INFO("{}: {}x{}", profile.stream_name(), profile.width(), profile.height());
	}

	MLController ml_controller;
	// TODO Read command line parameters for which models to use?
	
	//MLImplDepth ml_depth;
	//MLImplRGB ml_rgb;

	
	// TODO Add correct paths for testing
	// TODO Add command line parameter for files to use?
	
	auto profile = config.get_stream(RS2_STREAM_COLOR).as<rs2::video_stream_profile>();
	CaffeModelImpl caffe_own("./models/no_bn.prototxt", "./models/no_bn.caffemodel", "./models/no_bn-classnames.txt");
	CaffeModelImpl caffe_pre("./models/MobileNetSSD_deploy.prototxt", "./models/MobileNetSSD_deploy.caffemodel", "./models/MobileNetSSD_deploy-classnames.txt");
	
	// Add more ML implementations here as needed
	//ml_controller.add_model(ml_depth);
	//ml_controller.add_model(ml_rgb);
	ml_controller.add_model(caffe_own);
	ml_controller.add_model(caffe_pre);

	SPDLOG_INFO("Created MLController and added {} ml models", ml_controller.model_count());

	std::string name_prio_depth = "depth_prio";
	depth_priority* prio_depth = new depth_priority(&name_prio_depth);
	SPDLOG_INFO("Using prioritiser module: {}", prio_depth->get_name());

	//std::string name_prio_size = "size";
	//priority_module* prio_depth = new size_priority(&name_prio_size);


	SPDLOG_INFO("Setting up Prioritiser");
	Prioritiser* prioritiser = new Prioritiser;
	//add modules
	prioritiser->add_module(*prio_depth);
	// Todo: load prio model from flag
	prioritiser->set_module(name_prio_depth);
	prioritiser->load_module();

	SPDLOG_INFO("Setting up output API and API users");
	ApiController api;
	ApiWebSocketImpl websocket_api_user;
	api.add_user(websocket_api_user);

	SPDLOG_INFO("Setting up Output Stream Windows");
	OutputStreamController output_stream_controller(stream_depth, stream_color);
  
	ServiceController service(pipe, ml_controller, *prioritiser, api, output_stream_controller, profile);
	service.main();
}
