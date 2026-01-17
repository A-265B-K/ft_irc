#include <EventLoop.hpp>

// TODO parse nickname for invalid options
void EventLoop::nick(Client *client, const std::deque<std::string>& p)
{
	if (p.size() != 1) {
		// send error message
		return ;
	}

	if (client->isUser() == false) {
		client->createUser();
	}

	User *user = client->getUser();
	user->setNickname(p.front());

	::printMessage("NICK");
}
