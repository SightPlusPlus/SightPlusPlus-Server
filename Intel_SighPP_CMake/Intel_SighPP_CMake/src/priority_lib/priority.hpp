// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2020 Sight++. All Rights Reserved.

#pragma once

enum class Priority
{
	UNDEFINED = 0,
	LOW = 1,
	MEDIUM = 2,
	HIGH = 3,
	URGENT = 4
};


enum class Location
{
	CENTRE = 0,
	IN_LEFT = 1,
	IN_RIGHT = 2,
	OUT_LEFT = 3,
	OUT_RIGHT = 4
};

enum class Height
{
	CENTRE = 0,
	ABOVE = 1,
	BELOW = 2
};


struct two_deci
{
	static std::string to_string_precise(double& d) {
		std::ostringstream streamObj;
		//Add double to stream
		streamObj << std::setprecision(2);
		streamObj << d;
		// Get string from output string stream
		std::string strObj = streamObj.str();
		return strObj;
	}
};

