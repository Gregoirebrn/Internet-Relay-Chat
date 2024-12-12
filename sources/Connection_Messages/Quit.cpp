//
// Created by grebrune on 12/12/24.
//

#include "Channel.hpp"

void	Channel::Quit(int fd_cli) {
	std::string nick = Get_Client_Name(fd_cli);
	for (chan_t it = _channel.begin(); it != _channel.end() ; ++it) {
		if (it->first.find(nick))
			_channel[it->first].erase(nick);
	}
	Remove(fd_cli);
	send_error(fd_cli, ERR_GONE(nick));
}
