#include <Channel.hpp>
#include <EventLoop.hpp>
#include <function_declarations.hpp>
#include <static_declarations.hpp>
#include <Target.hpp>

#include <climits>
#include <utility>

void EventLoop::mode(Client *client, const std::deque<std::string>& p)
{
	User *const user = client->getUser();

	if (!user || !user->isRegistered()) {
		client->response(
				server.getName(),
				ERR_NOTREGISTERED,
				user->getNickname() + " " ERR_NOTREGISTERED_MESSAGE
		);

		return;
	}

	if (p.empty()) {
		client->response(
				server.getName(),
				ERR_NEEDMOREPARAMS,
				user->getNickname() + " MODE " ERR_NEEDMOREPARAMS_MESSAGE
		);

		return;
	}

	const std::string& nick = user->getNickname();
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
						nick + ' ' + t.str + " " ERR_NOSUCHCHANNEL_MESSAGE
				);	
			else if (modestring.empty()) {
				::printMessage("Channel mode information requested on " + t.str);

				client->response(
						server.getName(),
						RPL_CHANNELMODEIS,
						nick + ' ' + t.str + ' ' + targetChannel->getChannelModes(client)
				);
				client->response(
						server.getName(),
						RPL_CREATIONTIME,
						nick + ' ' + t.str + ' ' + targetChannel->getTimestamp()
				);
			}
			else {
				::printMessage("Channel mode edition requested on " + t.str);

				if (!targetChannel->hasClient(client)) {
					client->response(
						server.getName(),
						ERR_NOTONCHANNEL,
						nick + ' ' + t.str + " " ERR_NOTONCHANNEL_MESSAGE
					);
					continue;
				}
				if (!targetChannel->isOperator(client)) {
					client->response(
						server.getName(),
						ERR_CHANOPRIVSNEEDED,
						nick + ' ' + t.str + " " ERR_CHANOPRIVSNEEDED_MESSAGE
					);
					continue;
				}

				std::string changedModes;
				std::set<std::pair<char, std::string> > invalidParam;
				std::set<std::string> notInChannel;
				const unsigned result = targetChannel->editModes(
						changedModes, invalidParam, notInChannel, modestring, pcit, p.end());

				if (!changedModes.empty())
					targetChannel->broadcast(client, "MODE", changedModes, true);

				for (const std::pair<char, std::string>& badParam : invalidParam)
					client->response(
						server.getName(),
						ERR_INVALIDMODEPARAM,
						nick + ' ' + t.str + ' ' + badParam.first + ' ' + badParam.second
						+ (badParam.first == 'l' ?
								" :Integer in the range [0, " XSTR(INT_MAX) "] expected" :
								" :Owners may not downgrade")
					);
				for (const std::string& badNick : notInChannel)
					client->response(
						server.getName(),
						ERR_USERNOTINCHANNEL,
						nick + ' ' + badNick + ' ' + t.str + " " ERR_USERNOTINCHANNEL_MESSAGE
					);
				if (result & UNKNOWNFLAG)
					client->response(
							server.getName(),
							ERR_UMODEUNKNOWNFLAG,
							nick + " " ERR_UMODEUNKNOWNFLAG_MESSAGE
					);
				if (result & INVALIDKEY)
					client->response(
							server.getName(),
							ERR_INVALIDKEY,
							nick + ' ' + t.str + " " ERR_INVALIDKEY_MESSAGE
					);
			}
		}
		else { // User mode
			Client *const targetClient = clientReg.getClientByNick(t.str);

			if (!targetClient)
				client->response(
						server.getName(),
						ERR_NOSUCHNICK,
						nick + ' ' + t.str + " " ERR_NOSUCHNICK_MESSAGE
				);
			else if (t.str != nick)
				client->response(
						server.getName(),
						ERR_USERSDONTMATCH,
						nick + (modestring.empty() ?
								" " ERR_USERSDONTMATCH_MESSAGE_VAR :
								" " ERR_USERSDONTMATCH_MESSAGE)
				);
			else if (modestring.empty()) {
				::printMessage("User mode information requested by " + nick);

				client->response(
						server.getName(),
						RPL_UMODEIS,
						nick + ' ' + user->getModestring()
				);
			}
			else {
				::printMessage("User mode edition requested by " + nick);

				std::string changedModes;
				const unsigned result = user->editModes(changedModes, modestring);

				if (!changedModes.empty())
					client->replyTo(
						server.getName(),
						"MODE",
						t.str + ' ' + changedModes
					);

				if (result & UNKNOWNFLAG)
					client->response(
							server.getName(),
							ERR_UMODEUNKNOWNFLAG,
							nick + " " ERR_UMODEUNKNOWNFLAG_MESSAGE
					);
			}
		}
}
