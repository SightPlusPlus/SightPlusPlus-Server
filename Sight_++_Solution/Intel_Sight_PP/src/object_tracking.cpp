#include <iostream>
#include <string>
#include <vector>
#include <opencv2/tracking.hpp>
#include "classification_result.hpp"

struct TrackingItem
{
	std::string name;
	std::vector<double> distance;
	std::vector<cv::Rect2d> rec;
	cv::Ptr<cv::Tracker> tracker;
	int counter;
	int track_point;
	bool lock;
	double speed;
	int last_seen;

	TrackingItem(const std::string name, const std::vector<double> distance, const std::vector<cv::Rect2d> rec, const int counter, const int track_point, const bool lock, const double speed,const int last_seen, const cv::Mat color_matrix, const cv::Rect2d init_rec) :
		name(name), distance(distance), rec(rec),  counter(counter), track_point(track_point), lock(lock), speed(speed), last_seen(last_seen)
	{
		tracker = cv::TrackerMOSSE::create();
		tracker->init(color_matrix, init_rec);
	}

};
struct ObjectTracking {
	std::vector<TrackingItem> items;
	const int track_frame = 5;
	std::vector<cv::Rect2d> update_items;
	std::vector<double> frame_time;
	std::vector<ClassificationItem> result_objects;

	void frame_update(cv::Mat color_matrix) {
		update_items.clear();
		for (int i = 0; i < items.size(); i++) {
			cv::Rect2d update_rec;
			items[i].tracker->update(color_matrix,update_rec);
			update_items.push_back(update_rec);
		}
	}
	std::vector<ClassificationItem> post_process(double stamp) {
		result_objects.clear();
		for (int i = 0; i < items.size(); i++) {
			if (!items[i].lock) {
				items[i].track_point--;
				if (items[i].track_point <= 0) {
					items.erase(items.begin() + i);
				}
			}
			else {
				items[i].lock = false;
			}
		}
		frame_time.push_back(stamp);
		if (frame_time.size() > track_frame+1) {
			frame_time.erase(frame_time.begin());
		}
		for (int i = 0; i < items.size(); i++) {
			if (items[i].track_point ==5 && items[i].distance.size() >= 2) {
				int distance_size = items[i].distance.size();
				int time_size = frame_time.size();
				items[i].speed = (items[i].distance[distance_size - 2]- items[i].distance[distance_size - 1]) / (frame_time[time_size - 1]- frame_time[time_size -1- items[i].last_seen]);
				items[i].last_seen = 0;
			}
			items[i].last_seen++;
		}
		for (int i = 0; i < items.size(); i++) {
			int rec_size = items[i].distance.size();
			cv::Point tl = items[i].rec[rec_size - 1].tl();
			cv::Point br = items[i].rec[rec_size - 1].br();
			point left_bottom(tl.x, tl.y);
			point right_top(br.x, br.y);
			point right_bottom(br.x, tl.y);
			point left_top(tl.x, br.y);
			int distance_size = items[i].distance.size();
			ClassificationItem new_result(items[i].name, items[i].distance[distance_size -1], left_bottom, right_top, right_bottom, left_top, items[i].counter, items[i].track_point, items[i].speed);
			result_objects.push_back(new_result);
		}
		return result_objects;
	}
	void object_check(const cv::Mat color_matrix, cv::Rect2d object_rec,std::string name, double distance) {
		bool add_object = true;

		for (int i = 0; i < update_items.size(); i++) {
			cv::Rect overlap = static_cast<cv::Rect2d>(object_rec) & update_items[i];
			if (overlap.area() > ((update_items[i].area() + object_rec.area()) / 4)
				&& name == items[i].name
				&& items[i].lock == false) {
					add_object = false;
					items[i].counter++;
					items[i].track_point = 5;
					items[i].lock = true;
					items[i].distance.push_back(distance);
					if (items[i].distance.size() > track_frame) {
						items[i].distance.erase(items[i].distance.begin());
					}
					items[i].rec.push_back(object_rec);
					if (items[i].rec.size() > track_frame) {
						items[i].rec.erase(items[i].rec.begin());
					}
					break;
			}
			
		}
		if (add_object) {
			std::vector<double> current_distance;
			std::vector<cv::Rect2d> current_rec;
			current_distance.push_back(distance);
			current_rec.push_back(object_rec);
			TrackingItem new_object(name,current_distance,current_rec,1,5,true,0,0,color_matrix, object_rec);
			items.push_back(new_object);
			
		}

	}

};