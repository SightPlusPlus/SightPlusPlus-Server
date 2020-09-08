#include <iostream>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/dnn.hpp>
#include <iomanip>
#include "spdlog/spdlog.h"
#include "ml_interface.hpp"
#include "model_helper.hpp"
#include <opencv2/imgproc.hpp>

#include "tbb/concurrent_vector.h"
#include "tbb/parallel_for_each.h"
#include "../classification_result.hpp"

// Based on the rs-dnn example

struct CaffeModelImpl : public ModelInterface {

	cv::dnn::Net net;
	std::vector<std::string> class_names;

	CaffeModelImpl(std::string prototxt_path, std::string caffemodel_path, const std::string class_names_path)
	{
		SPDLOG_INFO("Constructing a caffe model impl, using {}, {}, {}", prototxt_path, caffemodel_path, class_names_path);

		net = cv::dnn::readNetFromCaffe(prototxt_path, caffemodel_path);
		class_names = read_class_name_file(class_names_path);
	}

	// This is the same as the rs-dnn example
	ClassificationResult do_work(cv::Mat color_matrix, cv::Mat depth_matrix) override {

		SPDLOG_INFO("Using Caffe model to find objects");

		// TODO Should this input blob be "standardised" and calculated in ml-controller
		auto input_blob = cv::dnn::blobFromImage(color_matrix, inScaleFactor, cv::Size(inWidth, inHeight), meanVal, false);
		net.setInput(input_blob, "data");

		net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
		// Use configuration to speed up the net.forward
		net.setPreferableTarget(cv::dnn::DNN_TARGET_OPENCL);

		// Use net.forwardAsync instead?
		auto detection = net.forward("detection_out");

		cv::Mat detection_matrix(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());

		ClassificationResult classification_result("caffe");
		std::vector<int> object_id;
		for (int i = 0; i < detection_matrix.rows; i++)
		{
			object_id.push_back(i);
		}

		tbb::parallel_for_each(
			object_id.begin(),
			object_id.end(),
			[&](int id)
			{
				auto confidence = detection_matrix.at<float>(id, 2);

				if (confidence > confidence_threshold)
				{
					auto object_class = static_cast<size_t>(detection_matrix.at<float>(id, 1));

					auto x_left_bottom = static_cast<int>(detection_matrix.at<float>(id, 3) * color_matrix.cols);
					auto y_left_bottom = static_cast<int>(detection_matrix.at<float>(id, 4) * color_matrix.rows);

					auto x_right_top = static_cast<int>(detection_matrix.at<float>(id, 5) * color_matrix.cols);
					auto y_right_top = static_cast<int>(detection_matrix.at<float>(id, 6) * color_matrix.rows);

					cv::Rect object(
						static_cast<int>(x_left_bottom), static_cast<int>(y_left_bottom),
						static_cast<int>(x_right_top - x_left_bottom), static_cast<int>(y_right_top - y_left_bottom));
					object = object & cv::Rect(0, 0, depth_matrix.cols, depth_matrix.rows);
					// Calculate valid depth inside the detection region
					cv::Mat object_depth = depth_matrix(object);

					cv::Mat pixel_array;

					object_depth.convertTo(pixel_array, CV_32F);
					pixel_array = pixel_array.reshape(1, pixel_array.total());
					cv::Mat label, centers;

					kmeans(pixel_array, 2, label,
						cv::TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 10, 0.1), 1,
						cv::KMEANS_PP_CENTERS, centers);


					// replace pixel values with their center value:
					float* p = pixel_array.ptr<float>();
					int group_zero = 0;
					int group_one = 0;
					for (size_t i = 0; i < pixel_array.rows; i++) {
						int center_id = label.at<int>(i);
						if (center_id == 0) {
							group_zero++;

						}
						else if (center_id == 1) {
							group_one++;
						}
						p[i] = centers.at<float>(center_id);
					}
					float distance = 0;
					if (group_zero > group_one) {
						distance = centers.at<float>(0);
					}
					else {
						distance = centers.at<float>(1);
					}

					point left_bottom(x_left_bottom, y_left_bottom);
					point right_top(x_right_top, y_right_top);
					classification_result.objects.emplace_back(class_names[object_class], distance, left_bottom, right_top);
				}
			}
		);

		return classification_result;
	}

	void set_resolution(size_t width, size_t height) override {
		inWidth = width;
		inHeight = height;
	}

	void set_confidence_threshold(float threshold) override {
		confidence_threshold = threshold;
	}
};
