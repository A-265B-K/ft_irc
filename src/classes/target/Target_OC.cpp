#include <Target.hpp>

Target::Target() { }

Target::Target(const Target& target): type(target.type)
{
	if (type == TargetType::USER) {
		this->user = target.user;
		this->host = target.host;
	} else
		this->str = target.str;
}

Target& Target::operator=(const Target& target)
{
	if (this != &target) {
		this->type = target.type;
		if (type == TargetType::USER) {
			this->user = target.user;
			this->host = target.host;
		} else
			this->str = target.str;
	}

	return *this;
}

Target::~Target() { }
