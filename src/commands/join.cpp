#include <EventLoop.hpp>

void EventLoop::join(Client *client, const std::deque<std::string>& p)
{
	// TODO SPLIT CHANNEL AND PASSWORDS
	User *user = client->getUser();
	if (!user->isRegistered()) {
		client->response(server.getName(), ERR_NOTREGISTERED,
							 user->getNickname() + ' ' + ERR_NOTREGISTERED_MESSAGE);
		return ;
	}
	if (p.size() < 1) {
		client->response(server.getName(), ERR_NEEDMOREPARAMS,
							 user->getNickname() + " JOIN " + ERR_NEEDMOREPARAMS_MESSAGE);		
		return ;
	}

	std::string channelName = p.front();
	std::string password;

	p.size() == 2 ? password = p.back() : nullptr;

	switch (channelReg.joinChannel(channelName, client, password)) {
		case (1):
			// TODO send topic
		case (-1):
			client->response(server.getName(), ERR_NOSUCHCHANNEL,
							 user->getNickname() + ' ' + channelName + ' ' + ERR_NOSUCHCHANNEL_MESSAGE);
		case (-2):
			client->response(server.getName(), ERR_BADCHANNELKEY,
							 user->getNickname() + ' ' + channelName + ' ' + ERR_BADCHANNELKEY_MESSAGE);
		default:
			return ;
	}
}
