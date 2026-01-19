#include <EventLoop.hpp>

// TODO
void EventLoop::user(Client *client, const std::deque<std::string>& p)
{
	/* Check if the correct amount of parameters are present */
	if (p.size() < 4) {
		// TODO: send error message
		return ;
	}


	User *user = client->getUser();

	/* Send an error message if the user is already registered */
	if (user->isRegistered() == true) {
		// TODO: send error message
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
