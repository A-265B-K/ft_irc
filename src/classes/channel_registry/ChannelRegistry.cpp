#include <ChannelRegistry.hpp>

#include <algorithm>

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
		-3 if the channel is invite-only
		-4 if the channel is full */
int		ChannelRegistry::joinChannel(const std::string& channelName, Client *client, std::string key) {
	if (!this->isValidChannelName(channelName)) {
		std::cout << "TEST" << std::endl;
		return -1;
	}

	Channel *channel;
	auto it = _channels.find(channelName);

	if (it == _channels.end()) {
		channel = new Channel(channelName, key);
		_channels[channelName] = channel;
	} else {
		channel = it->second;
	}

	if (channel->hasClient(client)) {
		return 0;
	}
	if (channel->isInviteOnly())
		return -3;
	if (channel->isFull())
		return -4;
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
		Channel *channel = it->second;
		channel->removeClient(client);
		if (channel->memberCount() == 0) {
			_channels.erase(channelName);
			delete channel;
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
	for (auto it = _channels.begin(); it != _channels.end(); ) {
		auto current = it++;
		partChannel(current->first, client);
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

void ChannelRegistry::forEachChannel(const std::function
		<void (std::pair<std::string, Channel *>)>& f) const
{
	std::for_each(_channels.begin(), _channels.end(), f);
}
