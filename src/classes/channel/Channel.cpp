# include <Channel.hpp>
# include <ctime>

Channel::Channel(std::string name) : _name(name), _userLimit(-1) {

}


Channel::~Channel() {

}

void	Channel::setTopic(Client *setter, const std::string& topic) {
	_topic = topic;
	_topicSetter = setter->getUser()->getNickname();
	_topicTime = std::to_string(std::time(nullptr));
}

void	Channel::addClient(Client *client) {
	/* If there is no-one in the channel, make the newest person the owner */
	if (static_cast<int>(_clients.size()) < _userLimit || _userLimit == -1)
	{
		if (_clients.empty()) {
			_clients.insert({client, 2});
		} else {
			_clients.insert({client, 0});
		}
	}
}

/* Removes a client from a channel */
void	Channel::removeClient(Client *client) {
	_clients.erase(client);
}

std::set<Client *> Channel::getClients() {
	std::set<Client *> clients;

	for (auto const& x : _clients){
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
			 " "  << _name <<
			 " :" << message << "\r\n";

	for (auto it : _clients) {
		Client *recipient = it.first;
		if (recipient != sender) {
			if (recipient->getUser()->isRegistered()) {
				recipient->handleWritable(text.str());
			}
		}
	}
}

bool	Channel::isOperator(Client *client) {
	auto it = _clients.find(client);

	if (it != _clients.end()) {
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
	return _password;
}

const std::string& Channel::getTopic() {
	return _topic;
}

bool Channel::isClientOn(Client *client) {
	if (client == nullptr) {
		return false;
	}
	if (_clients.find(client) == _clients.end()) {
		return false;
	} else {
		return true;
	}
}

const std::string& Channel::getName() {
	return _name;
}

void	Channel::sendTopic(Client *recipient) {
	User *user = recipient->getUser();

	recipient->handleWritable(std::to_string(RPL_TOPIC) + ' ' +
								user->getNickname() + ' ' +
								_name + " :" +
								_topic + "\r\n");
	recipient->handleWritable(std::to_string(RPL_TOPICWHOTIME) + ' ' +
								user->getNickname() + ' ' +
								_name + ' ' +
								_topicSetter + ' ' +
								_topicTime + "\r\n");
}

bool	Channel::topicRequiresOperator() {
	return true; // TODO fix this logic
}
