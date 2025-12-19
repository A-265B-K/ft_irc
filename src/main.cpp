# include <Server.hpp>
# include <ChannelRegistry.hpp>
# include <ClientRegistry.hpp>
# include <EventLoop.hpp>
# include <function_declarations.hpp>
# include <iostream>

# define READ_SIZE 512

// Add some more descriptive error messages

int	main(int ac, char **av) {
	Server 			Server;
	ChannelRegistry ChannelRegistry;
	ClientRegistry	ClientRegistry;
	EventLoop		EventLoop;
	int				i, event_count, server_socket, client_socket;

	// if (input_is_invalid(ac, av)) {
	// 	std::cout << "INPUT INVALID" << std::endl; return 1;
	// }

	Server.setServerSocket(EventLoop.addEvent(0)); // mve this into server constructor
	while (RUNNING) {
		event_count = EventLoop.waitForEvents();
		for (i = 0; i < event_count; i += 1) {
			if (EventLoop.getEventSocket(i) == Server.getServerSocket()) {
				client_socket = ClientRegistry.addClient(Server.getServerSocket());
				if (client_socket >= 0) {
					EventLoop.addEvent(client_socket);
				}
			} else {
				char buf[1024];
				int bytes = recv(EventLoop.getEventSocket(i), buf, sizeof(buf), 0);
				if (bytes > 0) {
					buf[bytes] = '\0';
					std::cout << buf << std::endl;
				}

			}
		}
	}
	return 0;
}

	// while true:
	// 	events = eventloop.wait()
	
	// 	for event in events:
	// 		if event.socket == server_socket:
	// 			client_socket = serversocket.accept()
	// 			client = new client(client_socket)
	// 			client.host = client_scoket.remote_address
	// 			client_registry.clients.add(client)
	// 			eventloop.register(client_socket, READ)

	// 		else:
	// 			client = client_registry.get_by_socket(event.socket)
	// 			if client == null:
	// 				continue
	// 			if event.is_readable():
	// 				handle_readable(client, event, channel_registry)
	// 			if event.is_writable():
	// 				handle_writable(client)

// epoll_event events[10];
// int n = epoll_wait(epoll_fd, events, 10, -1);

// for (int i = 0; i < n; ++i) {
//     int fd = events[i].data.fd;

//     if (fd == server_fd) {
//         int client_fd = accept(server_fd, nullptr, nullptr);
//         if (client_fd == -1) continue;

//         epoll_event client_ev{};
//         client_ev.events = EPOLLIN;
//         client_ev.data.fd = client_fd;
//         epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &client_ev);
//     } else {
//         char buf[1024];
//         int bytes = recv(fd, buf, sizeof(buf), 0);

//         if (bytes <= 0) {
//             close(fd);
//             epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, nullptr);
//         }
//     }
// }