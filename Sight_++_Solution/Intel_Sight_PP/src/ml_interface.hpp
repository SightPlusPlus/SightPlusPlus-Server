#pragma once

#include "librealsense2/rs.hpp"
#include "classification_result.hpp"

template <typename T>
struct ModelInterface {

	// TODO Is void correct? Probably need to add output to concurrent list type thing in the MLController
	virtual ClassificationResult do_work(T frames) = 0;

};
