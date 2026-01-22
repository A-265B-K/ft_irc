#include <EventLoop.hpp>

void EventLoop::invite(Client *client, const std::deque<std::string>& p)
{
	User *user = client->getUser();
	if (!user->isRegistered()) {
		client->response(server.getName(), ERR_NOTREGISTERED,
							 user->getNickname() + ' ' + 
							 ERR_NOTREGISTERED_MESSAGE);
		return ;
	}
	if (p.size() < 2) {
		client->response(server.getName(), ERR_NEEDMOREPARAMS,
							 user->getNickname() + 
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
							 user->getNickname() + ' ' + 
							 channelName + ' ' + 
							 ERR_NOSUCHCHANNEL_MESSAGE);
		return ;		
	}
	if (!channel->isClientOn(client)) {
		client->response(server.getName(), ERR_NOTONCHANNEL,
							 user->getNickname() + ' ' + 
							 channelName + ' ' + 
							 ERR_NOTONCHANNEL_MESSAGE);
		return ;		
	}
	if (!channel->isOperator(client)) {
		client->response(server.getName(), ERR_CHANOPRIVSNEEDED,
							 user->getNickname() + ' ' + 
							 channelName + ' ' + 
							 ERR_CHANOPRIVSNEEDED_MESSAGE);
		return ;
	}
	if (channel->isClientOn(invitee)) {
		client->response(server.getName(), ERR_USERONCHANNEL,
							 user->getNickname() + ' ' + 
							 channelName + ' ' + 
							 invitee->getUser()->getNickname() + ' ' + 
							 ERR_USERONCHANNEL_MESSAGE);
		return ;		
	}

	/* Sending confirmation to the requester */
	client->response(server.getName(), RPL_INVITING,
					user->getNickname() + ' ' +
					invitee->getUser()->getNickname() + ' ' +
					channelName);

	/* Sending invite to invitee */
	std::deque<std::string>::const_iterator it = p.begin();
	std::string msg = user->getNickname() + " INVITE ";
	while (it != p.end()) msg += *it++; msg += "\r\n";
	client->handleWritable(Message(msg));
}
