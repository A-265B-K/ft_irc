#include <User.hpp>
#include <iostream>
#include <numerics.hpp>

void	User::setHasPassword() {
	this->hasPassword = true;
}

void	User::setNickname(const std::string& nickname) {
	this->nickname = nickname;
}

void	User::setUsername(const std::string& username) {
	this->username = username;
}

void	User::setHostname(const std::string& hostname) {
	this->hostname = hostname;
}

void	User::setServername(const std::string& servername) {
	this->servername = servername;
}

void	User::setRealname(const std::string& realname) {
	this->realname = realname;
}

std::string User::getNickname() {
	return this->nickname;
}

std::string User::getUsername() {
	return this->username;
}

std::string User::getHostname() {
	return this->hostname;
}

std::string User::getServername() {
	return this->servername;
}

std::string User::getRealname() {
	return this->realname;
}

bool	User::isRegistered() {
	if (!this->registered) {
		if (nickname.empty()   || 
			username.empty()   ||
			hostname.empty()   ||
			servername.empty() ||
			realname.empty()   ||
			hasPassword == false) {
			return this->registered = false;
		} else {
			std::stringstream message;
			message << "USER REGISTERED" << std::endl
					<< "NICKNAME:   " << nickname << std::endl
					<< "USERNAME:   " << username << std::endl
					<< "HOSTNAME:   " << hostname << std::endl
					<< "SERVERNAME: " << servername << std::endl
					<< "REALNAME:   " << realname << std::endl;
			::printMessage(message.str());
			client->response(servername, WELCOME, nickname + " " + WELCOME_MESSAGE);
			this->registered = true;
		}
	}
	return this->registered;
}

/*
 i: invisible user mode
 o: oper user moder
 r: registered user mode
*/
const std::string User::flags{"ior"};

// 1 mode set, 0 mode unset, -1 mode unrecognized
int User::isMode(char c) const
{
	const std::string::size_type idx = flags.find(c);

	if (idx != std::string::npos)
		return (modes & 1 << idx) > 0;
	return -1;
}

void User::setMode(char c)
{
	const std::string::size_type idx = flags.find(c);

	if (idx != std::string::npos)
		modes |= 1 << idx;
}

void User::unsetMode(char c)
{
	const std::string::size_type idx = flags.find(c);

	if (idx != std::string::npos)
		modes &= ~(1 << idx);
}

std::string User::getModestring() const
{
	std::string modestring(modes ? "+" : "");

	for (const char c : flags)
		if (isMode(c))
			modestring += c;

	return modestring;
}

int User::editModes(std::string& changedModes, const std::string& modestring)
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
					if (*cit != 'o') {
						setMode(*cit);
						setFlags += *cit;
					}
					// NOTE: +o is not allowed by the MODE command,
					//  and requires the appropriate OPER command
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
					// NOTE: However, a user may -o itself through
					//  a MODE command
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

	return unknownFlag;
}

bool	User::isNicknameValid(const std::string &nickname) {
	switch(nickname.front()) {
		case '#': case '&': case '$': case ':': case ' ':
			return false;
		default:
			return true;
	}
}

bool	User::getHasPassword() {
	return hasPassword;
}
