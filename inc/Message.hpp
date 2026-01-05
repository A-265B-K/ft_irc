#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <arpa/inet.h>
# include <deque>
# include <iostream>
# include <netinet/in.h>
# include <sstream>
# include <stdexcept>
# include <string>
# include <sys/socket.h>

# include "command.hpp"

# define CRLF "\r\n"

class Message {

	std::string _prefix;
	std::string _command;
	std::deque<std::string> _parameters;

	Message();

public:
	Message(const std::string&);
	Message(const Message&);
	~Message();

	Message& operator=(const Message&);

	const std::string& getPrefix() const;
	const std::string& getCommand() const;
	const std::deque<std::string>& getParameters() const;

	std::string build(const bool = true) const;

	struct BadMessageException : public std::invalid_argument {

		BadMessageException(const std::string&);

	};

	static void printMessage(int, const std::string&);

	static const std::string commands[];

// TODO "static" error replies for all commands

};

#endif
