#include <EventLoop.hpp>

// TODO
void EventLoop::topic(Client *client, const std::deque<std::string>& p)
{
	User *user = client->getUser();
	if (!user->isRegistered()) {
		client->response(server.getName(), ERR_NOTREGISTERED,
							 user->getNickname() + ' ' + ERR_NOTREGISTERED_MESSAGE);
		return ;
	}
	if (p.size() < 1) {
		client->response(server.getName(), ERR_NEEDMOREPARAMS,
							user->getNickname() + ' ' + ERR_NEEDMOREPARAMS_MESSAGE);
		return ;
	}

	Channel *channel = channelReg.getChannel(p.front());
	if (!channel) {
			client->response(server.getName(), ERR_NOSUCHCHANNEL,
							user->getNickname() + ' ' + 
							channel->getName() + ' ' +
							ERR_NOSUCHCHANNEL_MESSAGE);
		return ;
	}
	if (!channel->isClientOn(client)) {
			client->response(server.getName(), ERR_NOTONCHANNEL,
							user->getNickname() + ' ' + 
							channel->getName() + ' ' +
							ERR_NOTONCHANNEL_MESSAGE);
		return ;
	}

	if (p.size() == 1) {
		std::string topic = channel->getTopic();
		if (topic.empty()) {
			client->response(server.getName(), RPL_NOTOPIC,
							user->getNickname() + ' ' + 
							channel->getName() + ' ' + 
							RPL_NOTOPIC_MESSAGE);
			return ;
		} else {
			client->response(server.getName(), RPL_TOPIC,
							user->getNickname() + ' ' + 
							channel->getName() + ' ' +
							topic);
			return ;
		}
	}
	if (channel->topicRequiresOperator() && !channel->isOperator(client)) {
		client->response(server.getName(), ERR_CHANOPRIVSNEEDED,
						user->getNickname() + ' ' + 
						channel->getName() + ' ' +
						ERR_CHANOPRIVSNEEDED_MESSAGE);
		return ;
	}

	// TODO make to topic is entire string
	channel->setTopic(client, p.back());
	::printMessage("Set " + channel->getName() + " topic to " + p.back() );

	std::set<Client *> clients = channel->getClients();

	for (auto it : clients) {
		Client *recipient = it;
		channel->sendTopic(recipient);
	}
}
