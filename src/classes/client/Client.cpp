# include <Client.hpp>
# include <iostream>
# include <sys/socket.h>

int	Client::socketIsReadable() {
	return 1;
}

int	Client::socketIsWritable() {
	return 0;
}

void	Client::handleReadable(int event_socket) {
	char	data[200];
	int		bytes = recv(event_socket, data, sizeof(data), 0); // replace with diff fucntion
	if (bytes <= 0) { return ; }
	data[bytes] = '\0';
	// if data EOF disconnect client
	this->input_buffer.append(data);
	//if (this->input_buffer > 512) {
	//	disconnect
	//}
	std::cout << this->input_buffer << std::endl;

}

void	Client::handleWritable() {

}

				// char buf[1024];
				// int bytes = recv(event_socket, buf, sizeof(buf), 0);
				// if (bytes > 0) {
				// 	buf[bytes] = '\0';
				// 	std::cout << buf << std::endl;
				// }