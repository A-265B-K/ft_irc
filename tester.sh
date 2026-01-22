#!/usr/bin/bash

SERVER="127.0.0.1"
PORT=1111
DELAY=0.5
USER1=test1
USER2=test2

s() {
	printf "$1\r\n"
	sleep "$DELAY"
}

login() {
	local nick="$1"

	s "PASS 42irc"
	s "NICK $nick"
	s "USER $nick $nick 0 * :$nick"
}

behaviour() {
	local nick="$1"

	s "JOIN #channel"
	s "PRIVMSG #channel :hello from $nick"
	s "JOIN #channel2"
	s "PRIVMSG #channel2 :hello from $nick"
	s "PRIVMSG #channel3 :hello from $nick"
	s "PART #channel2"
	s "PART #channel3"
	s "TOPIC #channel :Setting channel topic"
	s "TOPIC #channel"
	s "TOPIC"
	s "TOPIC #channel :"
	s "TOPIC #channel"
	s "TOPIC #channel3"
	s "NICK"
	s "NICK test2"
	s "PASS password"
	s "PASS"
	s "PING ft_irc"
	s "PRIVMSG #channel3"
	s "PRIVMSG #channel3 :test message"
	s "USER"
	s "USER test * * :test"
	s "QUIT"
}

inc() {
	netcat -C "$SERVER" "$PORT"
}

irc_client() {
  local nick="$1"
  local behavior="$2"

  {
    login "$nick"
    "$behavior" "$nick"
    sleep 1
   # s "QUIT :bye"
  } | inc
}

irc_client user1 behaviour &

wait
