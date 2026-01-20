#ifndef COMMAND_HPP
# define COMMAND_HPP

# define COMMAND_TABLE	\
X(CAP, cap)				\
X(INVITE, invite)		\
X(JOIN, join)			\
X(KICK, kick)			\
X(MODE, mode)			\
X(NICK, nick)			\
X(NOTICE, notice)		\
X(PART, part)			\
X(PASS, pass)			\
X(PING, ping)			\
X(PRIVMSG, privmsg)		\
X(QUIT, quit)			\
X(TOPIC, topic)			\
X(USER, user)

# define X(A, B) A,
enum class Command {
	COMMAND_TABLE
	UNKNOWN
};
# undef X

# define COMMANDS (static_cast<size_t>(Command::UNKNOWN))

#endif
