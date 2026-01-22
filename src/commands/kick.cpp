#include <EventLoop.hpp>

void EventLoop::kick(Client *client, const std::deque<std::string>& p)
{
	User *user = client->getUser();
	if (!user->isRegistered()) {
		client->response(server.getName(), ERR_NOTREGISTERED,
							client->getName() + ' ' + ERR_NOTREGISTERED_MESSAGE);
		return ;
	}
	if (p.size() < 2) {
		client->response(server.getName(), ERR_NEEDMOREPARAMS,
							client->getName() + " KICK " + ERR_NEEDMOREPARAMS_MESSAGE);		
		return ;
	}

	std::string	channelName = p.front();
	Channel 	*channel = channelReg.getChannel(p.front());
	Client		*kickedClient = clientReg.getClientByNick(p[1]);

	if (!channel) {
		client->response(server.getName(), ERR_NOSUCHCHANNEL,
						client->getName() + ' ' + channel->getName()
						+ ' ' + ERR_NOSUCHCHANNEL_MESSAGE);
		return ;
	}
	if (!channel->hasClient(client)) {
		client->response(server.getName(), ERR_NOTONCHANNEL,
						client->getName() + ' ' + channel->getName()
						+ ' ' + ERR_NOTONCHANNEL_MESSAGE);
		return ;
	}
	if (!channel->isOperator(client)) {
		client->response(server.getName(), ERR_CHANOPRIVSNEEDED,
				client->getName() + ' ' + channel->getName()
				+ ' ' + ERR_CHANOPRIVSNEEDED_MESSAGE);
		return ;
	}
	if (!channel->hasClient(kickedClient)) {
		client->response(server.getName(), ERR_USERNOTINCHANNEL,
						client->getName() + ' ' + 
						p[1] + ' ' + channel->getName()
						+ ' ' + ERR_USERNOTINCHANNEL_MESSAGE);
		return ;
	}

	std::string kickMessage = client->getName() + " KICK " + p[0] + ' ' + p[1] + "\r\n";
	kickedClient->handleWritable(Message(kickMessage));
	channelReg.partChannel(p.front(), kickedClient);
	std::ostringstream message;
	message << BLUE << client->getUser()->getNickname() << " KICKED " << kickedClient->getUser()->getNickname() ;
	::printMessage(message.str());
}
