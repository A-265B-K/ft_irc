#include <EventLoop.hpp>

// TODO parse nickname for invalid options
void EventLoop::nick(Client *client, const std::deque<std::string>& p)
{
	User *user = client->getUser();

	if (p.size() < 1) {
		client->response(server.getName(), ERR_NEEDMOREPARAMS,
							 user->getNickname() + " NICK " + ERR_NEEDMOREPARAMS_MESSAGE);		
		return ;
	}

	user->setNickname(p.front());
	user->isRegistered();
}
