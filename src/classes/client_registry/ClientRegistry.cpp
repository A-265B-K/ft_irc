# include <ClientRegistry.hpp>

# include <arpa/inet.h>
# include <sys/socket.h>
# include <string>

int		ClientRegistry::addClient(int server_socket) {
	char buffer[256];
	socklen_t len = 256;

	const int client_socket = accept(server_socket, (struct sockaddr*)buffer,
			&len);
	const std::string addr(inet_ntoa(((struct sockaddr_in*)buffer)->sin_addr));

	std::string text("Incoming connection from ");
	text +=	addr;
	text += client_socket == -1 ? " FAILED" : " accepted";
	::printMessage(text);
	if (client_socket == -1)
		return -1;

	Client *client = new Client(client_socket, addr);
	this->clients.insert( {client_socket, client} );

	return client_socket;
}

void	ClientRegistry::removeClient(int clientSocket) {
	Client *c = getClientBySocket(clientSocket);
	this->clients.erase(clientSocket);
	delete c;
}

Client	*ClientRegistry::getClientBySocket(int client_socket) {
	return this->clients[client_socket];
}

Client	*ClientRegistry::getRegisteredClientByNick(std::string nickname) {
	for (auto const& x : this->clients){
		User *user = x.second->getUser();
		if (user) {
			if (nickname == user->getNickname() && user->isRegistered()) {
				return user;
			}
		}
	}
	return nullptr;
}
