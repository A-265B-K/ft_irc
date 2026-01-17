#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <array>
# include <deque>
# include <iomanip>
# include <iostream>
# include <sstream>
# include <stdexcept>
# include <string>

# include <Command.hpp>
# include <function_declarations.hpp>
# include <static_declarations.hpp>

class Message {

	std::string _prefix;
	bool _response;
	union {
		Command  _command;
		unsigned short _numeric;
	};
	std::deque<std::string> _parameters;

	void generate(const std::string&);
	Message();

public:
	size_t length = 0;

	Message(const std::string&);
	Message(const std::string&, unsigned short, const std::string&);
	Message(const Message&);
	~Message();

	Message& operator=(const Message&);

	const std::string& getPrefix() const;
	bool isResponse() const;
	Command getCommand() const;
	const std::deque<std::string>& getParameters() const;

	std::string build(const bool = true) const;

	struct BadMessageException : public std::invalid_argument {

		unsigned short _numeric;

		BadMessageException(unsigned short, const std::string&);

	};

# define WELCOME			001
# define RPL_PRIVMSG		353
# define ERR_UNKNOWNERROR   400
# define ERR_INPUTTOOLONG   417
# define ERR_UNKNOWNCOMMAND 421

};

#endif
