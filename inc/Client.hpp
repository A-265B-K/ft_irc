#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>

class Client {
	private:
		std::string	netwide_id; // see if this can be set to be more CPP, as well as the requirements
		int			client_socket;
	public:
		Client(); // make it so this cant be called
		Client(int server_socket);
};

#endif