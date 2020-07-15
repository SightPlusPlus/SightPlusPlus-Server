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

int main()
{
    std::cout << "Starting!\n";

    // TODO Allow for Reading a file or using stream from camera
    // TODO Read command line parameter to use recording or live stream
    rs2::pipeline pipe;
    rs2::config cfg;
    cfg.enable_device_from_file(".\\recordings\\outdoors.bag"); // stairs or outdoors
    pipe.start(cfg);

    MLController ml_controller;
    // TODO Read command line parameters for which models to use?
    MLImplDepth ml_depth;
    MLImplRGB ml_rgb;

    // Add more ML implementations here as needed
    ml_controller.add_depth_model(ml_depth);
    ml_controller.add_rgb_model(ml_rgb);

    // TODO Create prioritiser class here? with reference to MLController?

    ServiceController service(pipe, ml_controller /* Add prioritiser class here? */);
    service.main();
}