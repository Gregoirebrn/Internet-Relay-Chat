//
// Created by grebrune on 12/20/24.
//

#include "Channel.hpp"

int	Channel::Who(std::string buff, int fd_cli) {
	std::string channel = buff.substr(0, buff.size());
	if (std::string::npos != buff.find('\r'))
		channel = buff.substr(0, (buff.size() - 1));
	std::cout << "WHO :" << channel << std::endl;
	if (_channel.find(channel) == _channel.end())
		return (send_error(fd_cli, ERR_NOSUCHCHANNEL(channel)), 403);
	Topic(buff, fd_cli);
	send_rpl_name(channel, fd_cli);
	return (0);
}