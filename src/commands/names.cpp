#include <EventLoop.hpp>
#include <Target.hpp>

#include <utility>

static void printMembers(const Client *client, const std::string& serverName,
		const std::string& nickname, const Channel* channel)
{
	std::string namesList;

	channel->forEachClient([&](const std::pair<Client *, int>& p) {
		namesList += (namesList.empty() ? ':' : ' ') + p.first->getUser()->getNickname();
	});

	// '=' Public channel
	client->response(
			serverName,
			RPL_NAMREPLY,
			nickname + " = " + channel->getName() + ' ' + namesList
	);
	client->response(
			serverName,
			RPL_ENDOFNAMES,
			nickname + ' ' + channel->getName() + " " RPL_ENDOFNAMES_MESSAGE
	);	
}

void EventLoop::names(Client *client, const std::deque<std::string>& p)
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

	const std::string& nick = user->getNickname();

	// No target list given
	if (p.empty()) {
// TODO
	}

	// Specific target channels
	else {
		::printMessage(nick + " requested membership information of channel(s) " + p[0]);
		const std::list<Target> listOfTargets{Target::markTargets(p[0])};

		for (const Target& t : listOfTargets) {
			const Channel *const targetChannel = channelReg.getChannel(t.str);

			if (!targetChannel)
				client->response(
						server.getName(),
						RPL_ENDOFNAMES,
						nick + ' ' + t.str + " " RPL_ENDOFNAMES_MESSAGE
				);	
			else
				printMembers(client, server.getName(), nick, targetChannel);
		}
	}
}
