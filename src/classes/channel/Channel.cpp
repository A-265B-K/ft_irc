# include <Channel.hpp>

Channel::Channel(std::string name) : name(name) {

}


Channel::~Channel() {

}

void	Channel::setTopic(const std::string& topic) {
	this->topic = topic;
}

void	Channel::addClient(Client *client) {
	/* If there is no-one in the channel, make the newest person the owner */
	if (this->clients.empty()) {
		this->clients.insert({client, 2});
	} else {
		this->clients.insert({client, 0});
	}
}

/* Removes a client from a channel, if they were the last person it will be deleted */
void	Channel::removeClient(Client *client) {
	this->clients.erase(client);
	if (this->clients.empty()) {
		delete this;
	}
}