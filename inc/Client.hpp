#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <deque>
# include <static_declarations.hpp>
# include <string>
# include <sys/socket.h>

# include "Message.hpp"

class Client {
	private:
		std::string		netwide_id; // see if this can be set to be more CPP, as well as the requirements
		int				client_socket;
		std::string		input_buffer;
		std::string		output_buffer;
	public:
		Client(); // make it so this cant be called
		Client(int server_socket);

		int		socketIsReadable() const;
		int		socketIsWritable() const;
		void	handleReadable(std::deque<Message>&);
		void	handleWritable();
};

#endif
