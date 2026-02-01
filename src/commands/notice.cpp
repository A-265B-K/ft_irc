#include <EventLoop.hpp>
#include <Target.hpp>

void EventLoop::notice(Client *client, const std::deque<std::string>& p)
{
	User *const user = client->getUser();

	if (!user || !user->isRegistered() || p.size() < 2)
		return;

	const std::list<Target> listOfTargets{Target::markTargets(p[0])};
	const std::string text{p[1]};

	for (const Target& t : listOfTargets)
		if (t.type == TargetType::REGULAR_CHANNEL || t.type == TargetType::LOCAL_CHANNEL) {
			Channel *const targetChannel = channelReg.getChannel(t.str);

			if (targetChannel)
				targetChannel->broadcast(client, "NOTICE", text);
		} else {
			Client *const targetClient = clientReg.getClientByNick(t.str);

			if (targetClient && targetClient->getUser()->isRegistered())
				targetClient->replyTo(user->getNickname(), "NOTICE",
						targetClient->getUser()->getNickname() + " :" + text);
		}
}
