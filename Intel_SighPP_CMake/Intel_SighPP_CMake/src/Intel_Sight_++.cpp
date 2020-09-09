// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2020 Sight++. All Rights Reserved.

#include <iostream>
#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>
#include "spdlog/spdlog.h"

#include "classification_result.hpp"
#include "service_controller.hpp"
#include "setup_helper.hpp"
#include "interface_controller/api_controller.hpp"
#include "interface_controller/api_impl_websocket.cpp"
#include "interface_controller/output_stream_controller.hpp"
#include "ml_lib/caffe_impl.cpp"
#include "ml_lib/yolo_impl.cpp"
#include "ml_lib/ml_controller.hpp"
#include "ml_lib/model_creator.hpp"
#include "priority_lib/depth_priority.hpp"
#include "priority_lib/smart_priority.hpp"

int main(int argc, char** argv)
{
#define _SOLUTIONDIR = R"($(SolutionDir))"

	rs2::pipeline pipe;
	rs2::config cfg;
	std::vector<CaffeModelImpl> caffe_models;
	std::vector<YoloModelImpl> yolo_models;
	std::set<std::string> yolo_models_names;
	std::set<std::string> caffe_models_names;

	auto stream_depth = false;
	auto stream_color = false;
	auto port = 7979;
	auto theme = 0;
	std::set<std::string> outdoors_model = { "MobileNetSSD_deploy" }; 
	std::set<std::string> indoors_model = { "no_bn" }; 

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
	/// 6) -port			: This is used to select the port the websocket server runs on, default is 7979
	/// 7) -caffe no_bn		: This is used to import the caffe-based network named no_bn.caffemodel etc.
	/// 8) -yolo yolo		: This is used to import the darknet-based network (YoloV3).
	/// 9) -outdoors		: This is used to set up object detection networks, frame resolution and the prioritiser for outdoors environment.
	/// 10) -indoors		: This is used to set up object detection networks, frame resolution and the prioritiser for indoors environment.
	/// <summary>
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
				cfg.enable_all_streams();
				continue;
			}

			if (next_arg.compare("-rec") == 0 && (i + 1) < argc)
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
				continue;
			}

			if (next_arg.compare("-play") == 0 && (i + 1) < argc)
			{
				try
				{
					std::string file_ = argv[++i];
					std::string path_ = "./recordings/" + file_ + ".bag";
					SPDLOG_INFO("playing from file:  {}", path_);
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
				continue;
			}

			if (next_arg.compare("-depth") == 0)
			{
				SPDLOG_INFO("Streaming depth output to window");
				stream_depth = true;
				continue;
			}

			if (next_arg.compare("-color") == 0)
			{
				SPDLOG_INFO("Streaming color output to window");
				stream_color = true;
				continue;
			}

			if (next_arg.compare("-port") == 0 && (i + 1) < argc)
			{
				port = std::atoi(argv[++i]);
				SPDLOG_INFO("Using port {} for websocket", std::to_string(port));
				continue;
			}
			else if (next_arg.compare("-port") == 0 && !((i + 1) < argc))
			{
				SPDLOG_ERROR("Missing value for flag -port");
				continue;
			}

			if (next_arg.compare("-caffe") == 0 && (i + 1) < argc)
			{
				try
				{
					std::string file_ = argv[++i];
					std::pair<std::set<string>::iterator,bool> ret = caffe_models_names.insert(file_);
					if(ret.second){
						CaffeModelImpl caffe_model = create_caffe_network(file_);
						caffe_models.push_back(caffe_model);
						SPDLOG_INFO("Caffe-based network loaded:  {}", file_);
					}
					continue;
				}
				catch (const std::exception& exception)
				{
					SPDLOG_CRITICAL("Error with loading caffe-based network from file: {}", exception.what());
				}
			}
			else if (next_arg.compare("-caffe") == 0 && !((i + 1) < argc))
			{
				SPDLOG_ERROR("Missing flag/argument for loading the caffe-based network");
				continue;
			}
						
			if (next_arg.compare("-yolo") == 0 && (i + 1) < argc)
			{
				try
				{
					std::string file_ = argv[++i];
					std::pair<std::set<string>::iterator,bool> ret = yolo_models_names.insert(file_);
					if(ret.second){
						YoloModelImpl yolo_model = create_yolo_network(file_);
						yolo_models.push_back(yolo_model);
						SPDLOG_INFO("Yolo network loaded:  {}", file_);
					}
					continue;
				}
				catch (const std::exception& exception)
				{
					SPDLOG_CRITICAL("Error with loading darknet-based yolo network from file: {}", exception.what());
				}
			}
			else if (next_arg.compare("-yolo") == 0 && !((i + 1) < argc))
			{
				SPDLOG_ERROR("Missing flag/argument for loading the darknet-based yolo network");
				continue;
			}	

			if(next_arg.compare("-outdoors") == 0)
			{
				theme = 1;
				for(auto i = outdoors_model.begin(); i != outdoors_model.end(); ++i){
					std::pair<std::set<string>::iterator,bool> ret = caffe_models_names.insert(*i);
					if(ret.second){
						CaffeModelImpl caffe_model = create_caffe_network(*i);
						caffe_model.set_resolution(300,300);
						caffe_models.push_back(caffe_model);
						SPDLOG_INFO("Caffe-based network loaded for outdoors environment:  {}", *i);
					}
				}
			}
			else if(next_arg.compare("-indoors") == 0)
			{
				theme = 2;
				for(auto i = indoors_model.begin(); i != indoors_model.end(); ++i){
					std::pair<std::set<string>::iterator,bool> ret = caffe_models_names.insert(*i);
					if(ret.second){
						CaffeModelImpl caffe_model = create_caffe_network(*i);
						caffe_model.set_resolution(640,480);
						caffe_models.push_back(caffe_model);
						SPDLOG_INFO("Caffe-based network loaded for indoors environment:  {}", *i);
					}
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
	for (auto&& stream_profile : pipe.get_active_profile().get_streams())
	{
		auto profile = stream_profile.as<rs2::video_stream_profile>();
		SPDLOG_INFO("{}: {}x{}", profile.stream_name(), profile.width(), profile.height());
	}

	MLController ml_controller;
	// TODO Read command line parameters for which models to use?
	// TODO Add correct paths for testing
	// TODO Add command line parameter for files to use?

	auto profile = config.get_stream(RS2_STREAM_COLOR).as<rs2::video_stream_profile>();

	for (auto i = caffe_models.begin(); i != caffe_models.end(); ++i)
	{
		ml_controller.add_model(*i);
		SPDLOG_INFO("One caffe-based network added...");
	}
	
	for (auto i = yolo_models.begin(); i != yolo_models.end(); ++i)
	{
		ml_controller.add_model(*i);
		SPDLOG_INFO("One yolo network added...");
	}	

	SPDLOG_INFO("Created MLController and added {} ml models", ml_controller.model_count());

	std::string name_prio_depth = "prio_depth";
	std::string name_prio_smart = "prio_smart";
	depth_priority prio_depth{&name_prio_depth};
	smart_priority prio_smart(name_prio_smart);
	SPDLOG_INFO("Using prioritiser module: {}", prio_smart.get_name());

	SPDLOG_INFO("Setting up Prioritiser");
	Prioritiser* prioritiser = new Prioritiser;
	prioritiser->add_module(prio_smart);
	// Todo: load prio model from flag
	switch (theme)
	{
		case 1:
		case 2:
		default: prioritiser->set_module(name_prio_smart);
	}
	prioritiser->load_module();

	SPDLOG_INFO("Setting up output API and API users");
	ApiController api;
	ApiWebSocketImpl websocket_api_user(port, Priority::HIGH);
	api.add_user(websocket_api_user);

	SPDLOG_INFO("Setting up Output Stream Windows");
	OutputStreamController output_stream_controller(stream_depth, stream_color);

	ServiceController service(pipe, ml_controller, *prioritiser, api, output_stream_controller, profile);
	service.main();
}
