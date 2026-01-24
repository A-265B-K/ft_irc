#include <User.hpp>
#include <numerics.hpp>
#include <ctime>
#include <algorithm>

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
	return this->isMode('r');
}

// TODO add user modes
void	User::registerUser(const time_t &serverCreationTime, const int& serverVersion) {
	std::string version = std::to_string(serverVersion);
	if (nickname.empty()   || 
		username.empty()   ||
		hostname.empty()   ||
		servername.empty() ||
		realname.empty()   ||
		hasPassword == false) {
		return ;
	} else {
		(void)serverCreationTime;
		// std::string t = ctime(&serverCreationTime);
		// std::replace(t.begin(), t.end(), ':', ' ');
		client->response(servername, RPL_WELCOME, nickname + " " + RPL_WELCOME_MESSAGE);
		client->response(servername, RPL_YOURHOST, nickname + " :You host is " + servername + ", running version " + version);
		client->response(servername, RPL_CREATED, nickname + " " + RPL_CREATED_MESSAGE);
		client->response(servername, RPL_MYINFO, nickname + " " + servername + " " + version /*+ userModes */);
		client->response(servername, RPL_ISUPPORT, nickname + " CASEMAPPING=ascii");
		this->setMode('r');

		std::stringstream message;
		message << BLUE << "USER REGISTERED: " << nickname << RESET;
		::printMessage(message.str());
		return ;
	}
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
