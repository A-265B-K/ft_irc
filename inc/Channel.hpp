#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <map>
# include <Client.hpp>

class Client;

class Channel {
	private:
		std::string							name;
		std::map<Client *, int>				clients; // int 0 = users, 1 = operator, 2 = owner
		std::string							topic;
		std::map<char, bool>				modes;

	public:
		Channel(std::string name);
		~Channel();

		void	setTopic(const std::string& topic);
		void	addClient(Client *client);
		void	removeClient(Client *client);

};

#endif