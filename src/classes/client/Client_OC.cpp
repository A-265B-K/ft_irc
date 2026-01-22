# include <Client.hpp>

Client::Client(int clientSocket, const std::string& address) : _clientSocket(clientSocket), _address(address), _user(new User(this)), _requestedDisconnect(false) {
	std::cout << BLUE << "CLIENT CREATED" << RESET << std::endl;
}

Client::~Client() {
	std::cout << BLUE << "CLIENT DELETED" << RESET << std::endl;
	close(_clientSocket);
	delete _user;
}
