#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <deque>
# include <sstream>
# include <string>
# include <sys/socket.h>
# include <unistd.h>

# include <Message.hpp>
# include <function_declarations.hpp>
# include <User.hpp>

# define response(srvrName, reply, txt) generateResponse((srvrName), reply, #reply, (txt))

# define replyTo(srvrName, text) \
		handleWritable(Message(std::string(":") + (srvrName) + ' ' +  (text) + CRLF))

class User;

class Client {
	private:
		std::string		_netwideID; // see if this can be set to be more CPP, as well as the requirements
		int				_clientSocket;
		std::string		_address;
		std::string		_inputBuffer;
		User			*_user;
		bool			_requestedDisconnect;

	public:
		Client() = delete;
		Client(int server_socket, const std::string& address);
		~Client();

		int		getSocket();
		std::string		getAddress();
		int		socketIsReadable() const;
		int		socketIsWritable() const;
		void	generateResponse(const std::string&, const unsigned short, const char*,
					const std::string&) const;
		void	handleReadable(const std::string&, std::deque<Message>&);
		void	handleWritable(const Message&) const;
		void	printMessage(const std::string&) const;
		User	*getUser();
		void	markForRemoval();
		bool	requestedDisconnect();
};



#endif
