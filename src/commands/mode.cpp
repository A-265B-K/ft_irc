#include <Channel.hpp>
#include <EventLoop.hpp>
#include <Target.hpp>
#include <User.hpp>

void EventLoop::mode(Client *client, const std::deque<std::string>& p)
{
	const std::string nick = client->getUser()->getNickname();
	User *const user = client->getUser();

	if (p.empty()) {
		client->response(
				server.getName(),
				ERR_NEEDMOREPARAMS,
				"MODE :Not enough parameters"
		);

		return;
	}

	std::deque<std::string>::const_iterator pcit = p.begin();
	const std::list<Target> listOfTargets{Target::markTargets(*pcit++)};
	const std::string modestring{pcit == p.end() ? "" : *pcit++};

	for (const Target& t : listOfTargets)
		if (t.type == TargetType::REGULAR_CHANNEL || t.type == TargetType::LOCAL_CHANNEL) {
			Channel *const targetChannel = channelReg.getChannel(t.str);

			if (!targetChannel)
				client->response(
						server.getName(),
						ERR_NOSUCHCHANNEL,
						nick + ' ' + t.str + " :No such channel"
				);	
			else if (modestring.empty()) {
				std::string reply(nick + ' ' + t.str + ' ' + targetChannel->getModestring());
				while (pcit != p.end())
					reply += ' ' + *pcit++;

				client->response(
						server.getName(),
						RPL_CHANNELMODEIS,
						reply
				);
				// RPL_CREATIONTIME (329) not sent because time functions not allowed by subject
			}
			else
				; // TODO
		}
		else {
			Client *const targetClient = clientReg.getRegisteredClientByNick(t.str);

			if (!targetClient)
				client->response(
						server.getName(),
						ERR_NOSUCHNICK,
						nick + ' ' + t.str + " :No such nick"
				);
			else if (t.str != client->getUser()->getNickname())
				client->response(
						server.getName(),
						ERR_USERSDONTMATCH,
						nick + " :Can't change mode for other users"
				);
			else if (modestring.empty())
				client->response(
						server.getName(),
						RPL_UMODEIS,
						nick + ' ' + (user ? user->getModestring() : "")
				);
			else
				; // TODO
		}
//	::printMessage("MODE");
}
