# include <ChannelRegistry.hpp>

int ChannelRegistry::isValidChannelName(const std::string& channelName) {
	if (channelName.empty()) {
		return 0;
	}
	switch(channelName.front()) {
		case '#': case '&': case '$':
			break ;
		default:
			return 0;			
	}
	if (channelName.find(' ') != std::string::npos ||
		channelName.find(7)   != std::string::npos ||
		channelName.find(',') != std::string::npos) {
		return 0;
	}
	return 1;
}

/* Allows a client to join a channel, if it does not exist it will be created 
   Return
		-1 if the supplied channel name isn't valid
		-2 if key is required but not correct
		-3 if the channel is invite-only */
int		ChannelRegistry::joinChannel(const std::string& channelName, Client *client, std::string key) {
	if (!this->isValidChannelName(channelName)) {
		std::cout << "TEST" << std::endl;
		return -1;
	}

	Channel *channel;
	auto it = _channels.find(channelName);

	if (it == _channels.end()) {
		channel = new Channel(channelName);
		_channels[channelName] = channel;
	} else {
		channel = it->second;
	}

	if (channel->hasClient(client)) {
		return 0;
	}
	if (channel->isInviteOnly())
		return -3;
	if (channel->verifyKey(key)) {
		channel->addClient(client);
	} else {
		return -2;
	}
	if (!channel->getTopic().empty()) {
		return  1;
	}
	return 0;
}

/* Removes a client from a channel */
int	ChannelRegistry::partChannel(const std::string& channelName, Client *client) {
	auto it = _channels.find(channelName);

	if (it == _channels.end()) {
		return 0;
	} else {
		it->second->removeClient(client);
		if (!it->second) {
			_channels.erase(channelName);
		}
		return 1;
	}
}

/* Returns a pointer to a channel if it exists in the registry, else it returns nullptr */
Channel	*ChannelRegistry::getChannel(const std::string& channelName) {
	auto it = _channels.find(channelName);
	return (it != _channels.end()) ? it->second : nullptr;
}

void	ChannelRegistry::removeClient(Client *client) {
	for (auto const& channel : _channels) {
		partChannel(channel.first, client);
	}
}

std::deque<Channel *> ChannelRegistry::getClientChannels(Client *client) {
	std::deque<Channel *> clientChannels;

	for (auto const& channel : _channels) {
		if (channel.second->hasClient(client)) {
			clientChannels.push_front(channel.second);
		}
	}
	return clientChannels;
}
