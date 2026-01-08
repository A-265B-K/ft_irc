#include <function_declarations.hpp>

void logMessages(const Client *client, const std::string& head,
		const std::string& tail, const std::deque<Message>& messages)
{
	for_each(messages.begin(), messages.end(), [&](const Message& m) {
		client->printMessage(head + m.build(false) + tail);
	});
}
