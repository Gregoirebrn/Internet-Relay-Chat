//
// Created by grebrune on 12/2/24.
//

#include "Channel.hpp"

// Constructors & destructor
Channel::Channel(void) {
	std::cout << "Channel default constructor called!" << std::endl;
}

Channel::~Channel(void) {
	std::cout << "Channel default destructor called!" << std::endl;
}

// Public methods

int	Channel::Canal_Operators(std::string buff, int fd_cli) {
	int	(Channel::*fptr[])(std::string, int ) = {&Channel::Kick, &Channel::Invite, &Channel::Topic, &Channel::Mode, &Channel::Join};
	std::string tab_com[] = {"KICK", "INVITE", "TOPIC", "MODE", "JOIN"};

	for (int i = 0; i < 5; ++i) {
		try {
			if (!buff.compare(0, tab_com[i].size(), tab_com[i])) {
				std::string arg = buff.substr(tab_com[i].size() + 1, buff.size());
				return ((this->*fptr[i])(arg, fd_cli), 1);
			}
		}
		catch (std::exception &exe) {
			std::cout << "Channel: " << exe.what() << std::endl;
		}
	}
	return (0);
}

//utils for commands
bool Channel::get_rights(std::string name, std::string channel, int fd_cli) {
	if (_all_chan.end() == _all_chan.find(channel))
		return (send_error(fd_cli, ERR_NOSUCHCHANNEL(channel)), false);
	for (iter_chan_user_t it = _chan_user.begin(); it != _chan_user.end(); ++it) {
		if (it->first == channel) {
			iter_chan_t i = it->second.begin();
			while (i != it->second.end() && i->first != name)
				i++;
			if (i == it->second.end())
				return (send_error(fd_cli, ERR_NOTONCHANNEL(channel)), false);
			if (i->second)
				return true;
			return (send_error(fd_cli, ERR_CHANOPRIVSNEEDED(channel)), false);
		}
	}
	return false;
}

