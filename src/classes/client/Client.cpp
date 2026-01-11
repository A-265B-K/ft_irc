# include "Client.hpp"

int	Client::socketIsReadable() const {
	return 1;
}

int	Client::socketIsWritable() const {
	return 1;
}

void	Client::handleReadable(std::deque<Message>& messages) {
	char	data[INPUT_BUFFER];
	ssize_t	bytes = recv(client_socket, data, INPUT_BUFFER, 0); // replace with diff fucntion

	if (bytes <= 0) { return ; }
	// if data EOF disconnect client
	input_buffer.append(data, bytes);

	std::string::size_type pos;
	while ((pos = input_buffer.find(CRLF)) != std::string::npos) {
		const std::string line(input_buffer.substr(0, pos + 2));

		try {
			messages.emplace_back(line);
			printMessage(std::string("recv `") + messages.back().build(false) + '\'');
		} catch (const Message::BadMessageException& e) {
			printMessage(e.what());
		}

		input_buffer.erase(0, pos + 2);
	}
}

void	Client::handleWritable(const Message& message) {
	ssize_t bytes = send(client_socket, message.build().data(), message.length,
			MSG_DONTWAIT | MSG_NOSIGNAL);

	if (bytes == message.length)
		printMessage(std::string("send `") + message.build(false) + '\'');
}

void    Client::printMessage(const std::string& text) const
{
	std::ostringstream output;

	output << '[' << address << "] " << text;
	::printMessage(output.str());
}
