#include <EventLoop.hpp>
#include <Target.hpp>

#include <list>
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
		::printMessage(nick + " requested membership information from all channels");

		// print info from all (public) channels
		channelReg.forEachChannel([&](const std::pair<std::string, Channel *>& p) {
				printMembers(client, server.getName(), nick, p.second);
		});

		// allocate all clients with no channel membership into a spurious `*' channel
		std::list<Client *> noChannelClients;

		clientReg.forEachClient([&](Client *const client) {
				if (channelReg.getClientChannels(client).empty())
					noChannelClients.push_front(client);
		});
		if (!noChannelClients.empty()) {
			std::string namesList;

			for (Client *const c : noChannelClients)
				namesList += (namesList.empty() ? ':' : ' ')
						+ c->getUser()->getNickname();

			client->response(
					server.getName(),
					RPL_NAMREPLY,
					nick + " = * " + namesList
			);
			client->response(
					server.getName(),
					RPL_ENDOFNAMES,
					nick + " * " RPL_ENDOFNAMES_MESSAGE
			);
		}
	}

	// Specific target channels
	else {
		::printMessage(nick + " requested membership information from channel(s) "
				+ p.front());
		const std::list<Target> listOfTargets{Target::markTargets(p.front())};

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
