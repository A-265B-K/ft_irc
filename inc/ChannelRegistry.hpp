#ifndef CHANNELREGISTRY_HPP
# define CHANNELREGISTRY_HPP

# include <Channel.hpp>
# include <map>

class ChannelRegistry {
	private:
		std::map<std::string, Channel *>	channels;


	public:
		int isValidChannelName(const std::string& channelName);
		int		joinChannel(const std::string& channelName, Client *client, std::string password);
		int		partChannel(const std::string& channelName, Client *client);
		Channel	*getChannel(const std::string& channelName);
};

#endif