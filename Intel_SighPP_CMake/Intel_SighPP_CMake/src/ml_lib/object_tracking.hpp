#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <opencv2/tracking.hpp>
#include "../classification_result.hpp"
struct TrackingItem
{
	std::string name;
	std::vector<double> distance;
	std::vector<cv::Rect2d> rec;
	cv::Ptr<cv::Tracker> tracker;
	int id;
	double confidence;
	int counter;
	int track_point;
	bool lock;
	double speed;
	// the object is last seen X frames before 
	int last_seen;

	TrackingItem(const std::string name, const std::vector<double> distance, const std::vector<cv::Rect2d> rec, const int id, const double confidence, const int counter, const int track_point, const bool lock, const double speed, const int last_seen, const cv::Mat color_matrix, const cv::Rect2d init_rec) :
		name(name), distance(distance), rec(rec), counter(counter), id(id), confidence(confidence), track_point(track_point), lock(lock), speed(speed), last_seen(last_seen)
	{
		tracker = cv::TrackerBoosting::create();
		tracker->init(color_matrix, init_rec);
	}

};
struct ObjectTracking {

	std::vector<TrackingItem> items;
	const int track_frame = 5;
	std::vector<cv::Rect2d> update_items;
	std::vector<double> frame_time;
	std::vector<ClassificationItem> result_objects;

	// update the position for tracked objects based on current frame
	void frame_update(cv::Mat color_matrix);
	// remove redundant objects, calculate speed and return result dataset
	std::vector<ClassificationItem> post_process(double stamp);
	// check whether the current object is a new untracked object
	void object_check(const cv::Mat color_matrix, cv::Rect2d object_rec, std::string name, double distance, double confidence);

};