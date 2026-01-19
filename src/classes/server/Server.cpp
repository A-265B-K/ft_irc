# include <Server.hpp>

const std::string& Server::getName() const {
	return name;
}

int		Server::getServerSocket() const {
	return this->server_socket;
}

const std::string&	Server::getPassword() const  {
	return this->password;
}