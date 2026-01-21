#ifndef NUMERICS_HPP
# define NUMERICS_HPP




# define WELCOME							001
# define WELCOME_MESSAGE					":Welcome to the IRC network"

# define RPL_UMODEIS						221

# define RPL_NOTOPIC						331
# define RPL_NOTOPIC_MESSAGE				":No topic is set"

# define RPL_TOPIC							332

# define RPL_TOPICWHOTIME					333

# define RPL_CHANNELMODEIS					324

# define RPL_PRIVMSG						353

# define ERR_UNKNOWNERROR					400

# define ERR_NOSUCHNICK						401
# define ERR_NOSUCHNICK_MESSAGE 			":Error, no user by that nickname"

# define ERR_NOSUCHCHANNEL					403
# define ERR_NOSUCHCHANNEL_MESSAGE			":Error, no channel by that name"

# define ERR_INPUTTOOLONG					417

# define ERR_UNKNOWNCOMMAND					421

# define ERR_ERRONEUSNICKNAME				432
# define ERR_ERRONEUSNICKNAME_MESSAGE		":Nickname already in use"

# define ERR_NICKNAMEINUSE					433
# define ERR_NICKNAMEINUSE_MESSAGE			":Nickname is already in use"

# define ERR_USERNOTINCHANNEL				441
# define ERR_USERNOTINCHANNEL_MESSAGE		":They're not on that channel"

# define ERR_NOTONCHANNEL					442
# define ERR_NOTONCHANNEL_MESSAGE			":You're not on that channel"

# define ERR_NOTREGISTERED					451
# define ERR_NOTREGISTERED_MESSAGE			":You have not registered"

# define ERR_NEEDMOREPARAMS					461
# define ERR_NEEDMOREPARAMS_MESSAGE			":Not enough parameters"

# define ERR_ALREADYREGISTERED				462
# define ERR_ALREADYREGISTERED_MESSAGE		":You may not reregister"

# define ERR_PASSWDMISMATCH					464
# define ERR_PASSWDMISMATCH_MESSAGE			":Password incorrect"

# define ERR_BADCHANNELKEY						475
# define ERR_BADCHANNELKEY_MESSAGE			":Error, channel password is wrong"

# define ERR_CHANOPRIVSNEEDED				482
# define ERR_CHANOPRIVSNEEDED_MESSAGE		":You're not a channel operator"

# define ERR_UMODEUNKNOWNFLAG				501
# define ERR_USERSDONTMATCH					502


#endif