#include <EventLoop.hpp>

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
							p.front() + ' ' +
							ERR_NOSUCHCHANNEL_MESSAGE);
		return ;
	}
	if (!channel->hasClient(client)) {
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
							channel->getName() + " :" + 
							channel->getTopic());
			client->response(server.getName(), RPL_TOPICWHOTIME,
							user->getNickname() + ' ' + 
							channel->getName() + ' ' + 
							channel->getTopicSetter() + ' ' +
							channel->getTopicTime());
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

	const std::string oldTopic = channel->getTopic();
	if (p[1] != oldTopic) {
		channel->setTopic(client, p[1]);
		std::cout << BLUE << "SET " << YELLOW << channel->getName() << BLUE << " TOPIC TO " << YELLOW << p[1] << RESET << std::endl;
		channel->broadcast(client, "TOPIC", ":" + p[1], true);
	}
}
