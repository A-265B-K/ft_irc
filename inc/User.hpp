#ifndef USER_HPP
# define USER_HPP

# include <Channel.hpp>
# include <Client.hpp>
# include <static_declarations.hpp>

class User {
	private:
		std::string	nickname;
		std::string	hostname;
		std::string *channels;
		char		input_buffer[513];
		char		output_buffer[OP_BUFF_S];
		Client		client;
	public:

};

#endif