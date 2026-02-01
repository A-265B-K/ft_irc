#include <EventLoop.hpp>
#include <static_declarations.hpp>

void EventLoop::quit(Client *client, const std::deque<std::string>& p)
{
	std::string reason{p.empty() ? "" : p.front()};

	client->replyTo(
			server.getName(),
			"ERROR",
			":" QUIT_ACKNOWLEDGED
	);

	// find all of the client's peers in the registries
	std::set<Client *> peers;

	channelReg.forEachChannel([&, client](const std::pair<std::string, Channel *>& p1) {
			if (p1.second->hasClient(client))
				p1.second->forEachClient([&, client](const std::pair<Client *, int>& p2) {
						if (p2.first != client)
							peers.insert(p2.first);
				});
	});
	
	reason = ":Quit " + reason;
	for (Client *const cl : peers)
			cl->replyTo(
				client->getName(),
				"QUIT",
				reason
			);

	this->markClientForRemoval(client);
	::printMessage("QUIT");
}
