#include <Channel.hpp>
#include <EventLoop.hpp>
#include <Target.hpp>
#include <User.hpp>

void EventLoop::mode(Client *client, const std::deque<std::string>& p)
{
	User *const user = client->getUser();

	if (!user || !user->isRegistered()) {
			::printMessage("MODE requested from unregistered client");

			return;
	}

	if (p.empty()) {
		client->response(
				server.getName(),
				ERR_NEEDMOREPARAMS,
				"MODE :Not enough parameters"
		);

		return;
	}

	const std::string nick = user->getNickname();
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
			//	std::string reply(nick + ' ' + t.str + ' ' + targetChannel->getModestring());
			//	while (pcit != p.end())
			//		reply += ' ' + *pcit++;

				::printMessage("MODE information requested on channel " + t.str);
			//	client->response(
			//			server.getName(),
			//			RPL_CHANNELMODEIS,
			//			reply
			//	);
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
			else if (t.str != nick)
				client->response(
						server.getName(),
						ERR_USERSDONTMATCH,
						nick + " :Can't " + (modestring.empty() ? "view": "change") + " modes for other users"
				);
			else if (modestring.empty()) {
				::printMessage("MODE information requested by client " + nick);

				client->response(
						server.getName(),
						RPL_UMODEIS,
						nick + ' ' + user->getModestring()
				);
			}
			else {
				::printMessage("MODE edition requested by client " + nick);

				std::string changedModes;
				const int unknownFlags = user->editModes(changedModes, modestring);

				client->replyTo(
					server.getName(),
					std::string("MODE ") + t.str + (changedModes.empty() ? "" : " ") + changedModes
				);
				if (unknownFlags)
					client->response(
							server.getName(),
							ERR_UMODEUNKNOWNFLAG,
							nick + " :Unknown MODE flag"
					);
			}
		}
}
