#include <EventLoop.hpp>

// TODO
void EventLoop::join(Client *client, const std::deque<std::string>& p)
{

	/* Check if parameters are valid */
	if (p.size() >= 1 || p.size() <= 2) {
		return ;
	}

	std::string password;

	p.size() == 2 ? password = p.back() : nullptr;

	if (channelReg.joinChannel(p.front(), client, password)) {
		return ;
	} else {
		// TODO: send error message to client, invalid channel name
	}
/*
  Help can be found (I hope) in `command_help.txt'.

  Ideally, some descriptive account of the server's doings should be logged.
*/
}
