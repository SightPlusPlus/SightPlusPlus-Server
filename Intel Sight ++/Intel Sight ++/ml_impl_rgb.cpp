#include "ml_interface.hpp"
#include <iostream>

struct MLImplRGB : public ModelInterface<rs2::video_frame> {

	void do_work(rs2::video_frame frames) override {
		std::cout << "Doing some work with RGB frames\n";
	}

};