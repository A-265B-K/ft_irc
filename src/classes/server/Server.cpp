# include <Server.hpp>

void	Server::setPassword(std::string password) {
	this->password = password;
}

void	Server::setServerSocket(int socket_fd) {
	this->server_socket = socket_fd;
}

int		Server::getServerSocket() {
	return this->server_socket;
}
