#include <function_declarations.hpp>

int	VERBOSE = 1; // add this to parsing
int PRINT_USER_MESSAGE = 1;

void printUserMessage(const std::string& text)
{
	if (PRINT_USER_MESSAGE) {
		std::cout << text << std::endl;
	}
}

void printMessage(const std::string& text)
{
	if (VERBOSE) {
		std::cout << text << std::endl;
	}
}
