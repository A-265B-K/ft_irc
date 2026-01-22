#ifndef SERVER_HPP
# define SERVER_HPP

# include <function_declarations.hpp>

# include <arpa/inet.h>
# include <sstream>
# include <string>
# include <sys/socket.h>
# include <unistd.h>

// INADDR_ANY : IPv4 wildcard address, typically 0.0.0.0
# define HOST_IP_ADDRESS INADDR_ANY

class Server {
private:
	std::string 	name;
	int				version;
	uint16_t		port;
	std::string		password;
	int				server_socket;
	sockaddr_in		address;
	//t_time		creation_time; // I don't think we are allowed to use a function for this

public:
	Server(uint16_t port, const std::string& password);
	//Server(std::string name);
	~Server();

	const std::string& getName() const;
	int getServerSocket() const;
	void start();
	const std::string&	getPassword() const;
};

#endif
