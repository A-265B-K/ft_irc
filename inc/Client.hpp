#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <deque>
# include <sstream>
# include <string>
# include <sys/socket.h>

# include <Message.hpp>
# include <static_declarations.hpp>
# include <User.hpp>

# define response(srvrName, reply, txt) generateResponse((srvrName), reply, #reply, (txt))

# define replyTo(srvrName, text) \
		handleWritable(Message(std::string(":") + (srvrName) + ' ' +  (text) + CRLF))

class User;

class Client {
	private:
		std::string		netwide_id; // see if this can be set to be more CPP, as well as the requirements
		int				client_socket;
		std::string		address;
		std::string		input_buffer;
		User			*user;

	public:
		Client() = delete;
		Client(int server_socket, const std::string& address);

		int		getSocket();
		std::string		getAddress();
		int		socketIsReadable() const;
		int		socketIsWritable() const;
		void	generateResponse(const std::string&, const unsigned short, const char*,
					const std::string&) const;
		void	handleReadable(const std::string&, std::deque<Message>&);
		void	handleWritable(const Message&) const;
		void	printMessage(const std::string&) const;
		bool	isUser();
		void	createUser();
		void	deleteUser();
		User	*getUser();
};

# include <function_declarations.hpp>

#endif
