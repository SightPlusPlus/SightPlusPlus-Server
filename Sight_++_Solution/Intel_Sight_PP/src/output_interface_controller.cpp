#pragma once

#include "librealsense2/rs.hpp"
#include "opencv2/opencv.hpp"

// TODO Should output stream window stuff be here or in a seperate class/file?
class OutputInterfaceController {
	
    //std::vector<PrioritisedClassificationResult> prioritised_results_;
    std::string depth_output_window_;
	bool show_depth_output_;
	
    std::string color_output_window_;
	bool show_color_output_;

    rs2::colorizer color_map_;

	// TODO Receive prioritised info from ServiceController
    // TODO Output something?

public:

	OutputInterfaceController(const bool show_depth_window, const bool show_color_window) : show_depth_output_(show_depth_window), show_color_output_(show_color_window)
	{
		std::cout << "Constructing output interface controller" << std::endl;
		if (show_depth_window)
		{
			depth_output_window_ = "Depth Image";
			namedWindow(depth_output_window_, cv::WINDOW_AUTOSIZE);
		}
		if (show_color_window)
		{
			color_output_window_ = "Color Image";
			namedWindow(color_output_window_, cv::WINDOW_AUTOSIZE);
		}
		std::cout << "Constructed output interface controller" << std::endl;
	}

	void depth_window(const rs2::frame& frame) const
	{
        const auto width = frame.as<rs2::video_frame>().get_width();
        const auto height = frame.as<rs2::video_frame>().get_height();

		const cv::Mat depth_mat(cv::Size(width, height), CV_8UC3, const_cast<void*>(frame.get_data()), cv::Mat::AUTO_STEP);
        imshow(depth_output_window_, depth_mat);
    }

	void color_window(const rs2::frame& frame) const
	{
        const auto width = frame.as<rs2::video_frame>().get_width();
        const auto height = frame.as<rs2::video_frame>().get_height();
		
		const cv::Mat color_mat(cv::Size(width, height), CV_8UC3, const_cast<void*>(frame.get_data()), cv::Mat::AUTO_STEP);
		// Switch RGB to BGR as openCV uses BGR
		cv::Mat bgr;
		cvtColor(color_mat, bgr, cv::COLOR_RGB2BGR);
		
        imshow(color_output_window_, bgr);
    }

	bool should_stream_depth() const
	{
		return show_depth_output_;
	}

	bool should_stream_color() const
	{
		return show_color_output_;
	}

	bool is_window_ready() const
	{
		return wait_key() && is_depth_window_ready() && is_color_window_ready();
	}

	bool wait_key() const
	{
		if (show_depth_output_ || show_color_output_)
		{
			return cv::waitKey(1) < 0;
		}
		return true;
	}
	
	bool is_depth_window_ready() const
	{
		if (show_depth_output_)
		{
			return getWindowProperty(depth_output_window_, cv::WND_PROP_AUTOSIZE) >= 0;
		}
		return true;
	}

	bool is_color_window_ready() const
	{
		if (show_color_output_)
		{
			return getWindowProperty(color_output_window_, cv::WND_PROP_AUTOSIZE) >= 0;
		}
		return true;
	}

};