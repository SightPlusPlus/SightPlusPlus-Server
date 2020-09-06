#pragma once

#include <vector>

#include "../classification_result.hpp"
#include "ml_interface.hpp"
#include "../../cv-helpers.hpp"
#include "tbb/concurrent_vector.h"
#include "tbb/parallel_for_each.h"

#include "spdlog/spdlog.h"
#include "../setup_helper.hpp"

/// <summary>
/// This class is designed as the object to control all object recognition networks and define corresponding functions 
/// </summary>
class MLController {

	std::vector<ModelInterface*> models_;

	tbb::concurrent_vector<ClassificationResult> results_;

public:
	/// <summary>
	/// Get all objects with their classes, distance and corresponding realtive locations within the frame
	/// and clears them afterwards, so that they are not used again to avoid reusing the same result in the following frames
	/// </summary>
	std::vector<ClassificationResult> get_and_clear_results()
	{
		std::vector<ClassificationResult> to_send = { results_.begin(), results_.end() };

		// Note that the clear function below is not concurrency-safe
		results_.clear();
		results_.shrink_to_fit();

		SPDLOG_INFO("Got and cleared {} classification results", to_send.size());

		return to_send;
	}
	/// <summary>
	/// Receives new frames and pushes assign these frames to object recognition models for object dectection
	/// </summary>
	/// <param name="color_matrix">color matrix obtained from the camera</param>
 	/// <param name="depth_matrix">depth matrix obtained from the camera</param>
	void new_frames(cv::Mat color_matrix, cv::Mat depth_matrix) {

		// Run multiple object recognition models in parallel (multi-threading) to identify objects within the frame
		SPDLOG_INFO("Doing ML work with new frames");
		tbb::parallel_for_each(
			models_.begin(),
			models_.end(),
			[&](ModelInterface* model)
			{
				results_.push_back(model->do_work(color_matrix, depth_matrix));
			}
		);
	}
	/// <summary>
	/// Create a new object recognition model and add it to the list containing current object recognition models
	/// </summary>
	/// <param name="model">new object recognition model created</param>
	void add_model(ModelInterface& model)
	{
		models_.push_back(&model);
	}

	/// <summary>
	/// Gets the size of the list containing all object recognition models added so far
	/// </summary>
	/// <returns>the number of object recognition models added so far</returns>
	int model_count() const
	{
		return models_.size();
	}

};
