#include <Channel.hpp>

#include <sstream>
#include <stdexcept>
#include <utility>

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

std::string Channel::printModes(const unsigned char& m) const
{
	std::string str;

	for (const char c : flags)
		if (isMode(m, c))
			str += c;

	return str;
}

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
						invalidParam.emplace(std::make_pair('l', *modeArguments));
					}
					++modeArguments;
					break;
				// o: operator prefix
				case 'o':
					if (modeArguments == modeArgumentsEnd)
						continue; // ignore 'o' setting without target client(s)
					{
						std::istringstream iss(*modeArguments++);
						std::string target;

						while (std::getline(iss, target, ',')) {
							const std::map<Client *,int>::const_iterator newOperator =
									getClientByNick(target);

							if (newOperator == _clients.end())
								notInChannel.emplace(target);
							else {
								if (newOperator->second == 0)
									setOperators.emplace(newOperator->first);
								unsetOperators.erase(newOperator->first);
							}
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
					{
						std::istringstream iss(*modeArguments++);
						std::string target;

						while (std::getline(iss, target, ',')) {
							const std::map<Client *,int>::const_iterator newOperator =
									getClientByNick(target);

							if (newOperator == _clients.end())
								notInChannel.emplace(target);
							else {
								if (newOperator->second == 2) { // attempting to -o an owner!
									invalidParam.emplace(std::make_pair('o', target));
									continue;
								}
								if (newOperator->second == 1)
									unsetOperators.emplace(newOperator->first);
								setOperators.erase(newOperator->first);
							}
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

	_modes |= setFlags;
	_modes &= ~unsetFlags;

	if (!setOperators.empty())
		setMode(setFlags, 'o');
	if (!unsetOperators.empty())
		setMode(unsetFlags, 'o');

	if (setFlags)
		changedModes += '+' + printModes(setFlags);
	if (unsetFlags)
		changedModes += '-' + printModes(unsetFlags);

	for (const char c : flags)
		if (isMode(setFlags, c))
			switch (c) {
			case 'k':
				_key = newKey;
				changedModes += ' ' + _key;
				break;
			case 'l':
				_userLimit = newUserLimit;
				changedModes += ' ' + std::to_string(_userLimit);
				break;
			case 'o':
				for (auto cit = setOperators.begin(); cit != setOperators.end(); ++cit) {
					_clients[*cit] = 1;
					changedModes += (cit == setOperators.begin() ? ' ' : ',')
							+ (*cit)->getUser()->getNickname();
				}
			default: // 'i', 't'
				;
			}

	for (const char c : flags)
		if (isMode(unsetFlags, c))
			switch (c) {
			case 'k':
				_key = newKey;
				break;
			case 'l':
				_userLimit = newUserLimit;
				break;
			case 'o':
				for (auto cit = unsetOperators.begin(); cit != unsetOperators.end(); ++cit) {
					_clients[*cit] = 0;
					changedModes += (cit == unsetOperators.begin() ? ' ' : ',')
							+ (*cit)->getUser()->getNickname();
				}
			default: // 'i', 't'
				;
			}

	return result;
}
