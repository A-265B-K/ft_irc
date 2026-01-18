# include <User.hpp>
# include <iostream>

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

/* Registers a user and prints a message if succesful */
bool	User::registerUser() {
	if (getNickname().empty()   || 
		getUsername().empty()   ||
		getHostname().empty()   ||
		getServername().empty() ||
		getRealname().empty())
	{
		return false;
	}
	else
	{
			// TODO: Put this in a proper print function
		std::cout << "USER REGISTERED" << std::endl;
		std::cout << "NICKNAME:   " << getNickname() << std::endl;
		std::cout << "USERNAME:   " << getUsername() << std::endl;
		std::cout << "HOSTNAME:   " << getHostname() << std::endl;
		std::cout << "SERVERNAME: " << getServername() << std::endl;
		std::cout << "REALNAME:   " << getRealname() << std::endl;

			// TODO: Send message to user
		this->registered = true;
		return this->registered;
	}
}

bool	User::isRegistered() {
	return this->registered;
}

char User::flags[] = {
		'i', // invisible user mode
		'o', // oper user moder
		'O', // local oper user mode
		'r', // registered user mode
		'w'  // WALLOPS user mode
};

unsigned char User::mask[] = {
		001,
		002,
		004,
		010,
		020
};

std::string User::getModestring() const
{
	std::string modestring(modes ? "+" : "");

	for (size_t m = 5; m--; )
		if (modes & mask[m])
			modestring += flags[m];

	return modestring;
}

std::string User::editModes(const std::string& modestring)
{
	if (modestring.empty())
		return 0;

	std::string flagsTouched;
	std::string::const_iterator cit = modestring.begin();

	switch (*cit++) {
	case '+':
		flagsTouched += '+';
		while (cit != modestring.end()) {
			for (size_t m = 0; m < 5; m++)
				if (flags[m] == *cit) {
					modes |= mask[m];
					// NOTE: +o and +O are technically not allowed
					//	from MODE command, but the subject does not
					//	require the appropriate OPER command
					flagsTouched += *cit;
					break;
				}
			cit++;
		}
		break;
	case '-':
		flagsTouched += '-';
		while (cit != modestring.end()) {
			for (size_t m = 0; m < 5; m++)
				if (flags[m] == *cit) {
					modes &= ~mask[m];
					flagsTouched += *cit;
					break;
				}
			cit++;
		}
		break;
	default:
		;
	}

	return flagsTouched;
}
