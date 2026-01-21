#ifndef STATIC_DECLARATIONS_HPP
# define STATIC_DECLARATIONS_HPP

/* Colours and other terminal codes */

# define RED "\033[91m"
# define GREEN "\033[92m"
# define YELLOW "\033[93m"
# define BLUE "\033[94m"
# define RESET "\033[0m"


# define RUNNING 1		// boolean TRUE
# define MAX_EVENTS 5	// maximum events returned by epoll_wait
# define TIMEOUT 300000	// wait timeout in milliseconds
# define IBUFFER 200	// client input buffer
# define CRLF "\r\n"	// message termination sequence
# define LIMIT 512		// message character-limit
# define COMMAND_NAME "ircserv"	// executable name
# define DEFAULT_PORT 6667		// TCP port for plain text connections
# define DEFAULT_PASS ""		// default password
# define DISALLOWED " :\r\n"	// space 0x20, colon 0x3a, CR 0x0d, LF 0x0a
# define ERR_SYNTAX 1	// error: improper argument count
# define ERR_PORT 2		// error: invalid port
# define ERR_PASS 3		// error: invalid password

#endif
