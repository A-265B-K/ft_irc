#include <EventLoop.hpp>

// TODO parse nickname for invalid options
void EventLoop::nick(Client *client, const std::deque<std::string>& p)
{
	if (p.size() != 1) {
		// send error message
		return ;
	}

	User *user = client->getUser();
	user->setNickname(p.front());

	user->isRegistered();
}
