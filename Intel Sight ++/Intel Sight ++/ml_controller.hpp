#pragma once

#include <vector>
#include "ml_interface.hpp"

class MLController {

	std::vector<ModelInterface<rs2::video_frame>*> rgb_models;
	std::vector<ModelInterface<rs2::depth_frame>*> depth_models;

	/* concurrent? List of result objects?
		Map<frame_number?, list of result objects>
	*/

public:

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

		for (auto& rgb_model : rgb_models) {
			rgb_model->do_work(frameset.get_color_frame());
		}

		for (auto& depth_model : depth_models) {
			depth_model->do_work(frameset.get_depth_frame());
		}

	}

	void add_rgb_model(ModelInterface<rs2::video_frame>& model) {
		rgb_models.push_back(&model);
	}

	void add_depth_model(ModelInterface<rs2::depth_frame>& model) {
		depth_models.push_back(&model);
	}

};