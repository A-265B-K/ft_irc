# include <Client.hpp>
# include <iostream>
# include <sys/socket.h>
# include <string>
# include <sstream>

int	Client::socketIsReadable() {
	return 1;
}

int	Client::socketIsWritable() {
	return 0;
}

std::string	getLine(std::string *buffer)
{
	size_t newline_i = buffer->find('\n');
	if (newline_i == std::string::npos) {
		return "";
	}
	std::string line = buffer->substr(0, newline_i);
	if (line.back() == '\r') {
		line.pop_back();
	}
	buffer->erase(0, newline_i + 1);
	return line;
}

// Clean this up
void	Client::handleReadable(int event_socket) {
	char	data[200];
	int		bytes = recv(event_socket, data, sizeof(data), 0); // replace with diff fucntion
	if (bytes <= 0) { return ; }
	data[bytes] = '\0';
	// if data EOF disconnect client
	input_buffer.append(data);
	std::string line;
	while (!(line = getLine(&input_buffer)).empty()) {
		std::cout << "LINE: " << line << std::endl;
		// if (current_message)
		// 	delete current_message;
		// current_message = new Message(line);
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