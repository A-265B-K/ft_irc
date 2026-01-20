# include <User.hpp>

User::User(Client *client) : client(client), hasPassword(false), registered(false), modes(0) {

}

User::~User() {

}
