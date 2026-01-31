#include <EventLoop.hpp>
#include <static_declarations.hpp>

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

	if (!channel) {
		client->response(server.getName(), ERR_NOSUCHCHANNEL,
						user->getNickname() + ' ' + p.front()
						+ ' ' + ERR_NOSUCHCHANNEL_MESSAGE);
		return ;
	}
	if (!channel->hasClient(client)) {
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

	std::istringstream users(p[1]);
	const std::string reason{p.size() > 2 ? p[2] : KICK_DEFAULT_REASON};
	std::string target;
	std::deque<Client *> kickedClients;

	while (std::getline(users, target, ',')) {
		Client		*kickedClient = clientReg.getClientByNick(target);

		if (!channel->hasClient(kickedClient)) {
			client->response(server.getName(), ERR_USERNOTINCHANNEL,
							client->getName() + ' ' + 
							target + ' ' + channel->getName()
							+ ' ' + ERR_USERNOTINCHANNEL_MESSAGE);
			continue ;
		}

		channel->broadcast(client, "KICK", target + " :" + reason);
		kickedClients.emplace_back(kickedClient);
		std::ostringstream message;
		message << BLUE << client->getUser()->getNickname() << " KICKED " << kickedClient->getUser()->getNickname() ;
		::printMessage(message.str());
	}
	for (Client *const kickedClient : kickedClients)
		channelReg.partChannel(p.front(), kickedClient);
}
