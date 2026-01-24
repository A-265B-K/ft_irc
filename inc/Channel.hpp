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
		std::string							_key;
		int									_userLimit;
		std::string							_topic;
		std::string							_topicSetter;
		std::string							_topicTime;
        unsigned char	               		_modes;

		static const std::string			flags; // "iklot"

	public:
		Channel(std::string name);
		~Channel();

		void	addClient(Client *client);
		void	removeClient(Client *client);
		void	broadcast(Client *sender, const std::string& command, const std::string& message);
		std::set<Client *> getClients();
		const std::string& getName();

		bool	verifyKey(const std::string&) const;
		bool	isInviteOnly() const;

		bool	isOperator(Client *client) const;
		bool 	hasClient(Client *client) const;

		bool	topicRequiresOperator();
		const std::string& getTopic();
		void	sendTopic(Client *recipient);
		void	setTopic(Client* setter, const std::string& topic);

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
