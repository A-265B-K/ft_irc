#include <EventLoop.hpp>

void EventLoop::ping(Client *client, const std::deque<std::string>& p) {
	std::deque<std::string>::const_iterator it = p.begin();

	std::string returnStr = "PONG ";

	while (it != p.end())
		returnStr += *it++;

	returnStr += "\r\n";

	client->handleWritable(Message(returnStr));
}
