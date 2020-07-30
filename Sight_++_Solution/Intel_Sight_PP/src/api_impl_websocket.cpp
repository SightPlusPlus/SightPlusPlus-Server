#include <set>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include "api_user_interface.hpp"

class BroadcastServer
{
	std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> connections_;
	websocketpp::server<websocketpp::config::asio> server_;

public:
	
	BroadcastServer()
	{
		std::cout << "Creating BroadcastServer" << std::endl;
		server_.init_asio();

		server_.set_open_handler(websocketpp::lib::bind(&BroadcastServer::on_open, this, websocketpp::lib::placeholders::_1));
		server_.set_close_handler(websocketpp::lib::bind(&BroadcastServer::on_close, this, websocketpp::lib::placeholders::_1));
		server_.set_message_handler(websocketpp::lib::bind(&BroadcastServer::on_message, this, websocketpp::lib::placeholders::_1, websocketpp::lib::placeholders::_2));
		std::cout << "Created BroadcastServer" << std::endl;
	}
	
	void run()
	{
		std::cout << "Running BroadcastServer" << std::endl;
		server_.listen(7979);
		server_.start_accept();
		server_.run();
		std::cout << "Running BroadcastServer" << std::endl;
	}

	void on_open(const websocketpp::connection_hdl handle) {
		connections_.insert(handle);
	}
	
	void on_close(const websocketpp::connection_hdl handle) {
		connections_.erase(handle);
	}
	
	void on_message(const websocketpp::connection_hdl handle, const websocketpp::server<websocketpp::config::asio>::message_ptr message) {
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

struct ApiWebSocketImpl : ApiUserInterface
{
	BroadcastServer server;

	ApiWebSocketImpl()
	{
		websocketpp::lib::thread([s = &server] { s->run(); }).detach();		
	}
	
	void new_item(ClassificationItem item) override
	{
		server.send(item.to_string());		
	}
};
