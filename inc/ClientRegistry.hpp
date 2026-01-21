#ifndef CLIENTREGISTRY_HPP
# define CLIENTREGISTRY_HPP

# include <map>
# include <string>
# include <Client.hpp>
# include <User.hpp>

class ClientRegistry {
	private:
		std::map<int, Client *>	clients; // potentially change to reference
	public:
		int		addClient(int serverSocket);
		Client	*getClientBySocket(int clientSocket);
		void	removeClient(int clientSocket);
		Client	*getClientByNick(std::string nickname);
};

#endif