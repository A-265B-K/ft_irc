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
	ssize_t	bytes = recv(client_socket, data, IBUFFER, 0); // replace with diff fucntion

	if (bytes <= 0) { return ; }
	// if data EOF disconnect client
	input_buffer.append(data, bytes);

	std::string::size_type pos;
	while ((pos = input_buffer.find(CRLF)) != std::string::npos) {
		const std::string line(input_buffer.substr(0, pos + 2));

		if (line.length() != 2) // empty commands ("\r\n") are silently dropped
			try {
				messages.emplace_back(line);
				printMessage(std::string("recv `") + messages.back().build(false) + '\'');
			} catch (const Message::BadMessageException& e) {
				const std::string error("Message::BadMessageException: ");

				printMessage(error + e.what());
// TODO Substitute the "<client>" placeholder with user nickname when User is implemented
				handleWritable(Message(serverName, e._numeric,
						std::string("<client>") + " :" + e.what()));
			}

		input_buffer.erase(0, pos + 2);
	}
}

void	Client::handleWritable(const Message& message) const {
	ssize_t bytes = send(client_socket, message.build().data(), message.length,
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

	output << '[' << address << "] " << text;
	::printMessage(output.str());
}

int	Client::getSocket() {
	return this->client_socket;
}

std::string	Client::getAddress() {
	return this->address;
}

bool	Client::isUser() {
	if (user == nullptr) {
		return false;
	} else {
		return true;
	}
}

User	*Client::getUser() {
	return this->user;
}

void	Client::createUser() {
	this->user = new User();
}

void	Client::deleteUser() {
	delete this->user;
}