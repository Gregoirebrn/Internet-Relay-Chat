//
// Created by grebrune on 12/10/24.
//

#include "Channel.hpp"

int Channel::Mode(std::string buff, int fd_cli) {
	std::string channel = buff.substr(0, (buff.size() -1));
	std::cout << "MODE CHANNEL :" << channel << std::endl;
	if (_channel.find(channel) == _channel.end())
		return (send_error(fd_cli, ERR_NOSUCHCHANNEL(channel)), 403);
	std::string full_msg = ":irc_server 324 " + channel + " +nrt" + "\r\n";
	send_error(fd_cli, full_msg);
	return (0);
}