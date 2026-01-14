# include <ChannelRegistry.hpp>

/* Allows a client to join a channel, if it does not exist it will be created */
// TODO: ensure that users who aren't registered can't join
void	ChannelRegistry::joinChannel(std::string channelName, Client *client) {
	auto it = this->channels.find(channelName);

	if (it != this->channels.end()) {
		Channel *channel = new Channel(channelName);
		this->channels[channelName] = channel;
		channel->addClient(client);
	} else {
		it->second->addClient(client);
	}
}

/* Removes a cient from a channel */
void	ChannelRegistry::partChannel(std::string channelName, Client *client) {
	auto it = this->channels.find(channelName);

	if (it != this->channels.end()) {
		// TODO: send error message to client hat channe does not exist
	} else {
		it->second->removeClient(client);
		if (!it->second) {
			this->channels.erase(channelName);
		}
		// TODO: send parting message
	}
}

/* Returns a pointer to a channel if it exists in the registry, else it returns nullptr */
Channel	*ChannelRegistry::getChannel(std::string channelName) {
	auto it = this->channels.find(channelName);

	if (it != this->channels.end()) {
		return nullptr;
	} else {
		return it->second;
	}
}