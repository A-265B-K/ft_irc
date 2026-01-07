#include "function_declarations.hpp"

void logMessages(int clientSocket, const std::string& head,
		const std::string& tail, const std::deque<Message>& messages)
{
	for_each(messages.begin(), messages.end(), [&](const Message& m) {
		Message::printMessage(clientSocket, head + m.build(false) + tail);
	});
}
