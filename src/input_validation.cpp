# include <function_declarations.hpp>

static void validatePort(uint16_t& port, const char *port_in)
{
	errno = 0;
	char *port_end{};
	const long i = std::strtol(port_in, &port_end, 10);

	if (!*port_in || *port_end) {
		std::string error{"Syntax Error. `"};
		error += port_in;
		error += "' Numerical port expected";
		printMessage(error);
		std::exit(ERR_SYNTAX);
	}
	if (errno == ERANGE || i < 0 || i > 65535) {
		std::string error{"Input Error. Port "};
		error += port_in;
		error += " out of range";
		printMessage(error);
		std::exit(ERR_PORT);
	}
	if (i < 1024) {
		std::string error{"Input Error. Port "};
		error += port_in;
		error += " in the privileged range";
		printMessage(error);
		std::exit(ERR_PORT);
	}
	if (i > 49151) {
		std::string warning{"** Warning ** Port "};
		warning += port_in;
		warning += " in the dynamic range";
		printMessage(warning);
	}

	port = static_cast<uint16_t>(i);
}

static void validatePassword(const std::string& password) {
	const std::string::size_type x = password.find_first_of(DISALLOWED);

	if (x != std::string::npos) {
		std::string error{"Input Error. Disallowed character `"};
		error += password[x];
		error += "' in the password";
		printMessage(error);
		std::exit(ERR_PASS);
	}
}

void check_input(uint16_t& port, std::string& password, int ac, char **av) {
	const char *portStr = XSTR(DEFAULT_PORT);

	password = DEFAULT_PASS;
	switch(ac) {
	case 1: break;
	case 2: password = av[1]; break;
	case 3: portStr = av[1]; password = av[2]; break;
	default:
		printMessage("Syntax Error. Proper usage: ./"
				COMMAND_NAME " [[port] password]");
		std::exit(ERR_SYNTAX);
	}

	validatePort(port, portStr);
	validatePassword(password);
}
