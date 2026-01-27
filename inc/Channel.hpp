#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <deque>
# include <map>
# include <set>
# include <string>

# include <Client.hpp>
# include <static_declarations.hpp>

class Client;

class Channel {
	private:
		std::string					_name;
		std::map<Client *, int>		_clients; // int 0 = users, 1 = operator, 2 = owner
		std::string					_key;
		int							_userLimit; /* -1 means there is no limit */
		std::string					_topic;
		std::string					_topicSetter;
		std::string					_topicTime;
        unsigned char	           	_modes;

		static const std::string	flags; // "iklot"

		std::map<Client *,int>::const_iterator getClientByNick(const std::string&) const;

        // 1 mode set, 0 mode unset, -1 mode unrecognized
        int		isMode(const unsigned char&, char) const;
        int		isMode(char) const;
        void	setMode(unsigned char&, char);
        void	setMode(char);
        void	unsetMode(unsigned char &, char);
        void	unsetMode(char);
		std::string	printModes(const unsigned char&) const;
		

	public:
		Channel(std::string name);
		~Channel();

		void	addClient(Client *client);
		void	removeClient(Client *client);
		void	broadcast(const Client *sender, const std::string& command,
					const std::string& message) const;
		std::set<Client *> getClients();
		int		memberCount();

		const std::string& getName() const;

		bool	verifyKey(const std::string&) const;
		bool	isInviteOnly() const;

		bool	isOperator(Client *client) const;
		bool 	hasClient(Client *client) const;

		bool	topicRequiresOperator() const;
		const std::string& getTopic() const;
		void	sendTopic(Client *recipient) const;
		void	setTopic(Client* setter, const std::string& topic);

        std::string getChannelModes(Client*) const;
		unsigned	editModes(std::string&, std::set<std::pair<char, std::string> >&,
				std::set<std::string>&, const std::string&,
				std::deque<std::string>::const_iterator&,
				const std::deque<std::string>::const_iterator&);

		static bool	isValidKey(const std::string&);
};

#endif
