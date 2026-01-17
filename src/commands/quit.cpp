#include <EventLoop.hpp>

// TODO
void EventLoop::quit(Client *client, const std::deque<std::string>& p)
{
	if (p.size() > 1) {
		// send error message
		return ;
	}
	if (p.size() == 1) {
		// sendm message to clients
	}

	// Kick the client
	// remove client from channels
	int	clientSocket = client->getSocket();
	this->removeEvent(clientSocket);
	this->clientReg.removeClient(clientSocket);
	client->deleteUser();
/*
  Help can be found (I hope) in `command_help.txt'.

  Ideally, some descriptive account of the server's doings should be logged. As
  a placeholder...
*/
	::printMessage("QUIT");
}
