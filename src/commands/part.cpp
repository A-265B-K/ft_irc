#include <EventLoop.hpp>

// TODO
void EventLoop::part(Client *client, const std::deque<std::string>& p)
{
	std::stringstream		sstreamParams(p.front());
	std::stringstream		sstreamMessage;
	std::string				tmp;
	Channel					*channel;

	for (size_t i = 0; i < p.size(); ++i) {
		if (i != 0) sstreamMessage << p[i];
	}

	/* Split parameters up, they're structured as 'channel,channel', 'user,user', 'channel,user' and so forth */
	while (std::getline(sstreamParams, tmp, ',')) {
		if (tmp.empty()) {
			continue ;
		}
		/* If the parameters is a channel, check if it exists, if so request the list of users subscribed to that channel and add it to the users set */
		channel = this->channelReg.getChannel(tmp);
		if (channel) {
			if (this->channelReg.partChannel(tmp, client)) {
					// TODO: send succesfull part message
			} else {
				// TODO SEND ERR_NOTONCHANNEL
			}
		} else {
			// TODO ERR_NOSUCHCHANNEL:
		}
	}
}
