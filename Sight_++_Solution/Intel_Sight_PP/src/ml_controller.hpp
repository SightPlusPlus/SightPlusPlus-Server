#pragma once

#include <vector>

#include "classification_result.hpp"
#include "ml_interface.hpp"
#include "tbb/concurrent_vector.h"
#include "tbb/parallel_for_each.h"

class MLController {

	std::vector<ModelInterface<rs2::video_frame>*> rgb_models;
	std::vector<ModelInterface<rs2::depth_frame>*> depth_models;

	/* concurrent? List of result objects?
		Map<frame_number?, list of result objects>
	*/
	tbb::concurrent_vector<ClassificationResult> results_;

public:

	// Get's the results and clears them, so that they are not used again
    std::vector<ClassificationResult> get_and_clear_results()
    {
    	std::vector<ClassificationResult> to_send = {results_.begin(), results_.end()};

    	// TODO May not be a good idea, clear is not concurrency-safe
		results_.clear();
		results_.shrink_to_fit();
    	
		return to_send;
    }
	
	void new_frames(rs2::frameset& frameset) {

		// TODO Precalculate openCV Matrix?
		// TODO Concurrency? Fire-and-forget?
		/* TODO How to use results of do_work? So it can be called by prioritiser
			 List of result-object classes that all the do_work function calls add to?
			 Something like
				vector<ResultObject> results // or some concurrent list thingy
				for model : models
					results.add(model.do_work(data))
		*/

		// TODO Time testing, make the do_work perform some work(thread.wait?) and check how long tbb::parallel_for_each takes vs a regular for

		tbb::parallel_for_each(
			rgb_models.begin(),
			rgb_models.end(),
			[&] (ModelInterface<rs2::video_frame>* model)
			{
				std::cout << "Doing parallel rgb foreach\n";
				results_.push_back(model->do_work(frameset.get_color_frame()));
			}
		);

		tbb::parallel_for_each(
			depth_models.begin(),
			depth_models.end(),
			[&](ModelInterface<rs2::depth_frame>* model)
			{
				std::cout << "Doing parallel depth foreach\n";
				results_.push_back(model->do_work(frameset.get_depth_frame()));
			}
		);

		//for (auto& rgb_model : rgb_models) {
		//	std::cout << "Doing regular rgb foreach";
		//	rgb_model->do_work(frameset.get_color_frame());
		//}
		//
		//for (auto& depth_model : depth_models) {
		//	std::cout << "Doing regular depth foreach";
		//	depth_model->do_work(frameset.get_depth_frame());
		//}

	}

	void add_rgb_model(ModelInterface<rs2::video_frame>& model) {
		rgb_models.push_back(&model);
	}

	void add_depth_model(ModelInterface<rs2::depth_frame>& model) {
		depth_models.push_back(&model);
	}

};
