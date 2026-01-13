# include <Server.hpp>

Server::Server(uint16_t port, const std::string& password): port(port), password(password)
{
	::printMessage("Starting up the server");
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

void Server::setToPassive()
{
	std::ostringstream output;

	address.sin_family = AF_INET;
	address.sin_port = this->port;
	address.sin_addr.s_addr = HOST_IP_ADDRESS;

	bind(server_socket, (struct sockaddr*)&address, sizeof(address));
	output << "Binded to address " << inet_ntoa(address.sin_addr);
	::printMessage(output.str());
	output.str("");

	listen(server_socket, SOMAXCONN);
	output << "Listening to port " << ntohs(this->port) << "...";
	::printMessage(output.str());
}


Server::~Server() {
	::printMessage("Server is shutting down");
}
