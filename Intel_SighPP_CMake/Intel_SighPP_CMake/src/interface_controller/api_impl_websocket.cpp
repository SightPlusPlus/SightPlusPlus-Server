
#include "api_user_interface.hpp"
#include <set>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include "spdlog/spdlog.h"

struct ApiWebSocketImpl : ApiUserInterface
{
	Priority minimum_priority;

	std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> connections_;
	websocketpp::server<websocketpp::config::asio> server_;

	ApiWebSocketImpl(int port, Priority minimum_priority) : minimum_priority(minimum_priority)
	{
		SPDLOG_INFO("Creating BroadcastServer");
		server_.init_asio();
		
		server_.set_open_handler(websocketpp::lib::bind(&ApiWebSocketImpl::on_open, this, websocketpp::lib::placeholders::_1));
		server_.set_close_handler(websocketpp::lib::bind(&ApiWebSocketImpl::on_close, this, websocketpp::lib::placeholders::_1));
		server_.set_message_handler(websocketpp::lib::bind(&ApiWebSocketImpl::on_message, this, websocketpp::lib::placeholders::_1, websocketpp::lib::placeholders::_2));
		
		websocketpp::lib::thread([this, port] {run(port); }).detach();
	}

	void new_item(ClassificationItem item) override
	{
		SPDLOG_INFO("MinPriority: {} vs. ItemPriority: {}", static_cast<int>(minimum_priority), static_cast<int>(item.priority));
		if (item.priority >= minimum_priority)
		{
			SPDLOG_INFO("Sending item: {}", item.to_json());
			send(item.to_json());
		}
	}

	void run(int port)
	{
		SPDLOG_INFO("Running BroadcastServer");
		server_.listen(port);
		server_.start_accept();
		server_.run();
		SPDLOG_INFO("Running BroadcastServer");
	}

	void on_open(const websocketpp::connection_hdl handle) {
		connections_.insert(handle);
	}

	void on_close(const websocketpp::connection_hdl handle) {
		connections_.erase(handle);
	}

	void on_message(const websocketpp::connection_hdl handle, const websocketpp::server<websocketpp::config::asio>::message_ptr message) {
		// TODO Transform to relevant format
		// TODO Add to update instructions list

		for (const auto& connection : connections_)
		{
			server_.send(connection, message);
		}
	}

	void send(const std::string& message)
	{
		for (const auto& connection : connections_)
		{
			server_.send(connection, message, websocketpp::frame::opcode::TEXT);
		}
	}
};
