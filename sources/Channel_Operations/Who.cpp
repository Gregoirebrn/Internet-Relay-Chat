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
	for (user_t it = _channel[channel].begin(); it != _channel[channel].end(); ++it) {
		send_error(fd_cli, RPL_WHOREPLY(_client->GetWho(_client->GetFd(it->first), channel)));
	}
	send_error(fd_cli, RPL_ENDOFWHO(_client->GetName(fd_cli), channel));
	return (0);
}