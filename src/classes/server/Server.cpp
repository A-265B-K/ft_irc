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

const time_t& Server::getCreationTime() const {
	return this->creationTime;
}

const int& Server::getVersion() const {
	return this->version;
}