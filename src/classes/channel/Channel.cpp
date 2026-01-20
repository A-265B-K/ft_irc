# include <Channel.hpp>

Channel::Channel(std::string name) : name(name), _userLimit(-1) {

}


Channel::~Channel() {

}

void	Channel::setTopic(const std::string& topic) {
	this->topic = topic;
}

void	Channel::addClient(Client *client) {
	/* If there is no-one in the channel, make the newest person the owner */
	if (static_cast<int>(this->clients.size()) < _userLimit || _userLimit == -1)
	{
		if (this->clients.empty()) {
			this->clients.insert({client, 2});
		} else {
			this->clients.insert({client, 0});
		}
	}
}

/* Removes a client from a channel */
void	Channel::removeClient(Client *client) {
	this->clients.erase(client);
}

std::set<Client *> Channel::getClients() {
	std::set<Client *> clients;

	for (auto const& x : this->clients){
		Client *c = x.first;
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
			if (recipient->getUser()->isRegistered()) {
				recipient->handleWritable(text.str());
			}
		}
	}
}

bool	Channel::isOperator(Client *client) {
	auto it = this->clients.find(client);

	if (it != this->clients.end()) {
		if (it->second >= 1) {
			return true;
		}
	} else {
		// ERROR CLIENT NOT FOUND;
	}
	return false;
}

void	Channel::setUserLimit(int userLimit) {
	_userLimit = userLimit;
}

std::string	Channel::getPassword() const {
	return *_password;
}