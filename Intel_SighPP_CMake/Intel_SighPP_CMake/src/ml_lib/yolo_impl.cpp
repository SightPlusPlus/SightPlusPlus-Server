#include <iostream>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/dnn.hpp>
#include <iomanip>
#include "tbb/concurrent_vector.h"
#include "tbb/parallel_for_each.h"
#include "ml_interface.hpp"
#include "model_helper.hpp"
#include <opencv2/imgproc.hpp>

#include "../classification_result.hpp"
using namespace std;

// Based on the internet example

struct YoloModelImpl : public ModelInterface {

	cv::dnn::Net net;
	vector<string> class_names;
	
	YoloModelImpl(string modelConfiguration, string modelWeights, const string class_names_path)
	{
		cout << "Constructing a Yolo model impl\n";
		
		net = cv::dnn::readNetFromDarknet(modelConfiguration,modelWeights);
		
		net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
		// Use configuration to speed up the net.forward
		net.setPreferableTarget(cv::dnn::DNN_TARGET_OPENCL);		
		
		class_names = read_class_name_file(class_names_path);
	}

	// This is similar to the caffe_imple example
	ClassificationResult do_work(cv::Mat color_matrix, cv::Mat depth_matrix) override {
		
		cout << "Doing Yolo impl work" << endl;
		
		cv::Mat blob;

		cv::dnn::blobFromImage(color_matrix,blob,1/255.0,cv::Size(inWidth,inHeight));

		net.setInput(blob);

		vector<cv::Mat> outs;

	    static vector<cv::String> names;
	    //Get output names from output layers
	    if(names.empty()){
	        vector<int> outLayers = net.getUnconnectedOutLayers();
	        vector<cv::String> layersNames = net.getLayerNames();

	        names.resize(outLayers.size());
	        for(size_t i =0;i<outLayers.size();i++){
	            names[i] = layersNames[outLayers[i]-1];
	        }
	    }

		net.forward(outs,names);

		ClassificationResult classification_result("Yolo");

		cout << "Classified, foreaching" << endl;

	    for(size_t i=0;i<outs.size();i++){

	        float* data = (float*)outs[i].data;
	        for(int j=0;j<outs[i].rows;j++,data+=outs[i].cols){
	            cv::Mat scores = outs[i].row(j).colRange(5,outs[i].cols);
	            cv::Point classIdPoint;
	            double confidence;

	            cv::minMaxLoc(scores,0,&confidence,0,&classIdPoint);
	            if(confidence>confidence_threshold){
	                int centerX = (int)(data[0]*color_matrix.cols);
	                int centerY = (int)(data[1]*color_matrix.rows);
	                int width = (int)(data[2]*color_matrix.cols);
	                int height = (int)(data[3]*color_matrix.rows);
	                int left = centerX-width/2;
	                int top = centerY-height/2;

	                cv::Rect object(static_cast<int>(left), static_cast<int>(top),static_cast<int>(width), static_cast<int>(height));
					object = object & cv::Rect(0, 0, depth_matrix.cols, depth_matrix.rows);
					// calculate depth (distance) using k-mean algorithms
					// simialr to the caffe_impl file
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
					point left_bottom(left,top);
					point right_top(left+width, top+height);
					classification_result.objects.emplace_back(class_names[classIdPoint.x], distance, left_bottom, right_top);
	            }

	        }
	        
	    }

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
