#ifndef CHANNELREGISTRY_HPP
# define CHANNELREGISTRY_HPP

# include <Channel.hpp>

# include <functional>
# include <map>
# include <utility>

class ChannelRegistry {
	private:
		std::map<std::string, Channel *>	_channels;

	public:
		int 	isValidChannelName(const std::string& channelName);
		int		joinChannel(Channel **, const std::string& channelName, Client *client, std::string key);
		int		partChannel(const std::string& channelName, Client *client);
		Channel	*getChannel(const std::string& channelName);
		void	removeClient(Client *client);
		std::deque<Channel *> getClientChannels(Client *client);
		void 	forEachChannel(const std::function
						<void (std::pair<std::string, Channel *>)>&) const;
};

#endif
