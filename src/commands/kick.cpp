#include <EventLoop.hpp>

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
		client->response(server.getName(), ERR_NOSUCHCHANNEL,
						user->getNickname() + ' ' + channel->getName()
						+ ' ' + ERR_NOSUCHCHANNEL_MESSAGE);
		return ;
	}
	if (!channel->isClientOn(client)) {
		client->response(server.getName(), ERR_NOTONCHANNEL,
						user->getNickname() + ' ' + channel->getName()
						+ ' ' + ERR_NOTONCHANNEL_MESSAGE);
		return ;
	}
	if (!channel->isOperator(client)) {
		client->response(server.getName(), ERR_CHANOPRIVSNEEDED,
				user->getNickname() + ' ' + channel->getName()
				+ ' ' + ERR_CHANOPRIVSNEEDED_MESSAGE);
		return ;
	}
	if (!channel->isClientOn(kickedClient)) {
		client->response(server.getName(), ERR_USERNOTINCHANNEL,
						user->getNickname() + ' ' + 
						p.back() + ' ' + channel->getName()
						+ ' ' + ERR_USERNOTINCHANNEL_MESSAGE);
		return ;
	}

	channelReg.partChannel(p.front(), kickedClient);
	std::ostringstream message;
	message << kickedClient->getUser()->getUsername() << " was kicked by " << client->getUser()->getUsername();
	::printMessage(message.str());
}
