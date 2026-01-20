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
	if (p.size() < 3) {
		client->response(server.getName(), ERR_NEEDMOREPARAMS,
							 user->getNickname() + " PRIVMSG " + ERR_NEEDMOREPARAMS_MESSAGE);		
		return ;
	}

	std::stringstream		sstreamParams(p.front());
	std::stringstream		sstreamMessage;
	std::string				tmp;

	Channel *channel;
	Client	*recipient;

	for (size_t i = 0; i < p.size(); ++i) {
		if (i != 0) sstreamMessage << p[i];
	}

	/* Split parameters up, they're structured as 'channel,channel', 'user,user', 'channel,user' and so forth */
	while (std::getline(sstreamParams, tmp, ',')) {
		if (tmp.empty()) {
			continue ;
		}
		/* If the parameters is a channel, check if it exists, if so request the list of users subscribed to that channel and add it to the users set */
		if (channelReg.isValidChannelName(tmp)) {
			channel = this->channelReg.getChannel(tmp);
			if (channel) {
				channel->broadcast(client, "PRIVMSG", sstreamMessage.str());
			} else {
				// TODO: send error code, channel does not exist.
			}
		} else { /* Check if the user exists and is registered, and then add it to the users set */
			recipient = this->clientReg.getRegisteredClientByNick(tmp);
			if (recipient) {
				// TODO: send private message
			} else {
				// TODO: send error code, user does not exist.
			}
		}
	}
}
