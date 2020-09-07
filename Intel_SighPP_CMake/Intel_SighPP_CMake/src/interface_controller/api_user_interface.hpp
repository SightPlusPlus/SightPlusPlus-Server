#pragma once

#include "prioritiser_update_instructions.hpp"
#include "../classification_result.hpp"
#include <spdlog/spdlog.h>

/// <summary>
/// Interface that is used to send items to output interfaces.
/// </summary>
struct ApiUserInterface
{
	std::vector<PrioritiserUpdateInstruction> instructions;
	
	virtual ~ApiUserInterface() = default;

	/// <summary>
	/// Sends the classification item to the implementation's users.
	/// </summary>
	/// <param name="item">Item to send.</param>
	virtual void new_item(ClassificationItem item) = 0;

	/// <summary>
	/// Gets all update instructions from the API implementation
	/// and clears the list (to ensure instructions are not reapplied).
	/// Populating the instruction vector is optional, because user to service communication is optional.
	/// </summary>
	/// <returns>Vector of setting changes for the prioritiser</returns>
	std::vector<PrioritiserUpdateInstruction> get_and_clear_prioritiser_update_instructions()
	{
		std::vector<PrioritiserUpdateInstruction> to_send = { instructions.begin(), instructions.end() };

		// Note that the clear function below is not concurrency-safe
		instructions.clear();
		instructions.shrink_to_fit();

		SPDLOG_INFO("Got and cleared {} prioritiser update instructions", to_send.size());

		return to_send;		
	};

};