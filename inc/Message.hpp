#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <array>
# include <deque>
# include <iostream>
# include <sstream>
# include <stdexcept>
# include <string>

# include <Command.hpp>

# define CRLF "\r\n"
# define LIMIT 512

class Message {

	std::string _prefix;
	Command  _command;
	std::deque<std::string> _parameters;

	Message();

public:
	Message(const std::string&);
	Message(const Message&);
	~Message();

	Message& operator=(const Message&);

	const std::string& getPrefix() const;
	Command getCommand() const;
	const std::deque<std::string>& getParameters() const;

	std::string build(const bool = true) const;

	struct BadMessageException : public std::invalid_argument {

		BadMessageException(const std::string&);

	};

// TODO "static" error replies for all commands

};

#endif
