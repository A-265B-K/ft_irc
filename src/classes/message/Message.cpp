#include "Message.hpp"

typedef std::array<std::string, COMMANDS> arrayS_t;
typedef std::array<Command, COMMANDS> arrayC_t;

# define X(A, B) #A,
static const arrayS_t commString{ { COMMAND_TABLE } };
# undef X

# define X(A, B) Command::A,
static constexpr arrayC_t commValue{ { COMMAND_TABLE } };
# undef X

# define commandToString(A) commString[static_cast<arrayS_t::size_type>(A)]

void Message::generate(const std::string& str)
{
	const std::string::size_type length = str.length();
	std::string::size_type i0 = 0, i;

	if (length > LIMIT)
		throw Message::BadMessageException(ERR_INPUTTOOLONG,
				"Exceeded the " XSTR(LIMIT) "-character limit");

	// message separator
	if (length < 2
			|| str.rfind('\r') != length - 2
			|| str.rfind('\n') != length - 1)
		throw Message::BadMessageException(ERR_UNKNOWNERROR, "Unproper ending sequence");
	if (str.find('\0') != std::string::npos
			|| str.find('\r') != length - 2
			|| str.find('\n') != length - 1)
		throw Message::BadMessageException(ERR_UNKNOWNERROR, "Forbidden characters found");
	this->length = 2;

	// prefix
	if (str[0] == ':') {
		i = str.find_first_of(" \r", 1);
		if (i == 1)
			throw Message::BadMessageException(ERR_UNKNOWNERROR, "Empty prefix");
		_prefix = str.substr(1, i - 1);
		this->length += i + 1;
		i0 = str.find_first_not_of(' ', i);
	}

	// command
	if (str[i0] == ' ' || str[i0] == '\r')
		throw Message::BadMessageException(ERR_UNKNOWNERROR, "Empty command");
	i = str.find_first_of(" \r", i0);
	const std::string commandStr(str.substr(i0, i - i0));
	this->length += i - i0;
	i0 = str.find_first_not_of(' ', i);

	_response = commandStr.find_first_not_of("0123456789") == std::string::npos;
	if (_response) { // numeric command
		if (commandStr.length() != 3)
			throw Message::BadMessageException(ERR_UNKNOWNERROR,
					"Numeric command must contain 3 digits");
		_numeric = std::stoi(commandStr);
	}
	else {
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
	}

	// parameters
	while (str[i0] != '\r')
		if (str[i0] == ':') {
			i = str.find('\r', ++i0);
			_parameters.emplace_back(str.substr(i0, i - i0));
			this->length += i - i0 + 1
					+ (_parameters.back().find(' ') != std::string::npos);
			i0 = i;
		}
		else {
			i = str.find_first_of(" \r", i0 + 1);
			_parameters.emplace_back(str.substr(i0, i - i0));
			this->length += i - i0 + 1;
			i0 = str.find_first_not_of(' ', i);
		}

// TODO Regular expressions to verify the format of the prefix and command

}

Message::Message() { }

Message::Message(const std::string& str) { generate(str); }

Message::Message(const std::string& source, unsigned short numeric, const std::string& text)
{
	std::ostringstream response;

	response << ':' << source << ' ' << std::setfill('0') << std::setw(3) << numeric
			<< ' ' << text << CRLF;

	generate(response.str());
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
	if (_response) {
		std::ostringstream numericStr;

		numericStr << std::setfill('0') << std::setw(3) << _numeric;
		message += numericStr.str();
	}
	else {
		if (_command != Command::UNKNOWN)
			message += commandToString(_command);
		else
			message += _parameters.front();
	}

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

Message::BadMessageException::BadMessageException(unsigned short _numeric,
		const std::string& what_arg) :
		std::invalid_argument(what_arg), _numeric(_numeric) { }
