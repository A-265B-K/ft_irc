#include <EventLoop.hpp>

// TODO
void EventLoop::topic(Client *client, const std::deque<std::string>& p)
{
	User *user = client->getUser();
	if (!user->isRegistered()) {
		client->response(server.getName(), ERR_NOTREGISTERED,
							 user->getNickname() + ' ' + ERR_NOTREGISTERED_MESSAGE);
		return ;
	}
	if (p.size() < 2) {
		client->response(server.getName(), ERR_NEEDMOREPARAMS,
							 user->getNickname() + " TOPIC " + ERR_NEEDMOREPARAMS_MESSAGE);		
		return ;
	}
	(void)client;
	(void)p;
/*
  Help can be found (I hope) in `command_help.txt'.

  Ideally, some descriptive account of the server's doings should be logged. As
  a placeholder...
*/
	::printMessage("TOPIC");
}
