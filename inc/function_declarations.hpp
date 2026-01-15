#ifndef FUNCTION_DECLARATIONS_HPP
# define FUNCTION_DECLARATIONS_HPP

# include <static_declarations.hpp>

# include <cerrno>
# include <cstdint>
# include <cstdlib>
# include <iostream>
# include <string>

# define STR(S) #S
# define XSTR(S) STR(S)

void	check_input(uint16_t&, std::string&, int, char**);
void	printMessage(const std::string&);

#endif
