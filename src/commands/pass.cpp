#include <EventLoop.hpp>

void EventLoop::pass(Client *client, const std::deque<std::string>& p)
{
	User *user = client->getUser();
	if (p.size() < 1) {
		client->response(server.getName(), ERR_NEEDMOREPARAMS,
							 user->getNickname() + " KICK " + ERR_NEEDMOREPARAMS_MESSAGE);		
		return ;
	}

	if (user->isRegistered()) {
		client->response(server.getName(), ERR_ALREADYREGISTERED,
						 user->getNickname() + ' ' + ERR_ALREADYREGISTERED_MESSAGE);
		return ;
	}

	if (!(p.front() == server.getPassword())) {
		client->response(server.getName(), ERR_PASSWDMISMATCH,
						 user->getNickname() + ' ' + ERR_PASSWDMISMATCH_MESSAGE);
		return ;
	}
	
	user->setHasPassword();
	user->isRegistered();
}
