# include "Client.hpp"

int	Client::socketIsReadable() const {
	return 1;
}

int	Client::socketIsWritable() const {
	return 1;
}

void	Client::handleReadable(const std::string& serverName, std::deque<Message>& messages)
{
	char	data[IBUFFER];
	ssize_t	bytes = recv(_clientSocket, data, IBUFFER, 0); // replace with diff fucntion

	if (bytes <= 0) { return ; }
	// if data EOF disconnect client
	_inputBuffer.append(data, bytes);

	std::string::size_type pos;
	while ((pos = _inputBuffer.find(CRLF)) != std::string::npos) {
		const std::string line(_inputBuffer.substr(0, pos + 2));

		if (line.length() > LIMIT) // beyond the 512-limit
			response(
					serverName,
					ERR_INPUTTOOLONG,
					getUser()->getNickname() + " " ERR_INPUTTOOLONG_MESSAGE
			);
		else if (line.length() != 2) // empty commands ("\r\n") are silently dropped
			try {
				messages.emplace_back(line);
				printMessage(std::string("recv `") + messages.back().build(false) + '\'');
			} catch (const Message::BadMessageException& e) {
				const std::string error("Message::BadMessageException: ");

				printMessage(error + e.what());
				handleWritable(Message(serverName, e._numeric,
						_user->getNickname() + " :" + e.what()));
			}

		_inputBuffer.erase(0, pos + 2);
	}
}

void	Client::handleWritable(const Message& message) const {
	size_t bytes = send(_clientSocket, message.build().data(), message.length,
			MSG_DONTWAIT | MSG_NOSIGNAL);

	if (bytes == message.length)
		printMessage(std::string("send `") + message.build(false) + '\'');
}

void Client::generateResponse(const std::string& serverName, const unsigned short numeric,
		const char* replyName, const std::string& text) const
{
	printMessage(replyName);

	handleWritable(Message(serverName, numeric, text));
}

void Client::printMessage(const std::string& text) const
{
	std::ostringstream output;

	output << GREEN << '[' << _address << "] " << RESET << text;
	::printMessage(output.str());
}

int	Client::getSocket() {
	return _clientSocket;
}

std::string	Client::getAddress() {
	return _address;
}

User	*Client::getUser() {
	return _user;
}

void	Client::markForRemoval() {
	_requestedDisconnect = true;
}

bool	Client::requestedDisconnect() {
	return _requestedDisconnect;
}

std::string Client::getName() const {
	std::string clientName = 
				_user->getNickname() +
			 	"!~" + _user->getUsername() +
				"@"  + _user->getHostname();
	return clientName;
}
