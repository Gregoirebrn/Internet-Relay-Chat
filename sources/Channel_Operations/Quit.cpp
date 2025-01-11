//
// Created by grebrune on 12/12/24.
//

#include "Channel.hpp"

int	Channel::Quit(const std::string& buff, int fd_cli) {
	(void)buff;
	std::string nick = _client->GetName(fd_cli);
	for (chan_t it = _channel.begin(); it != _channel.end() ; ++it) {
		if (it->first.find(nick)) {
			send_chan_msg(it->first, RPL_PART(_client->GetPrefix(fd_cli), it->first, ":Disconnected"));
			_channel[it->first].erase(nick);
			_all_chan[it->first].in_user--;
			if (_all_chan[it->first].in_user == 0) {
				_channel.erase(it->first);
				_all_chan.erase(it->first);
				if (_channel.empty())
					break;
				it = _channel.begin();
			}
		}
	}
	_client->Remove(fd_cli);
	return (0);
}
