#include <EventLoop.hpp>

// TODO
void EventLoop::user(Client *client, const Message& message)
{
	std::deque<std::string> p = message.getParameters();

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

	/* Set username, hostname, servername and realname */
	p.front() == "=" ? user->setUsername("anon") : user->setUsername(p.front());
	p.pop_front();

	user->setHostname(client->getAddress());
	p.pop_front();

	p.front() == "=" ? user->setServername("noserv") : user->setServername(p.front());
	p.pop_front();

	std::string realname = "";
	for (const auto& s : p) {
		realname += s;
	}

	user->setRealname(realname);

	/* Register user */
	if (user->registerUser() == true) {
		client->response(server.getName(), 001, user->getNickname() + " :" + "Welcome to the IRC network");
	}
}
