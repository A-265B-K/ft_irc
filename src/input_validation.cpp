# include <function_declarations.hpp>

static void check_port(uint16_t& port, const char *port_in)
{
	errno = 0;
	char *port_end{};
	const long i = std::strtol(port_in, &port_end, 10);

	if (!*port_in || *port_end) {
		printMessage("Syntax Error. Numerical port expected");
		std::exit(ERR_SYNTAX);
	}
	if (errno == ERANGE || i < 0 || i > 65535) {
		printMessage("Input Error. Port out of range");
		std::exit(ERR_PORT);
	}
	if (i < 1024) {
		printMessage("Input Error. Port in the privileged range");
		std::exit(ERR_PORT);
	}
	if (i > 49151)
		printMessage("** Warning ** Port in the dynamic range");

	port = static_cast<uint16_t>(i);
}

static void check_password(const std::string& password) {
	// Add checks for forbidden characters
}

void check_input(uint16_t& port, std::string& password, int ac, char **av) {
	if (ac != 3) {
		printMessage("Syntax Error. Proper usage: ./"
				COMMAND_NAME " port password");
		std::exit(ERR_SYNTAX);
	}
	check_port(port, av[1]);
	password = av[2];
	check_password(password);
}
