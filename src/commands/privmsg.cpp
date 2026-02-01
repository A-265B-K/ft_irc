# include <EventLoop.hpp>
# include <set>
# include <string>


void EventLoop::privmsg(Client *client, const std::deque<std::string>& p)
{
	User *user = client->getUser();
	if (!user->isRegistered()) {
		client->response(server.getName(), ERR_NOTREGISTERED,
							 user->getNickname() + ' ' + ERR_NOTREGISTERED_MESSAGE);
		return ;
	}
	if (p.size() < 2) {
		client->response(server.getName(), ERR_NEEDMOREPARAMS,
							 user->getNickname() + " PRIVMSG " + ERR_NEEDMOREPARAMS_MESSAGE);		
		return ;
	}

	std::stringstream		sstreamParams(p.front());
	std::string				message{":"};
	std::string				tmp;

	Channel *channel;
	Client	*recipient;

	message += p[1];
	/* Split parameters up, they're structured as 'channel,channel', 'user,user', 'channel,user' and so forth */
	while (std::getline(sstreamParams, tmp, ',')) {
		if (tmp.empty()) {
			continue ;
		}
		/* If the parameters is a channel, check if it exists, if so request the list of users subscribed to that channel and add it to the users set */
		if (channelReg.isValidChannelName(tmp)) {
			channel = this->channelReg.getChannel(tmp);
			if (channel) {
				channel->broadcast(client, "PRIVMSG", message);
			} else {
				client->response(server.getName(), ERR_NOSUCHCHANNEL,
					user->getNickname() + ' ' + 
					tmp + ' ' + 
					ERR_NOSUCHCHANNEL_MESSAGE);
			}
		} else {
			recipient = this->clientReg.getClientByNick(tmp);
			if (recipient && recipient->getUser()->isRegistered()) {
				recipient->replyTo(user->getNickname(), "PRIVMSG",
											recipient->getUser()->getNickname() + ' ' + message);
			} else {
				client->response(server.getName(), ERR_NOSUCHNICK,
					user->getNickname() + ' ' +
					tmp + ' ' +
					ERR_NOSUCHNICK_MESSAGE);
			}
		}
	}
}
