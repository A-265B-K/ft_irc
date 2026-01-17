#include <Target.hpp>

std::list<Target> Target::markTargets(const std::string& targetList)
{
	std::istringstream iss(targetList);
	std::string target;
	std::list<Target> listOfTargets;

	while (std::getline(iss, target, ','))
		listOfTargets.emplace_back(Target(target));

	return listOfTargets;
}

Target::Target(const std::string& target)
{
	switch (target.front()) {
	case '#':
		type = TargetType::HASH;
		str = target.substr(1);
		break;
	case '&':
		type = TargetType::AMPERSAND;
		str = target.substr(1);
		break;
	case '$':
		type = TargetType::DOLLAR;
		str = target.substr(1);
		break;
	default:
		const std::string::size_type idx = target.find('@');

		if (idx != std::string::npos) {
			type = TargetType::USER;
			user = target.substr(0, idx);
			host = target.substr(idx + 1);
		} else {
			type = TargetType::NICK;
			str = target;
		}
	}
}
