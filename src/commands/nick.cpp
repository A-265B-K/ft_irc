#include <EventLoop.hpp>

void EventLoop::nick(Client *client, const std::deque<std::string>& p)
{
	User *user = client->getUser();

	if (user->isRegistered()) {
		client->response(server.getName(), ERR_ALREADYREGISTERED,
						 user->getNickname() + ' ' + ERR_ALREADYREGISTERED_MESSAGE);
		return ;
	}

	if (!user->getHasPassword()) {
		client->response(server.getName(), ERR_PASSWDMISMATCH,
						 user->getNickname() + ' ' + ERR_PASSWDMISMATCH_MESSAGE);
		this->markClientForRemoval(client);
		return ;
	}

	if (p.size() < 1) {
		client->response(server.getName(), ERR_NEEDMOREPARAMS,
							 user->getNickname() + " NICK " + ERR_NEEDMOREPARAMS_MESSAGE);		
		return ;
	}

	std::string nickname = p.front();


	if (!user->isNicknameValid(nickname)) {
		client->response(server.getName(), ERR_ERRONEUSNICKNAME,
							 user->getNickname() + user->getNickname() + ERR_ERRONEUSNICKNAME_MESSAGE);	
		return ;
	}

	if (clientReg.getClientByNick(nickname)) {
		client->response(server.getName(), ERR_ERRONEUSNICKNAME,
							 user->getNickname() + user->getNickname() + ERR_ERRONEUSNICKNAME_MESSAGE);	
		return ;
	}

	user->setNickname(p.front());

	user->isRegistered();
}
