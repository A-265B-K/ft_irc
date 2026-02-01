#include <EventLoop.hpp>

void EventLoop::nick(Client *client, const std::deque<std::string>& p)
{
	User *user = client->getUser();

	if (user->isRegistered()) {
		client->response(server.getName(), ERR_ALREADYREGISTERED,
						client->getName() + ' ' + ERR_ALREADYREGISTERED_MESSAGE);
		return ;
	}

	if (!user->getHasPassword()) {
		client->response(server.getName(), ERR_PASSWDMISMATCH,
						client->getName() + ' ' + ERR_PASSWDMISMATCH_MESSAGE);
		this->markClientForRemoval(client);
		return ;
	}

	if (p.size() < 1) {
		client->response(server.getName(), ERR_NEEDMOREPARAMS,
							client->getName() + " NICK " + ERR_NEEDMOREPARAMS_MESSAGE);		
		return ;
	}

	std::string nickname = p.front();


	if (!user->isNicknameValid(nickname)) {
		client->response(server.getName(), ERR_ERRONEUSNICKNAME,
							client->getName() + ' ' + user->getNickname()
							+ " " ERR_ERRONEUSNICKNAME_MESSAGE);
		return ;
	}

	if (clientReg.getClientByNick(nickname)) {
		client->response(server.getName(), ERR_NICKNAMEINUSE,
							client->getName() + ' ' + user->getNickname()
							+ " " ERR_NICKNAMEINUSE_MESSAGE);	
		return ;
	}

	user->setNickname(p.front());

	user->registerUser(server.getCreationTime(), server.getVersion());
}
