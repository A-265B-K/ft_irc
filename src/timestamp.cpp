#include <chrono>

#include <function_declarations.hpp>

std::string timestamp()
{
	return std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
}
