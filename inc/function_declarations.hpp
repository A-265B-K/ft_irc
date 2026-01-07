#ifndef FUNCTION_DECLARATIONS_HPP
# define FUNCTION_DECLARATIONS_HPP

# include <algorithm>
# include <deque>
# include <string>

# include "EventLoop.hpp"
# include "Message.hpp"
# include "Server.hpp"

int	input_is_invalid(int	ac, char **av);
void	logMessages(int, const std::string&, const std::string&, const std::deque<Message>&);
void	set_input_variables(char **av, Server *Server, EventLoop *EventLoop);

#endif
