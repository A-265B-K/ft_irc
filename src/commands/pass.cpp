#include <EventLoop.hpp>

// TODO
void EventLoop::pass(Client *client, const std::deque<std::string>& p)
{
	if (p.size() != 1) {
		return ;
		// TODO: send error
	}
	User *user = client->getUser();
	if (p.front() == server.getPassword()) {
		user->setHasPassword();
	}
	user->isRegistered();
}
