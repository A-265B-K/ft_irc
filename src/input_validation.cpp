# include <string>
# include <arpa/inet.h>
# include <Server.hpp>
# include <EventLoop.hpp>
# include <iostream>

static int	port_is_invalid(char *port_in) {
	// add further checks, such as reserved ports

	return 0;
}

static int password_is_invalid(char *password_in) {
	std::string	password_str = password_in;
	// Add checks for forbidden characters
	return 0;
}

void	set_input_variables(char **av, Server *Server, EventLoop *EventLoop) {
	EventLoop->setPort(htons(std::atoi(av[1])));
	Server->setPassword(av[2]);
}

int	input_is_invalid(int	ac, char **av) {
	if (ac != 3) { return 1; }
	if (port_is_invalid(av[1])) { return 1; }
	if (password_is_invalid(av[2])) { return 1; }
	return 0;
}
