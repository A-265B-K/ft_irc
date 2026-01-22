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
		int									_userLimit; /* -1 means there is no limit */
		std::string							_password;
		std::string							_topicSetter;
		std::string							_topicTime;
        unsigned char	               		modes;

		static const std::string			flags; // "iklot"

	public:
		Channel(std::string name);
		~Channel();

		void	setTopic(Client* setter, const std::string& topic);
		void	addClient(Client *client);
		void	removeClient(Client *client);
		void	broadcast(Client *sender, const std::string& command, const std::string& message);
		std::set<Client *> getClients();
		bool	isOperator(Client *client) const;
		void	setUserLimit(int userLimit);
		void	setPassword(const std::string& password);
		std::string	getPassword() const;
		const std::string& getTopic();
		bool 	hasClient(Client *client) const;
		const std::string& getName();
		void	sendTopic(Client *recipient);
		bool	topicRequiresOperator();

        // 1 mode set, 0 mode unset, -1 mode unrecognized
        int         isMode(char) const;
        void        setMode(char);
        void        unsetMode(char);
        std::string getChannelModes(Client*) const;
		int			editModes(std::string&, const std::string&,
				std::deque<std::string>::const_iterator&,
				const std::deque<std::string>::const_iterator&);
};

#endif
