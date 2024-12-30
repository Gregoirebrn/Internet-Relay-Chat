//
// Created by grebrune on 12/12/24.
//

#include "Channel.hpp"

int	Channel::Quit(std::string buff, int fd_cli) {
	(void)buff;
	std::string nick = _client->GetName(fd_cli);
	for (chan_t it = _channel.begin(); it != _channel.end() ; ++it) {
		if (it->first.find(nick))
			_channel[it->first].erase(nick);
	}
	_client->Remove(fd_cli);
	return (0);
}
