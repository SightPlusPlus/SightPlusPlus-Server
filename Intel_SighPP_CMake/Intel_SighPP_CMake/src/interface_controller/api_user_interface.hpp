#pragma once

#include "../classification_result.hpp"

/// <summary>
/// Interface that is used to send items to output interfaces.
/// </summary>
struct ApiUserInterface
{
	virtual ~ApiUserInterface() = default;

	/// <summary>
	/// Sends the classification item to the implementation's users.
	/// </summary>
	/// <param name="item">Item to send.</param>
	virtual void new_item(ClassificationItem item) = 0;

};