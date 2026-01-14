#ifndef CHANNELREGISTRY_HPP
# define CHANNELREGISTRY_HPP

# include <Channel.hpp>
# include <map>

class ChannelRegistry {
	private:
		std::map<std::string, Channel *>	channels;

	public:
		void	joinChannel(std::string channelName, Client *client);
		void	partChannel(std::string channelName, Client *client);
		Channel	*getChannel(std::string channelName);
};

#endif