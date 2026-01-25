#include <Channel.hpp>

#include <ctime>
#include <sstream>
#include <stdexcept>

Channel::Channel(std::string name) : _name(name), _userLimit(-1), _modes(0) {
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

/* Removes a client from a channel */
void	Channel::removeClient(Client *client) {
	_clients.erase(client);
}

std::map<Client *,int>::const_iterator Channel::getClientByNick(const std::string& nickname) const
{
	auto cit = _clients.begin();
	while (cit != _clients.end())
		if (cit++->first->getUser()->getNickname() == nickname)
			break;

	return cit;
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

bool Channel::verifyKey(const std::string& userKey) const
{
	return !isMode('k') || userKey == _key;
}

bool Channel::isInviteOnly() const
{
	return isMode('i');
}

const std::string Channel::flags{"iklot"};

// 1 mode set, 0 mode unset, -1 mode unrecognized
int Channel::isMode(const unsigned char& m, char c) const
{
	const std::string::size_type idx = flags.find(c);

	if (idx != std::string::npos)
		return (m & 1 << idx) > 0;
	return -1;
}

int Channel::isMode(char c) const { return isMode(_modes, c); }

void Channel::setMode(unsigned char& m, char c)
{
	const std::string::size_type idx = flags.find(c);

	if (idx != std::string::npos)
		m |= 1 << idx;
}

void Channel::setMode(char c) { setMode(_modes, c); }

void Channel::unsetMode(unsigned char& m, char c)
{
	const std::string::size_type idx = flags.find(c);

	if (idx != std::string::npos)
		m &= ~(1 << idx);
}

void Channel::unsetMode(char c) { unsetMode(_modes, c); }

std::string Channel::getChannelModes(Client *client) const
{
	std::string modestring(_modes ? "+" : "");
	std::string modeArguments;

	for (const char c : flags)
		if (isMode(c)) {
			modestring += c;
			switch(c) {
			case 'k':
				if (hasClient(client))
					modeArguments += ' ' + _key;
				break;
			case 'l':
				modeArguments += ' ' + std::to_string(_userLimit);
			default: // 'i', 't' ('o' never set)
				;
			}
		}

	return modestring + modeArguments;
}

unsigned  Channel::editModes(std::string& changedModes,
		std::set<std::pair<char, std::string> >& invalidParam,
		std::set<std::string>& notInChannel, const std::string& modestring,
		std::deque<std::string>::const_iterator& modeArguments,
		const std::deque<std::string>::const_iterator& modeArgumentsEnd)
{
	unsigned result = 0;
	unsigned char setFlags = 0;
	unsigned char unsetFlags = 0;
	std::string newKey;
	int	newUserLimit;
	std::set<Client*> setOperators;
	std::set<Client*> unsetOperators;
	std::string::const_iterator cit = modestring.begin();

	// parse modestring
	while(cit != modestring.end())
top:	switch (*cit) {
		case '+':
			while (++cit != modestring.end())
				switch (*cit) {
				// i: invite-only channel mode
				// t: protected topic mode
				case 'i': case 't':
					if (!isMode(*cit))
						setMode(setFlags, *cit);
					unsetMode(unsetFlags, *cit);
					break;
				// k: key channel mode
				case 'k':
					if (modeArguments == modeArgumentsEnd)
						continue; // ignore 'k' mode without key
					if (isValidKey(*modeArguments)) {
						newKey = *modeArguments;
						setMode(setFlags, *cit);
						unsetMode(unsetFlags, *cit);
					}
					else
						result |= INVALIDKEY;
					++modeArguments;
					break;
				// l: client limit channel mode
				case 'l':
					if (modeArguments == modeArgumentsEnd)
						continue; // ignore 'l' mode without integer limit
					try {
						std::size_t pos;
						const int l = std::stoi(*modeArguments, &pos);

						if (l < 0 || pos != modeArguments->size())
							throw std::invalid_argument("");
						newUserLimit = l;
						setMode(setFlags, *cit);
						unsetMode(unsetFlags, *cit);
					} catch (const std::exception&) {
						invalidParam.emplace({'l', *modeArguments});
					}
					++modeArguments;
					break;
				// o: operator prefix
				case 'o':
					if (modeArguments == modeArgumentsEnd)
						continue; // ignore 'o' setting without target client(s)
					std::istringstream iss(*modeArguments++);
					std::string target;

					while (std::getline(iss, target, ',')) {
						const auto newOperator = getClientByNick(target);

						if (newOperator == _clients.end())
							notInChannel.emplace(target);
						else {
							if (newOperator->second == 0)
								setOperators.emplace(newOperator->first);
							unsetOperators.erase(newOperator->first);
						}
					}
					break;
				case '+': case '-':
					goto top;
				default:
					result |= UNKNOWNFLAG;
				}
			break;
		case '-':
			while (++cit != modestring.end()) {
				if (*cit != 'o') {
					if (isMode(*cit) == 1)
						setMode(unsetFlags, *cit);
					unsetMode(setFlags, *cit);
				}
				switch (*cit) {
				// i: invite-only channel mode
				// t: protected topic mode
				case 'i': case 't':
					break;
				// k: key channel mode
				case 'k':
					newKey.clear();
					break;
				// l: client limit channel mode
				case 'l':
					newUserLimit = -1;
					break;
				// o: operator prefix
				case 'o':
					if (modeArguments == modeArgumentsEnd)
						continue; // ignore 'o' setting without target client(s)
					std::istringstream iss(*modeArguments++);
					std::string target;

					while (std::getline(iss, target, ',')) {
						const auto newOperator = getClientByNick(target);

						if (newOperator == _clients.end())
							notInChannel.emplace(target);
						else {
							if (newOperator->second == 2) {
								invalidParam.emplace({'o', target});
								continue;
							}
							if (newOperator->second == 1)
								unsetOperators.emplace(newOperator->first);
							setOperators.erase(newOperator->first);
						}
					}
					break;
				case '+': case '-':
					goto top;
				default:
					result |= UNKNOWNFLAG;
				}
			}
			break;
		default:
			if (isMode(*cit++) == -1)
				result |= UNKNOWNFLAG;
		}

	if (isMode(setFlags, 'k') && newKey == _key)
		unsetMode(setFlags, 'k');
	if (isMode(setFlags, 'l') && newUserLimit == _userLimit)
		unsetMode(setFlags, 'l');
	if (!setOperators.empty())
		
		
	if (!setFlags)
		changedModes += '+' + setFlags;
	if (!unsetFlags)
		changedModes += '-' + unsetFlags;

	for (const char c : setFlags)
		switch (c) {
		case 'k':
			changedModes += ' ' + _key;
			break;
		case 'l':
			changedModes += ' ' + std::to_string(_userLimit);
		default: // 'i', 'o', 't'
			;
		}

	return result;
}
