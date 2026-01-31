#include <EventLoop.hpp>
#include <Target.hpp>

void EventLoop::join(Client *client, const std::deque<std::string>& p)
{
	User *user = client->getUser();
	if (!user->isRegistered()) {
		client->response(server.getName(), ERR_NOTREGISTERED,
							 user->getNickname() + ' ' + ERR_NOTREGISTERED_MESSAGE);
		return ;
	}
	if (p.size() < 1) {
		client->response(server.getName(), ERR_NEEDMOREPARAMS,
							 user->getNickname() + " JOIN " + ERR_NEEDMOREPARAMS_MESSAGE);		
		return ;
	}

	const std::list<Target> listOfTargets{Target::markTargets(p.front())};
	std::istringstream passwords{p.size() >= 2 ? p[1] : ""};

	for (const Target& t : listOfTargets) {
		if ((t.type != TargetType::REGULAR_CHANNEL && t.type != TargetType::LOCAL_CHANNEL)
				|| t.str.find(0x07) != std::string::npos) {
			client->response(
					server.getName(),
					ERR_BADCHANMASK,
					user->getNickname() + ' ' + t.str + " " ERR_BADCHANMASK_MESSAGE
			);

			continue;
			}

		std::string key;
		std::getline(passwords, key, ',');

		switch (channelReg.joinChannel(t.str, client, key)) {
			case (1):
				channelReg.getChannel(t.str)->sendTopic(client);
				break ;
			case (-1):
				client->response(server.getName(), ERR_NOSUCHCHANNEL,
						user->getNickname() + ' ' + t.str + ' ' + ERR_NOSUCHCHANNEL_MESSAGE);
				break ;
			case (-2):
				client->response(server.getName(), ERR_BADCHANNELKEY,
						user->getNickname() + ' ' + t.str + ' ' + ERR_BADCHANNELKEY_MESSAGE);
				break ;
			case (-3):
				client->response(server.getName(), ERR_INVITEONLYCHAN,
						user->getNickname() + ' ' + t.str + " " ERR_INVITEONLYCHAN_MESSAGE);
				break;
			case (-4):
				client->response(server.getName(), ERR_CHANNELISFULL,
						user->getNickname() + ' ' + t.str + " " ERR_CHANNELISFULL_MESSAGE);
				break;
			default:
				break ;
		}
	}
}
