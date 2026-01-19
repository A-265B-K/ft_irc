#ifndef TARGET_HPP
# define TARGET_HPP

# include <list>
# include <sstream>
# include <string>

# define REGULAR_CHANNEL	HASH
# define LOCAL_CHANNEL		AMPERSAND
# define HOST_MASK			HASH
# define SERVER_MASK		DOLLAR

enum class TargetType {
	HASH,		// starts with '#' -> regular channel OR host mask
	AMPERSAND,	// starts with '&' -> local channel
	DOLLAR,		// starts with '$' -> server mask
	USER,
	NICK
};

struct Target {
	TargetType type;
	std::string	str;
	std::string host;

	Target(const Target&);
	~Target();

	static std::list<Target> markTargets(const std::string&);

private:
	Target();
	Target(const std::string&);

	Target& operator=(const Target&);
};

#endif
