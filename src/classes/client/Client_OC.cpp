# include <Client.hpp>

Client::Client(int client_socket, const std::string& address) : client_socket(client_socket), address(address), user(new User(this)) {
}

Client::~Client() {
	delete user;
}
