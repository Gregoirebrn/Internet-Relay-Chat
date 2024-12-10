//
// Created by grebrune on 12/10/24.
//

#include "Channel.hpp"

int Channel::Topic(std::string buff, int fd_cli) {
	if (buff.size() <= 2)
		return (send_error(fd_cli, ERR_NEEDMOREPARAMS(buff)), 462);//missing params
	size_t space = buff.find(' ');
	if (std::string::npos == space) {
		std::string channel = buff.substr(0, buff.size() -2);
		if (_all_chan.find(channel) == _all_chan.end())
			return (send_error(fd_cli, ERR_NOSUCHCHANNEL(channel)), false);
		if (_all_chan[channel].topic.empty())
			return (send_error(fd_cli, RPL_NOTOPIC(Get_Client_Name(fd_cli), channel)), 331);
		return (send_error(fd_cli, RPL_TOPIC(Get_Client_Name(fd_cli), channel, _all_chan[channel].topic)), 331);
	}
	std::string channel = buff.substr(0, space);
	std::string topic = buff.substr(space, buff.size() - 2);
	if (!get_rights(Get_Client_Name(fd_cli), channel, fd_cli))
		return (404);
	_all_chan[channel].topic = topic;
	if (_all_chan[channel].topic.empty())
		return (send_error(fd_cli, ERR_NEEDMOREPARAMS(buff)), 462);
	return (send_error(fd_cli, RPL_TOPIC(Get_Client_Name(fd_cli), channel, topic)), 462);
}
