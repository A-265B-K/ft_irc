#ifndef COMMAND_HPP
# define COMMAND_HPP

# define COMMAND_TABLE \
X(CAP)     \
X(DEBUG)   \
X(INVITE)  \
X(JOIN)    \
X(KICK)    \
X(MODE)    \
X(NOTICE)  \
X(NICK)    \
X(PART)    \
X(PASS)    \
X(PING)    \
X(PONG)    \
X(PRIVMSG) \
X(TOPIC)   \
X(QUIT)    \
X(USER)

# define X(a) a,
enum class Command {
	COMMAND_TABLE
	UNKNOWN
};
# undef X

#endif
