#include <chrono>
#include <ctime>

#include <function_declarations.hpp>

std::string timestamp()
{
	char ts[16];

	const std::time_t now =
			std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	std::strftime(ts, 16, "%Y%m%d_%H%M%S", std::localtime(&now));	

	return std::string(ts);
}
