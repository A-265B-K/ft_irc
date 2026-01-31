#include <EventLoop.hpp>

void EventLoop::invite(Client *client, const std::deque<std::string>& p)
{
	User *user = client->getUser();
	const std::string nick = user->getNickname();

	if (!user->isRegistered()) {
		client->response(server.getName(), ERR_NOTREGISTERED,
							nick + ' ' + 
							ERR_NOTREGISTERED_MESSAGE);
		return ;
	}

	switch (p.size()) {
	case 1:
		client->response(server.getName(), ERR_NEEDMOREPARAMS,
						nick + 
						 " INVITE " + 
						 ERR_NEEDMOREPARAMS_MESSAGE);		
		return ;

	case 0:
		{
			std::deque<std::string> invitedChannels;

			channelReg.forEachChannel([&](const std::pair<std::string, Channel *>& p) {
				if (p.second->isInvited(client))
					invitedChannels.emplace_back(p.first);
			});
			for (const std::string& invitedChannel : invitedChannels)
				client->response(
						server.getName(),
						RPL_INVITELIST,
						nick + ' ' + invitedChannel
				);
			client->response(
					server.getName(),
					RPL_ENDOFINVITELIST,
					nick + " " RPL_ENDOFINVITELIST_MESSAGE
			);
		}
		return;

	case 2: default:
		std::string	channelName = p[1];

		Client	*invitee = clientReg.getClientByNick(p.front());
		Channel	*channel = channelReg.getChannel(channelName);

		if (!invitee) {
			return ; /* For some reason no error response is provided in the IRC documentation */
		}
		if (!channel) {
			client->response(server.getName(), ERR_NOSUCHCHANNEL,
						nick + ' ' + 
						channelName + ' ' + 
						ERR_NOSUCHCHANNEL_MESSAGE);
			return ;		
		}
		if (!channel->hasClient(client)) {
			client->response(server.getName(), ERR_NOTONCHANNEL,
						nick + ' ' + 
						channelName + ' ' + 
						ERR_NOTONCHANNEL_MESSAGE);
			return ;		
		}
		if (!channel->isOperator(client)) {
			client->response(server.getName(), ERR_CHANOPRIVSNEEDED,
						nick + ' ' + 
						channelName + ' ' + 
						ERR_CHANOPRIVSNEEDED_MESSAGE);
			return ;
		}
		if (channel->hasClient(invitee)) {
			client->response(server.getName(), ERR_USERONCHANNEL,
						nick + ' ' + 
						channelName + ' ' + 
						invitee->getUser()->getNickname() + ' ' + 
						ERR_USERONCHANNEL_MESSAGE);
			return ;		
		}

		/* Sending confirmation to the requester */
		client->response(server.getName(), RPL_INVITING,
				nick + ' ' +
				invitee->getUser()->getNickname() + ' ' +
				channelName);

		/* Sending invite to invitee */
		std::string msg;
		for (const std::string& p1 : p)
			msg += ' ' + p1;
		invitee->replyTo(client->getName(), "INVITE", msg);	

		/* Adding invitee to the invitations set */
		channel->addInvitee(invitee);
	}
}
