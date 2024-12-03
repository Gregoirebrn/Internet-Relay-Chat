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
int Channel::Kick(std::string buff, int fd_cli) {
	(void)fd_cli;
	(void)buff;
	return (0);
}

int Channel::Invite(std::string buff, int fd_cli) {
	std::cout << "BUFF:" << buff << std::endl;
	size_t space = buff.find(' ');
	if (std::string::npos == space)
		return (send_error(fd_cli, ERR_NEEDMOREPARAMS(buff)), 462);
	std::string nick_target = buff.substr(0, space);
	std::string channel = buff.substr(space, buff.size());
	std::cout << "NICK_TARGET:" << nick_target << std::endl;
	std::cout << "CHANNEL:" << channel << std::endl;
	for (std::map<std::string , std::vector<std::string> >::iterator it = _all_chan.begin(); it != _all_chan.end(); ++it) {
		if (it->first == channel) {
			if (it->second.end() == std::find(it->second.begin(), it->second.end(), ))
				return (send_error(fd_cli, ERR_NOTONCHANNEL(channel)), 442);
			else
				it->second.push_back(nick_target);
		}
	}
	return (send_error(fd_cli, ERR_NOSUCHCHANNEL(channel)), 403);
	return (0);
}

int Channel::Topic(std::string buff, int fd_cli) {
	(void)fd_cli;
	(void)buff;
	return (0);
}

int Channel::Mode(std::string buff, int fd_cli) {
	(void)fd_cli;
	(void)buff;
	return (0);
}

int	Channel::Canal_Operators(std::string buff, int fd_cli) {
	int	(Channel::*fptr[])(std::string, int ) = {&Channel::Kick, &Channel::Invite, &Channel::Topic, &Channel::Mode};
	std::string tab_com[] = {"KICK", "INVITE", "TOPIC", "MODE"};

	for (int i = 0; i < 3; ++i) {
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