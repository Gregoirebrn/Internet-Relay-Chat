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
			return (send_error(fd_cli, ERR_CHANOPRIVSNEEDED(_client->GetName(fd_cli), channel)), false);
		}
	}
	return false;
}

// Public methods
bool	Channel::Canal_Operators(std::string buff, int fd_cli) {
	int	(Channel::*fptr[])(std::string, int ) = {&Channel::Kick, &Channel::Invite, \
		&Channel::Topic, &Channel::Mode, &Channel::Join, &Channel::Quit, &Channel::Privmsg, &Channel::Who};
	static std::string commands[] = {"KICK", "INVITE", "TOPIC", "MODE", "JOIN", "QUIT", "PRIVMSG", "WHO"};
	for (size_t i = 0; i < 8; ++i) {
		try {
			if (!buff.compare(0, commands[i].size(), commands[i])) {
				if (std::string::npos == buff.find(' ')) { //send nothing to the command so she throw the apropriate error
					(this->*fptr[i])("", fd_cli);
					return (false);
				}
				std::string arg = buff.substr((buff.find(' ') + 1), buff.size());
				if (arg.find('\r') != std::string::npos) // if we aree on Hexchat
					arg = arg.substr(0, arg.size() - 1);
//				std::cout << "CHANNEL ARG :" << arg << std::endl;
				if (!_client->IsRegister(fd_cli))
					return (send_error(fd_cli, ERR_NOTREGISTRED), 404);
				(this->*fptr[i])(arg, fd_cli);
				if (i == 5)
					return true;
				return false;
			}
		}
		catch (std::exception &exe) {
			std::cout << "Channel: " << exe.what() << std::endl;
		}
	}
	return (false);
}