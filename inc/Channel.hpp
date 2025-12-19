#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <map>

class Channel {
	private:
		std::string							name;
		std::map<std::string, int>			users; // int 0 = users, 1 = operator, 2 = owner
		std::string							topic;
		std::map<char, bool>				modes; 
};

#endif