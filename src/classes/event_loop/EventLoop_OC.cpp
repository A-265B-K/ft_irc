# include <EventLoop.hpp>
# include <iostream>

EventLoop::EventLoop() {
	this->epoll_fd = epoll_create(1);
	if (this->epoll_fd == -1) {
		std::cout << "EPOLL_CREATE failed" << std::endl;
	} // Implement check, returns -1 if it fails.
}

EventLoop::~EventLoop() {
	close(this->epoll_fd);
}
