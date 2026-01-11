#ifndef EVENTLOOP_HPP
# define EVENTLOOP_HPP

# include <ChannelRegistry.hpp>
# include <Client.hpp>
# include <ClientRegistry.hpp>
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
private:
	struct epoll_event	events[MAX_EVENTS];
	int		epoll_fd;

	int		addEvent(int fd);
	void	processMessages(Client*, const std::deque<Message>&) const;	
	int		waitForEvents();

public:
	EventLoop();
	~EventLoop();

	int		run(const Server&, ChannelRegistry&, ClientRegistry&);
};

#endif
