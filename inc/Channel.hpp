#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <map>
# include <set>
# include <Client.hpp>

class Client;

class Channel {
	private:
		std::string							_name;
		std::map<Client *, int>				_clients; // int 0 = users, 1 = operator, 2 = owner
		std::string							_topic;
		std::map<char, bool>				modes;
		int									_userLimit; /* -1 means there is no limit */
		std::string							_password;
		std::string							_topicSetter;
		std::string							_topicTime;

	public:
		Channel(std::string name);
		~Channel();

		void	setTopic(Client* setter, const std::string& topic);
		void	addClient(Client *client);
		void	removeClient(Client *client);
		void	broadcast(Client *sender, const std::string& command, const std::string& message);
		std::set<Client *> getClients();
		bool	isOperator(Client *client);
		void	setUserLimit(int userLimit);
		void	setPassword(const std::string& password);
		std::string	getPassword() const;
		const std::string& getTopic();
		bool 	isClientOn(Client *client);
		const std::string& getName();
		void	sendTopic(Client *recipient);
		bool	topicRequiresOperator();
};

#endif