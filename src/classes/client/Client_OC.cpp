# include <Client.hpp>

Client::Client(int clientSocket, const std::string& address) : _clientSocket(clientSocket), _address(address), _user(new User(this)), _requestedDisconnect(false) {
}

Client::~Client() {
	close(_clientSocket);
	delete _user;
	std::cout << RED << "CLIENT DELETED" << RESET << std::endl;
}
