#pragma once

/// <summary>
/// Create a caffe-based network given the files name
/// </summary>
/// <param name="name"></param>
CaffeModelImpl create_caffe_network(std::string name)
{
	std::string prototxt_path_ = "./models/" + name + ".prototxt";
	std::string caffemodel_path_ = "./models/" + name + ".caffemodel";
	std::string txt_path_ = "./models/" + name + ".txt";
	CaffeModelImpl caffe_model(prototxt_path_, caffemodel_path_, txt_path_);
	return caffe_model;
};

/// <summary>
/// Create a yolo network given the files name
/// </summary>
/// <param name="name"></param>
YoloModelImpl create_yolo_network(std::string name)
{
	std::string cfg_path_ = "./models/" + name + ".cfg";
	std::string weights_path_ = "./models/" + name + ".weights";
	std::string label_path_ = "./models/" + name + ".txt";
	YoloModelImpl yolo_model(cfg_path_, weights_path_, label_path_);
	return yolo_model;
};