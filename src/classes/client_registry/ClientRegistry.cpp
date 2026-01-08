# include <ClientRegistry.hpp>

# include <arpa/inet.h>
# include <sys/socket.h>
# include <string>
# include <sys/epoll.h>
# include <unistd.h>
# include <fcntl.h>


int set_nonblocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl(F_GETFL)");
        return -1;
    }
    if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl(F_SETFL)");
        return -1;
    }
    return 0;
}

int		ClientRegistry::addClient(int server_socket) {
	char buffer[256];
	socklen_t len = 256;

	const int client_socket = accept(server_socket, (struct sockaddr*)buffer,
			&len);
	if (client_socket == -1) {
		if (!(errno == EAGAIN || errno == EWOULDBLOCK)) {
			perror("accept");
			return -1 ;
		}
	}
	set_nonblocking(client_socket);
	const std::string addr(inet_ntoa(((struct sockaddr_in*)buffer)->sin_addr));

	std::string text("Incoming connection from ");
	text +=	addr;
	text += client_socket == -1 ? " FAILED" : " accepted";
	::printMessage(text);
	if (client_socket == -1)
		return -1;

	Client *client = new Client(client_socket, addr);
	this->clients.insert( {client_socket, client} );

	return client_socket;
}

Client	*ClientRegistry::getClientBySocket(int client_socket) {
	return this->clients[client_socket];
}
