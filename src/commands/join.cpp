#include <EventLoop.hpp>

// TODO
void EventLoop::join(Client *client, const Message& message)
{
	std::deque<std::string> p = message.getParameters();

	/* Check if parameters are valid */
	if (p.size() != 1) {
		return ;
	}

	if (channelReg.joinChannel(p.front(), client)) {
		return ;
	} else {
		// TODO: send error message to client, invalid channel name
	}
/*
  Help can be found (I hope) in `command_help.txt'.

  Ideally, some descriptive account of the server's doings should be logged.
*/
}
