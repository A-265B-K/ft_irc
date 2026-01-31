#include <EventLoop.hpp>

void EventLoop::part(Client *client, const std::deque<std::string>& p)
{
	User *user = client->getUser();
	if (!user->isRegistered()) {
		client->response(server.getName(), ERR_NOTREGISTERED,
							 user->getNickname() + ' ' + ERR_NOTREGISTERED_MESSAGE);
		return ;
	}
	if (!p.size()) {
		client->response(server.getName(), ERR_NEEDMOREPARAMS,
							 user->getNickname() + " PART " + ERR_NEEDMOREPARAMS_MESSAGE);		
		return ;
	}

	std::stringstream		sstreamParams(p.front());
	const std::string		reason{p.size() > 1 ? p[1] : ""};
	std::string				tmp;
	Channel					*channel;

	while (std::getline(sstreamParams, tmp, ',')) {
		if (tmp.empty()) {
			continue ;
		}
		channel = this->channelReg.getChannel(tmp);
		if (!channel) {
			client->response(server.getName(), ERR_NOSUCHCHANNEL,
							user->getNickname() + ' ' + 
							tmp + ' ' + 
							ERR_NOSUCHCHANNEL_MESSAGE);
			continue ;
		}
		if (!channel->hasClient(client)) {
			client->response(server.getName(), ERR_NOTONCHANNEL,
							user->getNickname() + ' ' + channel->getName()
							+ ' ' + ERR_NOTONCHANNEL_MESSAGE);
			continue ;
		}

		tmp = reason.empty() ? "" : (" :" + reason);
		client->replyTo(server.getName(), "PART", channel->getName() + tmp);
		channel->broadcast(client, "PART", tmp);
		this->channelReg.partChannel(channel->getName(), client);
	}
}
