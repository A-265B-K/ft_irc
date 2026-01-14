#ifndef EVENTLOOP_HPP
# define EVENTLOOP_HPP

# include <ChannelRegistry.hpp>
# include <Client.hpp>
# include <ClientRegistry.hpp>
# include <Command.hpp>
# include <function_declarations.hpp>
# include <Message.hpp>
# include <Server.hpp>
# include <static_declarations.hpp>

# include <algorithm>
# include <deque>
# include <netinet/in.h>
# include <sstream>
# include <sys/epoll.h>
# include <sys/socket.h>
# include <unistd.h>

class EventLoop {
	Server				server;
	ChannelRegistry		channelReg;
	ClientRegistry		clientReg;
	struct epoll_event	events[MAX_EVENTS];
	int					epoll_fd;

	typedef	void (EventLoop::*command_t)(Client*, const Message&);
# define X(A, B) &EventLoop::B,
	const command_t		commands[COMMANDS] = { COMMAND_TABLE };
# undef X

	int		addEvent(int fd);
	void	processMessages(Client*, const std::deque<Message>&);
	int		waitForEvents();

# define X(A, B) void B(Client*, const Message&);
	COMMAND_TABLE
# undef X

public:
	EventLoop(Server&, ChannelRegistry&, ClientRegistry&);
	~EventLoop();

	int		run();
};

#endif
