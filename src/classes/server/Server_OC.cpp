# include <Server.hpp>
# include <cerrno>
# include <cstring>
# include <netdb.h>
# include <sstream>
# include <arpa/inet.h>
# include <static_declarations.hpp>

Server::Server(uint16_t port, const std::string& password): port(port), password(password)
{
	this->name = "ft_irc";
	this->version = 0;
	this->server_socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP); // TODO THIS CAN FAIL!! MOVE OUT OF HERE (note to self, roger if u read this image the tone of voice is calmer :) 

	std::ostringstream output;

	output << BLUE << "Starting server" << std::endl << "NAME: " << RESET << this->name << BLUE << " | VERSION: " << RESET << this->version;
	::printMessage(output.str());
}

void Server::setToPassive()
{
	std::ostringstream output;
	std::ostringstream addresses;

	int opt = 1;
	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		std::cerr << RED << "Setting socket options failed: " << std::strerror(errno) << RESET << std::endl;
		exit(1);
	}

	/* Creating a variable for the addresses to be printed later */
	char hostname[256];
	gethostname(hostname, sizeof(hostname));
	hostent* host = gethostbyname(hostname);
	for (int i = 0; host->h_addr_list[i] != nullptr; i++) {
		char* ip = inet_ntoa(*(in_addr*)host->h_addr_list[i]);
		addresses << GREEN << ip << RESET << ":" << YELLOW << ntohs(this->port) << RESET << " ";
	}

	address.sin_family = AF_INET;
	address.sin_port = this->port;
	address.sin_addr.s_addr = INADDR_ANY;

	output << BLUE << "Mask is set to " << GREEN << inet_ntoa(address.sin_addr) << RESET;
	::printMessage(output.str());
	output.str("");

	/* Bind to address, and print a descriptive message */
	output << BLUE << "Binding to addresses:" << std::endl << addresses.str();
	::printMessage(output.str());
	if (bind(server_socket, (struct sockaddr*)&address, sizeof(address)) < 0) {
		std::cerr << RED << "Bind failed: " << std::strerror(errno) << RESET << std::endl;
		exit(1);
	}
	output.str("");

	/* Set the socket to listen, and print on which IP addresses it is doing so.*/
	output << BLUE << "Listening to addresses:" << std::endl << addresses.str();
	::printMessage(output.str());
	listen(server_socket, SOMAXCONN);
}


Server::~Server() {
	::printMessage("Server is shutting down");
}
