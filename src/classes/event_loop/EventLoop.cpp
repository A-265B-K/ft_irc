# include <EventLoop.hpp>
# include <static_declarations.hpp>

# define READ_SIZE 512
# include <iostream>

int		EventLoop::addEvent(int fd) {
	sockaddr_in			address{};
	struct epoll_event	event{};
	int					socket_fd;
	if (fd == 0) {
		socket_fd = socket(AF_INET, SOCK_STREAM, 0); // check if this can fail
	} else {
		socket_fd = fd;
	}
		address.sin_family = AF_INET;
		address.sin_port = htons(8080); // forbidden funcitom
		address.sin_addr.s_addr = INADDR_ANY;

	bind(socket_fd, (struct sockaddr*)&address, sizeof(address));
	listen(socket_fd, SOMAXCONN);

	event.events = EPOLLIN;
	event.data.fd = socket_fd;
	if (epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, socket_fd, &event) == -1) {
		std::cout << "EPOLL_CTL failed" << std::endl;
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

int		EventLoop::getEpollFD() {
	return this->epoll_fd;
}

struct epoll_event *EventLoop::getEvents() {
	return this->events;
}

int		EventLoop::getEventSocket(int i) {
	return this->events[i].data.fd;
}