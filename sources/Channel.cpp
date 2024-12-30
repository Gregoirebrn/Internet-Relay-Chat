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
//	std::cout << "GET_RIGHTS_NAME :"  << name << std::endl;
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
		&Channel::Topic, &Channel::Mode, &Channel::Join, &Channel::Quit, &Channel::Privmsg, &Channel::Who};
	static std::string commands[] = {"KICK", "INVITE", "TOPIC", "MODE", "JOIN", "QUIT", "PRIVMSG", "WHO"};
	for (size_t i = 0; i < 8; ++i) {
		try {
			if (!buff.compare(0, commands[i].size(), commands[i])) {
				if (std::string::npos == buff.find(' ')) { //send nothing to the command so she throw the apropriate error
					(this->*fptr[i])("", fd_cli);
					return (0);
				}
				std::string arg = buff.substr((buff.find(' ') + 1), buff.size());
				if (arg.find('\r') != std::string::npos) // if we aree on Hexchat
					arg = arg.substr(0, arg.size() - 1);
//				std::cout << "CHANNEL ARG :" << arg << std::endl;
				if (!_client->IsRegister(fd_cli))
					return (send_error(fd_cli, ERR_NOTREGISTRED), 404);
				return ((this->*fptr[i])(arg, fd_cli), 1);
			}
		}
		catch (std::exception &exe) {
			std::cout << "Channel: " << exe.what() << std::endl;
		}
	}
	return (0);
}