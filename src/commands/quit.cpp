#include <EventLoop.hpp>

// TODO
void EventLoop::quit(Client *client, const std::deque<std::string>& p)
{
	std::string msg = client->getName() + " QUIT :Quit: ";

	if (p.size() > 0) {
		std::deque<std::string>::const_iterator it = p.begin();
		while (it != p.end()) msg += *it++;
		msg += CRLF;
	}
	
	// TODO send ERROR ti client
	this->markClientForRemoval(client);
	::printMessage("QUIT");
}
