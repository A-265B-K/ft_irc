#!/usr/bin/bash

SERVER="127.0.0.1"
PORT=1111
DELAY=0.1
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

user_behaviour() {
	local nick="$1"

	s "JOIN #cat"
	s "PRIVMSG #cat :hello from $nick"
	s "JOIN #cat2"
	s "PRIVMSG #cat2 :hello from $nick"
	s "JOIN #c t"
	s "JOIN #cat3"
	s "PART #cat3"
	sleep 1
}

oper_behaviour() {
	local nick="$1"
	
	s "JOIN #cat"
	s "MODE #cat +o $nick"
	s "TOPIC #cat :operator set topic"
	s "KICK #cat user2 :testing kick"
}

listener_behaviour() {
	s "JOIN #cat"
	sleep 2
}

topic_tester() {
	s "JOIN #cat"
	s "TOPIC"
	s "TOPIC #cat"
	s "TOPIC #cat :meowing channel"
	s "TOPIC #cat"
	s "TOPIC #cat :"
	s "JOIN #privcat"
	s "TOPIC #privcat :my meowing channel"
	s "TOPIC #privcat"
	s "TOPIC #nonexistent"
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

irc_client user1 listener_behaviour &
# irc_client oper1 oper_behaviour &
irc_client user2 user_behaviour &
irc_client user3 topic_tester &
# irc_client user2 user_behaviour &


wait

# {
# 	s "PASS 42irc"
# 	s "NICK $USER1"
# 	s "USER $USER1 $USER1 $USER1 $USER1 :*"
# 	s "JOIN #cat"
# 	s "TOPIC #cat"
# 	s "TOPIC #cat :cat"
# 	s "TOPIC #cat :"
# 	s "TOPIC #cat"
# } | inc
