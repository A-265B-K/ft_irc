# include <User.hpp>

User::User(Client *client) : registered(false), modes(0), client(client), hasPassword(false) {

}

User::~User() {

}
