#ifndef FUNCTION_DECLARATIONS_HPP
# define FUNCTION_DECLARATIONS_HPP

# include <algorithm>
# include <deque>
# include <iostream>
# include <string>

# include <Client.hpp>
# include <EventLoop.hpp>
# include <Message.hpp>
# include <Server.hpp>

int	input_is_invalid(int ac, char **av);
void	logMessages(const Client*, const std::string&, const std::string&,
		const std::deque<Message>&);
void	printMessage(const std::string&);
void	set_input_variables(char **av, Server *Server, EventLoop *EventLoop);

#endif
