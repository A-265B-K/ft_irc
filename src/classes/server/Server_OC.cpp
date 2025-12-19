# include <Server.hpp>
# include <iostream>
# include <unistd.h>
# include <sys/socket.h>

Server::Server() {
	std::cout << "Starting up the server" << std::endl;
	this->name = "ft_irc";
	this->version = 0;
	this->server_socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0); // check if this can fail and set to nonblock
}

// Server::Server(std::string name) {
// 	this->name = name;
// 	this->version[0] = 0; this->version[1] = 0;
// 	this->socket = socket(AF_INET, SOCK_STREAM, SOCK_NONBLOCK);
// 	this->address.sin_family = AF_INET;
// 	this->address.sin_port = htons(8080); // forbidden funcitom
// 	this->address.sin_addr.s_addr = INADDR_ANY;
// }

Server::~Server() {
	std::cout << "Server is shutting down" << std::endl;
}