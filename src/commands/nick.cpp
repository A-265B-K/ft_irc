#include <EventLoop.hpp>

void EventLoop::nick(Client *client, const std::deque<std::string>& p)
{
	User *user = client->getUser();

	if (user->isRegistered()) {
		client->response(server.getName(), ERR_ALREADYREGISTERED,
						 user->getNickname() + ' ' + ERR_ALREADYREGISTERED_MESSAGE);
		return ;
	}

	if (p.size() < 1) {
		client->response(server.getName(), ERR_NEEDMOREPARAMS,
							 user->getNickname() + " NICK " + ERR_NEEDMOREPARAMS_MESSAGE);		
		return ;
	}

	std::string nickname = p.front();


	if (!user->isNicknameValid(nickname)) {
		// TODO SEND ERROR
		return ;
	}

	user->setNickname(p.front());
	user->isRegistered();
}
