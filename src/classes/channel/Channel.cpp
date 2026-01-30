#include <Channel.hpp>

#include <algorithm>
#include <sstream>

Channel::Channel(std::string name) : _name(name), _userLimit(-1), _timestamp(::timestamp()), _modes(0) {
	std::cout << BLUE << "CHANNEL CREATED: " << YELLOW << _name << RESET << std::endl;
}

Channel::~Channel() {
	std::cout << BLUE << "CHANNEL DELETED: " << YELLOW << _name << RESET << std::endl;
}

void	Channel::setTopic(Client *setter, const std::string& topic) {
	_topic = topic;
	_topicSetter = setter->getUser()->getNickname();
	_topicTime = std::to_string(std::time(nullptr));
}

void	Channel::addClient(Client *client) {
	/* If there is no-one in the channel, make the newest person the owner */
	if (!isMode('l') || static_cast<int>(_clients.size()) < _userLimit) {
		if (_clients.empty()) {
			_clients.insert({client, 2});
		} else {
			_clients.insert({client, 0});
		}
	}
}

int		Channel::memberCount() {
	return _clients.size();
}

/* Removes a client from a channel */
void	Channel::removeClient(Client *client) {
	_clients.erase(client);
}

// bool	Channel::isEmpty() {
// 	if (_clients.size() == 0) {
// 		return true;
// 	} else {
// 		return false;
// 	}
// }

std::map<Client *,int>::const_iterator Channel::getClientByNick(const std::string& nickname) const
{
	std::map<Client *,int>::const_iterator cit = _clients.begin();

	while (cit != _clients.end()) {
		if (cit->first->getUser()->getNickname() == nickname)
			break;
		++cit;
	}

	return cit;
}

void Channel::forEachClient(const std::function <void (std::pair<Client *, int>)>& f) const
{
	std::for_each(_clients.begin(), _clients.end(), f);
}

std::set<Client *> Channel::getClients() {
	std::set<Client *> clients;

	for (auto const& x : _clients){
		Client *c = x.first;
		clients.insert(c);
	}
	return clients;
}

void	Channel::broadcast(const Client *sender, const std::string& command,
		const std::string& message) const
{
	std::stringstream	text;
	text  << ':' + sender->getName() <<
			 ' '  << command <<
			 ' '  << _name <<
			 ' '  << message << CRLF;

	for (auto it : _clients) {
		Client *recipient = it.first;
		if (recipient && recipient->getUser()->isRegistered()) {
			recipient->handleWritable(text.str());
		}
	}
}

bool	Channel::isOperator(Client *client) const {
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

const std::string& Channel::getTopic() const {
	return _topic;
}

bool Channel::hasClient(Client *client) const {
	if (client == nullptr) {
		return false;
	}
	if (_clients.find(client) == _clients.end()) {
		return false;
	} else {
		return true;
	}
}

const std::string& Channel::getName() const {
	return _name;
}

const std::string& Channel::getTimestamp() const { return _timestamp; }

void	Channel::sendTopic(Client *recipient) const {
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

bool Channel::topicRequiresOperator() const
{
	return isMode('t');
}


bool Channel::isValidKey(const std::string& userKey)
{
	return userKey.find_first_of(" :\r\n") == std::string::npos;
}

bool Channel::verifyKey(const std::string& userKey) const
{
	return !isMode('k') || userKey == _key;
}

bool Channel::isInviteOnly() const
{
	return isMode('i');
}
