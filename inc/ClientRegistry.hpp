#ifndef CLIENTREGISTRY_HPP
# define CLIENTREGISTRY_HPP

# include <map>
# include <string>
# include <Client.hpp>

class ClientRegistry {
	private:
		std::map<int, Client *>	clients; // potentially change to reference
	public:
		int		addClient(int server_socket);
		Client	*getClientBySocket(int client_socket);
};

#endif