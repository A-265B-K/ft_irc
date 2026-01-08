#include "Message.hpp"

typedef std::array<std::string, static_cast<size_t>(Command::UNKNOWN)> arrayS_t;
typedef std::array<Command, static_cast<size_t>(Command::UNKNOWN)> arrayC_t;

# define X(a) #a,
static const arrayS_t commString{ {COMMAND_TABLE} };
# undef X

# define X(a) Command::a,
static constexpr arrayC_t commValue{ {COMMAND_TABLE} };
# undef X

# define commandToString(a) commString[static_cast<arrayS_t::size_type>(a)]

Message::Message() { }

Message::Message(const std::string& str)
{
	const std::string::size_type length = str.length();
	std::string::size_type i0 = 0, i;
	std::ostringstream error;

	error << "Message::BadMessageException: ";

	if (length > LIMIT) {
		error << "Exceeded the " << LIMIT << "-character limit";

		throw Message::BadMessageException(error.str());
	}

	// message separator
	if (length < 2
			|| str.rfind('\r') != length - 2
			|| str.rfind('\n') != length - 1) {
		error << "Unproper ending sequence";

		throw Message::BadMessageException(error.str());
	}
	if (str.find('\0') != std::string::npos
			|| str.find('\r') != length - 2
			|| str.find('\n') != length - 1) {
		error << "Forbidden characters found";
	}

	// prefix
	if (str[0] == ':') {
		i = str.find_first_of(" \r", 1);
		if (i == 1) {
			error << "Empty prefix";

			throw Message::BadMessageException(error.str());
		}
		_prefix = str.substr(1, i - 1);
		i0 = str.find_first_not_of(' ', i);
	}

	// command
	else if (str[0] == ' ' || str[0] == '\r') {
		error << "Empty command";

		throw Message::BadMessageException(error.str());
	}
	i = str.find_first_of(" \r", i0);
	const std::string commandStr(str.substr(i0, i - i0));
	i0 = str.find_first_not_of(' ', i);

	arrayC_t::const_iterator cit = commValue.begin();
	while (cit != commValue.end()) {
		if (commandStr == commandToString(*cit))
			break;
		++cit;
	}
	if (cit != commValue.end())
		_command = *cit;
	else {
		_command = Command::UNKNOWN;
		_parameters.emplace_back(commandStr);
	}

	// parameters
	while (str[i0] != '\r')
		if (str[i0] == ':') {
			i = str.find('\r', ++i0);
			_parameters.emplace_back(str.substr(i0, i - i0));
			i0 = i;
		}
		else {
			i = str.find_first_of(" \r", i0 + 1);
			_parameters.emplace_back(str.substr(i0, i - i0));
			i0 = str.find_first_not_of(' ', i);
		}

// TODO Regular expressions to verify the format of the prefix and command

}

Message::Message(const Message& m):
	_prefix(m._prefix),
	_command(m._command),
	_parameters(m._parameters) { }

Message::~Message() { }

const std::string& Message::getPrefix() const { return _prefix; }

Command Message::getCommand() const { return _command; }

const std::deque<std::string>& Message::getParameters() const { return _parameters; }

Message& Message::operator=(const Message& m)
{
	if (this != &m) {
		this->_prefix = m._prefix;
		this->_command = m._command;
		this->_parameters = m._parameters;
	}

	return *this;
}

std::string Message::build(const bool crlf) const
{
	std::string message(_prefix.empty() ? "" : ":");

	// prefix
	if (!_prefix.empty()) {
		message += _prefix;
		message += ' ';
	}

	// command
	if (_command != Command::UNKNOWN)
		message += commandToString(_command);
	else
		message += _parameters.front();

	// parameters
	for (std::deque<std::string>::const_iterator cit = _parameters.begin()
			+ (_command == Command::UNKNOWN); cit != _parameters.end(); ++cit) {
		message += ' ';
		if (cit + 1 == _parameters.end() && cit->find(' ') != std::string::npos)
			message += ':';
		message += *cit;
	}

	// message separator
	if (crlf)
		message += CRLF;

	return message;
}

Message::BadMessageException::BadMessageException(const std::string& what_arg):
		std::invalid_argument(what_arg) { }

