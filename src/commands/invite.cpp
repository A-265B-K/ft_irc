#include <EventLoop.hpp>

// TODO
void EventLoop::invite(Client *client, const std::deque<std::string>& p)
{
	(void)client;
	(void)p;
/*
  Help can be found (I hope) in `command_help.txt'.

  Ideally, some descriptive account of the server's doings should be logged. As
  a placeholder...
*/
	::printMessage("INVITE");
}
