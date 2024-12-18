//
// Created by grebrune on 12/2/24.
//

#include "Channel.hpp"

// Constructors & destructor
Channel::Channel(Client *cli) {
	_client = cli;
//	std::cout << "Channel default constructor called!" << std::endl;
}

Channel::~Channel(void) {
//	std::cout << "Channel default destructor called!" << std::endl;
}

// getters
bool Channel::get_rights(std::string name, std::string channel, int fd_cli) {
	std::cout << "GET_RIGHTS_NAME :"  << name << std::endl;
	if (_all_chan.end() == _all_chan.find(channel))
		return (send_error(fd_cli, ERR_NOSUCHCHANNEL(channel)), false);
	for (chan_t it = _channel.begin(); it != _channel.end(); ++it) {
		if (it->first == channel) {
			user_t i = it->second.begin();
			while (i != it->second.end() && i->first != name)
				i++;
			if (i == it->second.end())
				return (send_error(fd_cli, ERR_NOTONCHANNEL(channel)), false);
			if (i->second)
				return true;
			return (send_error(fd_cli, ERR_CHANOPRIVSNEEDED(GetName(fd_cli), channel)), false);
		}
	}
	return false;
}

int	Channel::check_max_joined(int fd_cli, std::vector<std::string> channel_v) { // check with the macro MAX_CHAN if he would be in too many channel
	size_t chan_joined = 0;
	for (chan_t it = _channel.begin(); it != _channel.end() ; ++it) {
		if (it->second.find(GetName(fd_cli)) != it->second.end())
			chan_joined++;
	}
	if (chan_joined >= MAX_CHAN + 1)
		return (send_error(fd_cli, ERR_TOOMANYCHANNELS(GetName(fd_cli), channel_v.front())), 405);
	return (0);
}

// Public methods

int	Channel::Canal_Operators(std::string buff, int fd_cli) {
	int	(Channel::*fptr[])(std::string, int ) = {&Channel::Kick, &Channel::Invite, \
		&Channel::Topic, &Channel::Mode, &Channel::Join, &Channel::Quit, &Channel::Privmsg};
	const char *comm[] = {"KICK", "INVITE", "TOPIC", "MODE", "JOIN", "QUIT", "PRIVMSG"};
	std::vector<std::string> comm_v(comm, end(comm));
	for (size_t i = 0; i < comm_v.size(); ++i) {
		try {
			if (!buff.compare(0, comm_v[i].size(), comm_v[i])) {
				std::string arg = buff.substr(comm_v[i].size() + 1, buff.size());
				return ((this->*fptr[i])(arg, fd_cli), 1);
			}
		}
		catch (std::exception &exe) {
			std::cout << "Channel: " << exe.what() << std::endl;
		}
	}
	return (0);
}


