#include <Target.hpp>

Target::Target() { }

Target::Target(const Target& target): type(target.type), str(target.str), host(target.host) { }

Target& Target::operator=(const Target& target)
{
	if (this != &target) {
		this->type = target.type;
		this->str = target.str;
		this->host = target.host;
	}

	return *this;
}

Target::~Target() { }
