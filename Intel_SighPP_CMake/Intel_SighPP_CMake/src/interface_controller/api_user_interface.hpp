#pragma once

#include "../classification_result.hpp"

struct ApiUserInterface
{
	virtual ~ApiUserInterface() = default;

	virtual void new_item(ClassificationItem item) = 0;
};