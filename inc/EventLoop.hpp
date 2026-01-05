#ifndef EVENTLOOP_HPP
# define EVENTLOOP_HPP

# include <static_declarations.hpp>
# include <unistd.h>
# include <sys/epoll.h>
# include <sys/socket.h>
# include <netinet/in.h>

class EventLoop {
	private:
		struct epoll_event	events[MAX_EVENTS];
		int			epoll_fd;
		ushort			port;
	public:
		EventLoop();
		~EventLoop();

		int		addEvent(int fd);
		void		setPort(ushort port);
		int		waitForEvents();
		void	printEvent(int	i);
		struct epoll_event *getEvents();
		int		getEpollFD();
		int		getEventSocket(int i);
};

#endif
