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

std::set<Client *> Channel::getClients() {
	std::set<Client *> clients;

	for (auto const& x : this->clients){
		Client *c = x.first;
		if (!c) {
			std::cerr << RED << "WARNING DANGLING CLIENT POINTER" << RESET << std::endl;
			continue ;
		}
		clients.insert(c);
	}
	return clients;
}

void	Channel::broadcast(Client *sender, const std::string& command, const std::string& message) {
	User 				*user = sender->getUser();
	std::stringstream	text;
	text  << ":"  << user->getNickname() <<
			 "!~" << user->getUsername() <<
			 "@"  << user->getHostname() << 
			 " "  << command <<
			 " "  << this->name <<
			 " :" << message << "\r\n";

	// TODO: Implement check, to ensure that sender has the correct permissions

	for (auto it : this->clients) {
		Client *recipient = it.first;
		if (recipient != sender) {
			if (recipient->getUser()->isRegistered())
			recipient->handleWritable(text.str());
		}
	}
}