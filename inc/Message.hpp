#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# define CRLF "\r\n"

# include <sstream>
# include <stdexcept>
# include <string>
# include <vector>

class Message {

	std::string _prefix;
	std::string _command;
	std::vector<std::string> _middleParams;
	std::string _trailingParam;

	Message();

public:
	Message(const std::string&);
	Message(const Message&);
	~Message();

	Message& operator=(const Message&);

	std::string build();

	struct BadMessageException : public std::invalid_argument {

		BadMessageException(const std::string&);

	}

};

#endif
