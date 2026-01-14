#include <EventLoop.hpp>

// TODO
void EventLoop::notice(Client *client, const Message& message)
{
/*
  Remember that inside this EventLoop instantiation, we have access to:

	server
	channelReg
	clientReg

  Numeric responses from the server can make use of the macro interface found in
  the Client.hpp header:

	response(srvrName, reply, txt)

  Bear in mind that a correct irc message must be formed, otherwise a program-
  terminating exception might be thrown. In particular, any and all of the blank
  spaces of the text should go in the last parameter, which starts with ':'. The
  ending sequence "\r\n" is automatically affixed.

	srvrName <- prefix. In our case, server.getName()
	reply <- named response. Please add them at the bottom of the Message header
	txt <- all of the parameters that form the message

  Currently, a working example of this response mechanism can be found in
  EventLoop::processMessages

	client->response(
			server.getName(),
			ERR_UNKNOWNCOMMAND,
			std::string("<client>")  <-  currently a placeholder
				+ ' ' + m.getParameters().front()  <- (*)
				+ " :Unknown command"
	);

  (*) In this particular case of a command not being recognized, the parser will
  store it as the first parameter of the message received. This is generally not
  the case!

  This response's parameters conform to the expected format

	"<client> <command> :Unknown command"

  Messages sent to some other Client* client2 can make use of the call

	client2->handleWritable(m)

  where m is some well-formed Message.

  Access the components of the message with getPrefix() and getParameters().
  Note that neither the colon (:) of the prefix nor that of a trailing parameter
  are stored within the Message attributes.

  Ideally, some descriptive account of the server's doings should be logged. As
  a placeholder...
*/
	::printMessage("NOTICE");
}
