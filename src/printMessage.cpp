#include <function_declarations.hpp>

int	VERBOSE = 1; // add this to parsing

void printMessage(const std::string& text)
{
	if (VERBOSE) {
		std::cout << text << std::endl;
	}
}
