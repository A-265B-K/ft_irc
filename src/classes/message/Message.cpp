#include "Message.hpp"

Message::Message() { }

Message::Message(const std::string& str)
{
	const std::string::size_type length = str.length();
	std::string::size_type i0 = 0, i;
	std::ostringstream error;

	error << "Message::BadMessageException: ";

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
		i = str.find(' ', 1);
		if (i == 1) {
			error << "Empty prefix";

			throw Message::BadMessageException(error.str());
		}
		else if (i == std::string::npos) {
			error << "Missing prefix separator";

			throw Message::BadMessageException(error.str());
		}
		_prefix = str.substr(1, i - 1);
		i0 = str.find_first_not_of(' ', ++i);
	}

	// command
	else if (str[0] == ' ') {
		error << "Empty command";

		throw Message::BadMessageException(error.str());
	}
	i = str.find(' ', i0 + 1);
	if (i == std::string::npos) {
		error << "Missing command separator";

		throw Message::BadMessageException(error.str());
	}
	_command = str.substr(i0, i - i0);
	i0 = str.find_first_not_of(' ', ++i);

	// parameters
	while (str[i0] != '\r')
		if (str[i0] == ':') {
			i = str.find('\r', ++i0);
			_parameters.emplace_back(str.substr(i0, i - i0));
			i0 = i;
		}
		else {
			i = str.find(' ', i0 + 1);
			if (i == std::string::npos) {
				error << "Missing parameter separator";

				throw Message::BadMessageException(error.str());
			}
			_parameters.emplace_back(str.substr(i0, i - i0));
			i0 = str.find_first_not_of(' ', ++i);
		}

// TODO Regular expressions to verify the format of the prefix and command

}

Message::Message(const Message& m):
	_prefix(m._prefix),
	_command(m._command),
	_parameters(m._parameters) { }

Message::~Message() { }

const std::string& Message::getPrefix() const { return _prefix; }

const std::string& Message::getCommand() const { return _command; }

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

std::string Message::build() const
{
	std::string message(_prefix.empty() ? "" : ":");

	// prefix
	if (!_prefix.empty()) {
		message += _prefix;
		message += ' ';
	}

	// command
	message += _command;
	message += ' ';

	// parameters
	for (std::deque<std::string>::const_iterator cit =_parameters.begin();
			cit != _parameters.end(); ++cit) {
		const bool last = (cit + 1 == _parameters.end());

		if (last)
			message += ':';
		message += *cit;
		if (!last)
			message += ' ';
	}

	// message separator
	message += CRLF;

	return message;
}

Message::BadMessageException::BadMessageException(const std::string& what_arg):
		std::invalid_argument(what_arg) { }

void Message::printMessage(const int socketFd, const std::string& text)
{
	if (socketFd >= 0) {
		char buffer[256];
		socklen_t len = 256;

		if (getsockname(socketFd, (struct sockaddr*)buffer, &len) != -1) {
			std::cout << "[ ";
			std::cout << inet_ntoa(((struct sockaddr_in*)buffer)->sin_addr);
			std::cout << "] ";
		}
	}
	std::cout << text << std::endl;
}
