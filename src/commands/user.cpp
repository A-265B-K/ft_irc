#include <EventLoop.hpp>

void EventLoop::user(Client *client, const std::deque<std::string>& p)
{
	User *user = client->getUser();

	if (!user->getHasPassword()) {
		client->response(server.getName(), ERR_PASSWDMISMATCH,
						 user->getNickname() + ' ' + ERR_PASSWDMISMATCH_MESSAGE);
		this->markClientForRemoval(client);
		return ;
	}

	if (p.size() < 4) {
		client->response(server.getName(), ERR_NEEDMOREPARAMS,
							 user->getNickname() + " PRIVMSG " + ERR_NEEDMOREPARAMS_MESSAGE);		
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
