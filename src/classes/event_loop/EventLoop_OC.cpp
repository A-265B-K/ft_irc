# include <EventLoop.hpp>

EventLoop::EventLoop(Server& server, ChannelRegistry& channelReg, ClientRegistry& clientReg):
		server(server), channelReg(channelReg), clientReg(clientReg)
{
	this->epoll_fd = epoll_create(1);
	if (this->epoll_fd == -1) {
		::printMessage("EPOLL_CREATE failed");
	} // Implement check, returns -1 if it fails.
}

EventLoop::~EventLoop() {
	close(this->epoll_fd);
}
