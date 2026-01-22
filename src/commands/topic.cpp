#include <EventLoop.hpp>

// TODO
void EventLoop::topic(Client *client, const std::deque<std::string>& p)
{
	User *user = client->getUser();
	if (!user->isRegistered()) {
		client->response(server.getName(), ERR_NOTREGISTERED,
							 client->getName() + ' ' + ERR_NOTREGISTERED_MESSAGE);
		return ;
	}
	if (p.size() < 1) {
		client->response(server.getName(), ERR_NEEDMOREPARAMS,
							client->getName() + ' ' + ERR_NEEDMOREPARAMS_MESSAGE);
		return ;
	}

	Channel *channel = channelReg.getChannel(p.front());
	if (!channel) {
			client->response(server.getName(), ERR_NOSUCHCHANNEL,
							client->getName() + ' ' + 
							p.front() + ' ' +
							ERR_NOSUCHCHANNEL_MESSAGE);
		return ;
	}
	if (!channel->hasClient(client)) {
			client->response(server.getName(), ERR_NOTONCHANNEL,
							client->getName() + ' ' + 
							channel->getName() + ' ' +
							ERR_NOTONCHANNEL_MESSAGE);
		return ;
	}

	if (p.size() == 1) {
		std::string topic = channel->getTopic();
		if (topic.empty()) {
			client->response(server.getName(), RPL_NOTOPIC,
							client->getName() + ' ' + 
							channel->getName() + ' ' + 
							RPL_NOTOPIC_MESSAGE);
			return ;
		} else {
			channel->sendTopic(client);
			return ;
		}
	}
	if (channel->topicRequiresOperator() && !channel->isOperator(client)) {
		client->response(server.getName(), ERR_CHANOPRIVSNEEDED,
						client->getName() + ' ' + 
						channel->getName() + ' ' +
						ERR_CHANOPRIVSNEEDED_MESSAGE);
		return ;
	}

	std::deque<std::string>::const_iterator it = p.begin();
	it++;
	std::string topic = "";
	while (it != p.end())
		topic += *it++;

	channel->setTopic(client, topic);
	std::cout << BLUE << "SET " << YELLOW << channel->getName() << BLUE << " TOPIC TO " << YELLOW << topic << RESET << std::endl;

	std::set<Client *> clients = channel->getClients();
	for (auto it : clients) {
		Client *recipient = it;
		channel->sendTopic(recipient);
	}
}
