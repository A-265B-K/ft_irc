# include <User.hpp>

User::User(Client *client) : client(client), hasPassword(false),  modes(0) {

}

User::~User() {

}
