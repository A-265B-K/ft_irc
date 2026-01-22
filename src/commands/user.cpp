#include <EventLoop.hpp>

void EventLoop::user(Client *client, const std::deque<std::string>& p)
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

	if (p.size() < 4) {
		client->response(server.getName(), ERR_NEEDMOREPARAMS,
							 client->getName() + " PRIVMSG " + ERR_NEEDMOREPARAMS_MESSAGE);		
		return ;
	}

	std::deque<std::string>::const_iterator it = p.begin();
	/* Set username, hostname, servername and realname */
	*it == "=" ? user->setUsername("anon") : user->setUsername(*it);
	it++;

	user->setHostname(client->getAddress());
	it++;

	user->setServername(server.getName());
	it++;

	std::string realname = "";
	while (it != p.end())
		realname += *it++;

	user->setRealname(realname);
	user->isRegistered();
}
