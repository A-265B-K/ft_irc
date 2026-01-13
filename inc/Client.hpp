#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <deque>
# include <Message.hpp>
# include <static_declarations.hpp>

# include <sstream>
# include <string>
# include <sys/socket.h>

# define response(srvrName, reply, txt) generateResponse(srvrName, reply, #reply, txt)

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
		void	generateResponse(const std::string&, const unsigned short, const char*,
					const std::string&) const;
		void	handleReadable(const std::string&, std::deque<Message>&);
		void	handleWritable(const Message&) const;
		void	printMessage(const std::string&) const;
};

# include <function_declarations.hpp>

#endif
