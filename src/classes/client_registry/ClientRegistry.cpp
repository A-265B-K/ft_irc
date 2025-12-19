# include <ClientRegistry.hpp>

# include <sys/socket.h>
# include <iostream>
# include <string>

int		ClientRegistry::addClient(int server_socket) {
	int client_socket = accept(server_socket, nullptr, nullptr);
	if (client_socket == -1) {
		std::cout << "ACCEPT failed" << std::endl;
		return -1;
	}
	Client *client = new Client(server_socket);
	this->clients.insert( {client_socket, client} );

	return client_socket;
}