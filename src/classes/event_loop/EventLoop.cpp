#include <EventLoop.hpp>

int EventLoop::run()
{
	const int server_socket = server.getServerSocket();
	int client_socket;
	std::deque<Message>	incomingMessages;

	addEvent(server_socket);

	while (RUNNING) {
		const int event_count = waitForEvents();

		for (epoll_event *event = events; event - events < event_count; event++) {
			const int event_socket = event->data.fd;

			if (event_socket == server_socket) {
				client_socket = clientReg.addClient(server_socket);
				if (client_socket >= 0) {
					addEvent(client_socket);
				}

			} else {
				Client *const client = clientReg.getClientBySocket(event_socket);

				if (client->socketIsReadable()) { // for now isreadable always returns 1
					client->handleReadable(server.getName(), incomingMessages);
				}
				processMessages(client, incomingMessages);
				incomingMessages.clear();
			}
		}
	}

	return 0;
}

int		EventLoop::addEvent(int socket_fd) {
	struct epoll_event	event{};

	event.events = EPOLLIN;
	event.data.fd = socket_fd;
	if (epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, socket_fd, &event) == -1) {
		::printMessage("EPOLL_CTL failed");
		return -1;
	} // this can fail, implement a safeguard
	return socket_fd;
}

int		EventLoop::waitForEvents() {
	int ec = epoll_wait(this->epoll_fd, this->events, MAX_EVENTS, TIMEOUT);
	if (ec < 0) {
		::printMessage("EPOLL_WAIT failed");
	}
	return ec;
}

void EventLoop::processMessages(Client *client, const std::deque<Message>& messages)
{
	for_each(messages.begin(), messages.end(), [client](const Message& m) {
		// testing: returning message to sender
		if (client->socketIsWritable()) {
			client->handleWritable(m);
		}
	});
}
