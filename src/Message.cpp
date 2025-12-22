#include "Message.hpp"

Message::Message() { }

Message::Message(const std::string& str)
{
	const std::string::size_type length = str.length(), i0 = 0, i;
	std::ostringstream error;

	error << "Message::BadMessageException: ";

	if (str.find('\r') != length - 2
			|| str.find('\n') != length - 1
			|| str.find('\0') != std::string::npos) {
		error << "Unproper ending characters found";

		throw Message::BadMessageException(error.str());
	}

	// prefix
	if (str[0] == ':') {
		i0 = str.find(' ', 1);
		_prefix(substr(1, i0 != std::string::npos ? i0 - 1 : i0));
		i0 = find_first_not_of(' ', i0);
	}

	// command
	if (

	
		
		token << iss;
		getline(iss, _command, ' ');

		
	} catch (const std::istringstream::failure& e) {

		error << "Message::BadMessageException: Invalid format `" << str << "'";

		throw Message::BadMessageException(error.str());
	}
	
