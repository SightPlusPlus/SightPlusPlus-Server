#include <iostream>
#include <opencv2/dnn/dnn.hpp>

#include "ml_interface.hpp"

struct CaffeModelImpl : public ModelInterface<rs2::video_frame> {

	cv::dnn::Net net;
	char* class_names;

	CaffeModelImpl(std::string prototxt_path, std::string caffemodel_path, std::string class_names_path)
	{
		// TODO Implement constructor and loading
		std::cout << "Constructing a caffe model impl\n";
		// net = cv::dnn::readNetFromCaffe(prototxt_path, caffemodel_path);
		// Read class names from file
	}

	ClassificationResult do_work(rs2::video_frame frames) override {
		// TODO Implement classification and what else is needed
		std::cout << "Doing some work using caffe model with rgb frame\n";
		return ClassificationResult();
	}

};
