#ifndef CHANNELREGISTRY_HPP
# define CHANNELREGISTRY_HPP

# include <Channel.hpp>
# include <map>

class ChannelRegistry {
	private:
		std::map<std::string, Channel *>	_channels;


	public:
		int 	isValidChannelName(const std::string& channelName);
		int		joinChannel(const std::string& channelName, Client *client, std::string key);
		int		partChannel(const std::string& channelName, Client *client);
		Channel	*getChannel(const std::string& channelName);
		void	removeClient(Client *client);
		std::deque<Channel *> getClientChannels(Client *client);
};

#endif
