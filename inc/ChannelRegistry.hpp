#ifndef CHANNELREGISTRY_HPP
# define CHANNELREGISTRY_HPP

# include <Channel.hpp>
# include <map>

class ChannelRegistry {
	private:
		std::map<std::string, Channel>	channels;
};

#endif