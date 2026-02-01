#include <EventLoop.hpp>
#include <Target.hpp>

#include <list>

void EventLoop::list(Client *client, const std::deque<std::string>& p)
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

	client->response(
			server.getName(),
			RPL_LISTSTART,
			nick + " " RPL_LISTSTART_MESSAGE
	);

	// No target list given
	if (p.empty()) {
		::printMessage(nick + " requested topic information from all channels");

		// print info from all (public) channels
		channelReg.forEachChannel([&](const std::pair<std::string, Channel *>& p) {
				const std::string& topic = p.second->getTopic();

				client->response(
						server.getName(),
						RPL_LIST,
						nick + ' ' + p.first + ' ' + std::to_string(p.second->memberCount())
								+ (topic.empty() ? "" : " :" + topic)
				);
		});
	}

	// Specific target channels
	else {
		::printMessage(nick + " requested topic information from channel(s) "
				+ p.front());
		const std::list<Target> listOfTargets{Target::markTargets(p.front())};

		for (const Target& t : listOfTargets) {
			const Channel *const targetChannel = channelReg.getChannel(t.str);

			if (targetChannel) { // No error response for non-existing channels
				const std::string& topic = targetChannel->getTopic();

				client->response(
						server.getName(),
						RPL_LIST,
						nick + ' ' + t.str + ' ' + std::to_string(targetChannel->memberCount())
								+ (topic.empty() ? "" : " :" + topic)
				);
			}
		}
	}

	client->response(
			server.getName(),
			RPL_LISTEND,
			nick + " " RPL_LISTEND_MESSAGE
	);
}
