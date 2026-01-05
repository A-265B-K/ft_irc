#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <static_declarations.hpp>

class Client {
	private:
		std::string		netwide_id; // see if this can be set to be more CPP, as well as the requirements
		int				client_socket;
		std::string		input_buffer;
		std::string		output_buffer;
	public:
		Client(); // make it so this cant be called
		Client(int server_socket);

		int		socketIsReadable();
		int		socketIsWritable();
		void	handleReadable();
		void	handleWritable();
};

#endif
