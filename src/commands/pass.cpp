#include <EventLoop.hpp>

// TODO
void EventLoop::pass(Client *client, const std::deque<std::string>& p)
{
	User *user = client->getUser();
	if (p.size() < 1) {
		client->response(server.getName(), ERR_NEEDMOREPARAMS,
							 user->getNickname() + " KICK " + ERR_NEEDMOREPARAMS_MESSAGE);		
		return ;
	}
	// TODO add error if already registered
	if (p.front() == server.getPassword()) {
		user->setHasPassword();
	}
	user->isRegistered();
}
