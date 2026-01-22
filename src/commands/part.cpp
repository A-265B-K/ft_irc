#include <EventLoop.hpp>

void EventLoop::part(Client *client, const std::deque<std::string>& p)
{
	User *user = client->getUser();
	if (!user->isRegistered()) {
		client->response(server.getName(), ERR_NOTREGISTERED,
							 user->getNickname() + ' ' + ERR_NOTREGISTERED_MESSAGE);
		return ;
	}
	if (p.size() < 2) {
		client->response(server.getName(), ERR_NEEDMOREPARAMS,
							 user->getNickname() + " PART " + ERR_NEEDMOREPARAMS_MESSAGE);		
		return ;
	}

	std::stringstream		sstreamParams(p.front());
	std::stringstream		sstreamMessage;
	std::string				tmp;
	Channel					*channel;

	for (size_t i = 0; i < p.size(); ++i) {
		if (i != 0) sstreamMessage << p[i];
	}

	/* TODO Split parameters up, they're structured as 'channel,channel', 'user,user', 'channel,user' and so forth */
	while (std::getline(sstreamParams, tmp, ',')) {
		if (tmp.empty()) {
			continue ;
		}
		channel = this->channelReg.getChannel(tmp);
		if (!channel) {
			client->response(server.getName(), ERR_NOSUCHCHANNEL,
							user->getNickname() + ' ' + channel->getName()
							+ ' ' + ERR_NOSUCHCHANNEL_MESSAGE);
			continue ;
		}
		if (!channel->hasClient(client)) {
			client->response(server.getName(), ERR_NOTONCHANNEL,
							user->getNickname() + ' ' + channel->getName()
							+ ' ' + ERR_NOTONCHANNEL_MESSAGE);
			continue ;
		}
		
		this->channelReg.partChannel(tmp, client);
	}
}
