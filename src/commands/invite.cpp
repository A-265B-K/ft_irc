#include <EventLoop.hpp>

void EventLoop::invite(Client *client, const std::deque<std::string>& p)
{
	User *user = client->getUser();
	if (!user->isRegistered()) {
		client->response(server.getName(), ERR_NOTREGISTERED,
							client->getName() + ' ' + 
							ERR_NOTREGISTERED_MESSAGE);
		return ;
	}
	if (p.size() < 2) {
		client->response(server.getName(), ERR_NEEDMOREPARAMS,
							client->getName() + 
							 " INVITE " + 
							 ERR_NEEDMOREPARAMS_MESSAGE);		
		return ;
	}

	std::string	channelName = p[1];

	Client	*invitee = clientReg.getClientByNick(p.front());
	Channel	*channel = channelReg.getChannel(channelName);

	if (!invitee) {
		return ; /* For some reason no error response is provided in the IRC documentation */
	}
	if (!channel) {
		client->response(server.getName(), ERR_NOSUCHCHANNEL,
							client->getName() + ' ' + 
							channelName + ' ' + 
							ERR_NOSUCHCHANNEL_MESSAGE);
		return ;		
	}
	if (!channel->hasClient(client)) {
		client->response(server.getName(), ERR_NOTONCHANNEL,
							client->getName() + ' ' + 
							channelName + ' ' + 
							ERR_NOTONCHANNEL_MESSAGE);
		return ;		
	}
	if (!channel->isOperator(client)) {
		client->response(server.getName(), ERR_CHANOPRIVSNEEDED,
							client->getName() + ' ' + 
							channelName + ' ' + 
							ERR_CHANOPRIVSNEEDED_MESSAGE);
		return ;
	}
	if (channel->hasClient(invitee)) {
		client->response(server.getName(), ERR_USERONCHANNEL,
							client->getName() + ' ' + 
							channelName + ' ' + 
							invitee->getUser()->getNickname() + ' ' + 
							ERR_USERONCHANNEL_MESSAGE);
		return ;		
	}

	/* Sending confirmation to the requester */
	client->response(server.getName(), RPL_INVITING,
					client->getName() + ' ' +
					invitee->getUser()->getNickname() + ' ' +
					channelName);

	/* Sending invite to invitee */
	std::string msg;
	for (const std::string& p1 : p)
		msg += ' ' + p1;
	invitee->replyTo(client->getName(), "INVITE", msg);	
}
