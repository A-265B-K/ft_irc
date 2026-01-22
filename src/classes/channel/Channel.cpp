#include <Channel.hpp>
#include <ctime>
#include <stdexcept>

Channel::Channel(std::string name) : _name(name), _userLimit(-1) {
	std::cout << BLUE << "CHANNEL CREATED: " << _name << RESET << std::endl;
}


Channel::~Channel() {
	std::cout << BLUE << "CHANNEL DELETED: " << _name << RESET << std::endl;
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
	std::stringstream	text;
	text  << sender->getName() <<
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

void	Channel::setUserLimit(int userLimit) {
	_userLimit = userLimit;
}

std::string	Channel::getPassword() const {
	return _password;
}

const std::string& Channel::getTopic() {
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

/*
 i: invite-only channel mode
 k: key channel mode
 l: client limit channel mode
 o: operator prefix
 t: protected topic mode
*/
const std::string Channel::flags{"iklot"};

// 1 mode set, 0 mode unset, -1 mode unrecognized
int Channel::isMode(char c) const
{
	const std::string::size_type idx = flags.find(c);

	if (idx != std::string::npos)
		return (modes & 1 << idx) > 0;
	return -1;
}

void Channel::setMode(char c)
{
	const std::string::size_type idx = flags.find(c);

	if (idx != std::string::npos)
		modes |= 1 << idx;
}

void Channel::unsetMode(char c)
{
	const std::string::size_type idx = flags.find(c);

	if (idx != std::string::npos)
		modes &= ~(1 << idx);
}

std::string Channel::getChannelModes(Client *client) const
{
	std::string modestring(modes ? "+" : "");
	std::string modeArguments;

	for (const char c : flags)
		if (isMode(c)) {
			modestring += c;
			switch(c) {
			case 'k':
				if (hasClient(client))
					modeArguments += ' ' + _password;
				break;
			case 'l':
				modeArguments += ' ' + _userLimit;
			default: // 'i', 't' ('o' never set)
				;
			}
		}

	return modestring + modeArguments;
}

int Channel::editModes(std::string& changedModes, const std::string& modestring,
		std::deque<std::string>::const_iterator& modeArguments,
		const std::deque<std::string>::const_iterator& modeArgumentsEnd)
{
	std::string setFlags;
	std::string unsetFlags;
	int unknownFlag = 0;
	std::string::const_iterator cit = modestring.begin();

	// parse modestring
	while(cit != modestring.end())
top:	switch (*cit) {
		case '+':
			while (++cit != modestring.end())
				switch (isMode(*cit)) {
				case 0: 
					switch (*cit) {
					case 'k':
						if (modeArguments == modeArgumentsEnd)
							continue; // ignore 'k' mode without key
						_password = *modeArguments++;
						break;
					case 'l':
						if (modeArguments == modeArgumentsEnd
								|| modeArguments->find_first_not_of("0123456789")
										!= std::string::npos)
							continue; // ignore 'l' mode without non-negative integer limit
						int l;
						try {
							l = std::stoi(*modeArguments);
							if (l < 0)
								throw std::invalid_argument("");
						} catch (const std::exception&) { continue; }
						_userLimit = l;
						++modeArguments;
					default: // 'i', 't'
						;
					}
					if (*cit != 'o') {
						setMode(*cit);
						setFlags += *cit;
					}
				case 1: break;
				default:
					if (*cit == '+' || *cit == '-')
						goto top;
					unknownFlag++;
				}
			break;
		case '-':
			while (++cit != modestring.end())
				switch (isMode(*cit)) {
				case 1: 
					unsetMode(*cit);
					unsetFlags += *cit;
				case 0: break;
				default:
					if (*cit == '+' || *cit == '-')
						goto top;
					unknownFlag++;
			}
			break;
		default:
			if (isMode(*cit++) == -1)
				unknownFlag++;
		}

	// remove cancellations
	std::string::size_type idx;
	while((idx = setFlags.find_first_of(unsetFlags)) != std::string::npos) {
		unsetFlags.erase(unsetFlags.find(setFlags[idx]), 1);
		setFlags.erase(idx, 1);
	}

	if (!setFlags.empty())
		changedModes += '+' + setFlags;
	if (!unsetFlags.empty())
		changedModes += '-' + unsetFlags;

	for (const char c : setFlags)
		switch (c) {
		case 'k':
			changedModes += ' ' + _password;
			break;
		case 'l':
			changedModes += ' ' + _userLimit;
		default: // 'i', 't'
			;
		}

	return unknownFlag;
}
