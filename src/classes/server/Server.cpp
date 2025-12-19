# include <Server.hpp>

void	Server::setServerSocket(int socket_fd) {
	this->server_socket = socket_fd;
}

int		Server::getServerSocket() {
	return this->server_socket;
}