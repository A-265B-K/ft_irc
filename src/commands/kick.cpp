#include <EventLoop.hpp>

// TODO
void EventLoop::kick(Client *client, const std::deque<std::string>& p)
{
	User *user = client->getUser();
	if (!user->isRegistered()) {
		client->response(server.getName(), ERR_NOTREGISTERED,
							 user->getNickname() + ' ' + ERR_NOTREGISTERED_MESSAGE);
		return ;
	}
	if (p.size() < 2) {
		client->response(server.getName(), ERR_NEEDMOREPARAMS,
							 user->getNickname() + " KICK " + ERR_NEEDMOREPARAMS_MESSAGE);		
		return ;
	}

	std::string	channelName = p.front();
	Channel 	*channel = channelReg.getChannel(p.front());
	Client		*kickedClient = clientReg.getRegisteredClientByNick(p.back());

	if (!channel) {
		// TODO: Send error message
		return ;
	}
	if (!kickedClient) {
		// TODO: send error message
		return ;
	}
	if (!channel->isOperator(client)) {
		// TODO: Send error message
		return ;
	}


	channelReg.partChannel(p.front(), kickedClient);

	std::ostringstream message;
	message << kickedClient->getUser()->getUsername() << " was kicked by " << client->getUser()->getUsername();
	::printMessage(message.str());
}
