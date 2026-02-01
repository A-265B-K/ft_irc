#include <EventLoop.hpp>

void EventLoop::ping(Client *client, const std::deque<std::string>& p) {
	if (p.empty()) {
		client->response(
				server.getName(),
				ERR_NEEDMOREPARAMS,
				client->getUser()->getNickname()
						+ " MODE " ERR_NEEDMOREPARAMS_MESSAGE
		);

	return;
	}

	client->replyTo(
			server.getName(),
			"PONG",
			':' + p.front()
	);
}
