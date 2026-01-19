# include <ChannelRegistry.hpp>

const int ChannelRegistry::isValidChannelName(const std::string& channelName) {
	if (channelName.empty()) {
		return 0;
	}
	switch(channelName.front()) {
		case '#': case '$':
			break ;
		default:
			return 0;			
	}
	// TODO: add other channel prefixes and check if no invalid characters
	return 1;
}

/* Allows a client to join a channel, if it does not exist it will be created 
   Return -1 if the supplied channel name isn't valid */
int		ChannelRegistry::joinChannel(const std::string& channelName, Client *client, std::string password) {
	if (!this->isValidChannelName(channelName)) {
		// TODO send error
		return -1;
	}

	Channel *channel;
	auto it = this->channels.find(channelName);

	if (it == this->channels.end()) {
		channel = new Channel(channelName);
		this->channels[channelName] = channel;
	} else {
		channel = it->second;
	}
	if (channel->getPassword() == password) {
		channel->addClient(client);
	} else {
		// TODO error wrong passowrd
	}
	return 0;
}

/* Removes a cient from a channel */
int	ChannelRegistry::partChannel(const std::string& channelName, Client *client) {
	auto it = this->channels.find(channelName);

	if (it != this->channels.end()) {
		return 0;
	} else {
		it->second->removeClient(client);
		if (!it->second) {
			// TODO: delete channel
			this->channels.erase(channelName);
		}
		return 1;
	}
}

/* Returns a pointer to a channel if it exists in the registry, else it returns nullptr */
Channel	*ChannelRegistry::getChannel(const std::string& channelName) {
	auto it = this->channels.find(channelName);
	return (it != this->channels.end()) ? it->second : nullptr;
}