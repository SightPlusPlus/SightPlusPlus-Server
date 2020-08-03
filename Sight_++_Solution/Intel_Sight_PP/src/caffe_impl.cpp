#include <iostream>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/dnn.hpp>
#include <iomanip>

#include "ml_interface.hpp"
#include "model_helper.hpp"
#include <opencv2\imgproc.hpp>

#include "classification_result.hpp"

// Based on the rs-dnn example

struct CaffeModelImpl : public ModelInterface {

	cv::dnn::Net net;
	std::vector<std::string> class_names;
	
	const size_t inWidth = 640;
	const size_t inHeight = 480;
	const float WHRatio = inWidth / (float)inHeight;
	const float inScaleFactor = 0.007843f;
	const float meanVal = 127.5;

	const float confidence_threshold = 0.8f;
	
	CaffeModelImpl(std::string prototxt_path, std::string caffemodel_path, const std::string class_names_path)
	{
		std::cout << "Constructing a caffe model impl\n";
		
		net = cv::dnn::readNetFromCaffe(prototxt_path, caffemodel_path);
		class_names = read_class_name_file(class_names_path);

	}

	// This is the same as the rs-dnn example
	ClassificationResult do_work(cv::Mat color_matrix, cv::Mat depth_matrix) override {

		std::cout << "Doing caffe impl work" << std::endl;
		
		// TODO Should this input blob be "standardised" and calculated in ml-controller
		auto input_blob = cv::dnn::blobFromImage(color_matrix, inScaleFactor, cv::Size(inWidth, inHeight), meanVal, false);
		net.setInput(input_blob, "data");

		// Use net.forwardAsync instead?
		auto detection = net.forward("detection_out");

		cv::Mat detection_matrix(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());

		ClassificationResult classification_result("caffe");

		std::cout << "Classified, foreaching" << std::endl;
		
		for(auto i = 0; i < detection_matrix.rows; i++)
		{
			auto confidence = detection_matrix.at<float>(i, 2);

			if (confidence > confidence_threshold)
			{
				auto object_class = static_cast<size_t>(detection_matrix.at<float>(i, 1));

				auto x_left_bottom = static_cast<int>(detection_matrix.at<float>(i, 3) * color_matrix.cols);
				auto y_left_bottom = static_cast<int>(detection_matrix.at<float>(i, 4) * color_matrix.rows);

				auto x_right_top = static_cast<int>(detection_matrix.at<float>(i, 5) * color_matrix.cols);
				auto y_right_top = static_cast<int>(detection_matrix.at<float>(i, 6) * color_matrix.rows);

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
		
		return classification_result;
	}
};
