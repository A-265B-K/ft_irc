#include <User.hpp>

/*
 i: invisible user mode
 o: oper user moder
 r: registered user mode
*/
const std::string User::flags{"ior"};

// 1 mode set, 0 mode unset, -1 mode unrecognized
int User::isMode(char c) const
{
	const std::string::size_type idx = flags.find(c);

	if (idx != std::string::npos)
		return (modes & 1 << idx) > 0;
	return -1;
}

void User::setMode(char c)
{
	const std::string::size_type idx = flags.find(c);

	if (idx != std::string::npos)
		modes |= 1 << idx;
}

void User::unsetMode(char c)
{
	const std::string::size_type idx = flags.find(c);

	if (idx != std::string::npos)
		modes &= ~(1 << idx);
}

std::string User::getModestring() const
{
	std::string modestring(modes ? "+" : "");

	for (const char c : flags)
		if (isMode(c))
			modestring += c;

	return modestring;
}

int User::editModes(std::string& changedModes, const std::string& modestring)
{
	std::string setFlags;
	std::string unsetFlags;
	int unknownFlag = 0;
	std::string::const_iterator cit = modestring.begin();

	// parse modestring
	while(cit != modestring.end())
top:	switch (*cit) {
		case '+':
			while (++cit != modestring.end())
				switch (isMode(*cit)) {
				case 0: 
					if (*cit != 'o') {
						setMode(*cit);
						setFlags += *cit;
					}
					// NOTE: +o is not allowed by the MODE command,
					//  and requires the appropriate OPER command
				case 1: break;
				default:
					if (*cit == '+' || *cit == '-')
						goto top;
					unknownFlag++;
				}
			break;
		case '-':
			while (++cit != modestring.end())
				switch (isMode(*cit)) {
				case 1: 
					unsetMode(*cit);
					unsetFlags += *cit;
					// NOTE: However, a user may -o itself through
					//  a MODE command
				case 0: break;
				default:
					if (*cit == '+' || *cit == '-')
						goto top;
					unknownFlag++;
			}
			break;
		default:
			if (isMode(*cit++) == -1)
				unknownFlag++;
		}

	// remove cancellations
	std::string::size_type idx;
	while((idx = setFlags.find_first_of(unsetFlags)) != std::string::npos) {
		unsetFlags.erase(unsetFlags.find(setFlags[idx]), 1);
		setFlags.erase(idx, 1);
	}

	if (!setFlags.empty())
		changedModes += '+' + setFlags;
	if (!unsetFlags.empty())
		changedModes += '-' + unsetFlags;

	return unknownFlag;
}
