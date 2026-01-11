#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <deque>
# include <Message.hpp>
# include <sstream>
# include <static_declarations.hpp>
# include <string>
# include <sys/socket.h>

class Client {
	private:
		std::string		netwide_id; // see if this can be set to be more CPP, as well as the requirements
		int		client_socket;
		std::string		address;
		std::string		input_buffer;

	public:
		Client(); // make it so this cant be called
		Client(int server_socket, const std::string& address);

		int		socketIsReadable() const;
		int		socketIsWritable() const;
		void	handleReadable(std::deque<Message>&);
		void	handleWritable(const Message&);
		void	printMessage(const std::string&) const;
};

# include <function_declarations.hpp>

#endif
