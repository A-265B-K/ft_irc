# include <EventLoop.hpp>
# include <function_declarations.hpp>
# include <static_declarations.hpp>

# define READ_SIZE 512
# include <iostream>
# include <errno.h>
# include <netdb.h>
# include <sstream>
# include <arpa/inet.h>

void	EventLoop::bindServer(int server_fd) {
	sockaddr_in			address{};
	std::ostringstream	output{};

	address.sin_family = AF_INET;
	address.sin_port = this->port;
	address.sin_addr.s_addr = INADDR_ANY;

	bind(server_fd, (struct sockaddr*)&address, sizeof(address));
	listen(server_fd, SOMAXCONN);

	char hostname[256];
	gethostname(hostname, sizeof(hostname));

	hostent* host = gethostbyname(hostname);
	for (int i = 0; host->h_addr_list[i] != nullptr; i++) {
		char* ip = inet_ntoa(*(in_addr*)host->h_addr_list[i]); //unsure if this is okay
		output << "Listening on IP " << ip << ", port " << ntohs(this->port) << "...\n";
	}
	::printMessage(output.str());
}

int		EventLoop::addEvent(int socket_fd) {
	struct epoll_event	event{};

	event.events = EPOLLIN | EPOLLET;
	event.data.fd = socket_fd;
	if (epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, socket_fd, &event) == -1) {
		std::cout << "EPOLL_CTL failed" << std::endl;
		close(socket_fd);
		return -1;
	} // this can fail, implement a safeguard
	return socket_fd;
}

int		EventLoop::waitForEvents() {
	int ec = epoll_wait(this->epoll_fd, this->events, MAX_EVENTS, 300000);
	if (ec < 0) {
		std::cout << "EPOLL_WAIT failed" << std::endl;
	}
	return ec;
}

void	EventLoop::printEvent(int	i) {
	char	read_buffer[READ_SIZE + 1];
	unsigned int	bytes_read = read(this->events[i].data.fd, read_buffer, READ_SIZE);
	read_buffer[bytes_read] = '\0';
	std::cout << "Read: " << read_buffer << std::endl;
}

void		EventLoop::setPort(ushort port) {
	this->port = port;
}

int		EventLoop::getEpollFD() {
	return this->epoll_fd;
}

struct epoll_event *EventLoop::getEvents() {
	return this->events;
}

int		EventLoop::getEventSocket(int i) {
	return this->events[i].data.fd;
}
