#include <Server.hpp>
#include <ChannelRegistry.hpp>
#include <ClientRegistry.hpp>
#include <EventLoop.hpp>
#include <function_declarations.hpp>

// Add some more descriptive error messages

int	main(int ac, char **av) {
	uint16_t		port;
	std::string		password;

	check_input(port, password, ac, av);

	Server 			server(htons(port), password);
	ChannelRegistry channelRegistry;
	ClientRegistry	clientRegistry;

	// TODO add signals
	server.start();

	EventLoop		eventLoop(server, channelRegistry, clientRegistry);
	eventLoop.run();

	return 0;
}