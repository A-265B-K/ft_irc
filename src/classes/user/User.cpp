#include <User.hpp>
#include <numerics.hpp>

// #include <ctime>

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
