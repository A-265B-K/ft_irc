#ifndef SERVER_HPP
# define SERVER_HPP

# include <function_declarations.hpp>

# include <arpa/inet.h>
# include <sstream>
# include <string>
# include <sys/socket.h>
# include <unistd.h>
# include <ctime>

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
	time_t			creationTime;

public:
	Server(uint16_t port, const std::string& password);
	//Server(std::string name);
	~Server();

	const std::string& getName() const;
	int getServerSocket() const;
	void start();
	const std::string&	getPassword() const;
	const time_t&		getCreationTime() const;
	const int&			getVersion() const;
};

#endif
