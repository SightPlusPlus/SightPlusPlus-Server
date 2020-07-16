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

int main()
{
    std::cout << "Starting!\n";

    // TODO Allow for Reading a file or using stream from camera
    // TODO Read command line parameter to use recording or live stream
    // TODO Add protection against files not being found
    rs2::pipeline pipe;
    rs2::config cfg;
    cfg.enable_device_from_file(".\\recordings\\outdoors.bag");
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