#ifndef USER_HPP
# define USER_HPP

# include <string>
# include <deque>
# include <static_declarations.hpp>
# include <Client.hpp>

class Client;

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
		Client						*client;
		bool						hasPassword;
//		char		input_buffer[513];
//		char		output_buffer[OP_BUFF_S];
		bool						registered;
		unsigned char				modes;

		static const std::string flags; // "ioOrw"

	public:
		User() = delete;
		User(Client *client);
		~User();

		bool		isRegistered();
		bool		isNicknameValid(const std::string &nickname);

		// 1 mode set, 0 mode unset, -1 mode unrecognized
		int			isMode(char) const;
		void		setMode(char);
		void		unsetMode(char);
		std::string	getModestring() const;
		int 		editModes(std::string&, const std::string&);

		void		setNickname(const std::string& nickname);
		void		setUsername(const std::string& username);
		void		setHostname(const std::string& hostname);
		void		setServername(const std::string& servername);
		void		setRealname(const std::string& realname);
		void		setHasPassword();

		std::string getNickname();
		std::string getUsername();
		std::string getHostname();
		std::string getServername();
		std::string getRealname();

};

#endif
