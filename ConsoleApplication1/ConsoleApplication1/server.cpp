#include "stdafx.h"
#include "server.h"
#include "boost/timer/timer.hpp"
#include <cstdio>
#include <boost/asio.hpp>
#include <boost/chrono.hpp>
#include <ctime>


using namespace std;

bool server::startConnection()
{
	bool ret = true;
	server_acceptor->non_blocking(true);
	boost::system::error_code error;
	do
	{
		server_acceptor->accept(*socket_forServer, error);
	} while ((error.value() == WSAEWOULDBLOCK));
	if (error)
	{
		std::cout << "Error while trying to listen to " << PORT << "Port " << error.message() << std::endl;
		ret = false;
	}
	socket_forServer->non_blocking(true);
	return ret;
}

char * server:: recieve_message()
{
	boost::system::error_code error;
	char buf[512];
	size_t len = 0;
	do
	{
		len = socket_forServer->read_some(boost::asio::buffer(buf), error);
		if (!error)
		{
			buf[len] = '\0';
		}
	}
	while (error.value() == WSAEWOULDBLOCK);
	if (!error)
	{
		return &buf[0];
	}
	else
	{
		cout << "Error while trying to connect to client. " << error.message() << std::endl;
		return NULL;
	}
			
}

void server:: send_message(FILE * file,char * path)
{
	char date[80];
	char date2[80];
	char * html;
	long lSize;
	time_t rawtime;
	struct tm *  timeinfo,* timeinfo2;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	timeinfo2 = localtime(&rawtime);
	timeinfo2->tm_min += 1;
	strftime(date, 80, "%a, %d %b %Y %X %Z\r\n", timeinfo);
	strftime(date2, 80, "%a, %d %b %Y %X %Z\r\n", timeinfo2);
	fseek(file, 0, SEEK_END);
	lSize = ftell(file);
	rewind(file);
	html = (char*)calloc(lSize,1);
	
	
	string line1 = "HTTP/1.1 200 OK\r\n";		//respuesta del server si sale todo bien
	string line2 = "Date:";					//Tue, 04 Sep 2018 18:21:19 GMT
	string line2a = date;
	string line3 = "Location:";
	string line3a(path);
	string line4 = "\r\nCache-Control: max-age=30\r\n";
	string line5 = "Expires:";
	string line5a = date2;
	string line6 = "Content length:";
	string line6a = "2KB \r\n";					//PONER ACA EL SIZE
	string line7 = "Content-Type: text/html; charset=iso-8859-1\r\n";
	fread(html, lSize, 1, file);
	

	string pagina = html;

	string str = line1 + line2 + line2a + line3 + line4 + line5 + line5a + line6 + line7 + pagina;

	char * buf = new char [str.size() + 1];
	strcpy(buf, str.c_str());

	size_t len = 0;
	boost::system::error_code error;

	do
	{
		len += socket_forServer->write_some(boost::asio::buffer(buf, strlen(buf)), error);
	} while ((error.value() == WSAEWOULDBLOCK) && len < strlen(buf));
	if (error)
	{
		std::cout << "Error while trying to connect to server " << error.message() << std::endl;
	}
}

void server:: send_404()
{
	char date[80];
	char date2[80];
	time_t rawtime;
	struct tm *  timeinfo;
	struct tm * timeinfo2;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	timeinfo2 = timeinfo;
	timeinfo2->tm_min += 1;
	strftime(date, 80, "%a, %d %b %Y %X %Z\r\n", timeinfo);
	strftime(date2, 80, "%a, %d %b %Y %X %Z\r\n", timeinfo2);
	

	string line1 = "HTTP/1.1 404 Not Found\r\n";		//respuesta del server si sale mal
	string line2 = "Date:";								//Tue, 04 Sep 2018 18:21:19 GMT
	string line2a = date;
	string line4 = "Cache-Control: max-age=30\r\n";
	string line5 = "Expires:";
	string line5a = date2;
	string line6 = "Content length: 0\r\n";

	boost::system::error_code error;

	string str = line1 + line2 + line2a + line4 + line5 + line5a + line6;

	char * buf = new char[str.size() + 1];
	strcpy(buf, str.c_str());

	size_t len = 0;

	do
	{
		len += socket_forServer->write_some(boost::asio::buffer(buf, strlen(buf)), error);
	} while ((error.value() == WSAEWOULDBLOCK) && len < strlen(buf));
	if (error)
	{
		std::cout << "Error while trying to connect to server " << error.message() << std::endl;
	}
}

server:: server()
{
	IO_handler = new boost::asio::io_service();
	boost::asio::ip::tcp::endpoint ep(boost::asio::ip::tcp::v4(), PORT);

	socket_forServer = new boost::asio::ip::tcp::socket(*IO_handler);
	server_acceptor = new boost::asio::ip::tcp::acceptor(*IO_handler, ep);
}

server:: ~server()
{
	server_acceptor->close();
	socket_forServer->close();
	delete server_acceptor;
	delete socket_forServer;
	delete IO_handler;
}
