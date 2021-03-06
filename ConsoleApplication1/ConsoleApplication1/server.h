#include "stdafx.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

#define PORT 80
#define CR 13
#define LF 10

class server
{
public:
	server();
	bool startConnection();
	void send_message(FILE*,char*);
	void send_404();
	char * recieve_message();
	~server();

private:
	boost::asio::io_service*  IO_handler;
	boost::asio::ip::tcp::socket* socket_forServer;
	boost::asio::ip::tcp::acceptor* server_acceptor;
};
