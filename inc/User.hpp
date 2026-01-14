#ifndef USER_HPP
# define USER_HPP

# include <deque>

# include <Channel.hpp>
# include <static_declarations.hpp>

class User {
	private:
	/* Used in the USER command */
		std::string					username;
		std::string					hostname;
		std::string					servername;
		std::string					realname;

	/* Other information */
		std::string					nickname;
		std::deque<std::string>  	channels;
//		char		input_buffer[513];
//		char		output_buffer[OP_BUFF_S];
		bool						registered;

	public:
		User();
		~User();
		bool		isRegistered();
		bool		registerUser();
		void		setNickname(const std::string& nickname);
		void		setUsername(const std::string& username);
		void		setHostname(const std::string& hostname);
		void		setServername(const std::string& servername);
		void		setRealname(const std::string& realname);

		std::string getNickname();
		std::string getUsername();
		std::string getHostname();
		std::string getServername();
		std::string getRealname();

};

#endif
