#ifndef NUMERICS_HPP
# define NUMERICS_HPP

# include <climits>
# include <function_declarations.hpp>

# define RPL_WELCOME						001
# define RPL_WELCOME_MESSAGE				":Welcome to the IRC network"

# define RPL_YOURHOST						002
# define RPL_YOURHOST_MESSAGE				":"

# define RPL_CREATED						003
# define RPL_CREATED_MESSAGE				":Server was created at "

# define RPL_MYINFO							004

# define RPL_ISUPPORT						005

# define RPL_UMODEIS						221

# define RPL_NOTOPIC						331
# define RPL_NOTOPIC_MESSAGE				":No topic is set"

# define RPL_TOPIC							332

# define RPL_TOPICWHOTIME					333

# define RPL_CHANNELMODEIS					324

# define RPL_INVITING						341

# define RPL_PRIVMSG						353

# define ERR_UNKNOWNERROR					400

# define ERR_NOSUCHNICK						401
# define ERR_NOSUCHNICK_MESSAGE 			":No user by that nickname"

# define ERR_NOSUCHCHANNEL					403
# define ERR_NOSUCHCHANNEL_MESSAGE			":No channel by that name"

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

# define ERR_USERONCHANNEL					443
# define ERR_USERONCHANNEL_MESSAGE			":is already on channel"

# define ERR_NOTREGISTERED					451
# define ERR_NOTREGISTERED_MESSAGE			":You have not registered"

# define ERR_NEEDMOREPARAMS					461
# define ERR_NEEDMOREPARAMS_MESSAGE			":Not enough parameters"

# define ERR_ALREADYREGISTERED				462
# define ERR_ALREADYREGISTERED_MESSAGE		":You may not reregister"

# define ERR_PASSWDMISMATCH					464
# define ERR_PASSWDMISMATCH_MESSAGE			":Password incorrect"

# define ERR_BADCHANNELKEY					475
# define ERR_BADCHANNELKEY_MESSAGE			":Error, channel password is wrong"

# define ERR_CHANOPRIVSNEEDED				482
# define ERR_CHANOPRIVSNEEDED_MESSAGE		":You're not a channel operator"

# define ERR_UMODEUNKNOWNFLAG				501
# define ERR_UMODEUNKNOWNFLAG_MESSAGE		":Unknown MODE flag"

# define ERR_USERSDONTMATCH					502
# define ERR_USERSDONTMATCH_MESSAGE			":Can't change mode for other users"
# define ERR_USERSDONTMATCH_MESSAGE_VAR		":Can't change view for other users"

# define ERR_INVALIDKEY						525
# define ERR_INVALIDKEY_MESSAGE				":Key is not well-formed"

# define ERR_INVALIDMODEPARAM				696
# define ERR_INVALIDMODEPARAM_MESSAGE_L		":Integer in the range [0, " XSTR(INT_MAX) "] expected"
# define ERR_INVALIDMODEPARAM_MESSAGE_O		":Owners may not downgrade"

#endif
