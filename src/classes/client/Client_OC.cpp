# include <Client.hpp>

Client::Client(int client_socket, const std::string& address) {
	this->client_socket = client_socket;
	this->address = address;
}
