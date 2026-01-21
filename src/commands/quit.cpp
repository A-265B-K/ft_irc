#include <EventLoop.hpp>

// TODO
void EventLoop::quit(Client *client, const std::deque<std::string>& p)
{
	// TODO send message to channels member was a part of
	(void)p;

	// TODO send ERROR
	this->markClientForRemoval(client);
/*
  Help can be found (I hope) in `command_help.txt'.

  Ideally, some descriptive account of the server's doings should be logged. As
  a placeholder...
*/
	::printMessage("QUIT");
}
