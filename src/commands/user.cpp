#include <EventLoop.hpp>

// TODO
void EventLoop::user(Client *client, const std::deque<std::string>& p)
{
	/* Check if the correct amount of parameters are present */
	if (p.size() < 4) {
		// TODO: send error message
		return ;
	}
	

	/* If no User object exists yet, create one */
	if (client->isUser() == false) {
		client->createUser();
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

	*it == "=" ? user->setServername("noserv") : user->setServername(*it);
	it++;

	std::string realname = "";
	while (it != p.end())
		realname += *it++;

	user->setRealname(realname);

	/* Register user */
	if (user->registerUser() == true) {
		client->response(server.getName(), 001, user->getNickname() + " :" + "Welcome to the IRC network");
	}
}
