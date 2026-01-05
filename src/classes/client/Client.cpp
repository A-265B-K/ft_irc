# include "Client.hpp"

int	Client::socketIsReadable() const {
	return 1;
}

int	Client::socketIsWritable() const {
	return 0;
}

// Clean this up
void	Client::handleReadable(std::deque<Message>& messages) {
	char	data[200];
	ssize_t	bytes = recv(client_socket, data, sizeof(data), 0); // replace with diff fucntion

	if (bytes <= 0) { return ; }
	// if data EOF disconnect client
	input_buffer.append(data, bytes);

	std::string::size_type pos;
	while ((pos = input_buffer.find(CRLF)) != std::string::npos) {
		const std::string line(input_buffer.substr(pos + 2));

		try {
			messages.emplace_back(line);
		} catch (const Message::BadMessageException& e) {
			Message::printMessage(client_socket, e.what());
		}

		input_buffer.erase(0, pos + 2);
	}
}

void	Client::handleWritable() {

}

				// char buf[1024];
				// int bytes = recv(event_socket, buf, sizeof(buf), 0);
				// if (bytes > 0) {
				// 	buf[bytes] = '\0';
				// 	std::cout << buf << std::endl;
				// }
