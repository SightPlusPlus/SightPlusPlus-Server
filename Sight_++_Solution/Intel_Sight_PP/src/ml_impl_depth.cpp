#include "ml_interface.hpp"
#include <iostream>

struct MLImplDepth : public ModelInterface<rs2::depth_frame> {

	void do_work(rs2::depth_frame frames) override {
		std::cout << "Doing some work with Depth frames\n";
	}

};