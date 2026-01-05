#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>


class Server {
	private:
		std::string 		name;
		int					version;
		int					port;
		std::string			password; // hash passsword
		int					server_socket;
		//t_time		creation_time; // I don't think we are allowed to use a function for this
	public:
		Server();
		Server(std::string name);
		~Server();

		void	setPassword(std::string password);
		void	setServerSocket(int socket_fd);
		int		getServerSocket();

};

#endif
