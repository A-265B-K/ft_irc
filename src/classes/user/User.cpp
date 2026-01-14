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